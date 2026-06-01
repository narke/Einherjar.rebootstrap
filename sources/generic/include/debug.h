/*
 * Copyright (c) 2005 Martin Decky.
 * All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#pragma once

/*
 * ASSERT is intentionally a no-op in the current freestanding kernel.
 * Enable a real implementation when a debug output path exists.
 */
#define ASSERT(expr)
