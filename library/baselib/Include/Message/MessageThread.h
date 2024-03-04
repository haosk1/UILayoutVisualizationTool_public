/**
 * 
 *
 * 消息循环线程工具
**/

#ifndef __ASL_BASE__MSG_FRW__MESSAGE_THREAD_H__
#define __ASL_BASE__MSG_FRW__MESSAGE_THREAD_H__

#include "asl_base/asl_memory.h"
#include "asl_message_framework/Thread.h"
#include "asl_message_framework/MessageHandler.h"
#include "asl_message_framework/Message.h"

namespace asl {
    class MessageQueue;
    class IMessageLooper;
    class ASL_MSGFRAMEWORK_EXPORT MessageThread : public Thread, public IMessageDisposer {
    public:
        /*
          @brief 构造函数
          @param szThreadName 线程名字
          @return 无
          @detail 注意在linux系统下线程名字不能超过16字符，会自动截断
        */
        MessageThread(const char *szThreadName, int size = 2048);
        MessageThread(const char *szThreadName, MessageQueue *queue);
        /*
         @brief 析构函数
        */
        ~MessageThread();

        /*
          @brief 退出线程
          @param wait 是否等待线程退出，函数返回
          @return 无
        */
        bool start();
        
        /*
         @brief 获取线程MessageHandler
         @return 返回MessageHandler，
         @detail 返回的MessageThread由MessageThread管理声明周期，外部不得释放
        */
        MessageHandler* getHandler();
        
        /*
         @brief 退出线程
         @param wait 是否等待线程退出，函数返回
         @return 无
         @detail 此函数向消息队列发出quit消息，等待线程排队处理，quit消息处理完毕，线程消息循环退出
        */
        void quit(bool wait = true);

        /*
         @brief 获取线程消息队列即时消息个数
         @return 消息队列即时消息个数
        */
        uint32_t queueSize() const;
        
        /*
         @brief 获取线程消息队列延时消息个数
         @param delay 获取最近延时消息触发时间间隔
         @return 消息队列延时消息个数
        */
        uint32_t heapSize(uint32_t * delay = NULL) const;
        
        /*
          @brief 线程循环退出后，销毁未执行的消息
          @param msg 待销毁的消息
          @param isRunnable 是否为Runnable
          @param delay_message 是否延时消息
          @return 无
          @detail 线程循环退出时，在线程上下文中被调用
        */
        virtual void disposeMessage(Message *msg, bool isRunnable, bool delay_message);
        
    protected:
        
        /*
         @brief 线程主循环
         @return 无
        */
        void run();
        
        /*
          @brief 线程初始化
          @return 无
          @detail 线程循环开始前，在线程上下文中被调用
        */
        virtual void onInit() {}
        
        /*
          @brief 消息处理函数
          @param message 将要处理的消息
          @return 忽略
          @detail 处理来自MessageHandler(仅通过this->getHandler获取到的)发送的消息
                  注意，外部构造的MessageHandler发送的消息，不回调此函数
        */
        virtual int onMessage(Message& message) { return 0; };
        
        /*
          @brief 线程循环退出
          @return 无
          @detail 线程循环退出时，在线程上下文中被调用
        */
        virtual void onDestroy() {};
        
        friend class MessageThreadHandler;
        
    private:
        asl::unique_ptr<MessageHandler> _handler;
        asl::unique_ptr<IMessageLooper> _looper;
    };

} //namespace asl

#endif // __ASL_BASE__MSG_FRW__MESSAGE_THREAD_H__




