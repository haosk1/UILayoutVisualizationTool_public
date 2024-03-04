#include "TextActor.h"

void CTextActor::Inflate(TiXmlAttribute *attribute)
{
    CVisualizationActor::Inflate(attribute);
    std::string path = "";
    CUILib::Instance()->getFilePath(m_font.FontFamily, path);
    CUILib::Instance()->SetCharacterLibraryPath(path.c_str());
}

void CTextActor::SetActorAttribute(const char *attributeName, const char *attributeValue)
{
    CVisualizationActor::SetActorAttribute(attributeName, attributeValue);
    // 处理自己的 SetActorAttribute
    if (!strcmp(attributeName, "Text"))
    {
        std::string stdAttributeValue = attributeValue;
        if(std::string::npos == stdAttributeValue.find("@String/")){
            m_string.Chinese = stdAttributeValue;
            m_string.Chinese_Cantonese = stdAttributeValue;
            m_string.Chinese_Complex = stdAttributeValue;
            m_string.English = stdAttributeValue;
        }else{
            std::string name = stdAttributeValue.substr(8, stdAttributeValue.length() - 8); // 8:@String/
            CUILib::Instance()->GetString(name, m_string);
        }
    }
    else if (!strcmp(attributeName, "TextFont"))
    {
        std::string stdAttributeValue = attributeValue;
        std::string name = stdAttributeValue.substr(6, stdAttributeValue.length() - 6); // 6:@Font/
        CUILib::Instance()->GetFont(name, m_font);
    }
    else if (!strcmp(attributeName, "TextColor"))
    {
        std::string stdAttributeValue = attributeValue;
        std::string name = stdAttributeValue.substr(7, stdAttributeValue.length() - 7); // 7:@Color/
        CUILib::Instance()->GetColor(name, m_color);
    }
    else if (!strcmp(attributeName, "SingleLine"))
    {
        if (!strcmp(attributeValue, "false"))
        {
            m_SingleLine = false;
        }
    }
    else if (!strcmp(attributeName, "LineIntervalAddFactor"))
    {
        m_LineSpace = atoi(attributeValue);
    }
}

void CTextActor::OnSizeChanged(int sNewWidth, int sNewHeight, int sNewLength, int sOldWidth, int sOldHeight, int sOldLength)
{
    return CVisualizationActor::OnSizeChanged(sNewWidth, sNewHeight, sNewLength, sOldWidth, sOldHeight, sOldLength);
}

void CTextActor::OnMeasure(int iWidthMeasureSpec, int iHeightMeasureSpec, int iLengthMeasureSpec)
{
    CVisualizationActor::OnMeasure(iWidthMeasureSpec, iHeightMeasureSpec, iLengthMeasureSpec);

    UIRect limitRect(m_pstPaddingPlug->m_iPaddingLeft, m_pstPaddingPlug->m_iPaddingTop, 0,
                    iWidthMeasureSpec - m_pstPaddingPlug->m_iPaddingLeft - m_pstPaddingPlug->m_iPaddingRight - GetLayoutPlug()->m_sLeftMargin - GetLayoutPlug()->m_sRightMargin, 
                    iHeightMeasureSpec - m_pstPaddingPlug->m_iPaddingTop - m_pstPaddingPlug->m_iPaddingBottom - GetLayoutPlug()->m_sTopMargin - GetLayoutPlug()->m_sBottomMargin, 
                    0);
    if (m_pstLayoutParam->m_iLayoutWidth >0){
        limitRect.m_sWidth = UI_MIN(limitRect.m_sWidth, m_pstLayoutParam->m_iLayoutWidth);
    }
    if(m_pstLayoutParam->m_iLayoutHeight >0){
        limitRect.m_sHeight = UI_MIN(limitRect.m_sHeight, m_pstLayoutParam->m_iLayoutHeight);
    }

    if (m_SingleLine)
    {
        limitRect.m_sHeight = -1; // 单行要求高度设置为-1
    }
    CUILib::Instance()->getTextSize(m_string.Chinese, m_font.FontHeight, m_LineSpace, limitRect, m_stringRect, m_uiMostTextHeight);

    if(m_pstLayoutParam->m_iLayoutWidth == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT){
        m_stringRect.m_sWidth = UI_MAX(m_stringRect.m_sWidth, m_sMeasureWidth);
        m_stringRect.m_sWidth = UI_MIN(m_stringRect.m_sWidth, iWidthMeasureSpec);
        SetMeasuredDimension(m_stringRect.m_sWidth, m_sMeasureHeight, 0);
    }
    if(m_pstLayoutParam->m_iLayoutHeight == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT){
        m_stringRect.m_sHeight = UI_MAX(m_stringRect.m_sHeight, m_sMeasureHeight);
        m_stringRect.m_sHeight = UI_MIN(m_stringRect.m_sHeight, iHeightMeasureSpec);
        SetMeasuredDimension(m_sMeasureWidth, m_stringRect.m_sHeight, 0);
    }
}

void CTextActor::OnDraw(CUICanvasInterface &stCanvas)
{
    CVisualizationActor::OnDraw(stCanvas);
    if (m_IsDraw)
    {
        // 处理自己的draw
        UIRect stOutRect;
        UIRect parentRect;
        GetDrawRect(parentRect);
        CUIGravity::Apply(m_pstUIGravityPlug->m_ulGravity, parentRect, 
                          m_stringRect.m_sWidth, m_stringRect.m_sHeight, 0,
                          m_pstPaddingPlug->m_iPaddingLeft,m_pstPaddingPlug->m_iPaddingTop, 0,
                          m_pstPaddingPlug->m_iPaddingRight, m_pstPaddingPlug->m_iPaddingBottom, 0,
                          stOutRect);
        stCanvas.DrawString(m_string.Chinese.c_str(), stOutRect, m_uiMostTextHeight, m_font.FontHeight, m_color.color_RGB >> 16, m_color.color_RGB >> 8, m_color.color_RGB);
    }
}