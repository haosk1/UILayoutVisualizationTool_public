/*!
    \file TimerThread.h
    \brief 定时器轮询线程的实现文件
    \details 无
    \author haosk
*/
#ifndef __BASE_LIB__TIMER__THREAD__H__
#define __BASE_LIB__TIMER__THREAD__H__

#include <thread>
#include <unistd.h>
#include <mutex>
#include <condition_variable>

namespace baselib {

class TimerRunable {
public:
    virtual void run() = 0;
};
class TimerThread {
public:
    TimerThread(){};
    ~TimerThread();

    bool start(TimerRunable* runable);     //开启任务
    bool stop();                           //开启任务
private:
    void startPolling();                   //开始轮询

    std::thread* m_Thread = nullptr;
    TimerRunable* m_Runable = nullptr;     //任务
 
    std::mutex m_startMutex;               //start锁
    bool m_IsPause = true;                 //是否暂停 
    bool m_IsStop = false;                  //是否停止    
    std::condition_variable m_condition;   //条件变量
};

inline bool TimerThread::start(TimerRunable* runable)
{
    m_IsPause = false;
    this->m_condition.notify_one();
    std::unique_lock<std::mutex> lock(this->m_startMutex);
    if(nullptr != this->m_Runable || nullptr == runable){
        return false;
    }
    this->m_Runable = runable;
    this->startPolling();
    return true;
}
 
inline void TimerThread::startPolling(){
    if(nullptr == m_Thread){
        m_Thread = new std::thread([this]{
        for(;;){
            {
                std::unique_lock<std::mutex> lock(this->m_startMutex);
                this->m_condition.wait(lock,[this]{ return !this->m_IsPause;});
                if(m_IsStop == true)
                {
                    return;
                }
            }
            
            if(nullptr != this->m_Runable){
                this->m_Runable->run();
            }
        }
        });
    }
}

inline bool TimerThread::stop()
{
    this->m_IsPause = true;
    return true;
}

inline TimerThread::~TimerThread(){
    {
        std::unique_lock<std::mutex> lock(this->m_startMutex);
        this->m_IsPause = false;
    }
    this->m_IsStop = true;
    this->m_condition.notify_all();
    if(nullptr != m_Thread && m_Thread->joinable()){
        m_Thread->join();
        delete m_Thread;
        m_Thread = nullptr;
        this->m_Runable = nullptr;
    }
}

}// end namespace baselib 
#endif  //__BASE_LIB__TIMER__THREAD__H__
