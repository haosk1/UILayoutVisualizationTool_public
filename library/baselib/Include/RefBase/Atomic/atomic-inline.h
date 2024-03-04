/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FOUNDATION_CUTILS_ATOMIC_INLINE_H
#define FOUNDATION_CUTILS_ATOMIC_INLINE_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Inline declarations and macros for some special-purpose atomic
 * operations.  These are intended for rare circumstances where a
 * memory barrier needs to be issued inline rather than as a function
 * call.
 *
 * Most code should not use these.
 *
 * Anything that does include this file must set FOUNDATION_SMP to either
 * 0 or 1, indicating compilation for UP or SMP, respectively.
 *
 * Macros defined in this header:
 *
 * void FOUNDATION_MEMBAR_FULL(void)
 *   Full memory barrier.  Provides a compiler reordering barrier, and
 *   on SMP systems emits an appropriate instruction.
 */

#if defined(__aarch64__)
#include "atomic-arm64.h"
#elif defined(__arm__)
#include "atomic-arm.h"
#elif defined(__i386__)
#include "atomic-x86.h"
#elif defined(__x86_64__)
#include "atomic-x86_64.h"
#elif defined(__mips64)
#include "atomic-mips64.h"
#elif defined(__mips__)
#include "atomic-mips.h"
#else
#ifdef _WIN32
#include "atomic-win32.h"
#else
#error atomic operations are unsupported
#endif
#endif

#if FOUNDATION_SMP == 0
#define FOUNDATION_MEMBAR_FULL foundation_compiler_barrier
#else
#define FOUNDATION_MEMBAR_FULL foundation_memory_barrier
#endif

#ifdef __cplusplus
}
#endif

#endif /* FOUNDATION_CUTILS_ATOMIC_INLINE_H */
