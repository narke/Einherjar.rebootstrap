/*
 * Copyright (c) 2005 Martin Decky
 * All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <lib/typedefs.h>
#include <lib/stdarg.h>

#define TASKMAP_MAX_RECORDS 			32
#define MEMMAP_MAX_RECORDS  			32
#define BOOTINFO_TASK_NAME_BUFFER_LENGTH	32
#define MAX_OFW_ARGS				12

#define OFW_TREE_PATH_MAX_LENGTH		256
#define OFW_TREE_PROPERTY_MAX_NAME_LENGTH	32
#define OFW_TREE_PROPERTY_MAX_VALUE_LENGTH	64

typedef sysarg_t ofw_arg_t;
typedef native_t ofw_ret_t;
typedef uint32_t ofw_prop_t;
typedef uint32_t ihandle;
typedef uint32_t phandle;

/*
 * OpenFirmware command structure
 */
typedef struct
{
	ofw_arg_t service;             /* Command name. */
	ofw_arg_t nargs;               /* Number of in arguments. */
	ofw_arg_t nret;                /* Number of out arguments. */
	ofw_arg_t args[MAX_OFW_ARGS];  /* List of arguments. */
} ofw_args_t;

typedef struct
{
	void *addr;
	size_t size;
	char name[BOOTINFO_TASK_NAME_BUFFER_LENGTH];
} utask_t;

typedef struct
{
	size_t count;
	utask_t tasks[TASKMAP_MAX_RECORDS];
} taskmap_t;

typedef struct
{
	void *start;
	size_t size;
} memzone_t;

typedef struct
{
	uint64_t total;
	size_t count;
	memzone_t zones[MEMMAP_MAX_RECORDS];
} memmap_t;

typedef struct
{
	uintptr_t base;
	size_t size;
} ballocs_t;

typedef struct
{
	char name[OFW_TREE_PROPERTY_MAX_NAME_LENGTH];
	size_t size;
	void *value;
} ofw_tree_property_t;

typedef struct
{
	struct ofw_tree_node_t *parent;
	struct ofw_tree_node_t *peer;
	struct ofw_tree_node_t *child;

	phandle node_handle;
	char da_name;
	size_t properties;
	ofw_tree_property_t *property;
	void *device;
} ofw_tree_node_t;

typedef struct
{
	memmap_t memmap;
	taskmap_t taskmap;
	ballocs_t ballocs;
	ofw_tree_node_t *ofw_root;
} bootinfo_t;

extern phandle ofw_chosen;
extern ihandle ofw_stdout;

void ofw_init(void);

void ofw_putchar(const char);

ofw_arg_t ofw_get_property(const phandle, const char *, void *, const size_t);

phandle ofw_find_device(const char *);

ofw_arg_t ofw(ofw_args_t *);

ofw_arg_t ofw_call(const char *, const size_t, const size_t, ofw_arg_t *, ...);
