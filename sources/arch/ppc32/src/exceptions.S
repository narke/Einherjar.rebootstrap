/*
 * Exception vectors for PowerPC 32-bit (Book III-S).
 * This section must be copied to physical 0x0 (or 0xfffff000 if MSR[IP]=1)
 * at boot so the CPU jumps here on exception (0x500 = external, 0x900 = decrementer).
 */
#include <arch/asm/regname.h>

#define FRAME_SIZE  80

#define FRAME_R0    0
#define FRAME_R3    4
#define FRAME_R4    8
#define FRAME_R5    12
#define FRAME_R6    16
#define FRAME_R7    20
#define FRAME_R8    24
#define FRAME_R9    28
#define FRAME_R10   32
#define FRAME_R11   36
#define FRAME_R12   40
#define FRAME_SRR0  44
#define FRAME_SRR1  48
#define FRAME_LR    52
#define FRAME_CR    56
#define FRAME_CTR   60
#define FRAME_XER   64
#define C_ABI_FRAME 16

.macro SAVE_VOLATILE_FRAME
	/* Save r3 in SPRG1 to use it as scratch */
	mtsprg1 r3
	/* Get interrupt stack from SPRG0 */
	mfspr r3, sprg0
	subi r3, r3, FRAME_SIZE
	/* Save registers we will clobber */
	stw r0, FRAME_R0(r3)
	mfspr r0, sprg1   /* original r3 */
	stw r0, FRAME_R3(r3)
	stw r4, FRAME_R4(r3)
	stw r5, FRAME_R5(r3)
	stw r6, FRAME_R6(r3)
	stw r7, FRAME_R7(r3)
	stw r8, FRAME_R8(r3)
	stw r9, FRAME_R9(r3)
	stw r10, FRAME_R10(r3)
	stw r11, FRAME_R11(r3)
	stw r12, FRAME_R12(r3)
	/* Save SRR0, SRR1, LR, CR, CTR, XER */
	mfspr r4, srr0
	mfspr r5, srr1
	stw r4, FRAME_SRR0(r3)
	stw r5, FRAME_SRR1(r3)
	mflr r4
	mfcr r5
	mfctr r6
	mfxer r7
	stw r4, FRAME_LR(r3)
	stw r5, FRAME_CR(r3)
	stw r6, FRAME_CTR(r3)
	stw r7, FRAME_XER(r3)
	/* Set stack pointer for C call and create ABI back chain. */
	mr r1, r3
	stwu r1, -C_ABI_FRAME(r1)
.endm

.macro CALL_ABSOLUTE handler
	/* bl is PC-relative and will not work after vectors are copied. */
	lis r4, \handler@ha
	addi r4, r4, \handler@l
	mtctr r4
	bctrl
.endm

.macro RESTORE_VOLATILE_FRAME
	addi r1, r1, C_ABI_FRAME
	/* Restore SRR0, SRR1 */
	lwz r4, FRAME_SRR0(r1)
	lwz r5, FRAME_SRR1(r1)
	mtspr srr0, r4
	mtspr srr1, r5
	/* Restore LR, CR, CTR, XER */
	lwz r4, FRAME_LR(r1)
	lwz r5, FRAME_CR(r1)
	lwz r6, FRAME_CTR(r1)
	lwz r7, FRAME_XER(r1)
	mtlr r4
	mtcr r5
	mtctr r6
	mtxer r7
	/* Restore GPRs */
	lwz r0, FRAME_R0(r1)
	lwz r3, FRAME_R3(r1)
	lwz r4, FRAME_R4(r1)
	lwz r5, FRAME_R5(r1)
	lwz r6, FRAME_R6(r1)
	lwz r7, FRAME_R7(r1)
	lwz r8, FRAME_R8(r1)
	lwz r9, FRAME_R9(r1)
	lwz r10, FRAME_R10(r1)
	lwz r11, FRAME_R11(r1)
	lwz r12, FRAME_R12(r1)
	rfi
.endm

.macro EXCEPTION_HANDLER handler
	SAVE_VOLATILE_FRAME
	CALL_ABSOLUTE \handler
	RESTORE_VOLATILE_FRAME
.endm

.section .vectors, "ax"
.align 12

.global _vectors_start
_vectors_start:
	.space 0x500

	/*
	 * Vector 0x500 - External interrupt.
	 *
	 * On PowerPC Macs, this vector is taken when the PIC (Heathrow or
	 * OpenPIC) asserts the external interrupt line.  The keyboard (via
	 * ADB/CUDA) is one of the devices that generates this interrupt.
	 *
	 * We save full volatile state and call the C handler
	 * external_interrupt_handler(), which is the dispatch point for
	 * all hardware IRQs including the keyboard.
	 */
.global exc_external
exc_external:
	EXCEPTION_HANDLER external_interrupt_handler

	.org 0x900
	/* Vector 0x900 - Decrementer */
.global exc_decrementer
exc_decrementer:
	EXCEPTION_HANDLER decrementer_tick

	.org 0x1000
.global _vectors_end
_vectors_end:
