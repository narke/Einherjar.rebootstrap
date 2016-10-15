/*
 * Copyright (c) 2006 Ondrej Palkovsky.
 * All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <lib/typedefs.h>
#include <arch/irq.h>

#define PIC_PENDING_LOW   8
#define PIC_PENDING_HIGH  4
#define PIC_MASK_LOW      9
#define PIC_MASK_HIGH     5
#define PIC_ACK_LOW       10
#define PIC_ACK_HIGH      6

void pic_init(uintptr_t, size_t, cir_t *, void **);
void pic_enable_interrupt(inr_t);
void pic_disable_interrupt(inr_t);
void pic_ack_interrupt(void *, inr_t);
uint8_t pic_get_pending(void);
