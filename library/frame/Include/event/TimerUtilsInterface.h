/**
 * frame
 *
 * Timer封装接口类
**/
#ifndef _CTIMER_UTILS_INTERFACE_H_
#define _CTIMER_UTILS_INTERFACE_H_

#include <future>
#include <functional>

class TimerUtilsInterface
{
public:
    virtual ~TimerUtilsInterface(){};

    /** 以下方法来自于baselib::Timer */
    virtual bool start(std::function<void()> func, long delay, bool isRepeat = false) = 0;

    virtual bool stop() = 0;

    virtual bool isStoped() = 0;

    virtual void TimerAgain() = 0;
}; 
#endif //_CTIMER_UTILS_INTERFACE_H_
