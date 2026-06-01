/*
 * Copyright (c) 2006 Ondrej Palkovsky.
 * All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#pragma once

/*
 * Programmable interrupt controller API for future Mac PIC/OpenPIC support.
 * No pic.c implementation is linked in the current build.
 */
#include <lib/typedefs.h>
#include <arch/irq.h>

#define PIC_PENDING_LOW   8
#define PIC_PENDING_HIGH  4
#define PIC_MASK_LOW      9
#define PIC_MASK_HIGH     5
#define PIC_ACK_LOW       10
#define PIC_ACK_HIGH      6

void pic_init(uintptr_t base, size_t size, cir_t *clear_funcs, void **device_data);
void pic_enable_interrupt(inr_t inr);
void pic_disable_interrupt(inr_t inr);
void pic_ack_interrupt(void *device_data, inr_t inr);
uint8_t pic_get_pending(void);
