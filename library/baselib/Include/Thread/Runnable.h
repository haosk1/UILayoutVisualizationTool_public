/**
 * BaseLib
 *
 * runable
**/

#ifndef __BASE_LIB__RUNNABLE_H__
#define __BASE_LIB__RUNNABLE_H__

#include "../Message/MessageCtrl.h"

namespace baselib{

    class MessageCtrl;
    class Runnable {

    protected:
        Runnable();

    public:
        /*
            @brief 异步Runnable被成功取消后，调用此函数通知使用者释放资源
         */
        virtual void run() = 0;
        /*
            @brief 异步Runnable被成功取消后，调用此函数通知使用者释放资源
                 另外还有种情况会调用onCancel：消息在线程退出后，MessageThread释放前，发到了线程队列里；
                    或者线程退出时，延时任务得不到执行；调用此函数释放关联的资源
         */
        virtual void onCancel() {};
        
        virtual ~Runnable() {}
        
    private:
        /*
            @brief 取消异步Runnable的执行，调用cancel的瞬间，可能有三种情况  1. Runnalbe还未执行 2. 此Runnable正在执行   3. 此Runnable被执行完毕
                情况1，3返回ture，情况2返回false。当取消成功后，此函数不调用Runnable::onCancel
         */
        bool cancel();
        /*
            @brief 功能同cancel，此外，在情况1时，调用Runnable::onCancel
         */
        bool cancelWithCallback();
        
    private:
        friend class Message;
        friend class MessageHandler;
    };

} //namespace baselib 

#endif // __BASE_LIB__RUNNABLE_H__



