#ifndef _BASE_LIB_SCOPED_PTR_H_
#define _BASE_LIB_SCOPED_PTR_H_

#include <stdint.h>
#include <stdlib.h>
#include "../RefBase/Atomic/Atomic.h"

/*
    智能指针支持
    基础库有多套智能指针(ref_base.h memory.h scoped_ptr.h)分别适用不同的场景
 
    scoped_ptr.h文件中的智能指针是一个轻量级的工具，不支持弱指针。
    特点为：
        负载小（仅在对象中占用一个int32_t），适用于小对象
        使用简单高效，计数操作支持加锁和不加锁两种
        侵入式，需要继承统一基础类
 */
namespace baselib {
    template <typename T>
    struct DefaultRefCountedTrails {
        static void release(const T * p) {
            delete p;
        }
        enum { START_REF = 1 << 28 };
    };
    
    template <typename T, typename _TRAILS = DefaultRefCountedTrails<T> >
    class RefCounted {
    public:
        RefCounted();
        ~RefCounted();
        
        void addRef() const;
        bool release() const;
        
    private:
        RefCounted(const RefCounted&);
        RefCounted& operator=(const RefCounted&);
        
    private:
        mutable int32_t _refcount;
    };
    
    template <typename T, typename _TRAILS = DefaultRefCountedTrails<T> >
    class RefCountedThreadSafe {
    public:
        RefCountedThreadSafe();
        ~RefCountedThreadSafe();
        
        void addRef() const;
        bool release() const;
        
    private:
        RefCountedThreadSafe(const RefCountedThreadSafe&);
        RefCountedThreadSafe& operator=(const RefCountedThreadSafe&);
        
    private:
        mutable int32_t _refcount;
    };
    
    template <typename T>
    class ScopedRefptr {
        typedef T* ScopedRefptr::*Testable;
    public:
        typedef T element_type;
        
        ScopedRefptr(T* p = NULL);
        ScopedRefptr(const ScopedRefptr& r);
        template <typename T1>
        ScopedRefptr(T1 * p);
        template <typename T1>
        ScopedRefptr(const ScopedRefptr<T1>& r);
        ~ScopedRefptr();
        
        T* get() const;
        T& operator*() const;
        T* operator->() const;
        
        template <typename T1>
        ScopedRefptr& operator=(T1* p);
        
        ScopedRefptr& operator=(T* p);
        
        template <typename T1>
        ScopedRefptr& operator=(const ScopedRefptr<T1>&);
        
        ScopedRefptr& operator=(const ScopedRefptr&);
        
        void reset();
        void swap(ScopedRefptr&);
        
        operator Testable() const;
        
        template <typename T1>
        bool operator == (const ScopedRefptr<T1> &);
        
        template <typename T1>
        bool operator == (const T1 * p);
        
        bool operator == (const T * p);
        
        bool operator == (const ScopedRefptr&);
        
    private:
        template <typename U>
        friend class ScopedRefptr;
        
        void assign(T * p);
        
    private:
        T * _ptr;
    };

    //***************************** RefCounted implamentation ***************************************
    template <typename T, typename _TRAILS>
    RefCounted<T, _TRAILS>::RefCounted() : _refcount(_TRAILS::START_REF){
    }
    
    template <typename T, typename _TRAILS>
    RefCounted<T, _TRAILS>::~RefCounted() {
    }
    
    template <typename T, typename _TRAILS>
    void RefCounted<T, _TRAILS>::addRef() const{
        if (_refcount == _TRAILS::START_REF) {
            _refcount = 0;
        }
        _refcount++;
    }
    
    template <typename T, typename _TRAILS>
    bool RefCounted<T, _TRAILS>::release() const {
        int oldRefcount = _refcount--;
        if(oldRefcount == 1 || oldRefcount == _TRAILS::START_REF) {
            _TRAILS::release((const T*)this);
            return true;
        }
        return false;
    }
    
    //***************************** RefCountedThreadSafe implamentation ***************************************
    template <typename T, typename _TRAILS>
    RefCountedThreadSafe<T, _TRAILS>::RefCountedThreadSafe() : _refcount(_TRAILS::START_REF) {
    }
    
