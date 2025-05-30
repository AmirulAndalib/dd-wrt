/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 *  Low level TLB miss handlers for Book3E
 *
 *  Copyright (C) 2008-2009
 *      Ben. Herrenschmidt (benh@kernel.crashing.org), IBM Corp.
 */

#include <linux/pgtable.h>
#include <asm/processor.h>
#include <asm/reg.h>
#include <asm/page.h>
#include <asm/mmu.h>
#include <asm/ppc_asm.h>
#include <asm/asm-offsets.h>
#include <asm/cputable.h>
#include <asm/exception-64e.h>
#include <asm/ppc-opcode.h>
#include <asm/kvm_asm.h>
#include <asm/kvm_booke_hv_asm.h>
#include <asm/feature-fixups.h>

#define VPTE_PMD_SHIFT	(PTE_INDEX_SIZE)
#define VPTE_PUD_SHIFT	(VPTE_PMD_SHIFT + PMD_INDEX_SIZE)
#define VPTE_PGD_SHIFT	(VPTE_PUD_SHIFT + PUD_INDEX_SIZE)
#define VPTE_INDEX_SIZE (VPTE_PGD_SHIFT + PGD_INDEX_SIZE)

/**********************************************************************
 *                                                                    *
 * TLB miss handling for Book3E with a bolted linear mapping          *
 * No virtual page table, no nested TLB misses                        *
 *                                                                    *
 **********************************************************************/

/*
 * Note that, unlike non-bolted handlers, TLB_EXFRAME is not
 * modified by the TLB miss handlers themselves, since the TLB miss
 * handler code will not itself cause a recursive TLB miss.
 *
 * TLB_EXFRAME will be modified when crit/mc/debug exceptions are
 * entered/exited.
 */
.macro tlb_prolog_bolted intnum addr
	mtspr	SPRN_SPRG_GEN_SCRATCH,r12
	mfspr	r12,SPRN_SPRG_TLB_EXFRAME
	std	r13,EX_TLB_R13(r12)
	std	r10,EX_TLB_R10(r12)
	mfspr	r13,SPRN_SPRG_PACA

	mfcr	r10
	std	r11,EX_TLB_R11(r12)
#ifdef CONFIG_KVM_BOOKE_HV
BEGIN_FTR_SECTION
	mfspr	r11, SPRN_SRR1
END_FTR_SECTION_IFSET(CPU_FTR_EMB_HV)
#endif
	DO_KVM	\intnum, SPRN_SRR1
	std	r16,EX_TLB_R16(r12)
	mfspr	r16,\addr		/* get faulting address */
	std	r14,EX_TLB_R14(r12)
	ld	r14,PACAPGD(r13)
	std	r15,EX_TLB_R15(r12)
	std	r10,EX_TLB_CR(r12)
START_BTB_FLUSH_SECTION
	mfspr r11, SPRN_SRR1
	andi. r10,r11,MSR_PR
	beq 1f
	BTB_FLUSH(r10)
1:
END_BTB_FLUSH_SECTION
	std	r7,EX_TLB_R7(r12)
.endm

.macro tlb_epilog_bolted
	ld	r14,EX_TLB_CR(r12)
	ld	r7,EX_TLB_R7(r12)
	ld	r10,EX_TLB_R10(r12)
	ld	r11,EX_TLB_R11(r12)
	ld	r13,EX_TLB_R13(r12)
	mtcr	r14
	ld	r14,EX_TLB_R14(r12)
	ld	r15,EX_TLB_R15(r12)
	ld	r16,EX_TLB_R16(r12)
	mfspr	r12,SPRN_SPRG_GEN_SCRATCH
.endm

/* Data TLB miss */
	START_EXCEPTION(data_tlb_miss_bolted)
	tlb_prolog_bolted BOOKE_INTERRUPT_DTLB_MISS SPRN_DEAR

	/* We need _PAGE_PRESENT and  _PAGE_ACCESSED set */

	/* We do the user/kernel test for the PID here along with the RW test
	 */
	/* We pre-test some combination of permissions to avoid double
	 * faults:
	 *
	 * We move the ESR:ST bit into the position of _PAGE_BAP_SW in the PTE
	 * ESR_ST   is 0x00800000
	 * _PAGE_BAP_SW is 0x00000010
	 * So the shift is >> 19. This tests for supervisor writeability.
	 * If the page happens to be supervisor writeable and not user
	 * writeable, we will take a new fault later, but that should be
	 * a rare enough case.
	 *
	 * We also move ESR_ST in _PAGE_DIRTY position
	 * _PAGE_DIRTY is 0x00001000 so the shift is >> 11
	 *
	 * MAS1 is preset for all we need except for TID that needs to
	 * be cleared for kernel translations
	 */

	mfspr	r11,SPRN_ESR

	srdi	r15,r16,60		/* get region */
	rldicl.	r10,r16,64-PGTABLE_EADDR_SIZE,PGTABLE_EADDR_SIZE+4
	bne-	dtlb_miss_fault_bolted	/* Bail if fault addr is invalid */

	rlwinm	r10,r11,32-19,27,27
	rlwimi	r10,r11,32-16,19,19
	cmpwi	r15,0			/* user vs kernel check */
	ori	r10,r10,_PAGE_PRESENT
	oris	r11,r10,_PAGE_ACCESSED@h

	bne	tlb_miss_kernel_bolted

