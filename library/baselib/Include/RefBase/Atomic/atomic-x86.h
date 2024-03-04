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

#ifndef FOUNDATION_CUTILS_ATOMIC_X86_H
#define FOUNDATION_CUTILS_ATOMIC_X86_H

#include <stdint.h>

#ifndef FOUNDATION_ATOMIC_INLINE
#define FOUNDATION_ATOMIC_INLINE inline __attribute__((always_inline))
#endif

extern FOUNDATION_ATOMIC_INLINE void foundation_compiler_barrier(void)
{
    __asm__ __volatile__ ("" : : : "memory");
}

#if FOUNDATION_SMP == 0
extern FOUNDATION_ATOMIC_INLINE void foundation_memory_barrier(void)
{
    foundation_compiler_barrier();
}
#else
extern FOUNDATION_ATOMIC_INLINE void foundation_memory_barrier(void)
{
    __asm__ __volatile__ ("mfence" : : : "memory");
}
#endif

extern FOUNDATION_ATOMIC_INLINE int32_t
foundation_atomic_acquire_load(volatile const int32_t *ptr)
{
    int32_t value = *ptr;
    foundation_compiler_barrier();
    return value;
}

extern FOUNDATION_ATOMIC_INLINE int32_t
foundation_atomic_release_load(volatile const int32_t *ptr)
{
    foundation_memory_barrier();
    return *ptr;
}

extern FOUNDATION_ATOMIC_INLINE void
foundation_atomic_acquire_store(int32_t value, volatile int32_t *ptr)
{
    *ptr = value;
    foundation_memory_barrier();
}

extern FOUNDATION_ATOMIC_INLINE void
foundation_atomic_release_store(int32_t value, volatile int32_t *ptr)
{
    foundation_compiler_barrier();
    *ptr = value;
}

extern FOUNDATION_ATOMIC_INLINE int
foundation_atomic_cas(int32_t old_value, int32_t new_value, volatile int32_t *ptr)
{
    int32_t prev;
    __asm__ __volatile__ ("lock; cmpxchgl %1, %2"
                          : "=a" (prev)
                          : "q" (new_value), "m" (*ptr), "0" (old_value)
                          : "memory");
    return prev != old_value;
}

extern FOUNDATION_ATOMIC_INLINE int
foundation_atomic_acquire_cas(int32_t old_value,
                           int32_t new_value,
                           volatile int32_t *ptr)
{
    /* Loads are not reordered with other loads. */
    return foundation_atomic_cas(old_value, new_value, ptr);
}

extern FOUNDATION_ATOMIC_INLINE int
foundation_atomic_release_cas(int32_t old_value,
                           int32_t new_value,
                           volatile int32_t *ptr)
{
    /* Stores are not reordered with other stores. */
    return foundation_atomic_cas(old_value, new_value, ptr);
}

extern FOUNDATION_ATOMIC_INLINE int32_t
foundation_atomic_add(int32_t increment, volatile int32_t *ptr)
{
    __asm__ __volatile__ ("lock; xaddl %0, %1"
                          : "+r" (increment), "+m" (*ptr)
                          : : "memory");
    /* increment now holds the old value of *ptr */
    return increment;
}

extern FOUNDATION_ATOMIC_INLINE int32_t
foundation_atomic_inc(volatile int32_t *addr)
{
    return foundation_atomic_add(1, addr);
}

extern FOUNDATION_ATOMIC_INLINE int32_t
foundation_atomic_dec(volatile int32_t *addr)
{
    return foundation_atomic_add(-1, addr);
}

extern FOUNDATION_ATOMIC_INLINE int32_t
foundation_atomic_and(int32_t value, volatile int32_t *ptr)
{
    int32_t prev, status;
    do {
        prev = *ptr;
        status = foundation_atomic_cas(prev, prev & value, ptr);
    } while (__builtin_expect(status != 0, 0));
    return prev;
}

extern FOUNDATION_ATOMIC_INLINE int32_t
foundation_atomic_or(int32_t value, volatile int32_t *ptr)
{
    int32_t prev, status;
    do {
        prev = *ptr;
        status = foundation_atomic_cas(prev, prev | value, ptr);
    } while (__builtin_expect(status != 0, 0));
    return prev;
}

#endif /* FOUNDATION_CUTILS_ATOMIC_X86_H */
