#
# Makefile for MIPS-specific library files..
#

lib-y	+= bitops.o csum_partial.o memset.o memcmp.o \
	   mips-atomic.o strlen_user.o strncpy_user.o \
	   strnlen_user.o uncached.o
 
ifndef CONFIG_ARCH_HAS_DELAY
lib-y	+= delay.o
endif
ifndef CONFIG_ARCH_HAS_MEMCPY
lib-y	+= memcpy.o
endif
obj-y			+= iomap.o
obj-$(CONFIG_PCI)	+= iomap-pci.o
lib-$(CONFIG_GENERIC_CSUM)	:= $(filter-out csum_partial.o, $(lib-y))

obj-$(CONFIG_CPU_GENERIC_DUMP_TLB) += dump_tlb.o
obj-$(CONFIG_CPU_R3000)		+= r3k_dump_tlb.o
obj-$(CONFIG_CPU_TX39XX)	+= r3k_dump_tlb.o

# libgcc-style stuff needed in the kernel
obj-y += ashldi3.o ashrdi3.o bswapsi.o bswapdi.o cmpdi2.o lshrdi3.o multi3.o \
	 ucmpdi2.o

CFLAGS_memcmp.o += $(DISABLE_LTO)
