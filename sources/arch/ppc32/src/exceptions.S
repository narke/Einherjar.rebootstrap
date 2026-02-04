/*
 * Exception vectors for PowerPC 32-bit (Book III-S).
 * This section must be copied to physical 0x0 (or 0xfffff000 if MSR[IP]=1)
 * at boot so the CPU jumps here on exception (0x500 = external, 0x900 = decrementer).
 */
#include <arch/asm/regname.h>

.section .vectors, "ax"
.align 12

.global _vectors_start
_vectors_start:
	.space 0x500
	/* Vector 0x500 - External interrupt */
.global exc_external
exc_external:
	mfspr r1, sprg0
	stwu r1, -16(r1)
	mflr r0
	stw r0, 8(r1)
	/* Could call extint_handler here; for now just return */
	lwz r0, 8(r1)
	mtlr r0
	addi r1, r1, 16
	rfi

	.org 0x900
	/* Vector 0x900 - Decrementer */
.global exc_decrementer
exc_decrementer:
	mfspr r1, sprg0
	stwu r1, -16(r1)
	mflr r0
	stw r0, 8(r1)
	bl decrementer_restart
	lwz r0, 8(r1)
	mtlr r0
	addi r1, r1, 16
	rfi

	.org 0x1000
.global _vectors_end
_vectors_end:
