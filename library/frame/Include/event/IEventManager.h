/**
 * frame
 *
 * 事件管理类接口
**/

#ifndef __FRAME__IEVENTMANAGER_H__
#define __FRAME__IEVENTMANAGER_H__

#include "EventDef.h"

namespace frame {
class IEventCallback {
public:
    virtual ~IEventCallback(){};
    virtual void DispatchMouseEvent(Mouse_Event event) = 0;
    virtual void DispatchKeyEvent(Keyboard_Event event) = 0;
    virtual void DispatchInputCharEvent(Input_Char_Event event) = 0;
    virtual void onEndJourney() = 0;
};
class IEventManager
{
public:
    virtual ~IEventManager(){};
    virtual void SetEventListener(IEventCallback* callback) = 0;
    virtual void DispatchEvent() = 0;
    virtual void EndJourney() = 0;
    virtual void SendEvent(EventType type, EventBody body) = 0;
};

} // namespace frame 

#endif // __FRAME__IEVENTMANAGER_H__
