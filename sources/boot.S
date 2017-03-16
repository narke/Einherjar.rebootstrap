# Copyright (c) 2015 Konstantin Tcholokachvili
# Copyright (c) 2006 Martin Decky.
# Copyright (c) 2005 Jakub Jermar
# All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

#include <arch/asm/regname.h>

.global start
.global halt

.section BOOTSTRAP, "ax"

start:
	lis r4, ofw_cif@ha
	addi r4, r4, ofw_cif@l
	stw r5, 0(r4)

	bl ofw_init
	bl einherjar

.text

.global kernel_image_start
kernel_image_start:
	# load temporal kernel stack
	lis sp, kernel_stack@ha
	addi sp, sp, kernel_stack@l

	# set kernel stack for interrupt handling
	mr r31, sp
	subis r31, r31, 0x8000
	mtsprg0 r31

	# r3 contains physical address of bootinfo_t
	addis r3, r3, 0x8000
	bl arch_pre_main

halt:
	b halt

.data
.align 12
kernel_stack_bottom:
	.space 16384
kernel_stack:
