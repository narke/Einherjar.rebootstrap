/*
 * Copyright (c) 2005 Martin Decky
 * All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include <lib/printf.h>
#include <lib/typedefs.h>
#include <lib/halt.h>
#include "ofw.h"

#define RED(i)    (((i) >> 5) & ((1 << 3) - 1))
#define GREEN(i)  (((i) >> 3) & ((1 << 2) - 1))
#define BLUE(i)   ((i) & ((1 << 3) - 1))
#define CLIP(i)   ((i) <= 255 ? (i) : 255)

uintptr_t ofw_cif;
typedef int (*ofw_entry_t)(ofw_args_t *args);

phandle ofw_chosen;
ihandle ofw_stdout;
phandle ofw_root;
ihandle ofw_mmu;
ihandle ofw_memory_prop;
phandle ofw_memory;


void 
ofw_init(void)
{
	ofw_chosen = ofw_find_device("/chosen");
	if (ofw_chosen == (phandle) -1)
		halt();
	
	if ((ofw_ret_t) ofw_get_property(ofw_chosen, "stdout", &ofw_stdout,
	    sizeof(ofw_stdout)) <= 0)
		ofw_stdout = 0;
	
	ofw_root = ofw_find_device("/");
	if (ofw_root == (phandle) -1) {
		printf("Error: Unable to find / device, halting.\n");
		halt();
	}
	
	if ((ofw_ret_t) ofw_get_property(ofw_chosen, "mmu", &ofw_mmu,
	    sizeof(ofw_mmu)) <= 0) {
		printf("Error: Unable to get mmu property, halting.\n");
		halt();
	}
	if ((ofw_ret_t) ofw_get_property(ofw_chosen, "memory", &ofw_memory_prop,
	    sizeof(ofw_memory_prop)) <= 0) {
		printf("Error: Unable to get memory property, halting.\n");
		halt();
	}
	
	ofw_memory = ofw_find_device("/memory");
	if (ofw_memory == (phandle) -1) {
		printf("Error: Unable to find /memory device, halting.\n");
		halt();
	}
}


/*
 * Perform a call to OpenFirmware client interface.
 *
 * Args:
 * service String identifying the service requested.
 * nargs   Number of input arguments.
 * nret    Number of output arguments. This includes the return value.
 * rets    Buffer for output arguments or NULL. The buffer must accommodate 
 *             nret - 1 items.
 *
 * Return:
 * Return value returned by the client interface.
 *
 */
ofw_arg_t 
ofw_call(const char *service, const size_t nargs,
		const size_t nret, ofw_arg_t *rets, ...)
{
	ofw_args_t args;
	args.service = (ofw_arg_t) service;
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
	
	(void) ofw(&args);
	
	for (i = 1; i < nret; i++)
		rets[i - 1] = args.args[i + nargs];
	
	return args.args[nargs];
}


phandle 
ofw_find_device(const char *name)
{
	return (phandle) ofw_call("finddevice", 1, 1, NULL, name);
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


ofw_arg_t 
ofw(ofw_args_t *args)
{
	return ((ofw_entry_t) ofw_cif)(args);
}

