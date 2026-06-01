/*
 * Copyright (c) 2006 Martin Decky
 * All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#pragma once

/*
 * Convenience typedefs and constants built on common.h + types.h.
 * Include this header for bool, NULL, inr_t, and machine types.
 */
#include "common.h"
#include "types.h"

#define NULL  ((void *) 0)

#define false  0
#define true   1

typedef uint8_t bool;
typedef int32_t inr_t;
