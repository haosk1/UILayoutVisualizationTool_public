/**
 * frame
 *
 * 帧动画之平移动画
**/
#ifndef ACTORTRANSLATEANIMATOR_H
#define ACTORTRANSLATEANIMATOR_H

#include "ActorAnimator.h"

class CActorTranslateAnimator : public CActorAnimator
{
  public:
    explicit CActorTranslateAnimator(float fFromXDelta, float fToXDelta, float fFromYDelta, float fToYDelta, float fFromZDelta, float fToZDelta);
    explicit CActorTranslateAnimator(int iFromXType, float fFromXValue, int iToXType, float fToXValue, 
                                     int iFromYType,float fFromYValue, int iToYType, float fToYValue);

    explicit CActorTranslateAnimator(const CActorTranslateAnimator &stOther);
    virtual ~CActorTranslateAnimator();

    virtual RefCountPtr<CAnimator> Clone(void) const;

  protected:
    virtual void Initialize_impl(const UIRect &stSelfRect, const UIRect &stParentRect, const UIRect &stFormRect);

  private:
    virtual void AnimatorUpdate_Impl(float fInterpolatedFraction);

    float m_fFromXDelta = 0.0f;
    float m_fToXDelta = 0.0f;
    float m_fFromYDelta = 0.0f;
    float m_fToYDelta = 0.0f;
    float m_fFromZDelta = 0.0f;
    float m_fToZDelta = 0.0f;
    int m_iFromXType = XYType_Absolute;
    float m_fFromXValue = 0.0f;
    int m_iToXType= XYType_Absolute;
    float m_fToXValue = 0.0f;
    int m_iFromYType= XYType_Absolute;
    float m_fFromYValue = 0.0f;
    int m_iToYType= XYType_Absolute;
    float m_fToYValue = 0.0f;
};

#endif /*ACTORTRANSLATEANIMATOR_H*/
