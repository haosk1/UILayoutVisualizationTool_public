#include "AbsoluteLayout.h"

void CAbsoluteLayout::OnMeasure(int iWidthMeasureSpec, int iHeightMeasureSpec, int iLengthMeasureSpec)
{
  //先测量自己
  CVisualizationStage::OnMeasure(iWidthMeasureSpec, iHeightMeasureSpec, iLengthMeasureSpec);

  int iChildCount = GetChildCount();

  int iMeasureWidth = 0;
  int iMeasureHeight = 0;
  int iChildMeasureWidth = 0;
  int iChildMeasureHeight = 0;
  int iChildMeasureLength = 0;
  for (int i = 0; i < iChildCount; ++i)
  {
    CActor *pstActor = GetChildAt(i);
    const CVisiablePlug* pstVisiablePlug = pstActor->GetVisiablePlug();
    if (pstActor && pstVisiablePlug->m_Visible != CVisiablePlug::Gone)
    {
      MeasureChild(pstActor, m_sMeasureWidth, m_sMeasureHeight, 0);
      pstActor->GetMeasuredDimension(iChildMeasureWidth, iChildMeasureHeight, iChildMeasureLength);
      const CAbsLayoutPlug* pstAbsLayoutPlug = pstActor->GetAbsLayoutPlug();
      iChildMeasureWidth += pstAbsLayoutPlug->m_iLayoutX;
      iChildMeasureHeight += pstAbsLayoutPlug->m_iLayoutY;
      iMeasureWidth = UI_MAX(iMeasureWidth,iChildMeasureWidth);
      iMeasureHeight = UI_MAX(iMeasureHeight, iChildMeasureHeight);
    }
  }
  const CPaddingPlug *ParentPstPaddingPlug = GetPaddingPlug();

  int iMyselfMeasureWidth = 0;
  int iMyselfMeasureHeight = 0;
  GetMyselfMeasure(iMyselfMeasureWidth, iMyselfMeasureHeight);
  if(m_pstLayoutParam->m_iLayoutWidth == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT){
    iMeasureWidth += ParentPstPaddingPlug->m_iPaddingLeft + ParentPstPaddingPlug->m_iPaddingRight;
    iMeasureWidth = UI_MAX(iMeasureWidth, iMyselfMeasureWidth);
    iMeasureWidth = UI_MIN(iMeasureWidth, iWidthMeasureSpec);
    SetMeasuredDimension(iMeasureWidth,m_sMeasureHeight, 0);
  }
  if(m_pstLayoutParam->m_iLayoutHeight == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT){
    iMeasureHeight += ParentPstPaddingPlug->m_iPaddingTop + ParentPstPaddingPlug->m_iPaddingBottom;
    iMeasureHeight = UI_MAX(iMeasureHeight, iMyselfMeasureHeight);
    iMeasureHeight = UI_MIN(iMeasureHeight, iHeightMeasureSpec);
    SetMeasuredDimension(m_sMeasureWidth,iMeasureHeight, 0);
  }
}

void CAbsoluteLayout::OnLayout(bool bChanged, int sLeft, int sTop, int sDepth, int sWidth, int sHeight, int length)
{
  int iChildCount = GetChildCount();

  int iChildMeasureWidth = 0;
  int iChildMeasureHeight = 0;
  int iChildMeasureLength = 0;
  for (int i = 0; i < iChildCount; ++i)
  {
    CActor *pstActor = GetChildAt(i);
    const CVisiablePlug* pstVisiablePlug = pstActor->GetVisiablePlug();
    if (pstActor && pstVisiablePlug->m_Visible != CVisiablePlug::Gone)
    {
      const CAbsLayoutPlug* pstAbsLayoutPlug = pstActor->GetAbsLayoutPlug();
      int sChildLeft = m_pstPaddingPlug->m_iPaddingLeft + pstAbsLayoutPlug->m_iLayoutX;
      int sChildTop = m_pstPaddingPlug->m_iPaddingTop + pstAbsLayoutPlug->m_iLayoutY;
      pstActor->GetMeasuredDimension(iChildMeasureWidth, iChildMeasureHeight, iChildMeasureLength);
      pstActor->Layout(sChildLeft, sChildTop, sDepth, iChildMeasureWidth, iChildMeasureHeight, length);
    }
  }
}