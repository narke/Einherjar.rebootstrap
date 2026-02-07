/*
 * Copyright (c) 2015 Konstantin Tcholokachvili.
 * All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * Keyboard driver for PowerPC Macs.
 *
 * On PowerPC Macs, the keyboard is connected via ADB (Apple Desktop Bus)
 * managed by the CUDA or PMU chip. Key presses generate external interrupts
 * (vector 0x500) routed through the PIC (Heathrow or OpenPIC).
 *
 * This driver currently uses OpenFirmware's stdin for keyboard input,
 * polled from the decrementer interrupt handler (vector 0x900).
 * The external interrupt handler (vector 0x500) is wired up as the
 * correct infrastructure for future native ADB/CUDA keyboard support.
 */

#pragma once

/**
 * Initialize the keyboard driver.
 * Sets up the ring buffer and prepares for input via OFW stdin.
 */
void keyboard_init(void);

/**
 * Poll the keyboard for new input.
 * Reads available characters from OFW stdin and stores them in the
 * internal ring buffer. Called from the decrementer interrupt handler.
 *
 * Safe to call from interrupt context: if OFW is busy (e.g., printf
 * is in progress), the poll is silently skipped.
 */
void keyboard_poll(void);

/**
 * Read one character from the keyboard buffer.
 * Returns the next character (0-255) from the ring buffer, or -1 if
 * the buffer is empty.
 */
int keyboard_getchar(void);

/**
 * Check whether keyboard input is available.
 * Returns non-zero if there is at least one character in the buffer.
 */
int keyboard_has_data(void);
