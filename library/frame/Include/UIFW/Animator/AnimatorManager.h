

/**
 * frame
 *
 * Animator管理类
**/
#ifndef ANIMATORMANAGER_H
#define ANIMATORMANAGER_H

#include <unistd.h>
#include <list>

#include "event/TimerUtilsInterface.h"

#define ANIMATOR_UPDATE_TIME (5)
class CAnimator;
class CAnimatorManager
{
  public:
    CAnimatorManager();
    ~CAnimatorManager();
    void Start(CAnimator *);
    void Stop(CAnimator *);
    bool IsAnimatorRunning() const;


    void dealTimeout();

  private:
    void UpdateAnimator();

    TimerUtilsInterface* m_stTimer;
    uint32_t m_ulRunningAnimatorCnt;

    typedef std::list<CAnimator*> AnimatorContainer;
    AnimatorContainer m_stAnimatorContainer;

    std::list<AnimatorContainer::iterator> m_stAnimatorForErase;
    bool m_bIsProcessing;
};

#endif /*ANIMATORMANAGER_H*/
