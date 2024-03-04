/**
 * frame
 *
 * 动画基类
**/
#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <stdint.h>

#include "RefBase/ref_base.h"
#include "Interpolator.h"
#include "UIFW/UIRect.h"

using namespace baselib;
class UIController;
class CActorAnimator;
class CAnimator : public RefBase
{
  public:
    enum
    {
        Status_Uninitialized,
        Status_Initialized,
        Status_WaitStart,
        Status_Started,
    };

    enum
    {
        RepeatMode_Restart,
        RepeatMode_Reverse,
    };

    //以元器件视口左上角为原点的坐标系
    enum
    {
        //绝对距离
        XYType_Absolute,
        //自身的范围
        XYType_Self,
        // Parent的范围
        XYType_Parent,
        // Parent的范围，向左平移Self宽度，向上平移Self高度后的范围
        XYType_SelfAdjustParent,
        // Form范围，一般即是屏幕范围
        XYType_Form,
        // Form的范围，向左平移Self宽度，向上平移Self高度后的范围
        XYType_SelfAdjustForm,
        // Parent的范围减去一个Self的范围
        XYType_ParentSubSelf,
    };

  public:
    CAnimator();
    CAnimator(const CAnimator &stOther);
    virtual ~CAnimator();

    virtual CActorAnimator *ToActorAnimator();


    bool HasStarted() const;
    bool HasInitialized(void) const;

    virtual void SetInterpolator(const RefCountPtr<CInterpolator> &pstInterpolator);
    const RefCountPtr<CInterpolator> &GetInterpolator(void) const;

    virtual void SetStartOffset(uint32_t ulStartOffset);
    uint32_t GetStartOffset(void) const;

    virtual void SetDuration(uint32_t ulDuration);
    uint32_t GetDuration(void) const;

    virtual void SetRepeatMode(int iRepeatMode);
    int GetRepeatMode(void) const;

    virtual void SetRepeatCount(uint32_t ulRepeatCount);
    uint32_t GetRepeatCount(void) const;

    virtual void SetFillBefore(bool bFillBefore);
    bool IsFillBefore(void) const;

    virtual void SetKeepAnimatedStatus(bool bValue);
    bool IsKeepAnimatedStatus(void) const;

    float GetFraction(void) const;

    virtual RefCountPtr<CAnimator> Clone(void) const;
    virtual bool Inflate();

    virtual bool Start();
    virtual bool Stop();
    //就地停止，动画属性值保持应有的值
    virtual bool StopOnTheSpot();
    virtual void ResumeInitState();
    virtual uint32_t ComputeDurationHint();

    void SetUIResource();

  protected:
    float ResolveSizeWidth(int iType, float fValue, const UIRect &stSelfRect, const UIRect &stParentRect,
                           const UIRect &stFormRect);
    float ResolveSizeHeight(int iType, float fValue, const UIRect &stSelfRect, const UIRect &stParentRect,
                            const UIRect &stFormRect);

    float CalcInterpolatedFraction(uint32_t ulCurrentTime);

    void EnsureInterpolator(void);
    virtual void Reset(void);
    virtual bool Initialized();
    virtual void Initialize_impl(const UIRect &stSelfRect, const UIRect &stParentRect, const UIRect &stFormRect);
    bool Start_Impl();

  private:
    friend class CAnimatorManager;
    void Update();
    virtual void AnimatorUpdate_Impl(float fInterpolatedFraction);

  protected:
    uint32_t m_ulDuration;
    uint32_t m_ulRepeatCount;
    int m_iRepeatMode;
    uint32_t m_ulStartTime;
    int m_iStatus;
    bool m_bCycleFlipped;
    uint32_t m_ulRepeated;
    uint32_t m_ulStartOffset;
    uint32_t m_ulStartOffsetExtra;
    float m_fFraction;
    bool m_bFillBefore;
    bool m_bKeepAnimatedStatus;

  private:
    bool m_bLastFrameDrawn;
  
  protected:
    RefCountPtr<CInterpolator> m_pstInterpolator;
};

#endif /*ANIMATOR_H*/
