#include "LeaderActor.h"
#include <math.h>
#include "UIFW/UIController.h"

void CLeaderActor::SetLeaderIndex(LeaderIndex index)
{
    m_leaderIndex = index;
}

void CLeaderActor::SetLeaderTemp(int iTemp)
{
    m_Temp = iTemp;
}

void CLeaderActor::OnSizeChanged(int sNewWidth, int sNewHeight, int sOldWidth, int sOldHeight)
{
    return;
}

void CLeaderActor::AddPointFromLeftToLeftUp(const UIPoint &center, std::map<int, PointWithDirection> &pointMap)
{
    PointWithDirection pointLeftUp;
    pointLeftUp.direction = Direction_LeftUp;
    pointLeftUp.point = UIPoint(center.m_iX - m_Temp, center.m_iY - m_Temp);
    PointWithDirection pointUp;
    pointUp.direction = Direction_Up;
    pointUp.point = UIPoint(center.m_iX, center.m_iY - m_Temp);
    PointWithDirection pointRightUp;
    pointRightUp.direction = Direction_RightUp;
    pointRightUp.point = UIPoint(center.m_iX + m_Temp, center.m_iY - m_Temp);
    PointWithDirection pointLeft;
    pointLeft.direction = Direction_Left;
    pointLeft.point = UIPoint(center.m_iX - m_Temp, center.m_iY);

    int distanceLeftUp = abs(m_PosX - pointLeftUp.point.m_iX) + abs(m_PosY - pointLeftUp.point.m_iY);
    pointMap.insert(std::make_pair(distanceLeftUp, pointLeftUp));
    int distanceUp = abs(m_PosX - pointUp.point.m_iX) + abs(m_PosY - pointUp.point.m_iY);
    pointMap.insert(std::make_pair(distanceUp, pointUp));
    int distanceRightUp = abs(m_PosX - pointRightUp.point.m_iX) + abs(m_PosY - pointRightUp.point.m_iY);
    pointMap.insert(std::make_pair(distanceRightUp, pointRightUp));
    int distanceLeft = abs(m_PosX - pointLeft.point.m_iX) + abs(m_PosY - pointLeft.point.m_iY);
    pointMap.insert(std::make_pair(distanceLeft, pointLeft));

}

void CLeaderActor::AddPointFromRightToLeftDown(const UIPoint &center, std::map<int, PointWithDirection> &pointMap)
{
    PointWithDirection pointRight;
    pointRight.direction = Direction_Right;
    pointRight.point = UIPoint(center.m_iX + m_Temp, center.m_iY);
    PointWithDirection pointLeftDown;
    pointLeftDown.direction = Direction_LeftDown;
    pointLeftDown.point = UIPoint(center.m_iX - m_Temp, center.m_iY + m_Temp);
    PointWithDirection pointDown;
    pointDown.direction = Direction_Down;
    pointDown.point = UIPoint(center.m_iX, center.m_iY + m_Temp);
    PointWithDirection pointRightDown;
    pointRightDown.direction = Direction_RightDown;
    pointRightDown.point = UIPoint(center.m_iX + m_Temp, center.m_iY + m_Temp);

    int distanceRight = abs(m_PosX - pointRight.point.m_iX) + abs(m_PosY - pointRight.point.m_iY);
    pointMap.insert(std::make_pair(distanceRight, pointRight));
    int distanceLeftDown = abs(m_PosX - pointLeftDown.point.m_iX) + abs(m_PosY - pointLeftDown.point.m_iY);
    pointMap.insert(std::make_pair(distanceLeftDown, pointLeftDown));
    int distanceDown = abs(m_PosX - pointDown.point.m_iX) + abs(m_PosY - pointDown.point.m_iY);
    pointMap.insert(std::make_pair(distanceDown, pointDown));
    int distanceRightDown = abs(m_PosX - pointRightDown.point.m_iX) + abs(m_PosY - pointRightDown.point.m_iY);
    pointMap.insert(std::make_pair(distanceRightDown, pointRightDown));
}

int a = 1;
bool CLeaderActor::OnMouseEvent(Mouse_Event event)
{
    bool res = false;
    if(m_leaderIndex ==  Leader_One && event.mouse_type == EVENT_MOUSE_TYPE_DOWN)
    {
        m_PosX = event.iPosX;
        m_PosY = event.iPosY;	
        m_PassedPoints.clear();
        res = true;
        // if(nullptr == m_pAnimotor)
        // {
        //     m_pAnimotor = new CActorTranslateAnimator(0,0,0,-10);
        //     m_pAnimotor->SetTarget(this);
        //     m_pAnimotor->SetInterpolator(new CLinearInterpolator);
        //     m_pAnimotor->SetFillBefore(false);
        //     m_pAnimotor->SetDuration(250);
        //     m_pAnimotor->SetRepeatMode(CAnimator::RepeatMode_Reverse);
        //     m_pAnimotor->SetRepeatCount(100000);
        // }
        // //容错处理：指针判空
        // if (nullptr != m_pAnimotor)
        // {
        //     m_pAnimotor->Start();

        // }
        // if (nullptr == m_Timer) {
        //     m_Timer = new TimerUtils();
        // }
        // if (m_Timer) {
        //     m_Timer->start([this]{
        //         switch (a)
        //         {
        //         case 1:
        //             SetDrawablePath("frame/UI/res/jaks1.jpg");
        //             break;
        //         case 2:
        //             SetDrawablePath("frame/UI/res/jaks2.jpg");
        //             break;
        //         case 3:
        //             SetDrawablePath("frame/UI/res/jaks.jpg");
        //             break;
        //         default:
        //             break;
        //         }
        //         a++;
        //         if(a == 4){
        //             a = 1;
        //         }
                
        // }, 250, true);
        // }
    }
    return res;
}

