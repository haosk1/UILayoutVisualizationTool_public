/**
 * frame
 *
 * 动画插值器
**/
#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

#include <stdint.h>
#include <float.h> // FLT_EPSILON
#include <math.h>

#include "RefBase/ref_base.h"

class CInterpolator : public baselib::RefBase
{
  public:
    CInterpolator();
    virtual ~CInterpolator()
    {
    }

    void SetID(uint32_t ulID);
    uint32_t GetID(void) const;
    /**
     * Maps a point on the timeline to a multiplier to be applied to the
     * transformations of an animation.
     *
     * param fInput must be between 0.0f - 1.0f.
     **/
    virtual float GetInterpolation(float fInput) const = 0;

  private:
    uint32_t m_ulID;
};

/**
 * An interpolator where the rate of change is constant
 *
 */
class CLinearInterpolator : public CInterpolator
{
  public:
    CLinearInterpolator();
    virtual ~CLinearInterpolator();

    virtual float GetInterpolation(float fInput) const;
};

/**
 * An interpolator where the rate of change starts out slowly and
 * and then accelerates.
 *
 */
class CAccelerateInterpolator : public CInterpolator
{
  public:
    CAccelerateInterpolator();
    explicit CAccelerateInterpolator(float fFactor);
    virtual ~CAccelerateInterpolator();

    virtual float GetInterpolation(float fInput) const;

  private:
    float m_fFactor;
    float m_fDoubleFactor;
};

/**
 * An interpolator where the rate of change starts out quickly and
 * and then decelerates.
 *
 */
class CDecelerateInterpolator : public CInterpolator
{
  public:
    CDecelerateInterpolator();
    explicit CDecelerateInterpolator(float fFactor);
    virtual ~CDecelerateInterpolator();

    virtual float GetInterpolation(float fInput) const;
    
  private:
    float m_fFactor;
};

/**
 * An interpolator where the rate of change starts and ends slowly but
 * accelerates through the middle.
 *
 */
class CAccelerateDecelerateInterpolator : public CInterpolator
{
  public:
    CAccelerateDecelerateInterpolator();
    virtual ~CAccelerateDecelerateInterpolator();

    virtual float GetInterpolation(float fInput) const;
};

/**
 * An interpolator where the change starts backward then flings forward.
 */
class CAnticipateInterpolator : public CInterpolator
{
  public:
    CAnticipateInterpolator();
    /**
     * @param fFactor Amount of anticipation. When fFactor equals 0.0f, there is
     *                no anticipation and the interpolator becomes a simple
     *                acceleration interpolator.
     */
    explicit CAnticipateInterpolator(float fFactor);
    virtual ~CAnticipateInterpolator();

    virtual float GetInterpolation(float fInput) const;

  private:
    float m_fFactor;
};

/**
 * An interpolator where the change flings forward and overshoots the last value
 * then comes back.
 */
class COvershootInterpolator : public CInterpolator
{
  public:
    COvershootInterpolator();
    /**
     * @param fFactor Amount of overshoot. When tension equals 0.0f, there is
     *                no overshoot and the interpolator becomes a simple
     *                deceleration interpolator.
     */
    explicit COvershootInterpolator(float fFactor);
    virtual ~COvershootInterpolator();

    virtual float GetInterpolation(float fInput) const;

  private:
    float m_fFactor;
};

/**
 * An interpolator where the change starts backward then flings forward and overshoots
 * the target value and finally goes back to the final value.
 */
class CAnticipateOvershootInterpolator : public CInterpolator
{
  public:
    CAnticipateOvershootInterpolator();
    /**
     * @param fFactor Amount of anticipation/overshoot. When fFactor equals 0.0f,
     *                there is no anticipation/overshoot and the interpolator becomes
     *                a simple acceleration/deceleration interpolator.
     */
    explicit CAnticipateOvershootInterpolator(float fFactor);

    /**
     * @param fFactor Amount of anticipation/overshoot. When fFactor equals 0.0f,
     *                there is no anticipation/overshoot and the interpolator becomes
     *                a simple acceleration/deceleration interpolator.
     * @param fExtraFactor Amount by which to multiply the fFactor. For instance,
     *                     to get the same overshoot as an OvershootInterpolator with
     *                     a fFactor of 2.0f, you would use an fExtraFactor of 1.5f.
     */
    CAnticipateOvershootInterpolator(float fFactor, float fExtraFactor);
    virtual ~CAnticipateOvershootInterpolator();

