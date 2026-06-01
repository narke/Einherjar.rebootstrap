/*
 * Copyright (c) 2015 Konstantin Tcholokachvili.
 * All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * Keyboard driver for PowerPC Macs (future path, not linked in current build).
 *
 * Intended design: poll OFW stdin from the decrementer handler (vector 0x900)
 * into a ring buffer; external IRQ (vector 0x500) is reserved for native ADB/PIC.
 *
 * Active boot today uses ofw_getchar() directly in main.c. Do not call OFW from
 * IRQ until vectors replace OFW and reentrancy is fully understood.
 */

#include <lib/printf.h>
#include <lib/typedefs.h>
#include <openfirmware/ofw.h>
#include <drivers/keyboard.h>

#define KBD_BUFFER_SIZE  64

/*
 * Lock-free ring buffer: single producer (keyboard_poll), single consumer
 * (keyboard_getchar). One slot is always unused to distinguish empty from full.
 */
static volatile char     kbd_buffer[KBD_BUFFER_SIZE];
static volatile uint32_t kbd_write_pos;
static volatile uint32_t kbd_read_pos;

static uint32_t
kbd_next_pos(uint32_t pos)
{
	return (pos + 1) % KBD_BUFFER_SIZE;
}

static int
kbd_buffer_empty(void)
{
	return kbd_read_pos == kbd_write_pos;
}

static int
kbd_buffer_full(void)
{
	return kbd_next_pos(kbd_write_pos) == kbd_read_pos;
}

static void
kbd_buffer_push(char ch)
{
	if (kbd_buffer_full())
		kbd_read_pos = kbd_next_pos(kbd_read_pos);

	kbd_buffer[kbd_write_pos] = ch;
	kbd_write_pos = kbd_next_pos(kbd_write_pos);
}

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
	 * Experimental/future: OFW from IRQ is guarded by ofw_busy only.
	 * interrupts.c explicitly warns against calling OFW from decrementer_tick.
	 */
	while ((ch = ofw_getchar()) >= 0)
		kbd_buffer_push((char)ch);
}

int
keyboard_getchar(void)
{
	char ch;

	if (kbd_buffer_empty())
		return -1;

	ch = kbd_buffer[kbd_read_pos];
	kbd_read_pos = kbd_next_pos(kbd_read_pos);

	return (unsigned char)ch;
}

int
keyboard_has_data(void)
{
	return !kbd_buffer_empty();
}
