/*
 * Copyright (c) 2015 Konstantin Tcholokachvili.
 * All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * Keyboard driver for PowerPC Macs.
 *
 * Input is obtained via OpenFirmware's stdin "read" service, polled
 * periodically from the decrementer interrupt (vector 0x900).
 * Characters are stored in a lock-free ring buffer (single producer
 * from interrupt context, single consumer from main context).
 *
 * The external interrupt handler (vector 0x500) is set up as the
 * proper interrupt path for future native ADB keyboard support
 * when PIC and CUDA/PMU drivers are implemented.
 */

#include <lib/printf.h>
#include <lib/typedefs.h>
#include <lib/types.h>
#include <openfirmware/ofw.h>
#include <drivers/keyboard.h>

#define KBD_BUFFER_SIZE  64

/*
 * Lock-free ring buffer for keyboard input.
 *
 * Single producer (interrupt context via keyboard_poll) writes at kbd_write_pos.
 * Single consumer (main context via keyboard_getchar) reads at kbd_read_pos.
 *
 * Buffer is empty when read_pos == write_pos.
 * Buffer is full when (write_pos + 1) % SIZE == read_pos.
 * One slot is always unused to distinguish empty from full.
 */
static volatile char     kbd_buffer[KBD_BUFFER_SIZE];
static volatile uint32_t kbd_write_pos;
static volatile uint32_t kbd_read_pos;


void
keyboard_init(void)
{
	kbd_write_pos = 0;
	kbd_read_pos  = 0;

	if (ofw_stdin == 0)
		printf("Keyboard: WARNING - no OFW stdin available\n");
	else
		printf("Keyboard: initialized (OpenFirmware stdin)\n");
}


void
keyboard_poll(void)
{
	int ch;

	/*
	 * Read all available characters from OFW stdin.
	 * ofw_getchar() is safe from interrupt context: it checks the
	 * ofw_busy flag and returns -1 if OFW is currently in use.
	 */
	while ((ch = ofw_getchar()) >= 0) {
		uint32_t next = (kbd_write_pos + 1) % KBD_BUFFER_SIZE;

		if (next == kbd_read_pos) {
			/* Buffer full, discard oldest character */
			kbd_read_pos = (kbd_read_pos + 1) % KBD_BUFFER_SIZE;
		}

		kbd_buffer[kbd_write_pos] = (char)ch;
		kbd_write_pos = next;
	}
}


int
keyboard_getchar(void)
{
	if (kbd_read_pos == kbd_write_pos)
		return -1;

	char ch = kbd_buffer[kbd_read_pos];
	kbd_read_pos = (kbd_read_pos + 1) % KBD_BUFFER_SIZE;

	return (unsigned char)ch;
}


int
keyboard_has_data(void)
{
	return kbd_read_pos != kbd_write_pos;
}
