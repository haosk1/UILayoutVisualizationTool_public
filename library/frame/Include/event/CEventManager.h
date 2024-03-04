/**
 * frame
 *
 * 事件管理类实现
**/
#ifndef __FRAME__CEVENTMANAGER_H__
#define __FRAME__CEVENTMANAGER_H__

#include "IEventManager.h"
#include "EventQueue.h"

using namespace frame;
class CEventManager : public IEventManager
{
public:
    static CEventManager* Instance();
    //for Interface
    virtual ~CEventManager();
    virtual void SetEventListener(IEventCallback* callback);
    virtual void DispatchEvent();//描画线程调用
    virtual void EndJourney();
    virtual void SendEvent(EventType type, EventBody body);

    template<typename F, typename... Args>
    void SendEvent(F&& f, Args&&... args);

private:
    CEventManager();

    static CEventManager* m_Instance;
    IEventCallback* m_EventCallback = nullptr;
    EventQueue m_eventQueue;
      
};
#endif  // __FRAME__CEVENTMANAGER_H__

template <typename F, typename... Args>
inline void CEventManager::SendEvent(F &&f, Args &&...args)
{
    m_eventQueue.SendEvent(f, args...);
}
