
/**
 * frame
 *
 * 帧动画之缩放动画
**/
#ifndef ACTORSCALEANIMATOR_H
#define ACTORSCALEANIMATOR_H

#include "ActorAnimator.h"

class CActorScaleAnimator : public CActorAnimator
{
  public:
    explicit CActorScaleAnimator(float fFromXScale, float fToXScale, float fFromYScale, float fToYScale, float fPivotX, float fPivotY
                          , float fFromZScale = 1.0f, float fToZScale = 1.0f, float fPivotZ = 0.0f);
    explicit CActorScaleAnimator(float fFromXScale, float fToXScale, float fFromYScale, float fToYScale, 
                        int iPivotXType, float fPivotXValue, int iPivotYType, float fPivotYValue,
                        float fFromZScale = 1.0f, float fToZScale = 1.0f, float fPivotZ = 0.0f);
    explicit CActorScaleAnimator(const CActorScaleAnimator &stOther);
    virtual ~CActorScaleAnimator();

    virtual RefCountPtr<CAnimator> Clone(void) const;

  protected:
    virtual void Initialize_impl(const UIRect &stSelfRect, const UIRect &stParentRect, const UIRect &stFormRect);

  private:
    virtual void AnimatorUpdate_Impl(float fInterpolatedFraction);

  private:
    float m_fFromXScale = 1.0f;
    float m_fToXScale = 1.0f;
    float m_fFromYScale = 1.0f;
    float m_fToYScale = 1.0f;
    float m_fPivotX = 0.0f;
    float m_fPivotY = 0.0f;
    int m_iPivotXType = XYType_Absolute;
    float m_fPivotXValue = 0.0f;
    int m_iPivotYType = XYType_Absolute;
    float m_fPivotYValue = 0.0f;
    float m_fFromZScale = 1.0f;
    float m_fToZScale = 1.0f;
    float m_fPivotZ = 0.0f;
};

#endif /*ACTORSCALEANIMATOR_H*/
