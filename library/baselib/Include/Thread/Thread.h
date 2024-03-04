/**
 * BaseLib
 *
 * 线程
**/
#ifndef __BASE_LIB__THREAD_H__
#define __BASE_LIB__THREAD_H__

#include <stdint.h>
#include <thread>
#include <unistd.h>

#include "Runnable.h"

typedef pthread_t ThreadHandle;
#define OS_Sleep(A)		usleep(A * 1000)

/*!
    \brief     线程类
    \details   非后台线程,在启动后线程且未完成之前不要销毁Thread对象
    \author
*/
namespace baselib {

class Thread : public Runnable {
private:
    Runnable* task = nullptr;            //!< 需要此线程运行的任务,必须实现IRunnable接口
    ThreadHandle threadHandle = 0;  //!< 线程句柄
    std::thread* m_Thread = nullptr;//线程
    
    unsigned long _tid = 0;

    bool started = false;           //!< 线程是否已经启动
    bool joined = false;            //!< 线程是否被joined

public:
    static bool setThreadName(const char * str);
    static int getThreadName(char * buf, int len);
    static uint32_t getPID();
    
public:
    /*!
        \brief          默认构造函数
        \return         {void}.无返回值
    */
    Thread(const char * szThreadName = NULL);

    /*!
        \brief          构造函数,task为运行的任务
        \return         {void}.无返回值
    */
    Thread(Runnable* task, const char * szThreadName = NULL);

    /*!
        \brief          析构函数
        \return         {void}.无返回值
    */
    virtual ~Thread();

    /*!
        \brief          启动线程,即在另一个线程中执行task->run(),每个线程只能启动一次,不能重复启动
        \return         {void}.无返回值
    */
    virtual bool start() /*throw ( char* )*/;

    /*!
        \brief          在一个线程中调用另一个线程的Thread对象的join方法,可以让当前线程挂起,一直到另一个线程结束
                        切记:不要调用当前线程对象的join方法,这样会造成线程永远挂起
        \return         {void}.无返回值
    */
    virtual void join() /*throw ( char* )*/;

    /*!
        \brief          获取已运行线程的内核句柄,如果线程没有start则返回NULL
        \return         {void}.无返回值
    */
    virtual ThreadHandle getThreadHandle()
    {
        return this->threadHandle;
    }

public:
    /*!
        \brief          使当前线程Sleep指定毫秒数
        \param[in]      millisecs {int}. Sleep的时间，单位毫秒
        \return         {void}.无返回值
    */
    static void sleep(int millisecs);

    /*!
        \brief          获取线程对象的id,注意,此函数只有在线程启动后才不返回0
        \return         {unsigned long}.无返线程ID
    */
    unsigned long getThreadId();

    /*!
        \brief          获取当前线程的threadId
        \return         {unsigned long}.当前线程ID
    */
    static unsigned long getCurrentThreadId();
    
    /* 取得线程名字
       return 返回构造函数传入的线程名字
     */
    const char * getThreadName();

protected:
    /*!
        \brief          实现IRunnable接口,当线程没有指定task时,缺省执行此方法,即不做任何动作,不对外部公开
        \return         {void}.无返回值
    */
    virtual void run() {};

protected:
    char mThreadName[32] = {0};
};

}// namespace baselib

#endif // __BASE_LIB__THREAD_H__




