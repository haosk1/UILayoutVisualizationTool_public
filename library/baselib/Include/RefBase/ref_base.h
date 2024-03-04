/*
 * Copyright (C) 2005 The Android Open Source Project
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

#ifndef __BASE_LIB__REF_BASE_H__
#define __BASE_LIB__REF_BASE_H__

#if !defined(_WIN32) && !defined(UNDER_CE)
#include <sys/types.h>
#endif
#include <stdlib.h>
#include <stdint.h>
 // ---------------------------------------------------------------------------
namespace baselib {

template<typename T> class RefCountWeakPtr;

// ---------------------------------------------------------------------------

#define COMPARE(_op_)                                           \
inline bool operator _op_ (const RefCountPtr<T>& o) const {              \
return m_ptr _op_ o.m_ptr;                                  \
}                                                               \
inline bool operator _op_ (const RefCountWeakPtr<T>& o) const {              \
return m_ptr _op_ o.m_ptr;                                  \
}                                                               \
inline bool operator _op_ (const T* o) const {                  \
return m_ptr _op_ o;                                        \
}                                                               \
template<typename U>                                            \
inline bool operator _op_ (const RefCountPtr<U>& o) const {              \
return m_ptr _op_ o.m_ptr;                                  \
}                                                               \
template<typename U>                                            \
inline bool operator _op_ (const RefCountWeakPtr<U>& o) const {              \
return m_ptr _op_ o.m_ptr;                                  \
}                                                               \
template<typename U>                                            \
inline bool operator _op_ (const U* o) const {                  \
return m_ptr _op_ o;                                        \
}

    // ---------------------------------------------------------------------------

class RefBase
{
public:
    void            incStrong(const void* id) const;
    void            decStrong(const void* id) const;

    void            forceIncStrong(const void* id) const;

    //! DEBUGGING ONLY: Get current strong ref count.
    int32_t         getStrongCount() const;

    class weakref_type
    {
    public:
        RefBase*            refBase() const;

        void                incWeak(const void* id);
        void                decWeak(const void* id);

        bool                attemptIncStrong(const void* id);

        //! This is only safe if you have set OBJECT_LIFETIME_FOREVER.
        bool                attemptIncWeak(const void* id);

        //! DEBUGGING ONLY: Get current weak ref count.
        int32_t             getWeakCount() const;

        //! DEBUGGING ONLY: Print references held on object.
        void                printRefs() const;

        //! DEBUGGING ONLY: Enable tracking for this object.
        // enable -- enable/disable tracking
        // retain -- when tracking is enable, if true, then we save a stack trace
        //           for each reference and dereference; when retain == false, we
        //           match up references and dereferences and keep only the
        //           outstanding ones.

        void                trackMe(bool enable, bool retain);
    };

    weakref_type*   createWeak(const void* id) const;

    weakref_type*   getWeakRefs() const;

    //! DEBUGGING ONLY: Print references held on object.
    inline  void            printRefs() const { getWeakRefs()->printRefs(); }

    //! DEBUGGING ONLY: Enable tracking of object.
    inline  void            trackMe(bool enable, bool retain)
    {
        getWeakRefs()->trackMe(enable, retain);
    }

protected:
    RefBase();
    virtual ~RefBase();

    //! Flags for extendObjectLifetime()
    enum {
        OBJECT_LIFETIME_WEAK = 0x0001,
        OBJECT_LIFETIME_FOREVER = 0x0003
    };

    void            extendObjectLifetime(int32_t mode);

    //! Flags for onIncStrongAttempted()
    enum {
        FIRST_INC_STRONG = 0x0001
    };

    virtual void            onFirstRef();
    virtual void            onLastStrongRef(const void* id);
    virtual bool            onIncStrongAttempted(uint32_t flags, const void* id);
    virtual void            onLastWeakRef(const void* id);

private:
    friend class weakref_type;
    class weakref_impl;

    RefBase(const RefBase& o);
    RefBase& operator=(const RefBase& o);

    weakref_impl* const mRefs;
};

typedef RefBase RefCountObjectBase;
// ---------------------------------------------------------------------------

template <typename T>
class RefCountPtr
{
public:
    friend class RefCountWeakPtr<T>;
    template<typename U> friend class RefCountPtr;
    typedef typename RefCountObjectBase::weakref_type weakref_type;

    RefCountPtr(T* ptr = NULL);
    RefCountPtr(const RefCountPtr<T>& other);
    template<typename U> RefCountPtr(U* other);
    template<typename U> RefCountPtr(const RefCountPtr<U>& other);

    ~RefCountPtr();

    // Assignment

    RefCountPtr& operator = (T* other);
    RefCountPtr& operator = (const RefCountPtr<T>& other);

    template<typename U> RefCountPtr& operator = (const RefCountPtr<U>& other);
    template<typename U> RefCountPtr& operator = (U* other);

    // Reset

    void clear();

    // Accessors

    inline  T&      operator* () const { return *m_ptr; }
    inline  T*      operator-> () const { return m_ptr; }
    inline  T*      get() const { return m_ptr; }

    // Operators

    COMPARE(== )
        COMPARE(!= )
        COMPARE(> )
        COMPARE(< )
        COMPARE(<= )
        COMPARE(>= )

private:
    template<typename Y> friend class sp;
    template<typename Y> friend class wp;

    // Optimization for wp::promote().
    RefCountPtr(T* p, weakref_type* refs);

    T*              m_ptr;
};

//    template <typename T>
//    TextOutput& operator<<(TextOutput& to, const sp<T>& val);

    // ---------------------------------------------------------------------------

template <typename T>
class RefCountWeakPtr
{
public:
    typedef typename RefCountObjectBase::weakref_type weakref_type;

    RefCountWeakPtr(T* ptr = NULL);
    RefCountWeakPtr(const RefCountWeakPtr<T>& other);
    RefCountWeakPtr(const RefCountPtr<T>& other);
    template<typename U> RefCountWeakPtr(U* other);
    template<typename U> RefCountWeakPtr(const RefCountPtr<U>& other);
    template<typename U> RefCountWeakPtr(const RefCountWeakPtr<U>& other);

    ~RefCountWeakPtr();

    // Assignment

    RefCountWeakPtr& operator = (T* other);
    RefCountWeakPtr& operator = (const RefCountWeakPtr<T>& other);
    RefCountWeakPtr& operator = (const RefCountPtr<T>& other);

    template<typename U> RefCountWeakPtr& operator = (U* other);
    template<typename U> RefCountWeakPtr& operator = (const RefCountWeakPtr<U>& other);
    template<typename U> RefCountWeakPtr& operator = (const RefCountPtr<U>& other);

    void set_object_and_refs(T* other, weakref_type* refs);

    // promotion to sp

    RefCountPtr<T> promote() const;

    // Reset

    void clear();

    // Accessors

    inline  weakref_type* get_refs() const { return m_refs; }

    inline  T* unsafe_get() const { return m_ptr; }

    // Operators

    COMPARE(== )
        COMPARE(!= )
        COMPARE(> )
        COMPARE(< )
        COMPARE(<= )
        COMPARE(>= )

private:
	  friend class RefCountPtr<T>;
    template<typename Y> friend class sp;
    template<typename Y> friend class wp;

    T*              m_ptr;
    weakref_type*   m_refs;
};


//    template <typename T>
//    TextOutput& operator<<(TextOutput& to, const wp<T>& val);

#undef COMPARE

    // ---------------------------------------------------------------------------
    // No user serviceable parts below here.

template<typename T>
RefCountPtr<T>::RefCountPtr(T* ptr)
    : m_ptr(ptr)
{
    if (m_ptr) m_ptr->incStrong(this);
}

template<typename T>
RefCountPtr<T>::RefCountPtr(const RefCountPtr<T>& other)
    : m_ptr(other.m_ptr)
{
    if (m_ptr) m_ptr->incStrong(this);
}

template<typename T> template<typename U>
RefCountPtr<T>::RefCountPtr(U* other) : m_ptr(other)
{
    if (m_ptr) m_ptr->incStrong(this);
}

template<typename T> template<typename U>
RefCountPtr<T>::RefCountPtr(const RefCountPtr<U>& other)
    : m_ptr(other.m_ptr)
{
    if (m_ptr) m_ptr->incStrong(this);
}

template<typename T>
RefCountPtr<T>::~RefCountPtr()
{
    if (m_ptr) m_ptr->decStrong(this);
}

template<typename T>
RefCountPtr<T>& RefCountPtr<T>::operator = (const RefCountPtr<T>& other) {
    if (other.m_ptr) other.m_ptr->incStrong(this);
    if (m_ptr) m_ptr->decStrong(this);
    m_ptr = other.m_ptr;
    return *this;
}

template<typename T>
RefCountPtr<T>& RefCountPtr<T>::operator = (T* other)
{
    if (other) other->incStrong(this);
    if (m_ptr) m_ptr->decStrong(this);
    m_ptr = other;
    return *this;
}

template<typename T> template<typename U>
RefCountPtr<T>& RefCountPtr<T>::operator = (const RefCountPtr<U>& other)
{
    if (other.m_ptr) other.m_ptr->incStrong(this);
    if (m_ptr) m_ptr->decStrong(this);
    m_ptr = other.m_ptr;
    return *this;
}

template<typename T> template<typename U>
RefCountPtr<T>& RefCountPtr<T>::operator = (U* other)
{
    if (other) other->incStrong(this);
    if (m_ptr) m_ptr->decStrong(this);
    m_ptr = other;
    return *this;
}

template<typename T>
void RefCountPtr<T>::clear()
{
    if (m_ptr) {
        m_ptr->decStrong(this);
        m_ptr = 0;
    }
}

template<typename T>
RefCountPtr<T>::RefCountPtr(T* p, weakref_type* refs)
    : m_ptr((p && refs->attemptIncStrong(this)) ? p : 0)
{
}

//    template <typename T>
//    inline TextOutput& operator<<(TextOutput& to, const sp<T>& val)
//    {
//        to << "sp<>(" << val.get() << ")";
//        return to;
//    }

    // ---------------------------------------------------------------------------

template<typename T>
RefCountWeakPtr<T>::RefCountWeakPtr(T* ptr)
    : m_ptr(ptr), m_refs(NULL)
{
    if (m_ptr) m_refs = m_ptr->createWeak(this);
}

template<typename T>
RefCountWeakPtr<T>::RefCountWeakPtr(const RefCountWeakPtr<T>& other)
    : m_ptr(other.m_ptr), m_refs(other.m_refs)
{
    if (m_ptr) m_refs->incWeak(this);
}

template<typename T>
RefCountWeakPtr<T>::RefCountWeakPtr(const RefCountPtr<T>& other)
    : m_ptr(other.get()), m_refs(NULL)
{
    if (m_ptr) {
        m_refs = m_ptr->createWeak(this);
    }
}

template<typename T> template<typename U>
RefCountWeakPtr<T>::RefCountWeakPtr(U* other)
    : m_ptr(other), m_refs(NULL)
{
    if (m_ptr) m_refs = m_ptr->createWeak(this);
}

template<typename T> template<typename U>
RefCountWeakPtr<T>::RefCountWeakPtr(const RefCountWeakPtr<U>& other)
    : m_ptr(other.m_ptr)
{
    if (m_ptr) {
        m_refs = other.m_refs;
        m_refs->incWeak(this);
    }
}

template<typename T> template<typename U>
RefCountWeakPtr<T>::RefCountWeakPtr(const RefCountPtr<U>& other)
    : m_ptr(other.m_ptr), m_refs(NULL)
{
    if (m_ptr) {
        m_refs = m_ptr->createWeak(this);
    }
}

template<typename T>
RefCountWeakPtr<T>::~RefCountWeakPtr()
{
    if (m_ptr) m_refs->decWeak(this);
}

template<typename T>
RefCountWeakPtr<T>& RefCountWeakPtr<T>::operator = (T* other)
{
    weakref_type* newRefs =
        other ? other->createWeak(this) : 0;
    if (m_ptr) m_refs->decWeak(this);
    m_ptr = other;
    m_refs = newRefs;
    return *this;
}

template<typename T>
RefCountWeakPtr<T>& RefCountWeakPtr<T>::operator = (const RefCountWeakPtr<T>& other)
{
    if (other.m_ptr) other.m_refs->incWeak(this);
    if (m_ptr) m_refs->decWeak(this);
    m_ptr = other.m_ptr;
    m_refs = other.m_refs;
    return *this;
}

template<typename T>
RefCountWeakPtr<T>& RefCountWeakPtr<T>::operator = (const RefCountPtr<T>& other)
{
    weakref_type* newRefs =
        other != NULL ? other->createWeak(this) : 0;
    if (m_ptr) m_refs->decWeak(this);
    m_ptr = other.get();
    m_refs = newRefs;
    return *this;
}

template<typename T> template<typename U>
RefCountWeakPtr<T>& RefCountWeakPtr<T>::operator = (U* other)
{
    weakref_type* newRefs =
        other ? other->createWeak(this) : 0;
    if (m_ptr) m_refs->decWeak(this);
    m_ptr = other;
    m_refs = newRefs;
    return *this;
}

template<typename T> template<typename U>
RefCountWeakPtr<T>& RefCountWeakPtr<T>::operator = (const RefCountWeakPtr<U>& other)
{
    if (other.m_ptr) other.m_refs->incWeak(this);
    if (m_ptr) m_refs->decWeak(this);
    m_ptr = other.m_ptr;
    m_refs = other.m_refs;
    return *this;
}

template<typename T> template<typename U>
RefCountWeakPtr<T>& RefCountWeakPtr<T>::operator = (const RefCountPtr<U>& other)
{
    weakref_type* newRefs =
        other != NULL ? other->createWeak(this) : 0;
    if (m_ptr) m_refs->decWeak(this);
    m_ptr = other.get();
    m_refs = newRefs;
    return *this;
}

template<typename T>
void RefCountWeakPtr<T>::set_object_and_refs(T* other, weakref_type* refs)
{
    if (other) refs->incWeak(this);
    if (m_ptr) m_refs->decWeak(this);
    m_ptr = other;
    m_refs = refs;
}

template<typename T>
RefCountPtr<T> RefCountWeakPtr<T>::promote() const
{
    return RefCountPtr<T>(m_ptr, m_refs);
}

template<typename T>
void RefCountWeakPtr<T>::clear()
{
    if (m_ptr) {
        m_refs->decWeak(this);
        m_ptr = 0;
    }
}

//    template <typename T>
//    inline TextOutput& operator<<(TextOutput& to, const wp<T>& val)
//    {
//        to << "wp<>(" << val.unsafe_get() << ")";
//        return to;
//    }

}; // namespace baselib

// ---------------------------------------------------------------------------

#endif // __BASE_LIB__REF_BASE_H__
