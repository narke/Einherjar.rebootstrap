/*
 * Copyright (c) 2005 Martin Decky
 * All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <lib/typedefs.h>
#include <lib/stdarg.h>

#define MAX_OFW_ARGS  12

typedef sysarg_t ofw_arg_t;
typedef native_t ofw_ret_t;
typedef uint32_t ihandle;
typedef uint32_t phandle;

typedef struct {
	ofw_arg_t service;
	ofw_arg_t nargs;
	ofw_arg_t nret;
	ofw_arg_t args[MAX_OFW_ARGS];
} ofw_args_t;

extern uintptr_t ofw_cif;
extern phandle ofw_chosen;
extern ihandle ofw_stdout;
extern ihandle ofw_stdin;

/** Non-zero when OFW client interface is being called (prevents reentrant calls). */
extern volatile uint32_t ofw_busy;

void ofw_init(void);
void ofw_putchar(const char ch);
int ofw_getchar(void);

ofw_arg_t ofw_get_property(const phandle, const char *, void *, const size_t);
phandle ofw_find_device(const char *);
ofw_arg_t ofw(ofw_args_t *);
ofw_arg_t ofw_call(const char *, const size_t, const size_t, ofw_arg_t *, ...);
