/**
 * BaseLib
 *
 * 定时器
**/

#ifndef __BASE__LIB__TIMER_H__
#define __BASE__LIB__TIMER_H__

#include "TimerManager.h"

namespace baselib {

class Timer;
class TimerCallback {
public:
    virtual ~TimerCallback(){};
    virtual void onTimer(const Timer* timer) = 0;
};

class Timer : public baselib::TimerManagerCallback {
public:
    Timer();
    virtual ~Timer();

public:
    bool start(TimerCallback* ob, long delay, uint64_t from = 0);
    bool stop();
    bool isStoped() { return mStoped; }

    // for Runnable
    void onTimeOut();

private:
    TimerCallback*  mObserver;
    bool mStoped;
};

} // namespace baselib 

#endif // __BASELIB__TIMER_H__



