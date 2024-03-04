
/**
 * frame
 *
 * 帧动画之旋转动画
**/
#ifndef ACTORROTATEANIMATOR_H
#define ACTORROTATEANIMATOR_H

#include "ActorAnimator.h"

class CActorRotateAnimator : public CActorAnimator
{
  public:
    explicit CActorRotateAnimator(float fFromDegree, float fToDegree, MatrixRouteType type = MatrixRouteType::Z);
    explicit CActorRotateAnimator(float fFromDegree, float fToDegree, float fPivotX, float fPivotY, float fPivotZ = 0.0, MatrixRouteType type = MatrixRouteType::Z);
    explicit CActorRotateAnimator(float fFromDegree, float fToDegree, int iPivotXType, float fPivotXValue, int iPivotYType,float fPivotYValue, MatrixRouteType type = MatrixRouteType::Z);
    explicit CActorRotateAnimator(const CActorRotateAnimator &stOther);
    virtual ~CActorRotateAnimator();

    virtual RefCountPtr<CAnimator> Clone(void) const;

  protected:
    virtual void Initialize_impl(const UIRect &stSelfRect, const UIRect &stParentRect, const UIRect &stFormRect);

  private:
    virtual void AnimatorUpdate_Impl(float fInterpolatedFraction);

  private:
    float m_fFromDegree = 0.0f;
    float m_fToDegree = 0.0f;
    MatrixRouteType m_eType = MatrixRouteType::Z;
    float m_fPivotX = 0.0f;
    float m_fPivotY = 0.0f;
    float m_fPivotZ = 0.0f;
    int m_iPivotXType = XYType_Absolute;
    float m_fPivotXValue = 0.0f;
    int m_iPivotYType = XYType_Absolute;
    float m_fPivotYValue = 0.0f;
};

#endif /*ACTORROTATEANIMATOR_H*/
