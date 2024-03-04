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

#ifndef FOUNDATION_CUTILS_ATOMIC_WIN32_H
#define FOUNDATION_CUTILS_ATOMIC_WIN32_H


#include <Windows.h>
#include <stdint.h>
#include <intrin.h>

#define InterlockedOr _InterlockedOr
#define InterlockedAnd _InterlockedAnd

#pragma intrinsic(_InterlockedOr)
#pragma intrinsic(_InterlockedAnd) 
#pragma intrinsic(_ReadWriteBarrier)



#ifndef FOUNDATION_ATOMIC_INLINE
#define FOUNDATION_ATOMIC_INLINE __forceinline
#endif

extern FOUNDATION_ATOMIC_INLINE
void foundation_compiler_barrier(void)
{
    // https://msdn.microsoft.com/en-us/library/f20w0x5e(v=vs.80).aspx
    _ReadWriteBarrier();
}

extern FOUNDATION_ATOMIC_INLINE
void foundation_memory_barrier(void)
{
    // https://msdn.microsoft.com/en-us/library/windows/desktop/ms684208(v=vs.85).aspx
    MemoryBarrier();
}

extern FOUNDATION_ATOMIC_INLINE
int32_t foundation_atomic_acquire_load(volatile const int32_t *ptr)
{
    int32_t value = *ptr;
    foundation_compiler_barrier();
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
    foundation_compiler_barrier();
    *ptr = value;
}

extern FOUNDATION_ATOMIC_INLINE
int foundation_atomic_cas(int32_t old_value, int32_t new_value,
                       volatile int32_t *ptr)
{
    // https://msdn.microsoft.com/en-us/library/windows/desktop/ms683560(v=vs.85).aspx
    int32_t prev;
    prev = (int32_t)InterlockedCompareExchange((volatile long *)ptr, new_value, old_value);
    return prev != old_value;
}

extern FOUNDATION_ATOMIC_INLINE
int foundation_atomic_acquire_cas(int32_t old_value, int32_t new_value,
                               volatile int32_t *ptr)
{
    /* Loads are not reordered with other loads. */
    return foundation_atomic_cas(old_value, new_value, ptr);
}

extern FOUNDATION_ATOMIC_INLINE
int foundation_atomic_release_cas(int32_t old_value, int32_t new_value,
                               volatile int32_t *ptr)
{
    /* Stores are not reordered with other stores. */
    return foundation_atomic_cas(old_value, new_value, ptr);
}

extern FOUNDATION_ATOMIC_INLINE
int32_t foundation_atomic_add(int32_t increment, volatile int32_t *ptr)
{
    // https://msdn.microsoft.com/en-us/library/windows/desktop/ms683597(v=vs.85).aspx
    int32_t prev;
    prev = (int32_t)InterlockedExchangeAdd((volatile long *)ptr, (long)increment);
    /* increment now holds the old value of *ptr */
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
    // https://msdn.microsoft.com/en-us/library/windows/desktop/ms683516(v=vs.85).aspx
    int32_t prev;
    prev = (int32_t)InterlockedAnd((volatile long *)ptr, (long)value);
    return prev;
}

extern FOUNDATION_ATOMIC_INLINE
int32_t foundation_atomic_or(int32_t value, volatile int32_t *ptr)
{
    // https://msdn.microsoft.com/en-us/library/windows/desktop/ms683626(v=vs.85).aspx
    int32_t prev;
    prev = (int32_t)InterlockedOr((volatile long *)ptr, (long)value);
    return prev;
}

#endif /* FOUNDATION_CUTILS_ATOMIC_WIN32_H */
