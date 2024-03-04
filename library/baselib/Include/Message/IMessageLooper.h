#ifndef _ASL_ILOOPER_H_
#define _ASL_ILOOPER_H_

#include "Message.h"

#define ASL_NOINLINE
#define ASL_DEPRECATED(MSG)

namespace baselib {
    
    class LooperHandler;
    class MessageHandler;
    class MessageQueue;
    class IMessageLooper {
    public:
        //定义观察者
        class Observer {
        public:
            virtual ~Observer() {}
            
            virtual void onLooperStart(IMessageLooper * looper, int queue_size, int delay_queue_size) {};
            virtual void onLooperPostMsg(IMessageLooper * looper, Message * msg, uint32_t delay) {};
            virtual void onLooperStartMsg(IMessageLooper * looper, Message * msg, uint64_t timing, uint64_t now) {};
            virtual void onLooperEndMsg(IMessageLooper * looper, Message * msg, uint64_t timing, uint64_t now, uint32_t duration) {};
            virtual void onLooperBusy(IMessageLooper * looper) {};
            virtual void onLooperIdle(IMessageLooper * looper, int delay_queue_size) {};
            virtual void onLooperQuit(IMessageLooper * looper) {};
            virtual void onLooperDestroy(IMessageLooper * looper) {};
            virtual void onLooperCancelMsg(IMessageLooper * looper, Message * msg, uint64_t timing, uint64_t now) {}
        };
        
    public:
        //获取设置的唯一looper
        static IMessageLooper* getForThread();
        //设置唯一looper，应该是单例懒汉，在线程开始后设置进来
        static void setForThread(const IMessageLooper* looper);
        
    public:
        IMessageLooper() : attachId(-1), quitting(false), quitted(false) {}
        virtual ~IMessageLooper() {}
        
        virtual void loop() = 0;
        virtual void quit() = 0;
        
        virtual bool onProcMessage(Message* msg) = 0;
        virtual bool addObserver(Observer * observer) = 0;
        virtual bool removeObserver(Observer * observer) = 0;
        
        virtual bool postMessage(Message* message, unsigned long when) = 0;
        virtual bool postDelayMessage(Message * message, uint32_t delay);
        
        virtual MessageHandler * getDefHandler() = 0;
        virtual baselib::ScopedRefptr<LooperHandler> getLooperHandler() = 0;
        
        virtual void clearQueue(IMessageDisposer* disposer = NULL) = 0;
        
        virtual uint32_t getQueueSize() const;
        virtual uint32_t getDelayQueueSize(uint32_t* delay) const;
        virtual uint32_t getHigherQueueSize() const;
        virtual uint32_t getHighestQueueSize() const;
        
        ASL_DEPRECATED("will not support") virtual void removeMsgByID(int msgid);
        
    public:
        struct DebugInfo {
            unsigned long id;
            const char* name;
            unsigned int finishCounter;
            unsigned int queueCounter;
        };

        ASL_DEPRECATED("will not support") DebugInfo* DEBUG_INFO;
        int attachId;
        bool quitting;
        bool quitted;
    };

    class LooperHandler : public RefCountedThreadSafe<LooperHandler> {
    public:
        LooperHandler(IMessageLooper * looper);
        ~LooperHandler();
        
        IMessageLooper * getLooper() const;
        bool postDelayMessage(Message * message, uint32_t delay);
        bool isQuit() const;
        bool quit(bool blocking = true);
        
    private:
        uint32_t _flags;
        IMessageLooper * _looper;
    };
    
}

#endif
