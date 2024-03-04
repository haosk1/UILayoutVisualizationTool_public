#ifndef __BASE_LIB__MSG_FRW__MESSAGE_CTRL_H_
#define __BASE_LIB__MSG_FRW__MESSAGE_CTRL_H_

#include "scoped_ptr.h"
#include "Synchronizable.h"
#include <stdio.h>

namespace baselib {

class Message;
class AckorRunnable;
class CompleteNotifier;

class MessageCtrl : public RefCountedThreadSafe<MessageCtrl> {

public:

    MessageCtrl();

    /**
     * 取消消息执行
     * @return true 取消成功，任务或者变为取消状态或者为完成状态，可以通过isComplete()判断  false 取消失败，消息正在执行
     * @note 如果调用此方法成功，说明消息处于完成状态或取消状态，如果为取消状态，如果为消息为Runnable调用，Runnable的run永远不会执行,onCancel函数也不会被调用，可以在接下来处理资源释放，例如可以delete掉Runnanble。如果失败说明消息处于运行状态，不能删除或改变状态。
     * @note 调用此函数取消Runnable，内存生命周期管理策略：在Runnable::run后释放内存或者在此函数调用返回true，且Runnable处于Cancel状态下，释放内存。
     * @note 例子：
     * @note   if(msgctrl->cancel() && msgctrl->isCanceled())
     * @note     delete runnable;
     */
    bool cancel() {
        return cancelInternal(false) != -1;
    }
    
    /**
     * 取消Runanble执行，如果取消成功在目标线程中调用Runnable的onCancel函数（注意：调用完此函数后，不可直接删除此Runnable）
     * @return true 取消成功，消息或者变为取消状态或者为完成状态，可以通过isComplete() 和 isCacel()判断  false 取消失败，消息正在执行
     * @note 如果调用此方法取消Runnable执行，说明Runnable处于完成状态或取消状态，如果为取消状态，Runnable的onCancel函数会在目标线程中被调用，切勿马上删除此Runnable，应在onCancel内释放，如果失败说明runnable处于运行状态，不能删除或改变状态。
     */
    bool cancelWithCallback() {
        return cancelInternal(true) != -1;
    }
    
    bool isComplete(){
        return mFlag == STATE_COMPLETE;
    }
    
    bool isCanceled(){
        return mFlag == STATE_CANCELED;
    }
    
    bool isNeedCallOnCancel(){
        return mFlag == STATE_CANCEL_WITH_CALLBACK;
    }
    
    bool isInProgress() {
        return mFlag == STATE_IN_PROGRESS;
    }
    
    bool isInQueue() {
        return mFlag == STATE_IN_QUEUE;
    }
    
    /*
     @brief 等待消息执行完成
     @param timeout 等待的毫秒数， 0 不等待 INFINIT 等待执行完毕
     @return 消息是否执行完毕
     */
    bool waitComplete(uint32_t timeout = INFINITE);
    
    ~MessageCtrl();
private:
    friend class Message;
    friend class AckorRunnable;
    int setInProgress();
    int setComplete();
    int cancelInternal(bool bCallback);
    bool doCompleteNotify();

private:
    
    enum { STATE_IN_QUEUE, STATE_IN_PROGRESS, STATE_COMPLETE, STATE_CANCELED, STATE_CANCEL_WITH_CALLBACK };

    volatile int32_t mFlag;
    volatile CompleteNotifier* mNotifier;
};

typedef ScopedRefptr<MessageCtrl> * MessageHandleObjPtr;
typedef ScopedRefptr<MessageCtrl> MessageHandleObj;

}

#endif // __BASE_LIB__MSG_FRW__MESSAGE_CTRL_H_



