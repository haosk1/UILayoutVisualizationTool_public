#include "ImageActor.h"
CImageActor::CImageActor(){
  m_pstUIGravityPlug->m_ulGravity = CUIGravity::CENTER_HORIZENTAL | CUIGravity::CENTER_VERTICAL;
}

void CImageActor::Inflate(TiXmlAttribute *attribute) {
  CVisualizationActor::Inflate(attribute);
}

void CImageActor::SetActorAttribute(const char *attributeName,
                                    const char *attributeValue) {
  CVisualizationActor::SetActorAttribute(attributeName, attributeValue);
  // 处理自己的 SetActorAttribute
  if (!strcmp(attributeName, "Drawable")) {
    std::string stdtemp = attributeValue;
    std::string stdAttributeValue = stdtemp.substr(10, stdtemp.length() - 10);
    std::string drawable = "";
    CUILib::Instance()->parseDrawableXml(stdAttributeValue, drawable);
    SetDrawbale(drawable);
    if(std::string::npos != drawable.find(".9")){
      m_bIsNineGridDrawable = true;
    }
  }
}
void CImageActor::SetDrawbale(std::string strPath) {
  m_DrawabletextureId = GetDrawableTextureID(strPath);
}

void CImageActor::OnSizeChanged(int sNewWidth, int sNewHeight, int sNewLength, int sOldWidth, int sOldHeight, int sOldLength) 
{
  return CVisualizationActor::OnSizeChanged(sNewWidth, sNewHeight, sNewLength, sOldWidth, sOldHeight, sOldLength);
}

void CImageActor::OnMeasure(int iWidthMeasureSpec, int iHeightMeasureSpec, int iLengthMeasureSpec)
{
  CVisualizationActor::OnMeasure(iWidthMeasureSpec, iHeightMeasureSpec, iLengthMeasureSpec);
  if(m_pstLayoutParam->m_iLayoutWidth == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT ||
        m_pstLayoutParam->m_iLayoutHeight == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT){
    //获取描画内容物尺寸
    UIRect outRect;
    GetDrawableRect(m_DrawabletextureId,outRect);

    if(m_pstLayoutParam->m_iLayoutWidth == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT){
      outRect.m_sWidth  = outRect.m_sWidth + m_pstPaddingPlug->m_iPaddingLeft + m_pstPaddingPlug->m_iPaddingRight;
      outRect.m_sWidth = UI_MAX(outRect.m_sWidth, m_sMeasureWidth);
      outRect.m_sWidth = UI_MIN(outRect.m_sWidth, iWidthMeasureSpec);
      SetMeasuredDimension(outRect.m_sWidth, m_sMeasureHeight, 0);
    }
    if(m_pstLayoutParam->m_iLayoutHeight == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT){
      outRect.m_sHeight  = outRect.m_sHeight + m_pstPaddingPlug->m_iPaddingTop + m_pstPaddingPlug->m_iPaddingBottom;
    outRect.m_sHeight = UI_MAX(outRect.m_sHeight, m_sMeasureHeight);
      outRect.m_sHeight = UI_MIN(outRect.m_sHeight, m_sMeasureHeight);
      SetMeasuredDimension(m_sMeasureWidth, outRect.m_sHeight, 0);
    }
  }
}

void CImageActor::OnDraw(CUICanvasInterface &stCanvas) {
  CVisualizationActor::OnDraw(stCanvas);
  if (m_IsDraw && 0 != m_DrawabletextureId) {
    // 处理自己的draw
    UIRect stOutRect;
    UIRect stDrawableRect;
    GetDrawableRect(m_DrawabletextureId, stDrawableRect);
    if(m_bIsNineGridDrawable){
      GetDrawRectwithPadding(stOutRect);
      stCanvas.DrawNineTexture(m_DrawabletextureId, stDrawableRect, stOutRect);
    }else{
      UIRect parentRect;
      GetDrawRect(parentRect);
      CUIGravity::Apply(m_pstUIGravityPlug->m_ulGravity, parentRect, 
                        stDrawableRect.m_sWidth, stDrawableRect.m_sHeight, 0,
                        m_pstPaddingPlug->m_iPaddingLeft, m_pstPaddingPlug->m_iPaddingTop, 0,
                        m_pstPaddingPlug->m_iPaddingRight, m_pstPaddingPlug->m_iPaddingBottom, 0,
                        stOutRect);
      stCanvas.DrawTexture(m_DrawabletextureId, stOutRect);
    }
  }
}
