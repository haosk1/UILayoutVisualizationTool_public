/*
 * Copyright (C) 2014 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef FOUNDATION_CUTILS_ATOMIC_AARCH64_H
#define FOUNDATION_CUTILS_ATOMIC_AARCH64_H

#include <stdint.h>

#ifndef FOUNDATION_ATOMIC_INLINE
#define FOUNDATION_ATOMIC_INLINE inline __attribute__((always_inline))
#endif

/*
   TODOAArch64: Revisit the below functions and check for potential
   optimizations using assembly code or otherwise.
*/

extern FOUNDATION_ATOMIC_INLINE
void foundation_compiler_barrier(void)
{
    __asm__ __volatile__ ("" : : : "memory");
}

extern FOUNDATION_ATOMIC_INLINE
void foundation_memory_barrier(void)
{
    __asm__ __volatile__ ("dmb ish" : : : "memory");
}

extern FOUNDATION_ATOMIC_INLINE
int32_t foundation_atomic_acquire_load(volatile const int32_t *ptr)
{
    int32_t value = *ptr;
    foundation_memory_barrier();
    return value;
}

extern FOUNDATION_ATOMIC_INLINE
int32_t foundation_atomic_release_load(volatile const int32_t *ptr)
{
    foundation_memory_barrier();
    return *ptr;
}

extern FOUNDATION_ATOMIC_INLINE
void foundation_atomic_acquire_store(int32_t value, volatile int32_t *ptr)
{
    *ptr = value;
    foundation_memory_barrier();
}

extern FOUNDATION_ATOMIC_INLINE
void foundation_atomic_release_store(int32_t value, volatile int32_t *ptr)
{
    foundation_memory_barrier();
    *ptr = value;
}

extern FOUNDATION_ATOMIC_INLINE
int foundation_atomic_cas(int32_t old_value, int32_t new_value,
                       volatile int32_t *ptr)
{
    return __sync_val_compare_and_swap(ptr, old_value, new_value) != old_value;
}

extern FOUNDATION_ATOMIC_INLINE
int foundation_atomic_acquire_cas(int32_t old_value, int32_t new_value,
                               volatile int32_t *ptr)
{
    int status = foundation_atomic_cas(old_value, new_value, ptr);
    foundation_memory_barrier();
    return status;
}

extern FOUNDATION_ATOMIC_INLINE
int foundation_atomic_release_cas(int32_t old_value, int32_t new_value,
                               volatile int32_t *ptr)
{
    foundation_memory_barrier();
    return foundation_atomic_cas(old_value, new_value, ptr);
}

extern FOUNDATION_ATOMIC_INLINE
int32_t foundation_atomic_add(int32_t increment, volatile int32_t *ptr)
{
    int32_t prev, status;
    foundation_memory_barrier();
    do {
        prev = *ptr;
        status = foundation_atomic_cas(prev, prev + increment, ptr);
    } while (__builtin_expect(status != 0, 0));
    return prev;
}

extern FOUNDATION_ATOMIC_INLINE
int32_t foundation_atomic_inc(volatile int32_t *addr)
{
    return foundation_atomic_add(1, addr);
}

extern FOUNDATION_ATOMIC_INLINE
int32_t foundation_atomic_dec(volatile int32_t *addr)
{
    return foundation_atomic_add(-1, addr);
}

extern FOUNDATION_ATOMIC_INLINE
int32_t foundation_atomic_and(int32_t value, volatile int32_t *ptr)
{
    int32_t prev, status;
    foundation_memory_barrier();
    do {
        prev = *ptr;
        status = foundation_atomic_cas(prev, prev & value, ptr);
    } while (__builtin_expect(status != 0, 0));
    return prev;
}

extern FOUNDATION_ATOMIC_INLINE
int32_t foundation_atomic_or(int32_t value, volatile int32_t *ptr)
{
    int32_t prev, status;
    foundation_memory_barrier();
    do {
        prev = *ptr;
        status = foundation_atomic_cas(prev, prev | value, ptr);
    } while (__builtin_expect(status != 0, 0));
    return prev;
}

#endif /* FOUNDATION_CUTILS_ATOMIC_AARCH64_H */