tlb_miss_user_bolted:
#ifdef CONFIG_PPC_KUAP
	mfspr	r10,SPRN_MAS1
	rlwinm.	r10,r10,0,0x3fff0000
	beq-	tlb_miss_fault_bolted /* KUAP fault */
#endif

tlb_miss_common_bolted:
/*
 * This is the guts of the TLB miss handler for bolted-linear.
 * We are entered with:
 *
 * r16 = faulting address
 * r15 = crap (free to use)
 * r14 = page table base
 * r13 = PACA
 * r11 = PTE permission mask
 * r10 = crap (free to use)
 */
	rldicl	r15,r16,64-PGDIR_SHIFT+3,64-PGD_INDEX_SIZE-3
	cmpldi	cr0,r14,0
	clrrdi	r15,r15,3
	beq	tlb_miss_fault_bolted	/* No PGDIR, bail */

	ldx	r14,r14,r15		/* grab pgd entry */

	rldicl	r15,r16,64-PUD_SHIFT+3,64-PUD_INDEX_SIZE-3
	clrrdi	r15,r15,3
	cmpdi	cr0,r14,0
	bge	tlb_miss_fault_bolted	/* Bad pgd entry or hugepage; bail */
	ldx	r14,r14,r15		/* grab pud entry */

	rldicl	r15,r16,64-PMD_SHIFT+3,64-PMD_INDEX_SIZE-3
	clrrdi	r15,r15,3
	cmpdi	cr0,r14,0
	bge	tlb_miss_fault_bolted
	ldx	r14,r14,r15		/* Grab pmd entry */

	rldicl	r15,r16,64-PAGE_SHIFT+3,64-PTE_INDEX_SIZE-3
	clrrdi	r15,r15,3
	cmpdi	cr0,r14,0
	bge	tlb_miss_fault_bolted
	ldx	r14,r14,r15		/* Grab PTE, normal (!huge) page */

	/* Check if required permissions are met */
	andc.	r15,r11,r14
	rldicr	r15,r14,64-(PTE_RPN_SHIFT-PAGE_SHIFT),63-PAGE_SHIFT
	bne-	tlb_miss_fault_bolted

	/* Now we build the MAS:
	 *
	 * MAS 0   :	Fully setup with defaults in MAS4 and TLBnCFG
	 * MAS 1   :	Almost fully setup
	 *               - PID already updated by caller if necessary
	 *               - TSIZE need change if !base page size, not
	 *                 yet implemented for now
	 * MAS 2   :	Defaults not useful, need to be redone
	 * MAS 3+7 :	Needs to be done
	 */
	clrrdi	r11,r16,12		/* Clear low crap in EA */
	clrldi	r15,r15,12		/* Clear crap at the top */
	rlwimi	r11,r14,32-19,27,31	/* Insert WIMGE */
	rlwimi	r15,r14,32-8,22,25	/* Move in U bits */
	mtspr	SPRN_MAS2,r11
	andi.	r11,r14,_PAGE_DIRTY
	rlwimi	r15,r14,32-2,26,31	/* Move in BAP bits */

	/* Mask out SW and UW if !DIRTY (XXX optimize this !) */
	bne	1f
	li	r11,MAS3_SW|MAS3_UW
	andc	r15,r15,r11
1:
	mtspr	SPRN_MAS7_MAS3,r15
	tlbwe

tlb_miss_done_bolted:
	tlb_epilog_bolted
	rfi

itlb_miss_kernel_bolted:
	li	r11,_PAGE_PRESENT|_PAGE_BAP_SX	/* Base perm */
	oris	r11,r11,_PAGE_ACCESSED@h
tlb_miss_kernel_bolted:
	mfspr	r10,SPRN_MAS1
	ld	r14,PACA_KERNELPGD(r13)
	srdi	r15,r16,44		/* get kernel region */
	andi.	r15,r15,1		/* Check for vmalloc region */
	rlwinm	r10,r10,0,16,1		/* Clear TID */
	mtspr	SPRN_MAS1,r10
	bne+	tlb_miss_common_bolted

tlb_miss_fault_bolted:
	/* We need to check if it was an instruction miss */
	andi.	r10,r11,_PAGE_BAP_UX|_PAGE_BAP_SX
	bne	itlb_miss_fault_bolted
dtlb_miss_fault_bolted:
	tlb_epilog_bolted
	b	exc_data_storage_book3e
itlb_miss_fault_bolted:
	tlb_epilog_bolted
	b	exc_instruction_storage_book3e

