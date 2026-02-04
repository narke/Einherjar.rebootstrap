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
sync_cache(void *addr, size_t len)
{
	/* Flush data cache and invalidate instruction cache */
	unsigned char *p;
	for (p = addr; p < (unsigned char *)addr + len; p += 32) {
		asm volatile("dcbst 0, %0" :: "r"(p));
	}
	asm volatile("sync");
	for (p = addr; p < (unsigned char *)addr + len; p += 32) {
		asm volatile("icbi 0, %0" :: "r"(p));
	}
	asm volatile("sync; isync");
}

static void
vectors_copy_to_lowmem(void)
{
	uint32_t msr = msr_read();
	/* MSR[IP]: 0 = vectors at 0x0, 1 = vectors at 0xFFFFF000 */
	void *base = (void *)((msr & MSR_IP) ? 0xFFFFF000 : 0);
	unsigned char *dst = (unsigned char *)base;
	const unsigned char *src = (const unsigned char *)_vectors_start;
	size_t n = _vectors_end - _vectors_start;

	printf("Copying vectors to 0x%x (MSR=0x%x)\n", (unsigned)(uintptr_t)base, (unsigned)msr);

	while (n--)
		*dst++ = *src++;

	/* Sync caches so CPU fetches new code */
	sync_cache(base, _vectors_end - _vectors_start);
}

void
einherjar(void)
{
	printf("Einherjar starting...\n");
	vectors_copy_to_lowmem();
	interrupt_init();
	printf("Starting decrementer...\n");
	decrementer_start(0x00080000); /* tick more often for visibility */
	printf("Enabling interrupts...\n");
	interrupts_enable();
	printf("Hello World! Waiting for timer ticks...\n");
	for (;;) {
		uint32_t t = decrementer_ticks;
		if (t != 0) {
			printf("timer tick %d\n", (int)t);
			/* Print only on change; wait for next tick */
			while (decrementer_ticks == t)
				;
		}
	}
}
