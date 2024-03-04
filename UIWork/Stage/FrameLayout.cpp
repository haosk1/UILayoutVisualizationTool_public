#include "FrameLayout.h"

void CFrameLayout::Inflate(TiXmlAttribute *attribute)
{
  CVisualizationStage::Inflate(attribute);
}

void CFrameLayout::SetActorAttribute(const char *attributeName,
                                              const char *attributeValue)
{
  CVisualizationStage::SetActorAttribute(attributeName, attributeValue);
  // 处理自己的 SetActorAttribute
  if (!strcmp(attributeName, "MeasureAllChildren"))
  {
    if (!strcmp(attributeValue, "true")){
      m_MeasureAllChildren = true;
    }
  }
}

void CFrameLayout::OnMeasure(int iWidthMeasureSpec, int iHeightMeasureSpec, int iLengthMeasureSpec)
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
    if (pstActor && (pstVisiablePlug->m_Visible != CVisiablePlug::Gone || m_MeasureAllChildren))
    {
      MeasureChildWithMargin(pstActor, m_sMeasureWidth, m_sMeasureHeight, 0);
      pstActor->GetMeasuredDimension(iChildMeasureWidth, iChildMeasureHeight, iChildMeasureLength);
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

void CFrameLayout::OnLayout(bool bChanged, int sLeft, int sTop, int sDepth, int sWidth, int sHeight, int length)
{
  int iChildCount = GetChildCount();
  const CPaddingPlug *ParentPstPaddingPlug = GetPaddingPlug();
  UIRect stParentRect(ParentPstPaddingPlug->m_iPaddingLeft, ParentPstPaddingPlug->m_iPaddingTop, 0,
                      sWidth - ParentPstPaddingPlug->m_iPaddingLeft - ParentPstPaddingPlug->m_iPaddingRight,
                      sHeight - ParentPstPaddingPlug->m_iPaddingTop - ParentPstPaddingPlug->m_iPaddingBottom, 
                      0);

  int iChildMeasureWidth = 0;
  int iChildMeasureHeight = 0;
  int iChildMeasureLength = 0;
  for (int i = 0; i < iChildCount; ++i)
  {
    CActor *pstActor = GetChildAt(i);
    const CVisiablePlug* pstVisiablePlug = pstActor->GetVisiablePlug();
    if (pstActor && (pstVisiablePlug->m_Visible != CVisiablePlug::Gone || m_MeasureAllChildren))
    {
      const CMarginLayoutPlug *pstChildLayoutPlug = pstActor->GetLayoutPlug();
      const CUILayoutGravityPlug *pstChildGravityPlug = pstActor->GetUILayoutGravityPlug();
      pstActor->GetMeasuredDimension(iChildMeasureWidth, iChildMeasureHeight, iChildMeasureLength);
      UIRect stOutRect;
      CUIGravity::Apply(pstChildGravityPlug->m_ulGravity, stParentRect, 
                      iChildMeasureWidth, iChildMeasureHeight, 0,
                      pstChildLayoutPlug->m_sLeftMargin, pstChildLayoutPlug->m_sTopMargin, 0,
                      pstChildLayoutPlug->m_sRightMargin, pstChildLayoutPlug->m_sBottomMargin, 0,
                      stOutRect);

      pstActor->Layout(stOutRect.m_sLeft, stOutRect.m_sTop, sDepth, stOutRect.m_sWidth, stOutRect.m_sHeight, length);
    }
  }
}

void CFrameLayout::OnDraw(CUICanvasInterface &stCanvas)
{
  CVisualizationStage::OnDraw(stCanvas);
}