/* Instruction TLB miss */
	START_EXCEPTION(instruction_tlb_miss_bolted)
	tlb_prolog_bolted BOOKE_INTERRUPT_ITLB_MISS SPRN_SRR0

	rldicl.	r10,r16,64-PGTABLE_EADDR_SIZE,PGTABLE_EADDR_SIZE+4
	srdi	r15,r16,60		/* get region */
	bne-	itlb_miss_fault_bolted

	li	r11,_PAGE_PRESENT|_PAGE_BAP_UX	/* Base perm */

	/* We do the user/kernel test for the PID here along with the RW test
	 */

	cmpldi	cr0,r15,0			/* Check for user region */
	oris	r11,r11,_PAGE_ACCESSED@h
	beq	tlb_miss_user_bolted
	b	itlb_miss_kernel_bolted

/*
 * TLB miss handling for e6500 and derivatives, using hardware tablewalk.
 *
 * Linear mapping is bolted: no virtual page table or nested TLB misses
 * Indirect entries in TLB1, hardware loads resulting direct entries
 *    into TLB0
 * No HES or NV hint on TLB1, so we need to do software round-robin
 * No tlbsrx. so we need a spinlock, and we have to deal
 *    with MAS-damage caused by tlbsx
 * 4K pages only
 */

	START_EXCEPTION(instruction_tlb_miss_e6500)
	tlb_prolog_bolted BOOKE_INTERRUPT_ITLB_MISS SPRN_SRR0

	ld	r11,PACA_TCD_PTR(r13)
	srdi.	r15,r16,60		/* get region */
	ori	r16,r16,1

	bne	tlb_miss_kernel_e6500	/* user/kernel test */

	b	tlb_miss_common_e6500

	START_EXCEPTION(data_tlb_miss_e6500)
	tlb_prolog_bolted BOOKE_INTERRUPT_DTLB_MISS SPRN_DEAR

	ld	r11,PACA_TCD_PTR(r13)
	srdi.	r15,r16,60		/* get region */
	rldicr	r16,r16,0,62

	bne	tlb_miss_kernel_e6500	/* user vs kernel check */

/*
 * This is the guts of the TLB miss handler for e6500 and derivatives.
 * We are entered with:
 *
 * r16 = page of faulting address (low bit 0 if data, 1 if instruction)
 * r15 = crap (free to use)
 * r14 = page table base
 * r13 = PACA
 * r11 = tlb_per_core ptr
 * r10 = crap (free to use)
 * r7  = esel_next
 */
tlb_miss_common_e6500:
	crmove	cr2*4+2,cr0*4+2		/* cr2.eq != 0 if kernel address */

BEGIN_FTR_SECTION		/* CPU_FTR_SMT */
	/*
	 * Search if we already have an indirect entry for that virtual
	 * address, and if we do, bail out.
	 *
	 * MAS6:IND should be already set based on MAS4
	 */
	lhz	r10,PACAPACAINDEX(r13)
	addi	r10,r10,1
	crclr	cr1*4+eq	/* set cr1.eq = 0 for non-recursive */
1:	lbarx	r15,0,r11
	cmpdi	r15,0
	bne	2f
	stbcx.	r10,0,r11
	bne	1b
3:
	.subsection 1
2:	cmpd	cr1,r15,r10	/* recursive lock due to mcheck/crit/etc? */
	beq	cr1,3b		/* unlock will happen if cr1.eq = 0 */
10:	lbz	r15,0(r11)
	cmpdi	r15,0
	bne	10b
	b	1b
	.previous
END_FTR_SECTION_IFSET(CPU_FTR_SMT)

	lbz	r7,TCD_ESEL_NEXT(r11)

BEGIN_FTR_SECTION		/* CPU_FTR_SMT */
	/*
	 * Erratum A-008139 says that we can't use tlbwe to change
	 * an indirect entry in any way (including replacing or
	 * invalidating) if the other thread could be in the process
	 * of a lookup.  The workaround is to invalidate the entry
	 * with tlbilx before overwriting.
	 */

	rlwinm	r10,r7,16,0xff0000
	oris	r10,r10,MAS0_TLBSEL(1)@h
	mtspr	SPRN_MAS0,r10
	isync
	tlbre
	mfspr	r15,SPRN_MAS1
	andis.	r15,r15,MAS1_VALID@h
	beq	5f

BEGIN_FTR_SECTION_NESTED(532)
	mfspr	r10,SPRN_MAS8
	rlwinm	r10,r10,0,0x80000fff  /* tgs,tlpid -> sgs,slpid */
	mtspr	SPRN_MAS5,r10
END_FTR_SECTION_NESTED(CPU_FTR_EMB_HV,CPU_FTR_EMB_HV,532)

	mfspr	r10,SPRN_MAS1
	rlwinm	r15,r10,0,0x3fff0000  /* tid -> spid */
	rlwimi	r15,r10,20,0x00000003 /* ind,ts -> sind,sas */
	mfspr	r10,SPRN_MAS6
	mtspr	SPRN_MAS6,r15

	mfspr	r15,SPRN_MAS2
	isync
	tlbilxva 0,r15
	isync

	mtspr	SPRN_MAS6,r10

5:
BEGIN_FTR_SECTION_NESTED(532)
	li	r10,0
	mtspr	SPRN_MAS8,r10
	mtspr	SPRN_MAS5,r10
