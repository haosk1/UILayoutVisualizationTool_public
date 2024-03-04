/**
 * frame
 *
 * 事件队列
**/
#ifndef __FRAME__EVENTQUEUE_H__
#define __FRAME__EVENTQUEUE_H__

#include <map>
#include <list>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <queue>

#include "EventDef.h"

namespace frame {

class EventQueue
{
public:
    EventQueue();
    ~EventQueue(){};

    void SendEvent(EventType type, EventBody body);
    EventBody GetEvent(EventType type);
    
    void SendEvent(std::function<void()> func);
    std::function<void()> GetEvent();
    
private:
    EventQueue(const EventQueue&);
    EventQueue& operator=(const EventQueue&);

private:
  typedef std::list<EventBody> EventList;
  typedef std::map<EventType, EventList> EventMap;
  EventMap          m_eventQueue;
  std::queue<std::function<void()>> m_tasks;    //任务队列 
  std::condition_variable m_condition;          //条件变量
  std::mutex m_EventMutex;                      //event锁
  std::mutex m_FunMutex;                        //function锁
};

} // namespace frame 

#endif // __FRAME__EVENTQUEUE_H__
