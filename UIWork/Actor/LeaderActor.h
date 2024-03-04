/**
 * frame
 *
 * 角色元器件
**/
#ifndef __FRAME__LEADERCACTOR_H__
#define __FRAME__LEADERCACTOR_H__

#include <map>
#include <list>
#include <vector>

#include "UIFW/Actor.h"
// #include "Include/ActorTranslateAnimator.h"
// #include "ActorScaleAnimator.h"
// #include "TimerUtils.h"
class CLeaderActor : public CActor
{
public:
    explicit CLeaderActor(){};
    virtual ~CLeaderActor(){};
    typedef enum
    {
        Leader_Error,//错误
        Leader_One,//主角色
        Leader_Two,//次角色
    } LeaderIndex;

    void SetLeaderIndex(LeaderIndex index);
    void SetLeaderTemp(int iTemp);
    virtual bool OnMouseEvent(Mouse_Event event);
    virtual void OnDraw(CUICanvasInterface &stCanvas);
    virtual bool NeedCheckCollision();
    virtual void Collision();

protected:
    virtual void OnSizeChanged(int sNewWidth, int sNewHeight, int sOldWidth, int sOldHeight);
    
private:
    typedef enum
    {
        Direction_Error = 0,//错误
        Direction_LeftUp,//左上
        Direction_Up,//上
        Direction_RightUp,//右上
        Direction_Left,//左
        Direction_RightDown,//右下
        Direction_Down,//下
        Direction_LeftDown,//左下
        Direction_Right,//右
    } PreviousMoveDirection;

    struct PointWithDirection
    {
        UIPoint point;
        PreviousMoveDirection direction;
    };
    
    //添加左上、上、右上、左四个点
    void AddPointFromLeftToLeftUp(const UIPoint& center, std::map<int,PointWithDirection>& pointMap);
    //添加左下、下、右下、右四个点
    void AddPointFromRightToLeftDown(const UIPoint& center, std::map<int,PointWithDirection>& pointMap);

    LeaderIndex m_leaderIndex = Leader_Error;
    int m_PosX = 0;
    int m_PosY = 0;
    int m_Temp = 0;
    UIRect m_PreviousRect;
    std::vector<UIPoint> m_PassedPoints;
    bool m_IsCollisioning = false;
    PreviousMoveDirection m_PreviousCollisioning = Direction_Error;
    // CActorAnimator* m_pAnimotor = nullptr; //动画
    // TimerUtils* m_Timer = nullptr;
};
#endif  // __FRAME__LEADERCACTOR_H__