END_FTR_SECTION_NESTED(CPU_FTR_EMB_HV,CPU_FTR_EMB_HV,532)

	tlbsx	0,r16
	mfspr	r10,SPRN_MAS1
	andis.	r15,r10,MAS1_VALID@h
	bne	tlb_miss_done_e6500
FTR_SECTION_ELSE
	mfspr	r10,SPRN_MAS1
ALT_FTR_SECTION_END_IFSET(CPU_FTR_SMT)

	oris	r10,r10,MAS1_VALID@h
	beq	cr2,4f
	rlwinm	r10,r10,0,16,1		/* Clear TID */
4:	mtspr	SPRN_MAS1,r10

	/* Now, we need to walk the page tables. First check if we are in
	 * range.
	 */
	rldicl.	r10,r16,64-PGTABLE_EADDR_SIZE,PGTABLE_EADDR_SIZE+4
	bne-	tlb_miss_fault_e6500

	rldicl	r15,r16,64-PGDIR_SHIFT+3,64-PGD_INDEX_SIZE-3
	cmpldi	cr0,r14,0
	clrrdi	r15,r15,3
	beq-	tlb_miss_fault_e6500 /* No PGDIR, bail */
	ldx	r14,r14,r15		/* grab pgd entry */

	rldicl	r15,r16,64-PUD_SHIFT+3,64-PUD_INDEX_SIZE-3
	clrrdi	r15,r15,3
	cmpdi	cr0,r14,0
	bge	tlb_miss_huge_e6500	/* Bad pgd entry or hugepage; bail */
	ldx	r14,r14,r15		/* grab pud entry */

	rldicl	r15,r16,64-PMD_SHIFT+3,64-PMD_INDEX_SIZE-3
	clrrdi	r15,r15,3
	cmpdi	cr0,r14,0
	bge	tlb_miss_huge_e6500
	ldx	r14,r14,r15		/* Grab pmd entry */

	mfspr	r10,SPRN_MAS0
	cmpdi	cr0,r14,0
	bge	tlb_miss_huge_e6500

	/* Now we build the MAS for a 2M indirect page:
	 *
	 * MAS 0   :	ESEL needs to be filled by software round-robin
	 * MAS 1   :	Fully set up
	 *               - PID already updated by caller if necessary
	 *               - TSIZE for now is base ind page size always
	 *               - TID already cleared if necessary
	 * MAS 2   :	Default not 2M-aligned, need to be redone
	 * MAS 3+7 :	Needs to be done
	 */

	ori	r14,r14,(BOOK3E_PAGESZ_4K << MAS3_SPSIZE_SHIFT)
	mtspr	SPRN_MAS7_MAS3,r14

	clrrdi	r15,r16,21		/* make EA 2M-aligned */
	mtspr	SPRN_MAS2,r15

tlb_miss_huge_done_e6500:
	lbz	r16,TCD_ESEL_MAX(r11)
	lbz	r14,TCD_ESEL_FIRST(r11)
	rlwimi	r10,r7,16,0x00ff0000	/* insert esel_next into MAS0 */
	addi	r7,r7,1			/* increment esel_next */
	mtspr	SPRN_MAS0,r10
	cmpw	r7,r16
	iseleq	r7,r14,r7		/* if next == last use first */
	stb	r7,TCD_ESEL_NEXT(r11)

	tlbwe

tlb_miss_done_e6500:
	.macro	tlb_unlock_e6500
BEGIN_FTR_SECTION
	beq	cr1,1f		/* no unlock if lock was recursively grabbed */
	li	r15,0
	isync
	stb	r15,0(r11)
1:
END_FTR_SECTION_IFSET(CPU_FTR_SMT)
	.endm

	tlb_unlock_e6500
	tlb_epilog_bolted
	rfi

tlb_miss_huge_e6500:
	beq	tlb_miss_fault_e6500
	li	r10,1
	andi.	r15,r14,HUGEPD_SHIFT_MASK@l /* r15 = psize */
	rldimi	r14,r10,63,0		/* Set PD_HUGE */
	xor	r14,r14,r15		/* Clear size bits */
	ldx	r14,0,r14

	/*
	 * Now we build the MAS for a huge page.
	 *
	 * MAS 0   :	ESEL needs to be filled by software round-robin
	 *		 - can be handled by indirect code
	 * MAS 1   :	Need to clear IND and set TSIZE
	 * MAS 2,3+7:	Needs to be redone similar to non-tablewalk handler
	 */

	subi	r15,r15,10		/* Convert psize to tsize */
	mfspr	r10,SPRN_MAS1
	rlwinm	r10,r10,0,~MAS1_IND
	rlwimi	r10,r15,MAS1_TSIZE_SHIFT,MAS1_TSIZE_MASK
	mtspr	SPRN_MAS1,r10

	li	r10,-0x400
	sld	r15,r10,r15		/* Generate mask based on size */
	and	r10,r16,r15
	rldicr	r15,r14,64-(PTE_RPN_SHIFT-PAGE_SHIFT),63-PAGE_SHIFT
	rlwimi	r10,r14,32-19,27,31	/* Insert WIMGE */
	clrldi	r15,r15,PAGE_SHIFT	/* Clear crap at the top */
	rlwimi	r15,r14,32-8,22,25	/* Move in U bits */
	mtspr	SPRN_MAS2,r10
	andi.	r10,r14,_PAGE_DIRTY
	rlwimi	r15,r14,32-2,26,31	/* Move in BAP bits */

	/* Mask out SW and UW if !DIRTY (XXX optimize this !) */
	bne	1f
	li	r10,MAS3_SW|MAS3_UW
	andc	r15,r15,r10
