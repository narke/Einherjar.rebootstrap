/*
 * Copyright (c) 2006 Jakub Jermar.
 * All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be 
 * found in the LICENSE file.
 */

#pragma once

#include <lib/typedefs.h>

/** Type for function used to clear the interrupt. */
typedef void (* cir_t)(void *, inr_t);
