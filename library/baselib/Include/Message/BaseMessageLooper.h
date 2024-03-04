#ifndef _BASE_MESSASGE_LOOPER_H_
#define _BASE_MESSASGE_LOOPER_H_

#include "IMessageLooper.h"

namespace baselib {
    
    class LooperObserverMan {
    public:
       struct ObserverNode {
           IMessageLooper::Observer * observer;
           ObserverNode * next;
       };
       
       LooperObserverMan();
       ~LooperObserverMan();
       
       bool addObserver(IMessageLooper::Observer * observer);
       bool removeObserver(IMessageLooper::Observer * observer);
       
       bool isEmpty() const {
           return _observers == NULL;
       }
       
       void notifyStart(IMessageLooper * looper, int queue_size, int delay_queue_size);
       void notifyPostMsg(IMessageLooper * looper, Message * msg, uint32_t delay);
       void notifyStartMsg(IMessageLooper * looper, Message * msg, uint64_t timing, uint64_t now);
       void notifyEndMsg(IMessageLooper * looper, Message * msg, uint64_t timing, uint64_t now, uint32_t duration);
       void notifyBusy(IMessageLooper * looper);
       void notifyIdle(IMessageLooper * looper, int delay_queue_size);
       void notifyQuit(IMessageLooper * looper);
       void notifyDestroy(IMessageLooper * looper);
       void notifyCancelMsg(IMessageLooper * looper, Message * msg, uint64_t timing, uint64_t now);
       
   private:
       ObserverNode * _observers;
   };

    class MessageCountInfoInternal;
    class MessageHandler;
    class BaseMessageLooper : public IMessageLooper{
    public:
        BaseMessageLooper();
        ~BaseMessageLooper();
        bool onProcMessage(Message * msg);
        void quit();
        
        MessageHandler * getDefHandler();
        baselib::ScopedRefptr<LooperHandler> getLooperHandler();
        
        bool addObserver(Observer * observer);
        bool removeObserver(Observer * observer);
        
    protected:
        bool mQuitting;
        MessageHandler * mDefHandler;
        LooperObserverMan mObserverMan;
        baselib::ScopedRefptr<LooperHandler> mLooperHandler;
    };
    
};

#endif