1:
	mtspr	SPRN_MAS7_MAS3,r15

	mfspr	r10,SPRN_MAS0
	b	tlb_miss_huge_done_e6500

tlb_miss_kernel_e6500:
	ld	r14,PACA_KERNELPGD(r13)
	srdi	r15,r16,44		/* get kernel region */
	xoris	r15,r15,0xc		/* Check for vmalloc region */
	cmplwi	cr1,r15,1
	beq+	cr1,tlb_miss_common_e6500

tlb_miss_fault_e6500:
	tlb_unlock_e6500
	/* We need to check if it was an instruction miss */
	andi.	r16,r16,1
	bne	itlb_miss_fault_e6500
dtlb_miss_fault_e6500:
	tlb_epilog_bolted
	b	exc_data_storage_book3e
itlb_miss_fault_e6500:
	tlb_epilog_bolted
	b	exc_instruction_storage_book3e

/**********************************************************************
 *                                                                    *
 * TLB miss handling for Book3E with TLB reservation and HES support  *
 *                                                                    *
 **********************************************************************/


/* Data TLB miss */
	START_EXCEPTION(data_tlb_miss)
	TLB_MISS_PROLOG

	/* Now we handle the fault proper. We only save DEAR in normal
	 * fault case since that's the only interesting values here.
	 * We could probably also optimize by not saving SRR0/1 in the
	 * linear mapping case but I'll leave that for later
	 */
	mfspr	r14,SPRN_ESR
	mfspr	r16,SPRN_DEAR		/* get faulting address */
	srdi	r15,r16,44		/* get region */
	xoris	r15,r15,0xc
	cmpldi	cr0,r15,0		/* linear mapping ? */
	beq	tlb_load_linear		/* yes -> go to linear map load */
	cmpldi	cr1,r15,1		/* vmalloc mapping ? */

	/* The page tables are mapped virtually linear. At this point, though,
	 * we don't know whether we are trying to fault in a first level
	 * virtual address or a virtual page table address. We can get that
	 * from bit 0x1 of the region ID which we have set for a page table
	 */
	andis.	r10,r15,0x1
	bne-	virt_page_table_tlb_miss

	std	r14,EX_TLB_ESR(r12);	/* save ESR */
	std	r16,EX_TLB_DEAR(r12);	/* save DEAR */

	 /* We need _PAGE_PRESENT and  _PAGE_ACCESSED set */
	li	r11,_PAGE_PRESENT
	oris	r11,r11,_PAGE_ACCESSED@h

	/* We do the user/kernel test for the PID here along with the RW test
	 */
	srdi.	r15,r16,60		/* Check for user region */

	/* We pre-test some combination of permissions to avoid double
	 * faults:
	 *
	 * We move the ESR:ST bit into the position of _PAGE_BAP_SW in the PTE
	 * ESR_ST   is 0x00800000
	 * _PAGE_BAP_SW is 0x00000010
	 * So the shift is >> 19. This tests for supervisor writeability.
	 * If the page happens to be supervisor writeable and not user
	 * writeable, we will take a new fault later, but that should be
	 * a rare enough case.
	 *
	 * We also move ESR_ST in _PAGE_DIRTY position
	 * _PAGE_DIRTY is 0x00001000 so the shift is >> 11
	 *
	 * MAS1 is preset for all we need except for TID that needs to
	 * be cleared for kernel translations
	 */
	rlwimi	r11,r14,32-19,27,27
	rlwimi	r11,r14,32-16,19,19
	beq	normal_tlb_miss_user
	/* XXX replace the RMW cycles with immediate loads + writes */
1:	mfspr	r10,SPRN_MAS1
	rlwinm	r10,r10,0,16,1		/* Clear TID */
	mtspr	SPRN_MAS1,r10
	beq+	cr1,normal_tlb_miss

	/* We got a crappy address, just fault with whatever DEAR and ESR
	 * are here
	 */
	TLB_MISS_EPILOG_ERROR
	b	exc_data_storage_book3e

