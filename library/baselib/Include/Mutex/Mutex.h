
/*!
    \file Mutex.h
    \brief 互斥体类定义文件
    \details 无
    \author haosk
*/

#ifndef __BASE_LIB__MUTEX_H__
#define __BASE_LIB__MUTEX_H__

#include <mutex>
#include <thread>
#include <unistd.h>
#include <sys/syscall.h>

namespace baselib {

class Mutex {
public:
     Mutex() {};

    /*!
     \brief wait调用的错误码
     */
    enum {
        E_WAIT_SUCC, //等待信号成功
        E_WAIT_TIMEOUT,  //等待超时
        E_WAIT_INVALID   //对象无效
    };

    /*!
        \brief          锁定对象
        \return         {void}.无返回值
    */
    void lock();

    /*!
        \brief          解锁对象
        \return         {void}.无返回值
    */
    void unlock();
    /*!
        \brief          判断对象是否已经锁定
        \return         {bool}.true：已锁定 false：未锁定
    */
    bool islocked();
private:
    Mutex(const Mutex & mutex);                 //!< 不允许拷贝构造
    Mutex& operator = (const Mutex & mutex);    //!< 不允许赋值构造

private:
    std::mutex m_stdMutex;
    bool m_isLocked = false;

};

template <typename T>
class LockWrap {
public:
    LockWrap(T& Lock) : mLock(&Lock) {
        mLock->lock();
    }

    ~LockWrap() {
        mLock->unlock();
    }

    LockWrap(const LockWrap&) = delete;
    LockWrap& operator=(const LockWrap&) = delete;
protected:
    T* mLock;
};

typedef LockWrap<Mutex> MutexWrap;

}// end namespace baselib 

#endif // __BASE_LIB__MUTEX_H__