void CLeaderActor::OnDraw(CUICanvasInterface &stCanvas)
{
    if(m_leaderIndex ==  Leader_One){
        // int width = CUIController::Instance()->GetWindowRect().m_sWidth;
        // int height = CUIController::Instance()->GetWindowRect().m_sHeight;
        // int left = stCanvas.Action().m_current_matrix->getTranslateX();
        // int top = stCanvas.Action().m_current_matrix->getTranslateY();
        // UIPoint start(-left,-top);
        // UIPoint end(-left + width,-top + height);
        // UIPoint controller(GetWidth()/2,GetHeight()/2);
        // std::vector<float> points = CUIController::Instance()->GetBezierPoints(start,end,controller);
        // stCanvas.SetLineWidth(10.0f);
        // stCanvas.DrawLine(&points[0],points.size(),155,120,0);

        //如果碰撞，需要先退回来，然后再换方向走
        if(m_IsCollisioning){
            // Layout(m_PreviousRect.m_sLeft,m_PreviousRect.m_sTop, m_PreviousRect.m_sWidth,m_PreviousRect.m_sHeight);
        }
        UIRect rect;
        GetHitRect(rect);
        UIPoint center = UIPoint(rect.m_sLeft + rect.m_sWidth/2, rect.m_sTop + rect.m_sHeight/2);
        if((0 == m_PosX && 0 == m_PosY) || ((abs(m_PosX - center.m_iX) < m_Temp) && (abs(m_PosY - center.m_iY) < m_Temp))){
            //还没点击或已到达，无需移动
            // if (nullptr != m_pAnimotor)
            // {
            //     m_pAnimotor->Stop();
            //     m_Timer->stop();
            //     SetDrawablePath("frame/UI/res/jaks.jpg");
            // }
            return;
        }
        //计算下一帧位置前，将当前位置保存，以便碰撞后退回
        m_PreviousRect = rect;

        //华盛顿街区算法
        std::map<int,PointWithDirection> mapMoveDirection;
        //上下碰撞时应该改变行走点顺序
        if(m_IsCollisioning && (m_PreviousCollisioning == Direction_Up || m_PreviousCollisioning == Direction_Down)){
            AddPointFromLeftToLeftUp(center, mapMoveDirection);
            AddPointFromRightToLeftDown(center, mapMoveDirection);
        }else{
            AddPointFromRightToLeftDown(center, mapMoveDirection);
            AddPointFromLeftToLeftUp(center, mapMoveDirection);
        }
        
        //先删除走过的点（碰撞点也包含在这里）
        std::vector<UIPoint>::iterator iterPassedPoint =  m_PassedPoints.begin();
        for (; iterPassedPoint != m_PassedPoints.end(); iterPassedPoint++)
        {
            std::map<int, PointWithDirection>::iterator iterMoveDirectio =  mapMoveDirection.begin();
            for (; iterMoveDirectio != mapMoveDirection.end();iterMoveDirectio++)
            {
                if(iterPassedPoint->m_iX == iterMoveDirectio->second.point.m_iX && iterPassedPoint->m_iY == iterMoveDirectio->second.point.m_iY){
                    mapMoveDirection.erase(iterMoveDirectio);
                    //因为8个点里每次只有一个可能匹配，所以找到就可以break进行下一个点匹配了
                    break;
                }
            }
        }
        //将剩余点到终点距离距离赋值个list用于排序
        std::list<int> listMoveDirection;
        std::map<int, PointWithDirection>::iterator iter =  mapMoveDirection.begin();
        for (; iter != mapMoveDirection.end();iter++)
        {
            
            listMoveDirection.push_front(iter->first);
            
        }
        //距离排序，寻找最短的
        listMoveDirection.sort();
        std::map<int, PointWithDirection>::iterator iterEnd;
        iterEnd = mapMoveDirection.find(listMoveDirection.front());
        //将距离最短的点设置为下一帧要走的点，并将其保存为走过的点
        if(iterEnd != mapMoveDirection.end()){
            rect.m_sLeft = iterEnd->second.point.m_iX - rect.m_sWidth/2;
            rect.m_sTop = iterEnd->second.point.m_iY - rect.m_sHeight/2;
            m_PassedPoints.push_back(iterEnd->second.point);
            m_PreviousCollisioning = iterEnd->second.direction;
        }
        // Layout(rect.m_sLeft,rect.m_sTop, rect.m_sWidth,rect.m_sHeight);
    }
}

bool CLeaderActor::NeedCheckCollision()
{
    //角色需要检测碰撞
    m_IsCollisioning = false;
    return true;
}

void CLeaderActor::Collision()
{
    m_IsCollisioning = true;
}
