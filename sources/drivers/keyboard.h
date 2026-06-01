/*
 * Copyright (c) 2015 Konstantin Tcholokachvili.
 * All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * Keyboard driver for PowerPC Macs (future path, not linked in current build).
 *
 * On PowerPC Macs, the keyboard is connected via ADB (CUDA/PMU) and eventually
 * external IRQ (vector 0x500) through the PIC. This header describes the
 * planned ring-buffer driver polled from the decrementer (vector 0x900).
 *
 * Active input today: main.c calls ofw_getchar() in a tight loop.
 */

#pragma once

void keyboard_init(void);

/**
 * Poll OFW stdin into the internal ring buffer (intended for decrementer IRQ).
 * Not used while the kernel keeps OFW vectors and polls from main.
 */
void keyboard_poll(void);

int keyboard_getchar(void);
int keyboard_has_data(void);
