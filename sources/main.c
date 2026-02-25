/*
 * Copyright (c) 2015 Konstantin Tcholokachvili.
 * All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include <arch/asm.h>
#include <arch/interrupts.h>
#include <arch/msr.h>
#include <lib/printf.h>
#include <lib/types.h>
#include <drivers/console.h>
#include <openfirmware/ofw.h>

void einherjar(void);

void
einherjar(void)
{
	console_init();

	/*
	 * Do NOT install our exception vectors yet: Open Firmware needs
	 * its own vectors at 0x0 (or 0xFFFFF000) to handle hardware
	 * like the ADB keyboard controller (CUDA/PMU).  Overwriting
	 * them breaks OFW's stdin read (keyboard input).
	 *
	 * Our exception vectors (exceptions.S) and decrementer will be
	 * installed later when native PIC + ADB/CUDA drivers replace
	 * OFW for I/O.
	 */

	console_puts("Einherjar kernel ready.\n");
	console_puts("> ");

	/*
	 * Main loop: poll the keyboard via Open Firmware stdin.
	 *
	 * OFW "read" on stdin is non-blocking: returns 0 bytes if no
	 * key has been pressed.  We poll in a tight loop.
	 */
	for (;;) {
		int ch = ofw_getchar();

		if (ch < 0)
			continue;

		if (ch == '\r' || ch == '\n') {
			console_puts("\n> ");
		} else if (ch == 0x7f || ch == '\b') {
			console_putchar('\b');
			console_putchar(' ');
			console_putchar('\b');
		} else {
			console_putchar((char)ch);
		}
	}
}
