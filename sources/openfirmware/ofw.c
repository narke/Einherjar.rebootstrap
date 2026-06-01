/*
 * Copyright (c) 2005 Martin Decky
 * All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include <arch/asm.h>
#include <lib/printf.h>
#include <lib/typedefs.h>
#include <lib/halt.h>
#include "ofw.h"

typedef int (*ofw_entry_t)(ofw_args_t *args);

uintptr_t ofw_cif;
phandle ofw_chosen;
ihandle ofw_stdout;
ihandle ofw_stdin;

/** Non-zero when OFW client interface is being called (prevents reentrant calls). */
volatile uint32_t ofw_busy;

void
ofw_init(void)
{
	ofw_chosen = ofw_find_device("/chosen");
	if (ofw_chosen == (phandle)-1)
		halt();

	if ((ofw_ret_t)ofw_get_property(ofw_chosen, "stdout", &ofw_stdout,
	    sizeof(ofw_stdout)) <= 0)
		ofw_stdout = 0;

	if ((ofw_ret_t)ofw_get_property(ofw_chosen, "stdin", &ofw_stdin,
	    sizeof(ofw_stdin)) <= 0)
		ofw_stdin = 0;
}

ofw_arg_t
ofw_call(const char *service, const size_t nargs, const size_t nret,
    ofw_arg_t *rets, ...)
{
	ofw_args_t args;
	args.service = (ofw_arg_t)service;
	args.nargs = nargs;
	args.nret = nret;

	va_list list;
	va_start(list, rets);

	size_t i;
	for (i = 0; i < nargs; i++)
		args.args[i] = va_arg(list, ofw_arg_t);

	va_end(list);

	for (i = 0; i < nret; i++)
		args.args[i + nargs] = 0;

	(void)ofw(&args);

	for (i = 1; i < nret; i++)
		rets[i - 1] = args.args[i + nargs];

	return args.args[nargs];
}

phandle
ofw_find_device(const char *name)
{
	return (phandle)ofw_call("finddevice", 1, 1, NULL, name);
}

ofw_arg_t
ofw_get_property(const phandle device, const char *name, void *buf,
    const size_t buflen)
{
	return ofw_call("getprop", 4, 1, NULL, device, name, buf, buflen);
}

void
ofw_putchar(const char ch)
{
	if (ofw_stdout == 0)
		return;

	ofw_call("write", 3, 1, NULL, ofw_stdout, &ch, 1);
}

int
ofw_getchar(void)
{
	char ch;

	if (ofw_stdin == 0 || ofw_busy)
		return -1;

	ofw_arg_t actual = ofw_call("read", 3, 1, NULL, ofw_stdin, &ch, 1);

	if ((int)actual > 0)
		return (unsigned char)ch;

	return -1;
}

ofw_arg_t
ofw(ofw_args_t *args)
{
	ofw_arg_t ret;

	ipl_t ipl = interrupts_disable();
	ofw_busy = 1;
	ret = ((ofw_entry_t)ofw_cif)(args);
	ofw_busy = 0;
	interrupts_restore(ipl);

	return ret;
}