/* Instruction TLB miss */
	START_EXCEPTION(instruction_tlb_miss)
	TLB_MISS_PROLOG

	/* If we take a recursive fault, the second level handler may need
	 * to know whether we are handling a data or instruction fault in
	 * order to get to the right store fault handler. We provide that
	 * info by writing a crazy value in ESR in our exception frame
	 */
	li	r14,-1	/* store to exception frame is done later */

	/* Now we handle the fault proper. We only save DEAR in the non
	 * linear mapping case since we know the linear mapping case will
	 * not re-enter. We could indeed optimize and also not save SRR0/1
	 * in the linear mapping case but I'll leave that for later
	 *
	 * Faulting address is SRR0 which is already in r16
	 */
	srdi	r15,r16,44		/* get region */
	xoris	r15,r15,0xc
	cmpldi	cr0,r15,0		/* linear mapping ? */
	beq	tlb_load_linear		/* yes -> go to linear map load */
	cmpldi	cr1,r15,1		/* vmalloc mapping ? */

	/* We do the user/kernel test for the PID here along with the RW test
	 */
	li	r11,_PAGE_PRESENT|_PAGE_BAP_UX	/* Base perm */
	oris	r11,r11,_PAGE_ACCESSED@h

	srdi.	r15,r16,60			/* Check for user region */
	std	r14,EX_TLB_ESR(r12)		/* write crazy -1 to frame */
	beq	normal_tlb_miss_user

	li	r11,_PAGE_PRESENT|_PAGE_BAP_SX	/* Base perm */
	oris	r11,r11,_PAGE_ACCESSED@h
	/* XXX replace the RMW cycles with immediate loads + writes */
	mfspr	r10,SPRN_MAS1
	rlwinm	r10,r10,0,16,1			/* Clear TID */
	mtspr	SPRN_MAS1,r10
	beq+	cr1,normal_tlb_miss

	/* We got a crappy address, just fault */
	TLB_MISS_EPILOG_ERROR
	b	exc_instruction_storage_book3e

/*
 * This is the guts of the first-level TLB miss handler for direct
 * misses. We are entered with:
 *
 * r16 = faulting address
 * r15 = region ID
 * r14 = crap (free to use)
 * r13 = PACA
 * r12 = TLB exception frame in PACA
 * r11 = PTE permission mask
 * r10 = crap (free to use)
 */
normal_tlb_miss_user:
#ifdef CONFIG_PPC_KUAP
	mfspr	r14,SPRN_MAS1
	rlwinm.	r14,r14,0,0x3fff0000
	beq-	normal_tlb_miss_access_fault /* KUAP fault */
#endif
normal_tlb_miss:
	/* So we first construct the page table address. We do that by
	 * shifting the bottom of the address (not the region ID) by
	 * PAGE_SHIFT-3, clearing the bottom 3 bits (get a PTE ptr) and
	 * or'ing the fourth high bit.
	 *
	 * NOTE: For 64K pages, we do things slightly differently in
	 * order to handle the weird page table format used by linux
	 */
	srdi	r15,r16,44
	oris	r10,r15,0x1
	rldicl	r14,r16,64-(PAGE_SHIFT-3),PAGE_SHIFT-3+4
	sldi	r15,r10,44
	clrrdi	r14,r14,19
	or	r10,r15,r14

	ld	r14,0(r10)

finish_normal_tlb_miss:
	/* Check if required permissions are met */
	andc.	r15,r11,r14
	bne-	normal_tlb_miss_access_fault

	/* Now we build the MAS:
	 *
	 * MAS 0   :	Fully setup with defaults in MAS4 and TLBnCFG
	 * MAS 1   :	Almost fully setup
	 *               - PID already updated by caller if necessary
	 *               - TSIZE need change if !base page size, not
	 *                 yet implemented for now
	 * MAS 2   :	Defaults not useful, need to be redone
	 * MAS 3+7 :	Needs to be done
	 *
	 * TODO: mix up code below for better scheduling
	 */
	clrrdi	r10,r16,12		/* Clear low crap in EA */
	rlwimi	r10,r14,32-19,27,31	/* Insert WIMGE */
	mtspr	SPRN_MAS2,r10

	/* Check page size, if not standard, update MAS1 */
	rldicl	r10,r14,64-8,64-8
	cmpldi	cr0,r10,BOOK3E_PAGESZ_4K
	beq-	1f
	mfspr	r11,SPRN_MAS1
	rlwimi	r11,r14,31,21,24
	rlwinm	r11,r11,0,21,19
	mtspr	SPRN_MAS1,r11
1:
	/* Move RPN in position */
	rldicr	r11,r14,64-(PTE_RPN_SHIFT-PAGE_SHIFT),63-PAGE_SHIFT
	clrldi	r15,r11,12		/* Clear crap at the top */
	rlwimi	r15,r14,32-8,22,25	/* Move in U bits */
	rlwimi	r15,r14,32-2,26,31	/* Move in BAP bits */

	/* Mask out SW and UW if !DIRTY (XXX optimize this !) */
	andi.	r11,r14,_PAGE_DIRTY
	bne	1f
	li	r11,MAS3_SW|MAS3_UW
	andc	r15,r15,r11
1:
	srdi	r16,r15,32
	mtspr	SPRN_MAS3,r15
	mtspr	SPRN_MAS7,r16

	tlbwe

