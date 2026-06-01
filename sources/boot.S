# Copyright (c) 2015 Konstantin Tcholokachvili
# Copyright (c) 2006 Martin Decky.
# Copyright (c) 2005 Jakub Jermar
# All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

#include <arch/asm/regname.h>

#define KERNEL_STACK_SIZE  16384

.global start
.global halt

.section BOOTSTRAP, "ax"

start:
	lis r4, ofw_cif@ha
	addi r4, r4, ofw_cif@l
	stw r5, 0(r4)

	bl ofw_init

	# Set up interrupt stack in SPRG0 (use kernel_stack)
	lis r4, kernel_stack@ha
	addi r4, r4, kernel_stack@l
	mtsprg0 r4

	bl einherjar

.text

halt:
	b halt

.data
.align 12
kernel_stack_bottom:
	.space KERNEL_STACK_SIZE
kernel_stack:
