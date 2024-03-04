/**
 * BaseLib
 *
 * 循环体实现
**/

#ifndef __BASE_LIB__MSG_FRW__LOOPER_H__
#define __BASE_LIB__MSG_FRW__LOOPER_H__

#include "../Mutex/Mutex.h"
#include "BaseMessageLooper.h"
#include "Message.h"

namespace baselib {

class MessageQueue;
class MessageQueue::Observer;
class Looper : public BaseMessageLooper {
public:
    Looper(int size = 2048);
    Looper(MessageQueue *queue);
    ~Looper();

public:
    void loop();
    void removeMsgByID(int msgid);
    void clearQueue(IMessageDisposer* disposer = NULL);
    
    uint32_t getQueueSize() const;
    uint32_t getDelayQueueSize(uint32_t * timing) const;
    uint32_t getHigherQueueSize() const;
    uint32_t getHighestQueueSize() const;
private:
    bool postMessage(Message* message, unsigned long when);
    bool postDelayMessage(Message * message, uint32_t delay);
    
    friend class MessageQueue::Observer;
    void onQueueEmpty(int delay_queue);
    void onQueueMessageCanceled(Message * msg, uint64_t timing);
    
private:
    static void initTLSKey();
    static void threadDestructor(void *st);

private:
    MessageQueue* mQueue;
    MessageQueue::Observer * mQueueObserver;
    bool mIsBusy;
    bool mQueueInner;
};

} // namespace baselib

#endif // __BASE_LIB__MSG_FRW__LOOPER_H__