    template <typename T, typename _TRAILS>
    RefCountedThreadSafe<T, _TRAILS>::~RefCountedThreadSafe() {
    }
    
    template <typename T, typename _TRAILS>
    void RefCountedThreadSafe<T, _TRAILS>::addRef() const {
        int32_t old = foundation_atomic_inc(&_refcount);
        if(old == _TRAILS::START_REF) {
            foundation_atomic_add(-_TRAILS::START_REF, &_refcount);
        }
    }
    
    template <typename T, typename _TRAILS>
    bool RefCountedThreadSafe<T, _TRAILS>::release() const {
        int32_t r = foundation_atomic_dec(&_refcount);
        if(r == 1 || r == _TRAILS::START_REF) {
            _TRAILS::release((const T*)this);
            return true;
        }
        return false;
    }
    
    //***************************** ScopedRefptr implamentation ***************************************
    template <typename T>
    ScopedRefptr<T>::ScopedRefptr(T* p) : _ptr(p) {
        if (_ptr)
            _ptr->addRef();
    }
    
    template <typename T>
    ScopedRefptr<T>::ScopedRefptr(const ScopedRefptr& r) : _ptr(r.get()) {
        if (_ptr)
            _ptr->addRef();
    }

    template <typename T>
    template <typename T1>
    ScopedRefptr<T>::ScopedRefptr(T1* p) : _ptr(p) {
        if(_ptr)
            _ptr->addRef();
    }
    
    template <typename T>
    template <typename T1>
    ScopedRefptr<T>::ScopedRefptr(const ScopedRefptr<T1>& r) : _ptr(r.get()){
        if(_ptr)
            _ptr->addRef();
    }
    
    template <typename T>
    ScopedRefptr<T>::~ScopedRefptr(){
        if(_ptr)
            _ptr->release();
    }
    
    template <typename T>
    T* ScopedRefptr<T>::get() const {
        return _ptr;
    }
    
    template <typename T>
    T& ScopedRefptr<T>::operator*() const {
        return *_ptr;
    }
    
    template <typename T>
    T* ScopedRefptr<T>::operator->() const {
        return _ptr;
    }
    
    template <typename T>
    void ScopedRefptr<T>::assign(T * p) {
        if(p)
            p->addRef();

        T* old_ptr = _ptr;
        _ptr = p;

        if(old_ptr)
            old_ptr->release();
    }
    
    template <typename T>
    template <typename T1>
    ScopedRefptr<T>& ScopedRefptr<T>::operator=(T1* p) {
        assign(p);
        return *this;
    }
    
    template <typename T>
    ScopedRefptr<T>& ScopedRefptr<T>::operator=(T* p) {
        assign(p);
        return *this;
    }
    
    template <typename T>
    template <typename T1>
    ScopedRefptr<T>& ScopedRefptr<T>::operator=(const ScopedRefptr<T1>& r) {
        assign(r.get());
        return *this;
    }
    
    template <typename T>
    ScopedRefptr<T>& ScopedRefptr<T>::operator=(const ScopedRefptr& r) {
        assign(r.get());
        return *this;
    }
    
    template <typename T>
    void ScopedRefptr<T>::reset() {
        assign(NULL);
    }
    
    template <typename T>
    void ScopedRefptr<T>::swap(ScopedRefptr& r) {
        T * tmp = _ptr;
        _ptr = r._ptr;
        r._ptr = tmp;
    }
    
    template <typename T>
    ScopedRefptr<T>::operator Testable() const {
        return _ptr != NULL ? &ScopedRefptr::_ptr : NULL;
    }
    
    template <typename T>
    template <typename T1>
    bool ScopedRefptr<T>::operator== (const ScopedRefptr<T1> & r) {
        return _ptr == r._ptr;
    }
    
    template <typename T>
    template <typename T1>
    bool ScopedRefptr<T>::operator == (const T1 * p) {
        return _ptr == p;
    }
    
    template <typename T>
    bool ScopedRefptr<T>::operator == (const T * p) {
        return _ptr == p;
    }
    
    template <typename T>
    bool ScopedRefptr<T>::operator == (const ScopedRefptr& r) {
        return _ptr == r._ptr;
    }
}


#endif
