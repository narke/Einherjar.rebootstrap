/*
 * Copyright (c) 2005 Martin Decky
 * All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include <arch/asm.h>
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

static void
ofw_call_prepare(ofw_args_t *args, const char *service, const size_t nargs,
    const size_t nret, va_list list)
{
	size_t i;

	args->service = (ofw_arg_t)service;
	args->nargs = nargs;
	args->nret = nret;

	for (i = 0; i < nargs; i++)
		args->args[i] = va_arg(list, ofw_arg_t);

	for (i = 0; i < nret; i++)
		args->args[i + nargs] = 0;
}

static void
ofw_call_collect_returns(ofw_args_t *args, const size_t nargs, const size_t nret,
    ofw_arg_t *rets)
{
	size_t i;

	if (rets == NULL)
		return;

	for (i = 1; i < nret; i++)
		rets[i - 1] = args->args[i + nargs];
}

static ipl_t
ofw_critical_enter(void)
{
	ipl_t ipl = interrupts_disable();

	ofw_busy = 1;
	return ipl;
}

static void
ofw_critical_exit(ipl_t ipl)
{
	ofw_busy = 0;
	interrupts_restore(ipl);
}

static bool
ofw_stdout_ready(void)
{
	return ofw_stdout != 0;
}

static bool
ofw_stdin_ready(void)
{
	return ofw_stdin != 0 && ofw_busy == 0;
}

static ihandle
ofw_chosen_ihandle(const char *property)
{
	ihandle handle = 0;

	if ((ofw_ret_t)ofw_get_property(ofw_chosen, property, &handle,
	    sizeof(handle)) <= 0)
		return 0;

	return handle;
}

static void
ofw_stdout_write_char(const char ch)
{
	ofw_call("write", 3, 1, NULL, ofw_stdout, &ch, 1);
}

static ofw_arg_t
ofw_stdin_read_char(char *ch)
{
	return ofw_call("read", 3, 1, NULL, ofw_stdin, ch, 1);
}

void
ofw_init(void)
{
	ofw_chosen = ofw_find_device("/chosen");
	if (ofw_chosen == (phandle)-1)
		halt();

	ofw_stdout = ofw_chosen_ihandle("stdout");
	ofw_stdin = ofw_chosen_ihandle("stdin");
}

/*
 * Invoke an Open Firmware service.
 *
 * Return value: primary OFW return in args[nargs] (also returned by this function).
 * rets: optional buffer for additional returns when nret > 1; may be NULL when
 *       the caller only needs the primary return (nret == 1).
 */
ofw_arg_t
ofw_call(const char *service, const size_t nargs, const size_t nret,
    ofw_arg_t *rets, ...)
{
	ofw_args_t args;
	va_list list;

	va_start(list, rets);
	ofw_call_prepare(&args, service, nargs, nret, list);
	va_end(list);

	(void)ofw(&args);
	ofw_call_collect_returns(&args, nargs, nret, rets);

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
	if (!ofw_stdout_ready())
		return;

	ofw_stdout_write_char(ch);
}

int
ofw_getchar(void)
{
	char ch;
	ofw_arg_t actual;

	if (!ofw_stdin_ready())
		return -1;

	actual = ofw_stdin_read_char(&ch);

	if ((int)actual > 0)
		return (unsigned char)ch;

	return -1;
}

ofw_arg_t
ofw(ofw_args_t *args)
{
	ofw_arg_t ret;
	ipl_t ipl;

	ipl = ofw_critical_enter();
	ret = ((ofw_entry_t)ofw_cif)(args);
	ofw_critical_exit(ipl);

	return ret;
}
