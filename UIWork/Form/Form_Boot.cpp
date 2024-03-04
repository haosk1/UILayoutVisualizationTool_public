#include "UIFW/UIController.h"
#include "Form_Boot.h"

void CForm_Boot::Load(void)
{
    if(nullptr != m_pstLayout){
        return;
    }
    m_pstLayout = new CStage(); 
    //创建地图
    // m_pstLayout->SetDrawablePath("frame/UI/res/mars.jpg");
    UIRect backGroundRect = CUIController::Instance()->GetWindowRect();
    m_pstLayout->Layout(0,0,0,backGroundRect.m_sWidth,backGroundRect.m_sHeight,backGroundRect.m_sLength);
    //创建主角色
    CLeaderActor* leaderOne = CreatLeader(CLeaderActor::Leader_One);
    m_pstLayout->AddActor(leaderOne);
    //创建次角色
    CLeaderActor* leaderTwo = CreatLeader(CLeaderActor::Leader_Two);
    m_pstLayout->AddActor(leaderTwo);
    //创建空气墙
    // m_pstLayout->AddActor(CreatAirWall());
    //创建士兵
    m_pstLayout->AddActor(CreatSoldier());
}

CLeaderActor* CForm_Boot::CreatLeader(CLeaderActor::LeaderIndex index)
{
    CLeaderActor* leaderActor = new CLeaderActor();
    leaderActor->SetLeaderIndex(index);
    switch (index){
        case CLeaderActor::Leader_One:
            // leaderActor->SetDrawablePath("frame/UI/res/jaks.jpg");
            leaderActor->Layout(400,400,0,70,70,0);
            break;
        case CLeaderActor::Leader_Two:
            // leaderActor->SetDrawablePath("frame/UI/res/aoliber.jpg");
            leaderActor->Layout(550,550,0,70,70,0);
            break;
        default:
            // leaderActor->SetDrawablePath("frame/UI/res/jaks.jpg");
            leaderActor->Layout(400,400,0,70,70,0);
            break;
    }
    leaderActor->SetLeaderTemp(1);
    return leaderActor;
}

CActor* CForm_Boot::CreatAirWall()
{
    //待实现
    CActor* airWallActor = new CActor();
    return airWallActor;
}

CActor* CForm_Boot::CreatSoldier()
{
    //待实现
    CActor* soldierActor = new CActor();
    soldierActor->SetDrawablePath("frame/UI/res/mars2.jpg");
    soldierActor->Layout(600,600,0,30,30,0);
    return soldierActor;
}

void CForm_Boot::DispatchDraw(CUICanvasInterface &stCanvas)
{
    if(nullptr == m_pstLayout){
        return;
    }
    int size = m_pstLayout->GetChildCount();
    for (int index = 0; index < size; index++)
    {
        CActor* child = m_pstLayout->GetChildAt(index);
        if(child->NeedCheckCollision()){
            if(CheckIsCollision(child)){
                child->Collision();
            }
        }
    }
    int iSaveCount = stCanvas.GetSaveCount();
    stCanvas.Save();
    m_pstLayout->Draw(stCanvas);
    stCanvas.RestoreToCount(iSaveCount);
}

bool CForm_Boot::CheckIsCollision(CActor* pActor)
{
    //简单的碰撞检测算法 自研
    UIRect rect;
    pActor->GetHitRect(rect);

    UIRect otherRect;
    //先检查角色之间
    int size = m_pstLayout->GetChildCount();
    for (int index = 0; index < size; index++)
    {
        CActor* child = m_pstLayout->GetChildAt(index);
        if(child == pActor){
            //去掉自己
            continue;
        }
        child->GetHitRect(otherRect);
        if(otherRect.m_sLeft + otherRect.m_sWidth < rect.m_sLeft ||
            otherRect.m_sLeft > rect.m_sLeft + rect.m_sWidth ||
            otherRect.m_sTop + otherRect.m_sHeight < rect.m_sTop ||
            otherRect.m_sTop > rect.m_sTop + rect.m_sHeight){
            continue; 
        }else{
            return true;
        }
    }
    
    return false;
}