normal_tlb_miss_done:
	/* We don't bother with restoring DEAR or ESR since we know we are
	 * level 0 and just going back to userland. They are only needed
	 * if you are going to take an access fault
	 */
	TLB_MISS_EPILOG_SUCCESS
	rfi

normal_tlb_miss_access_fault:
	/* We need to check if it was an instruction miss */
	andi.	r10,r11,_PAGE_BAP_UX
	bne	1f
	ld	r14,EX_TLB_DEAR(r12)
	ld	r15,EX_TLB_ESR(r12)
	mtspr	SPRN_DEAR,r14
	mtspr	SPRN_ESR,r15
	TLB_MISS_EPILOG_ERROR
	b	exc_data_storage_book3e
1:	TLB_MISS_EPILOG_ERROR
	b	exc_instruction_storage_book3e


/*
 * This is the guts of the second-level TLB miss handler for direct
 * misses. We are entered with:
 *
 * r16 = virtual page table faulting address
 * r15 = region (top 4 bits of address)
 * r14 = crap (free to use)
 * r13 = PACA
 * r12 = TLB exception frame in PACA
 * r11 = crap (free to use)
 * r10 = crap (free to use)
 *
 * Note that this should only ever be called as a second level handler
 * with the current scheme when using SW load.
 * That means we can always get the original fault DEAR at
 * EX_TLB_DEAR-EX_TLB_SIZE(r12)
 *
 * It can be re-entered by the linear mapping miss handler. However, to
 * avoid too much complication, it will restart the whole fault at level
 * 0 so we don't care too much about clobbers
 *
 * XXX That code was written back when we couldn't clobber r14. We can now,
 * so we could probably optimize things a bit
 */
virt_page_table_tlb_miss:
	/* Are we hitting a kernel page table ? */
	srdi	r15,r16,60
	andi.	r10,r15,0x8

	/* The cool thing now is that r10 contains 0 for user and 8 for kernel,
	 * and we happen to have the swapper_pg_dir at offset 8 from the user
	 * pgdir in the PACA :-).
	 */
	add	r11,r10,r13

	/* If kernel, we need to clear MAS1 TID */
	beq	1f
	/* XXX replace the RMW cycles with immediate loads + writes */
	mfspr	r10,SPRN_MAS1
	rlwinm	r10,r10,0,16,1			/* Clear TID */
	mtspr	SPRN_MAS1,r10
#ifdef CONFIG_PPC_KUAP
	b	2f
1:
	mfspr	r10,SPRN_MAS1
	rlwinm.	r10,r10,0,0x3fff0000
	beq-	virt_page_table_tlb_miss_fault /* KUAP fault */
2:
#else
1:
#endif

	/* Now, we need to walk the page tables. First check if we are in
	 * range.
	 */
	rldicl	r10,r16,64-(VPTE_INDEX_SIZE+3),VPTE_INDEX_SIZE+3+4
	cmpldi	r10,0x80
	bne-	virt_page_table_tlb_miss_fault

	/* Get the PGD pointer */
	ld	r15,PACAPGD(r11)
	cmpldi	cr0,r15,0
	beq-	virt_page_table_tlb_miss_fault

	/* Get to PGD entry */
	rldicl	r11,r16,64-VPTE_PGD_SHIFT,64-PGD_INDEX_SIZE-3
	clrrdi	r10,r11,3
	ldx	r15,r10,r15
	cmpdi	cr0,r15,0
	bge	virt_page_table_tlb_miss_fault

	/* Get to PUD entry */
	rldicl	r11,r16,64-VPTE_PUD_SHIFT,64-PUD_INDEX_SIZE-3
	clrrdi	r10,r11,3
	ldx	r15,r10,r15
	cmpdi	cr0,r15,0
	bge	virt_page_table_tlb_miss_fault

	/* Get to PMD entry */
	rldicl	r11,r16,64-VPTE_PMD_SHIFT,64-PMD_INDEX_SIZE-3
	clrrdi	r10,r11,3
	ldx	r15,r10,r15
	cmpdi	cr0,r15,0
	bge	virt_page_table_tlb_miss_fault

	/* Ok, we're all right, we can now create a kernel translation for
	 * a 4K or 64K page from r16 -> r15.
	 */
	/* Now we build the MAS:
	 *
	 * MAS 0   :	Fully setup with defaults in MAS4 and TLBnCFG
	 * MAS 1   :	Almost fully setup
	 *               - PID already updated by caller if necessary
	 *               - TSIZE for now is base page size always
	 * MAS 2   :	Use defaults
	 * MAS 3+7 :	Needs to be done
	 *
	 * So we only do MAS 2 and 3 for now...
	 */
	clrldi	r11,r15,4		/* remove region ID from RPN */
	ori	r10,r11,1		/* Or-in SR */

	srdi	r16,r10,32
	mtspr	SPRN_MAS3,r10
	mtspr	SPRN_MAS7,r16

	tlbwe

	/* Return to caller, normal case */
	TLB_MISS_EPILOG_SUCCESS
	rfi

