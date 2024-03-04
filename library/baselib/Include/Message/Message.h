/**
 * BaseLib
 *
 * 消息对像，参考android
**/

#ifndef __BASE_LIB__MSG_FRW__MESSAGE_H__
#define __BASE_LIB__MSG_FRW__MESSAGE_H__

#include <stdlib.h>
#include "MessageCtrl.h"
#include "../Thread/Runnable.h"

#define MSSSAGE_ID_INTERNAL_BASE  -0x1000
#define MESSAGE_ID_INTERNAL_EXIT  MSSSAGE_ID_INTERNAL_BASE-1
#define MESSAGE_ID_INTERNAL_TIMER MSSSAGE_ID_INTERNAL_BASE-2
#define MESSAGE_ID_INTERNAL_RUNNABLE MSSSAGE_ID_INTERNAL_BASE-3



namespace baselib {

class Runnable;
class MessageHandler;
class MessageQueue;
class BaseMessageLooper;
    
class IMessageDisposer{
public:
    virtual void disposeMessage(Message * msg, bool isRunnable, bool delay_message) = 0;
};

typedef unsigned long msg_source_t;
class Message {
public:
    // 消息ID
    int what;

    // 消息返回值，同步调用时使用
    int result;

    // 消息 4 个自带参数
    int arg1;
    int arg2;
    int arg3;
    int arg4;

    // 对象指针
    void* ptr;

    // 回调消息处理接口
    Runnable* callback;

    // 消息处理句柄
    MessageHandler* target;

    // 消息发生时间
    int64_t when;

    Message * next;
    
    enum Priority {
        PRIORITY_HIGHEST,
        PRIORITY_HIGHER,
        PRIORITY_NORMAL,
        PRIORITY_NUM,
    };

public:
    bool postToTarget(msg_source_t from, MessageHandleObjPtr handle = NULL);
    bool postToTarget(MessageHandleObjPtr handle = NULL);
    bool sendToTarget(MessageHandleObjPtr handle = NULL);
    void recycle();
    bool isCanceled();
    uint32_t getNo() const { return mNo; }
    
    static void releasePool();

private:

    static Message * get();
    static Message * get(Runnable * r);
    static Message * get(MessageHandler* handler);
    static Message * get(MessageHandler* handler, int what);
    static Message *pop();
    static bool push(Message *m);

    Message() : what(0), target(NULL) { init(); }

    void init() {
        arg1 = arg2 = arg3 = arg4 = 0;
        ptr = NULL;
        result = 0;
        when = 0;
        callback = NULL;
        mPtr = NULL;
        next = NULL;
        mNo = 0;
        _post_from = 0;
        _post_time = 0;
        _priority = PRIORITY_NORMAL;
    }

    friend class MessageHandler;
    MessageHandleObj& genCtrl();

    friend class BaseMessageLooper;
    friend class MessageQueue;
    int enterInProgressState();
    int enterCompleteState();
    bool isNeedCallOnCancel();
    
public:
    void setPostTime(uint64_t post_time, uint32_t delay);
    uint64_t getPostTime(uint32_t * delay) const;
    void setPostFrom(msg_source_t post_from);
    msg_source_t getPostFrom() const;
    void setPriority(int priority) {
        _priority = Priority(priority & PRIORITY_NUM);
    }
    Priority getPriority() const {
        return _priority;
    }
    
private:
    MessageHandleObj mHandle;
    void * mPtr;
    volatile uint32_t mNo;
    uint64_t _post_time;
    msg_source_t _post_from;
    Priority _priority;
};

} //namespace baselib 

#endif // __BASE_LIB__MSG_FRW__MESSAGE_H__



