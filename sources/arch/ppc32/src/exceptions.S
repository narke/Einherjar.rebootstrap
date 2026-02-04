/*
 * Exception vectors for PowerPC 32-bit (Book III-S).
 * This section must be copied to physical 0x0 (or 0xfffff000 if MSR[IP]=1)
 * at boot so the CPU jumps here on exception (0x500 = external, 0x900 = decrementer).
 */
#include <arch/asm/regname.h>

#define FRAME_SIZE  80

.section .vectors, "ax"
.align 12

.global _vectors_start
_vectors_start:
	.space 0x500

	/* Vector 0x500 - External interrupt */
.global exc_external
exc_external:
	/* Save r3 in SPRG1 to use it as scratch */
	mtsprg1 r3
	/* Get interrupt stack from SPRG0 */
	mfspr r3, sprg0
	subi r3, r3, FRAME_SIZE
	/* Save critical registers */
	stw r0, 0(r3)
	mfspr r0, sprg1   /* original r3 */
	stw r0, 4(r3)
	stw r4, 8(r3)
	stw r5, 12(r3)
	mfspr r4, srr0
	mfspr r5, srr1
	stw r4, 64(r3)    /* SRR0 */
	stw r5, 68(r3)    /* SRR1 */
	mflr r4
	mfcr r5
	stw r4, 72(r3)    /* LR */
	stw r5, 76(r3)    /* CR */
	/* Set stack pointer */
	mr r1, r3
	/* For now, just return */
	/* Restore */
	lwz r4, 64(r1)
	lwz r5, 68(r1)
	mtspr srr0, r4
	mtspr srr1, r5
	lwz r4, 72(r1)
	lwz r5, 76(r1)
	mtlr r4
	mtcr r5
	lwz r0, 0(r1)
	lwz r3, 4(r1)
	lwz r4, 8(r1)
	lwz r5, 12(r1)
	rfi

	.org 0x900
	/* Vector 0x900 - Decrementer */
.global exc_decrementer
exc_decrementer:
	/* Save r3 in SPRG1 to use it as scratch */
	mtsprg1 r3
	/* Get interrupt stack from SPRG0 */
	mfspr r3, sprg0
	subi r3, r3, FRAME_SIZE
	/* Save registers we will clobber */
	stw r0, 0(r3)
	mfspr r0, sprg1   /* original r3 */
	stw r0, 4(r3)
	stw r4, 8(r3)
	stw r5, 12(r3)
	stw r6, 16(r3)
	stw r7, 20(r3)
	stw r8, 24(r3)
	stw r9, 28(r3)
	stw r10, 32(r3)
	stw r11, 36(r3)
	stw r12, 40(r3)
	/* Save SRR0, SRR1, LR, CR, CTR, XER */
	mfspr r4, srr0
	mfspr r5, srr1
	stw r4, 44(r3)    /* SRR0 */
	stw r5, 48(r3)    /* SRR1 */
	mflr r4
	mfcr r5
	mfctr r6
	mfxer r7
	stw r4, 52(r3)    /* LR */
	stw r5, 56(r3)    /* CR */
	stw r6, 60(r3)    /* CTR */
	stw r7, 64(r3)    /* XER */
	/* Set stack pointer for C call */
	mr r1, r3
	/* Create stack frame for C ABI (back chain) */
	stwu r1, -16(r1)
	/* Call C handler using ABSOLUTE address (bl is PC-relative and won't work after copy) */
	lis r4, decrementer_tick@ha
	addi r4, r4, decrementer_tick@l
	mtctr r4
	bctrl
	/* Remove C frame */
	addi r1, r1, 16
	/* Restore SRR0, SRR1 */
	lwz r4, 44(r1)
	lwz r5, 48(r1)
	mtspr srr0, r4
	mtspr srr1, r5
	/* Restore LR, CR, CTR, XER */
	lwz r4, 52(r1)
	lwz r5, 56(r1)
	lwz r6, 60(r1)
	lwz r7, 64(r1)
	mtlr r4
	mtcr r5
	mtctr r6
	mtxer r7
	/* Restore GPRs */
	lwz r0, 0(r1)
	lwz r3, 4(r1)
	lwz r4, 8(r1)
	lwz r5, 12(r1)
	lwz r6, 16(r1)
	lwz r7, 20(r1)
	lwz r8, 24(r1)
	lwz r9, 28(r1)
	lwz r10, 32(r1)
	lwz r11, 36(r1)
	lwz r12, 40(r1)
	rfi

	.org 0x1000
.global _vectors_end
_vectors_end:
