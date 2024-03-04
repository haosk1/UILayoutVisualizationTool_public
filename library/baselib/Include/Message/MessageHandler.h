/**
 * BaseLib
 *
 * 消息对像发送处理，包含looper的handler;也能通过static方法获取一个新的message
**/

#ifndef __BASE_LIB__MSG_FRW__MESSAGE_HANDLER_H__
#define __BASE_LIB__MSG_FRW__MESSAGE_HANDLER_H__

#include "Message.h"
#include "IMessageLooper.h"

namespace baselib {

#define FROM_HERE ((uint64_t)__FILE__ | (uint64_t)__LINE__ << 48)
inline const char * getFromHere(uint64_t from_here, int& line) {
    line = from_here >> 48;
    return (const char *)(from_here & 0xffffffffffff);
}

class Looper;
class IMessageLooper;
class MessageHandler {
public:
    //发送message时必须有looper
    MessageHandler() {
        IMessageLooper * looper = IMessageLooper::getForThread();
        if(looper)
            _looper_handler = looper->getLooperHandler();
    }
    //发送message时必须有looper
    MessageHandler(IMessageLooper* looper) : _looper_handler(looper->getLooperHandler()) { ; }
    virtual ~MessageHandler() { _looper_handler = NULL; }

    ASL_DEPRECATED("will not support")
    IMessageLooper * getLooper();

public:
    //获取一个新的message
    Message* obtainMessage() { return Message::get(this); }
    Message* obtainMessage(int what){ return Message::get(this, what); };

    /**
     * 向消息队列发送执行Runnable的消息
     * @param [in] r 待执行的Runnable，会在消息队列所在的线程环境执行
     * @param [in, out] pctrl消息控制，可以取消已经发送的消息，判断当前消息的执行状态等
     * @note 例子：
     * @note
     * @note MessageHandleObj pctrl;
     * @note handler->post(runnalbe, &pctrol);
     * @note pctrol->cancel();
     */
    bool post(Runnable* r, MessageHandleObjPtr pctrl = NULL) {
        return postToQueue(r, 0, 0, Message::PRIORITY_NORMAL, pctrl);
    }
    
    bool post(Runnable* r, msg_source_t from, MessageHandleObjPtr pctrl = NULL) {
        return postToQueue(r, 0, from, Message::PRIORITY_NORMAL, pctrl);
    }
    
    bool post(Runnable* r, msg_source_t from, int priority, MessageHandleObjPtr pctrl = NULL) {
        return postToQueue(r, 0, from, priority, pctrl);
    }
    /**
     * 向消息队列发送执行Runnable的消息，Runnable会延时执行
     * @param [in] r 待执行的Runnable，会在消息队列所在的线程环境执行
     * @param [in, out] pctrl消息控制，可以取消已经发送的消息，判断当前消息的执行状态等
     */
    bool postDelayed(Runnable* r, unsigned long delayMillis, MessageHandleObjPtr pctrl = NULL) {
        return postToQueue(r, (uint32_t)delayMillis, 0, Message::PRIORITY_NORMAL, pctrl);
    }
    
    bool postDelayed(Runnable* r, unsigned long delayMillis, msg_source_t from, MessageHandleObjPtr pctrl = NULL) {
        return postToQueue(r, (uint32_t)delayMillis, from, Message::PRIORITY_NORMAL, pctrl);
    }
    
    bool postDelayed(Runnable* r, unsigned long delayMillis, msg_source_t from, int priority, MessageHandleObjPtr pctrl = NULL) {
        return postToQueue(r, (uint32_t)delayMillis, from, priority, pctrl);
    }
    /**
     * 向消息队列发送消息
     * @param [in] Message待发送的消息，handleMessage函数通过what字段处理消息
     * @param [in] pctrl消息控制，可以取消已经发送的消息，判断当前消息的执行状态等
     * @note 例子：
     * @note Message * msg = MessageHandler::obtainMessage(cmd);
     * @note MessageHandleObj pctrl;
     * @note handler->sendMessage(msg, &pctrol);
     * @note pctrol->cancel();
     */
    bool sendMessage(Message* msg, MessageHandleObjPtr pctrl = NULL) {
        return postToQueue(msg, 0, 0, Message::PRIORITY_NORMAL, pctrl);
    }
    
    bool sendMessage(Message* msg, msg_source_t from, MessageHandleObjPtr pctrl = NULL) {
        return postToQueue(msg, 0, from, Message::PRIORITY_NORMAL, pctrl);
    }
    
    bool sendMessage(Message* msg, msg_source_t from, int priority, MessageHandleObjPtr pctrl = NULL) {
        return postToQueue(msg, 0, from, priority, pctrl);
    }
    
