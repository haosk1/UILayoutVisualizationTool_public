#ifndef __BASE_LIB__REFCNT_H__
#define __BASE_LIB__REFCNT_H__

#include <malloc.h>

namespace baselib {
/*The object which is wrapped by CRefCnt object must be created by the "new" operator.*/
template <typename T> class CRefCnt
{
  public:
    CRefCnt() : m_pstObject(nullptr)
    {
    }

    CRefCnt(T *pstObject) : m_pstObject(pstObject)
    {
        if (m_pstObject)
        {
            m_pstObject->AddRef();
        }
    }

    CRefCnt(const CRefCnt<T> &stOther) : m_pstObject(stOther.m_pstObject)
    {
        if (m_pstObject)
            m_pstObject->AddRef();
    }

    ~CRefCnt()
    {
        if (m_pstObject)
            m_pstObject->Release();
    }

    CRefCnt<T> &operator=(const CRefCnt<T> &stOther)
    {
        if (this != &stOther && *this != stOther)
        {
            if (m_pstObject)
                m_pstObject->Release();

            m_pstObject = stOther.m_pstObject;

            if (m_pstObject)
                m_pstObject->AddRef();
        }

        return *this;
    }

    bool operator==(const CRefCnt<T> &stOther) const
    {
        return m_pstObject == stOther.m_pstObject;
    }

    bool operator!=(const CRefCnt<T> &stOther) const
    {
        return m_pstObject != stOther.m_pstObject;
    }

    const T &operator*() const
    {
        return *m_pstObject;
    }

    T &operator*()
    {
        return *m_pstObject;
    }

    const T *operator->() const
    {
        return m_pstObject;
    }

    T *operator->()
    {
        return m_pstObject;
    }

    bool IsValid() const
    {
        return m_pstObject != nullptr;
    }

  private:
    T *m_pstObject;
};
// lint -restore

template <typename T> class CAutoRefCnt
{
  public:
    CAutoRefCnt() : m_pstObject(nullptr), m_pulCount(nullptr)
    {
    }

    CAutoRefCnt(T *pstObject) : m_pstObject(pstObject)
    {
        if (m_pstObject)
        {
            m_pulCount = (uint32_t *)malloc(sizeof(uint32_t));
            if (m_pulCount)
            {
                *m_pulCount = 1;
            }
        }
        else
        {
            m_pulCount = nullptr;
        }
    }

    CAutoRefCnt(const CAutoRefCnt<T> &stOther) : m_pstObject(stOther.m_pstObject), m_pulCount(stOther.m_pulCount)
    {
        if (m_pulCount)
            AddRef();
    }

    ~CAutoRefCnt()
    {
        if (m_pulCount)
            Release();
    }

    CAutoRefCnt<T> &operator=(const CAutoRefCnt<T> &stOther)
    {
        if (this != &stOther && *this != stOther)
        {
            if (m_pstObject)
                Release();

            m_pstObject = stOther.m_pstObject;
            m_pulCount = m_pstObject ? stOther.m_pulCount : nullptr;

            if (m_pulCount)
                AddRef();
        }

        return *this;
    }

    bool operator==(const CAutoRefCnt<T> &stOther) const
    {
        return m_pstObject == stOther.m_pstObject;
    }

    bool operator!=(const CAutoRefCnt<T> &stOther) const
    {
        return m_pstObject != stOther.m_pstObject;
    }

    const T &operator*() const
    {
        return *m_pstObject;
    }

    T &operator*()
    {
        return *m_pstObject;
    }

    const T *operator->() const
    {
        return m_pstObject;
    }

    T *operator->()
    {
        return m_pstObject;
    }

    bool IsValid() const
    {
        return m_pstObject != nullptr;
    }

  private:
    void AddRef()
    {
        ++(*m_pulCount);
    }

    void Release()
    {
        if (!--(*m_pulCount))
        {
            delete m_pstObject;
            free(m_pulCount);
            m_pstObject = nullptr;
            m_pulCount = nullptr;
        }
    }

    T *m_pstObject;
    uint32_t *m_pulCount;
};

}; // namespace baselib

#endif /*__BASE_LIB__REFCNT_H__*/