virt_page_table_tlb_miss_fault:
	/* If we fault here, things are a little bit tricky. We need to call
	 * either data or instruction store fault, and we need to retrieve
	 * the original fault address and ESR (for data).
	 *
	 * The thing is, we know that in normal circumstances, this is
	 * always called as a second level tlb miss for SW load or as a first
	 * level TLB miss for HW load, so we should be able to peek at the
	 * relevant information in the first exception frame in the PACA.
	 *
	 * However, we do need to double check that, because we may just hit
	 * a stray kernel pointer or a userland attack trying to hit those
	 * areas. If that is the case, we do a data fault. (We can't get here
	 * from an instruction tlb miss anyway).
	 *
	 * Note also that when going to a fault, we must unwind the previous
	 * level as well. Since we are doing that, we don't need to clear or
	 * restore the TLB reservation neither.
	 */
	subf	r10,r13,r12
	cmpldi	cr0,r10,PACA_EXTLB+EX_TLB_SIZE
	bne-	virt_page_table_tlb_miss_whacko_fault

	/* We dig the original DEAR and ESR from slot 0 */
	ld	r15,EX_TLB_DEAR+PACA_EXTLB(r13)
	ld	r16,EX_TLB_ESR+PACA_EXTLB(r13)

	/* We check for the "special" ESR value for instruction faults */
	cmpdi	cr0,r16,-1
	beq	1f
	mtspr	SPRN_DEAR,r15
	mtspr	SPRN_ESR,r16
	TLB_MISS_EPILOG_ERROR
	b	exc_data_storage_book3e
1:	TLB_MISS_EPILOG_ERROR
	b	exc_instruction_storage_book3e

virt_page_table_tlb_miss_whacko_fault:
	/* The linear fault will restart everything so ESR and DEAR will
	 * not have been clobbered, let's just fault with what we have
	 */
	TLB_MISS_EPILOG_ERROR
	b	exc_data_storage_book3e

/*
 * This is the guts of "any" level TLB miss handler for kernel linear
 * mapping misses. We are entered with:
 *
 *
 * r16 = faulting address
 * r15 = crap (free to use)
 * r14 = ESR (data) or -1 (instruction)
 * r13 = PACA
 * r12 = TLB exception frame in PACA
 * r11 = crap (free to use)
 * r10 = crap (free to use)
 *
 * In addition we know that we will not re-enter, so in theory, we could
 * use a simpler epilog not restoring SRR0/1 etc.. but we'll do that later.
 *
 * We also need to be careful about MAS registers here & TLB reservation,
 * as we know we'll have clobbered them if we interrupt the main TLB miss
 * handlers in which case we probably want to do a full restart at level
 * 0 rather than saving / restoring the MAS.
 *
 * Note: If we care about performance of that core, we can easily shuffle
 *       a few things around
 */
tlb_load_linear:
	/* For now, we assume the linear mapping is contiguous and stops at
	 * linear_map_top. We also assume the size is a multiple of 1G, thus
	 * we only use 1G pages for now. That might have to be changed in a
	 * final implementation, especially when dealing with hypervisors
	 */
	__LOAD_PACA_TOC(r11)
	LOAD_REG_ADDR_ALTTOC(r11, r11, linear_map_top)
	ld	r10,0(r11)
	tovirt(10,10)
	cmpld	cr0,r16,r10
	bge	tlb_load_linear_fault

	/* MAS1 need whole new setup. */
	li	r15,(BOOK3E_PAGESZ_1GB<<MAS1_TSIZE_SHIFT)
	oris	r15,r15,MAS1_VALID@h	/* MAS1 needs V and TSIZE */
	mtspr	SPRN_MAS1,r15

	/* Already somebody there ? */
	PPC_TLBSRX_DOT(0,R16)
	beq	tlb_load_linear_done

	/* Now we build the remaining MAS. MAS0 and 2 should be fine
	 * with their defaults, which leaves us with MAS 3 and 7. The
	 * mapping is linear, so we just take the address, clear the
	 * region bits, and or in the permission bits which are currently
	 * hard wired
	 */
	clrrdi	r10,r16,30		/* 1G page index */
	clrldi	r10,r10,4		/* clear region bits */
	ori	r10,r10,MAS3_SR|MAS3_SW|MAS3_SX

	srdi	r16,r10,32
	mtspr	SPRN_MAS3,r10
	mtspr	SPRN_MAS7,r16

	tlbwe

tlb_load_linear_done:
	/* We use the "error" epilog for success as we do want to
	 * restore to the initial faulting context, whatever it was.
	 * We do that because we can't resume a fault within a TLB
	 * miss handler, due to MAS and TLB reservation being clobbered.
	 */
	TLB_MISS_EPILOG_ERROR
	rfi

tlb_load_linear_fault:
	/* We keep the DEAR and ESR around, this shouldn't have happened */
	cmpdi	cr0,r14,-1
	beq	1f
	TLB_MISS_EPILOG_ERROR_SPECIAL
	b	exc_data_storage_book3e
1:	TLB_MISS_EPILOG_ERROR_SPECIAL
	b	exc_instruction_storage_book3e
