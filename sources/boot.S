# Copyright (c) 2015 Konstantin Tcholokachvili
# Copyright (c) 2006 Martin Decky. 
# All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

#include <lib/regname.h>

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

halt:
	b halt