    /**
     * 向消息队列发送消息
     * @param [in] Message待发送的消息，handleMessage函数通过what字段处理消息
     * @param [in] delayMillis 延时执行，消息在延时delayMillis毫秒后执行
     * @param [in] pctrl消息控制，可以取消已经发送的消息，判断当前消息的执行状态等
     */
    bool sendMessageDelayed(Message* msg, unsigned long delayMillis, MessageHandleObjPtr pctrl = NULL) {
        return postToQueue(msg, (uint32_t)delayMillis, 0, Message::PRIORITY_NORMAL, pctrl);
    }
    
    bool sendMessageDelayed(Message* msg, unsigned long delayMillis, msg_source_t from, MessageHandleObjPtr pctrl = NULL) {
        return postToQueue(msg, (uint32_t)delayMillis, from, Message::PRIORITY_NORMAL, pctrl);
    }
    
    bool sendMessageDelayed(Message* msg, unsigned long delayMillis, msg_source_t from, int priority, MessageHandleObjPtr pctrl = NULL) {
        return postToQueue(msg, (uint32_t)delayMillis, from, priority, pctrl);
    }
    
    ASL_DEPRECATED("use same name with delayMillis param")
    bool postAtTime(Runnable* r, unsigned long uptimeMillis, MessageHandleObjPtr pctrl = NULL);
    /**
     * 取消Runnable执行
     * @param [in] r 待取消的Runnable，此Runnable不能被释放或者为空
     * @return true 取消成功，任务或者变为取消状态或者为完成状态 false 取消失败，Runnable正在执行
     * @note 如果调用此方法成功，说明消息处于完成状态或取消状态。如果为取消状态，Runnable的run永远不会执行,onCancel函数也不会被调用，可以在接下来处理资源释放，例如可以delete掉Runnanble。如果失败说明消息处于运行状态，不能删除或改变状态。
     * @note 调用此函数取消Runnable，内存生命周期管理策略：在Runnable::run后释放内存或者在此函数调用返回true，释放内存。
     * @note 例子：
     * @note   if(handler->removeCallback(r))
     * @note     delete r;
     */
    ASL_DEPRECATED("will not support")
    bool removeCallback(Runnable* r) { return r ? r->cancel() : true; }
    
    /**
     * 取消Runnable执行
     * @param [in] r 待取消的Runnable，此Runnable不能被释放或者为空
     * @return true 取消成功，任务或者变为取消状态或者为完成状态 false 取消失败，Runnable正在执行
     * @note 如果调用此方法成功，说明消息处于完成状态或取消状态。如果为取消状态，Runnable的run永远不会执行,onCancel会稍后在线程上下文中调用。注意：不能马上释放此Runnable，目标线程还要调用onCancel，可以在内部释放。
     */
    ASL_DEPRECATED("will not support")
    bool removeCallbackWithCallback(Runnable * r) { return r ? r->cancelWithCallback() : true; }
    
    /**
     * 取消上一次Runnable的执行，重新放入新的runnable
     * @param [in] r 待操作的Runnable，此Runnable不能被释放或者为空
     */
    ASL_DEPRECATED("will not support")
    bool postAndMerge(Runnable* r);
    
    ASL_DEPRECATED("use same name with delayMillis param")
    bool sendMessageAtTime(Message* msg, unsigned long uptimeMillis, MessageHandleObjPtr pctrl = NULL);

    /**
     * 取消某类消息的执行
     * @param [in] MsgID 消息id，Message的what值
     * @note 由于消息是异步的，消息是否取消成功根消息目前执行状态有关，如果消息处于队列中，还没有被执行，消息会被成功取消，消息如果正在执行，则消息不能被取消
     */
    ASL_DEPRECATED("will not support")
    void removeMessage(int MsgID);
    
    ASL_DEPRECATED("will not support")
    bool looperNotReady() {
        if(_looper_handler.get() == NULL || _looper_handler->isQuit())
            return true;
        
        return false;
    }

public:
        /**
         * Handles a message.
         */
        virtual void handleMessage(Message& message) const { }

private:
        bool postToQueue(Message * msg, uint32_t delay, msg_source_t from, int priority, MessageHandleObjPtr pctrl);
        bool postToQueue(Runnable * run, uint32_t delay, msg_source_t from, int priority, MessageHandleObjPtr pctrl);
        
protected:
        baselib::ScopedRefptr<LooperHandler> _looper_handler;
};

} //namespace baselib 

#endif // __BASE_LIB__MSG_FRW__MESSAGE_HANDLER_H__




