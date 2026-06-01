/*
 * Copyright (c) 2006 Martin Decky
 * All rights reserved.
 *
 * Decrementer and external IRQ handlers for future native vector install.
 * Not linked in the current OFW-based build (see Makefile).
 * Do not call Open Firmware from these handlers until OFW I/O is replaced.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * - The name of the author may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <arch/interrupts.h>

static uint32_t decrementer_value;

/** Ticks since boot; updated by decrementer handler. */
volatile uint32_t decrementer_ticks;

void decrementer_start(uint32_t value)
{
	decrementer_value = value;
	decrementer_restart();
}

void decrementer_restart(void)
{
	asm volatile (
		"mtdec %[dec]\n"
		:: [dec] "r" (decrementer_value)
	);
}

/**
 * Called from the decrementer exception handler (ASM) on each timer tick.
 * Only increments the tick counter and restarts the decrementer.
 *
 * NOTE: Do NOT call Open Firmware from here. OFW is not reentrant and
 * does not work reliably from exception context (MSR[EE]=0).
 */
void decrementer_tick(void)
{
	decrementer_ticks++;
	decrementer_restart();
}

/** Stub for external interrupt vector 0x500. */
void external_interrupt_handler(void)
{
}

void interrupt_init(void)
{
}

