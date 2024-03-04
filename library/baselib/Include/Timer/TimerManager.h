/*!
    \file TimerManager.h
    \brief 定时器管理类
    \details 无
    \author haosk
*/
#ifndef __BASE_LIB__TIMERMANAGER__H__
#define __BASE_LIB__TIMERMANAGER__H__

#include <map>
#include <tuple>
#include "./TimerThread.h"
#include "Mutex/Mutex.h"



namespace baselib {
#define ALREADY_DONE (-1)

class TimerManagerCallback {
public:
    virtual ~TimerManagerCallback(){};
    virtual void onTimeOut() = 0;
};

class TimerManager : public baselib::TimerRunable {
private:                                                                                                             
    TimerManager(const TimerManager &);                                                                                  
    TimerManager &operator = (const TimerManager &); 
    TimerManager();  
    ~TimerManager();

    static TimerManager* m_pstObj;                                                                                                            
public:             
    //单例 饿汉                                                                                                 
    static TimerManager* getInstance()                                                                                 
    {                                                                                                    
        return m_pstObj;                                                                                                
    }                                                                                                                  
    // for Runnable
    virtual void run();

   bool startTimer(TimerManagerCallback* pstTimer, ulong outTime,std::string strThreadName);

   bool cancelTimer(TimerManagerCallback* pstTimer);

   ulong getCurrentTime();


private:
    // bl::MapEngineID changeThreadNameToBLEngineId(std::string stdThreadName);

    typedef std::map<TimerManagerCallback*, std::tuple<ulong, std::string>> TimerContainer;
    TimerContainer m_stTimerContainer;
    TimerThread* m_pTimerThread = nullptr;
    baselib::Mutex TimerContainerMutex;
};
} // namespace baselib 

#endif // __BASE_LIB__TIMERMANAGER__H__



