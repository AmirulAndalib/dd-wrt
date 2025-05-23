/* Copyright (C) 1998 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Philip Blundell <philb@gnu.org>

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
#if __ARM_ARCH < 7

#include "asmdefs.h"
.text
.global memset
.type memset,%function
.align 4

#if defined(THUMB1_ONLY)
.thumb_func
memset:
	mov	ip, r0
	cmp	r2, #8		@ at least 8 bytes to do?
	bcc	2f

	and	r1, r1, #0xFF
	lsl	r3, r1, #8
	orr	r1, r3
	lsl	r3, r1, #16
	orr	r1, r3

	mov	r3, #3
1:	@ Fill up to the first word boundary
	tst	r0, r3
	beq	1f
	strb	r1, [r0]
	add	r0, r0, #1
	sub	r2, r2, #1
	b	1b
1:	@ Fill aligned words
	str	r1, [r0]
	add	r0, r0, #4
	sub	r2, r2, #4
	cmp	r2, #4
	bcs	1b

2:	@ Fill the remaining bytes
	cmp	r2, #0
	beq	2f
1:
	strb	r1, [r0]
	add	r0, r0, #1
	sub	r2, r2, #1
	bne	1b
2:
	mov	r0, ip
	bx lr
#else
memset:
	mov	a4, a1
	cmp	a3, $8		@ at least 8 bytes to do?
	blo	2f
	and	a2, a2, #0xFF
	orr	a2, a2, a2, lsl $8
	orr	a2, a2, a2, lsl $16
1:
	tst	a4, $3		@ aligned yet?
#if defined(__thumb2__)
	itt	ne
	strbne	a2, [a4], $1
	subne	a3, a3, $1
#else
	strneb	a2, [a4], $1
	subne	a3, a3, $1
#endif
	bne	1b
	mov	ip, a2
1:
	cmp	a3, $8		@ 8 bytes still to do?
	blo	2f
	stmia	a4!, {a2, ip}
	sub	a3, a3, $8
	cmp	a3, $8		@ 8 bytes still to do?
	blo	2f
	stmia	a4!, {a2, ip}
	sub	a3, a3, $8
	cmp	a3, $8		@ 8 bytes still to do?
	blo	2f
	stmia	a4!, {a2, ip}
	sub	a3, a3, $8
	cmp	a3, $8		@ 8 bytes still to do?
#if defined(__thumb2__)
	itt	hs
	stmiahs	a4!, {a2, ip}
	subhs	a3, a3, $8
#else
	stmhsia	a4!, {a2, ip}
	subhs	a3, a3, $8
#endif
	bhs	1b
2:
	movs	a3, a3		@ anything left?
	IT(t, eq)
	BXC(eq, lr)			@ nope
#if defined (__thumb2__)
1:
	strb	a2, [a4], #1
	subs	a3, a3, #1
	bne	1b
	bx	lr
#else
	rsb	a3, a3, $7
	add	pc, pc, a3, lsl $2
	mov	r0, r0
	strb	a2, [a4], $1
	strb	a2, [a4], $1
	strb	a2, [a4], $1
	strb	a2, [a4], $1
	strb	a2, [a4], $1
	strb	a2, [a4], $1
	strb	a2, [a4], $1
	BX(lr)
#endif
#endif

.size memset,.-memset

#endif