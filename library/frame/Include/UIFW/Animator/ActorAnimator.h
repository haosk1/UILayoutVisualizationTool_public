/**
 * frame
 *
 * 帧动画基类
**/
#ifndef ACTORANIMATOR_H
#define ACTORANIMATOR_H

#include "UIFW/Actor.h"

#include "Animator.h"

class CActor;
class CActorAnimator : public CAnimator
{
  public:
    CActorAnimator();
    CActorAnimator(CActor *pstActor);
    CActorAnimator(const CActorAnimator &stOther);
    virtual ~CActorAnimator();

    virtual CActorAnimator *ToActorAnimator();
    virtual bool Start();
    virtual void SetTarget(CActor *pstActor);
    CActor *GetTarget() const;

    bool dealFormHide();
    void SetRunInAnimatorSet(bool bValue);
    bool IsRunInAnimatorSet() const;

  protected:
    friend class CActorAnimatorSet;
    virtual bool Initialized();

    RefCountWeakPtr<CActor> m_pstActor;

  private:
    bool m_bRunInAnimatorSet;
};

#endif /*ACTORANIMATOR_H*/
