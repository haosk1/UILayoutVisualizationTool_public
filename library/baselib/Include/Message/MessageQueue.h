/**
 * BaseLib
 *
 * 消息队列
**/

#ifndef __BASE_LIB__MSG_FRW__MESSAGE_QUEUE_H__
#define __BASE_LIB__MSG_FRW__MESSAGE_QUEUE_H__

#include <map>
#include <stdint.h>

#include "Message.h"
#include "../Mutex/Mutex.h"

namespace baselib {

class Runnable;

class LooperObserverMan;
class MessageQueue {
public:
    class Observer {
    public:
        virtual ~Observer() {}
        virtual void onQueueEmpty(int delay_queue) = 0;
        virtual void onQueueMessageCanceled(Message * msg, uint64_t timing) = 0;
    };

    MessageQueue(int size = 2048);
    ~MessageQueue();

    Message* next(uint64_t& timing);
    bool enqueueMessage(Message* message, int64_t when);
    void removeMsgByID(int msgid);
    void clear(IMessageDisposer * disposer = NULL);
    uint32_t queueSize() const;
    uint32_t delayQueueSize(uint64_t * timing) const;
    uint32_t higherQueueSize() const;
    uint32_t highestQueueSize() const;
    void setObserver(Observer *ob) {
        mObserver = ob;
    }
    
private:
    void wakeupConsumer();
    uint32_t getMsgCancelNo(int msgid);
    bool isMessageCanceled(Message * msg);
    Message* fetchNext(uint64_t& timing);

protected:
    virtual Message *fetchMessage(unsigned long &nextPollTimeoutMillis, uint64_t &timing);
    Message* fetchDelayed(unsigned long &nextPollTimeoutMillis, uint64_t &timing);
    
    class MessageQueueStorage {
        friend class MessageQueue;
    public:
        ComplexQueue<Message> mQueue;
        ComplexQueue<Message> mHigher;
        ComplexQueue<Message> mHighest;
    private:
        MessageQueueStorage(int size): mQueue(size), mHigher(size), mHighest(size) {}
        Mutex mLocker;
        MinHeap mTimerHeap;
        Mutex mTimerHeapLocker;
    };
    MessageQueueStorage *mStorage;
    
private:
    enum { FLAG_EMPTY, FLAG_NOT_EMPTY };
    volatile int32_t mFlag;
    volatile uint32_t mCurrNo;
    SpinLock mMapLocker;
    sgi::map<int, uint32_t> mCancelMap;
    Observer *mObserver;
};

} // namespace baselib

#endif // __BASE_LIB__MSG_FRW__MESSAGE_QUEUE_H__