    virtual float GetInterpolation(float fInput) const;

  private:
    inline float Anticipate(float fInput, float fFactor) const;
    inline float OverShoot(float fInput, float fFactor) const;

    float m_fFactor;
};

/**
 * An interpolator where the change bounces at the end.
 */
class CBounceInterpolator : public CInterpolator
{
  public:
    CBounceInterpolator();
    virtual ~CBounceInterpolator();

    virtual float GetInterpolation(float fInput) const;

  private:
    inline float Bounce(float fInput) const;
};

/**
 * Repeats the animation for a specified number of cycles. The
 * rate of change follows a sinusoidal pattern.
 *
 */
class CCycleInterpolator : public CInterpolator
{
  public:
    CCycleInterpolator();
    explicit CCycleInterpolator(float fCycles);
    virtual ~CCycleInterpolator();

    virtual float GetInterpolation(float fInput) const;

  private:
    float m_fCycles;
};
/*Easing equation function
 */

//三次缓入函数
class CCubicEaseInInterpolator : public CInterpolator
{
  public:
    CCubicEaseInInterpolator();
    virtual ~CCubicEaseInInterpolator();

    virtual float GetInterpolation(float fInput) const;
};

//三次缓出函数
class CCubicEaseOutInterpolator : public CInterpolator
{
  public:
    CCubicEaseOutInterpolator();
    virtual ~CCubicEaseOutInterpolator();

    virtual float GetInterpolation(float fInput) const;
};

//三次缓入缓出函数
class CCubicEaseInOutInterpolator : public CInterpolator
{
  public:
    CCubicEaseInOutInterpolator();
    virtual ~CCubicEaseInOutInterpolator();

    virtual float GetInterpolation(float fInput) const;
};

//四次缓入函数
class CQuarticEaseInInterpolator : public CInterpolator
{
  public:
    CQuarticEaseInInterpolator();
    virtual ~CQuarticEaseInInterpolator();

    virtual float GetInterpolation(float fInput) const;
};

//四次缓出函数
class CQuarticEaseOutInterpolator : public CInterpolator
{
  public:
    CQuarticEaseOutInterpolator();
    virtual ~CQuarticEaseOutInterpolator();

    virtual float GetInterpolation(float fInput) const;
};

//四次缓入缓出函数
class CQuarticEaseInOutInterpolator : public CInterpolator
{
  public:
    CQuarticEaseInOutInterpolator();
    virtual ~CQuarticEaseInOutInterpolator();

    virtual float GetInterpolation(float fInput) const;
};

//五次缓入函数
class CQuinticEaseInInterpolator : public CInterpolator
{
  public:
    CQuinticEaseInInterpolator();
    virtual ~CQuinticEaseInInterpolator();

    virtual float GetInterpolation(float fInput) const;
};

//五次缓出函数
class CQuinticEaseOutInterpolator : public CInterpolator
{
  public:
    CQuinticEaseOutInterpolator();
    virtual ~CQuinticEaseOutInterpolator();

    virtual float GetInterpolation(float fInput) const;
};

//五次缓入缓出函数
class CQuinticEaseInOutInterpolator : public CInterpolator
{
  public:
    CQuinticEaseInOutInterpolator();
    virtual ~CQuinticEaseInOutInterpolator();

    virtual float GetInterpolation(float fInput) const;
};

//正弦缓入函数
class CSineEaseInInterpolator : public CInterpolator
{
  public:
    CSineEaseInInterpolator();
    virtual ~CSineEaseInInterpolator();

    virtual float GetInterpolation(float fInput) const;
};

//正弦缓出函数
class CSineEaseOutInterpolator : public CInterpolator
{
  public:
    CSineEaseOutInterpolator();
    virtual ~CSineEaseOutInterpolator();

    virtual float GetInterpolation(float fInput) const;
};

//正弦缓入缓出函数
class CSineEaseInOutInterpolator : public CInterpolator
{
  public:
    CSineEaseInOutInterpolator();
    virtual ~CSineEaseInOutInterpolator();

    virtual float GetInterpolation(float fInput) const;
};
#endif /*INTERPOLATOR_H*/
