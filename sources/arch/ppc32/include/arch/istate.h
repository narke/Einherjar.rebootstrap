#ifndef _ISTATE_H_
#define _ISTATE_H_

#include <lib/typedefs.h>

#define ISTATE_OFFSET_SP_FRAME 0
#define ISTATE_SIZE_SP_FRAME 4
#define ISTATE_OFFSET_LR_FRAME 4
#define ISTATE_SIZE_LR_FRAME 4
#define ISTATE_OFFSET_R0 8
#define ISTATE_SIZE_R0 4
#define ISTATE_OFFSET_R2 12
#define ISTATE_SIZE_R2 4
#define ISTATE_OFFSET_R3 16
#define ISTATE_SIZE_R3 4
#define ISTATE_OFFSET_R4 20
#define ISTATE_SIZE_R4 4
#define ISTATE_OFFSET_R5 24
#define ISTATE_SIZE_R5 4
#define ISTATE_OFFSET_R6 28
#define ISTATE_SIZE_R6 4
#define ISTATE_OFFSET_R7 32
#define ISTATE_SIZE_R7 4
#define ISTATE_OFFSET_R8 36
#define ISTATE_SIZE_R8 4
#define ISTATE_OFFSET_R9 40
#define ISTATE_SIZE_R9 4
#define ISTATE_OFFSET_R10 44
#define ISTATE_SIZE_R10 4
#define ISTATE_OFFSET_R11 48
#define ISTATE_SIZE_R11 4
#define ISTATE_OFFSET_R13 52
#define ISTATE_SIZE_R13 4
#define ISTATE_OFFSET_R14 56
#define ISTATE_SIZE_R14 4
#define ISTATE_OFFSET_R15 60
#define ISTATE_SIZE_R15 4
#define ISTATE_OFFSET_R16 64
#define ISTATE_SIZE_R16 4
#define ISTATE_OFFSET_R17 68
#define ISTATE_SIZE_R17 4
#define ISTATE_OFFSET_R18 72
#define ISTATE_SIZE_R18 4
#define ISTATE_OFFSET_R19 76
#define ISTATE_SIZE_R19 4
#define ISTATE_OFFSET_R20 80
#define ISTATE_SIZE_R20 4
#define ISTATE_OFFSET_R21 84
#define ISTATE_SIZE_R21 4
#define ISTATE_OFFSET_R22 88
#define ISTATE_SIZE_R22 4
#define ISTATE_OFFSET_R23 92
#define ISTATE_SIZE_R23 4
#define ISTATE_OFFSET_R24 96
#define ISTATE_SIZE_R24 4
#define ISTATE_OFFSET_R25 100
#define ISTATE_SIZE_R25 4
#define ISTATE_OFFSET_R26 104
#define ISTATE_SIZE_R26 4
#define ISTATE_OFFSET_R27 108
#define ISTATE_SIZE_R27 4
#define ISTATE_OFFSET_R28 112
#define ISTATE_SIZE_R28 4
#define ISTATE_OFFSET_R29 116
#define ISTATE_SIZE_R29 4
#define ISTATE_OFFSET_R30 120
#define ISTATE_SIZE_R30 4
#define ISTATE_OFFSET_R31 124
#define ISTATE_SIZE_R31 4
#define ISTATE_OFFSET_CR 128
#define ISTATE_SIZE_CR 4
#define ISTATE_OFFSET_PC 132
#define ISTATE_SIZE_PC 4
#define ISTATE_OFFSET_SRR1 136
#define ISTATE_SIZE_SRR1 4
#define ISTATE_OFFSET_LR 140
#define ISTATE_SIZE_LR 4
#define ISTATE_OFFSET_CTR 144
#define ISTATE_SIZE_CTR 4
#define ISTATE_OFFSET_XER 148
#define ISTATE_SIZE_XER 4
#define ISTATE_OFFSET_DAR 152
#define ISTATE_SIZE_DAR 4
#define ISTATE_OFFSET_R12 156
#define ISTATE_SIZE_R12 4
#define ISTATE_OFFSET_SP 160
#define ISTATE_SIZE_SP 4
#define ISTATE_SIZE 164

typedef struct istate {
	uint32_t sp_frame;
	uint32_t lr_frame;
	uint32_t r0;
	uint32_t r2;
	uint32_t r3;
	uint32_t r4;
	uint32_t r5;
	uint32_t r6;
	uint32_t r7;
	uint32_t r8;
	uint32_t r9;
	uint32_t r10;
	uint32_t r11;
	uint32_t r13;
	uint32_t r14;
	uint32_t r15;
	uint32_t r16;
	uint32_t r17;
	uint32_t r18;
	uint32_t r19;
	uint32_t r20;
	uint32_t r21;
	uint32_t r22;
	uint32_t r23;
	uint32_t r24;
	uint32_t r25;
	uint32_t r26;
	uint32_t r27;
	uint32_t r28;
	uint32_t r29;
	uint32_t r30;
	uint32_t r31;
	uint32_t cr;
	uint32_t pc;
	uint32_t srr1;
	uint32_t lr;
	uint32_t ctr;
	uint32_t xer;
	uint32_t dar;
	uint32_t r12;
	uint32_t sp;
} istate_t;

#endif
