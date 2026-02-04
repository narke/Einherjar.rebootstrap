/*
 * Copyright (c) 2015 Konstantin Tcholokachvili.
 * All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include <arch/asm.h>
#include <arch/interrupts.h>
#include <lib/printf.h>
#include <lib/types.h>

extern char _vectors_start[];
extern char _vectors_end[];

void einherjar(void);
void debug(void);

void
debug(void)
{
	printf("debug\n");
}

static void
vectors_copy_to_lowmem(void)
{
	unsigned char *dst = (unsigned char *)0;
	const unsigned char *src = (const unsigned char *)_vectors_start;
	size_t n = _vectors_end - _vectors_start;
	while (n--)
		*dst++ = *src++;
}

void
einherjar(void)
{
	vectors_copy_to_lowmem();
	interrupt_init();
	decrementer_start(0x00800000); /* timer tick ~32M cycles */
	interrupts_enable();
	printf("Hello World!\n");
}
