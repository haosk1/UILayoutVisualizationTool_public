/**
 * frame
 *
 * UI元器件基类
**/
#ifndef __FRAME__CACTOR_H__
#define __FRAME__CACTOR_H__

#include <float.h> // FLT_EPSILON
#include <math.h>  // fabs()

#include "event/EventDef.h"
#include "UICanvasInterface.h"
#include "tinyxml/tinyxml.h"
#include "RefBase/ref_base.h"

#include "AttributePlug/IDPlug.h"
#include "AttributePlug/MarginLayoutPlug.h"
#include "AttributePlug/PaddingPlug.h"
#include "AttributePlug/UILayoutGravityPlug.h"
#include "AttributePlug/VisiablePlug.h"
#include "AttributePlug/AbsLayoutPlug.h"//专门为AbsoluteLayout适配的属性插件
#include "AttributePlug/LinearLayoutPlug.h"//专门为LinearLayout适配的属性插件
#include "AttributePlug/RelativeLayoutPlug.h"//专门为RelativeLayoutPlug适配的属性插件

using namespace frame;
class CUIController;
class CActor : public baselib::RefBase
{
public:
    explicit CActor();
    virtual ~CActor();
    //xml属性设置
    virtual void Inflate(TiXmlAttribute* attribute);

    void Invalidate();
    void Invalidate(const UIRect &stRect);
    //背景
    bool SetDrawablePath(std::string strPath);
    unsigned int GetDrawableTextureID(std::string strPath);

    //属性插件 start
    const CIDPlug *GetIDPlug(void);
    void SetIDPlug(const CIDPlug &stParam);

    const CPaddingPlug *GetPaddingPlug(void);
    void SetPaddingPlug(const CPaddingPlug &stParam);

    const CMarginLayoutPlug *GetLayoutPlug(void);
    void SetLayoutPLug(const CMarginLayoutPlug &stParam);

    const CUILayoutGravityPlug *GetUILayoutGravityPlug(void);
    void SetUILayoutGravityPLug(const CUILayoutGravityPlug &stParam);

    const CVisiablePlug *GetVisiablePlug(void);
    void SetVisiablePlug(const CVisiablePlug &stParam);

    const CAbsLayoutPlug *GetAbsLayoutPlug(void);
    void SetAbsLayoutPlug(const CAbsLayoutPlug &stParam);

    const CLinearLayoutPlug *GetLinearLayoutPlug(void);
    void SetLinearLayoutPlug(const CLinearLayoutPlug &stParam);

    const CRelativeLayoutPlug *GetRelativeLayoutPlug(void);
    void SetRelativeLayoutPlug(const CRelativeLayoutPlug &stParam);
    //属性插件 end

    //自身描画尺寸相关
    void SetMeasuredDimension(int iMeasuredWidth, int iMeasureHeight, int iMeasureLength);
    void GetMeasuredDimension(int& iMeasuredWidth, int& iMeasureHeight, int& iMeasureLength);
    bool GetDrawableRect(unsigned int textureId, UIRect& outRect);

    //自身描画位置尺寸相关
    void GetHitRect(UIRect &stRect) const;
    void GetDrawRect(UIRect &stRect) const;
    int GetLeft();
    int GetTop();
    int GetFront();
    int GetWidth();
    int GetHeight();
    int GetLength();

    //事件
    virtual bool DispatchMouseEvent(Mouse_Event event);
    virtual bool DispatchKeyEvent(Keyboard_Event event);
    virtual bool OnMouseEvent(Mouse_Event event);
    virtual bool OnKeyEvent(Keyboard_Event event);

    //Measure是测量自身宽高，包括padding，入参是parent给出的最大值
    virtual void Measure(int iWidthMeasureSpec, int iHeightMeasureSpec, int iLengthMeasureSpec);
    //Layout是确定自身宽位置，为描画做最后测量；
    //入参是parent给出的基本位置，包括margin计算后；一般宽高即是measure最后计算结果的宽高
    virtual void Layout(const UIRect &stRect);
    virtual void Layout(int sLeft, int sTop, int sFront, int sWidth, int sHeight, int length);
    //描画
    void Draw(CUICanvasInterface &stCanvas);    

    //碰撞检测相关
    virtual bool NeedCheckCollision();
    virtual void Collision();

    //动画相关start
    void SetPivotX(float fPivotX);
    void SetPivotY(float fPivotY);
    void SetPivotZ(float fPivotZ);
    void SetScaleX(float fValue);
    void SetScaleY(float fValue);
    void SetScaleZ(float fValue);
    float GetTranslationX() const;
    void SetTranslationX(float fValue);
    float GetTranslationY() const;
    void SetTranslationY(float fValue);
    float GetTranslationZ() const;
    void SetTranslationZ(float fValue);
    float GetRotation(const MatrixRouteType& type) const;
    void SetRotation(float fValue, const MatrixRouteType& type);
    unsigned char GetAlpha() const;
    void SetAlpha(unsigned char ucValue);

    bool NeedComputeMatrix();
    void ComputeMatrixByAnimatorAttributes(CUIMatrix3 &stMatrix);
    void SetAnimatorAttributes(CUICanvasInterface &stCanvas);
    //动画相关end

    //是否是容器
    virtual bool IsStage();

protected:
    virtual void OnSizeChanged(int sNewWidth, int sNewHeight, int sNewLength, int sOldWidth, int sOldHeight, int sOldLength);
    virtual void OnMeasure(int iWidthMeasureSpec, int iHeightMeasureSpec, int iLengthMeasureSpec);
    virtual void OnLayout(bool bChanged, int sLeft, int sTop, int sFront, int sWidth, int sHeight, int length);
    virtual void OnDraw(CUICanvasInterface &stCanvas);

private:
    virtual bool SetFrame(int sLeft, int sTop, int sFront, int sWidth, int sHeight, int sLength);

protected:
    //自身测量结果：宽高长
    int m_sMeasureWidth = 0;
    int m_sMeasureHeight = 0;
    int m_sMeasureLength = 0;
    //属性插件
    CIDPlug* m_pstIDPlug = nullptr;
    CMarginLayoutPlug *m_pstLayoutParam = nullptr;
    CPaddingPlug *m_pstPaddingPlug = nullptr;
    CUILayoutGravityPlug *m_pstUILayoutGravityPlug = nullptr;
    CVisiablePlug *m_pstVisiablePlug = nullptr;
    CAbsLayoutPlug* m_pstAbsLayoutPlug = nullptr;
    CLinearLayoutPlug* m_pstLinearLayoutPlug = nullptr;
    CRelativeLayoutPlug* m_pstRelativeLayoutPlug = nullptr;

    bool m_IsRequestLayout = false;

private:
    //背景纹理id
    unsigned int m_textureId = 0;

    //元器件描画位置尺寸
    int m_sLeft = 0;
    int m_sTop = 0;
    int m_sFront = 0;
    int m_sWidth = 0;
    int m_sHeight = 0;
    int m_sLength = 0;
    int m_sScrollX = 0;
    int m_sScrollY = 0;

    //动画相关start
    float m_fPivotX = 0;
    float m_fPivotY = 0;
    float m_fPivotZ = 0;
    float m_fScaleX = 0;
    float m_fScaleY = 0;
    float m_fScaleZ = 0;
    float m_fTranslationX = 0;
    float m_fTranslationY = 0;
    float m_fTranslationZ = 0;
    float m_fXAngle = 0;
    float m_fYAngle = 0;
    float m_fZAngle = 0;
    unsigned char m_ucAlpha = 255;
    //动画相关end
};
#endif  // __FRAME__CACTOR_H__
