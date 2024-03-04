
/**
 * frame
 *
 * actor容器基类
**/

#ifndef STAGE_H
#define STAGE_H

#include <vector>
#include <algorithm>

#include "Actor.h"

class CStage : public CActor
{
public:
    CStage();
    virtual ~CStage();
    
//Form实现类 调用start
    int GetChildCount(void) const;
    CActor *GetChildAt(int iIndex);
    const CActor *GetChildAt(int iIndex) const;
    virtual CActor *FindActorByID(unsigned int ulActorID);

    virtual bool AddActor(CActor *pstActor);
    virtual bool UpdateActorLayout(CActor *pstActor, const UIRect &stParam);
    virtual bool RemoveActor(CActor *pstActor);
//Form实现类 调用end

//Form基类 调用
    virtual bool DispatchMouseEvent(Mouse_Event event) override;
    virtual bool DispatchKeyEvent(Keyboard_Event event) override;
    virtual bool IsStage() override;
    
protected:
    void MeasureChild(CActor *pstActor, int iWidthParentMeasureSpec, int iHeightParentMeasureSpec, int iLengthParentMeasureSpec);
    void MeasureChildWithMargin(CActor *pstActor, int iWidthParentMeasureSpec, int iHeightParentMeasureSpec, int iLengthParentMeasureSpec);
    int GetChildMeasureSpec(CActor *pstActor, int iActorMeasureSpace, int iChildDimension);

    virtual void OnMeasure(int iWidthMeasureSpec, int iHeightMeasureSpec, int iLengthMeasureSpec) override;
    virtual void OnLayout(bool bChanged, int sLeft, int sTop, int sFront, int sWidth, int sHeight, int length) override;
    virtual void OnDraw(CUICanvasInterface &stCanvas) override;
    virtual void DispatchDraw(CUICanvasInterface &stCanvas);

private:
    CStage(const CStage &);
    CStage &operator=(const CStage &);

    typedef std::vector<CActor*> ChildVec;
    ChildVec m_stChildVec;

};

#endif /*#ifndef STAGE_H*/