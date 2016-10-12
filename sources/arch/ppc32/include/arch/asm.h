/*
 * Copyright (c) 2005 Martin Decky.
 * All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be 
 * found in the LICENSE file.
 */

#pragma once

#include <lib/typedefs.h>
#include <arch/msr.h>

static inline uint32_t
msr_read(void)
{
	uint32_t msr;

	asm volatile (
		"mfmsr %[msr]\n"
		: [msr] "=r" (msr)
	);

	return msr;
}

static inline void
msr_write(uint32_t msr)
{
	asm volatile (
		"mtmsr %[msr]\n"
		"isync\n"
		:: [msr] "r" (msr)
	);
}


/** Enable interrupts.
 *
 * Enable interrupts and return previous
 * value of EE.
 *
 * @return Old interrupt priority level.
 *
 */
static inline ipl_t
interrupts_enable(void)
{
	ipl_t ipl = msr_read();
	msr_write(ipl | MSR_EE);
	return ipl;
}

/** Disable interrupts.
 *
 * Disable interrupts and return previous
 * value of EE.
 *
 * @return Old interrupt priority level.
 *
 */
static inline ipl_t
interrupts_disable(void)
{
	ipl_t ipl = msr_read();
	msr_write(ipl & (~MSR_EE));
	return ipl;
}

/** Restore interrupt priority level.
 *
 * Restore EE.
 *
 * @param ipl Saved interrupt priority level.
 *
 */
static inline void
interrupts_restore(ipl_t ipl)
{
	msr_write((msr_read() & (~MSR_EE)) | (ipl & MSR_EE));
}

/** Return interrupt priority level.
 *
 * Return EE.
 *
 * @return Current interrupt priority level.
 *
 */
static inline ipl_t
interrupts_read(void)
{
	return msr_read();
}

/** Check whether interrupts are disabled.
 *
 * @return True if interrupts are disabled.
 *
 */
static inline bool
interrupts_disabled(void)
{
	return ((msr_read() & MSR_EE) == 0);
}
