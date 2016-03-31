/*
 * Copyright (c) 2005 Martin Decky
 * All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef _OFW_H_
#define _OFW_H_

#include <lib/typedefs.h>
#include <lib/stdarg.h>

#define MEMMAP_MAX_RECORDS  32
#define MAX_OFW_ARGS        12

#define OFW_TREE_PATH_MAX_LEN           256
#define OFW_TREE_PROPERTY_MAX_NAMELEN   32
#define OFW_TREE_PROPERTY_MAX_VALUELEN  64

typedef sysarg_t ofw_arg_t;
typedef native_t ofw_ret_t;
typedef uint32_t ofw_prop_t;
typedef uint32_t ihandle;
typedef uint32_t phandle;

/*
 * OpenFirmware command structure
 */
typedef struct {
	ofw_arg_t service;             /* Command name. */
	ofw_arg_t nargs;               /* Number of in arguments. */
	ofw_arg_t nret;                /* Number of out arguments. */
	ofw_arg_t args[MAX_OFW_ARGS];  /* List of arguments. */
} ofw_args_t;


extern uintptr_t ofw_cif;

extern phandle ofw_chosen;
extern ihandle ofw_stdout;

extern void ofw_init(void);

extern void ofw_putchar(const char);

extern ofw_arg_t ofw_get_property(const phandle, const char *, void *,
    const size_t);

extern phandle ofw_find_device(const char *);


extern ofw_arg_t ofw(ofw_args_t *);
extern ofw_arg_t ofw_call(const char *, const size_t, const size_t, ofw_arg_t *,
    ...);




#endif
