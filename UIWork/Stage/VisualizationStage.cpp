#include "VisualizationStage.h"
#include "UILib.h"

void CVisualizationStage::Inflate(TiXmlAttribute* attribute)
{
    while (attribute){
        SetActorAttribute(attribute->Name(), attribute->Value());
        attribute = attribute->Next();
    }
}

void CVisualizationStage::SetActorAttribute(const char* attributeName, const char* attributeValue)
{
    if(!strcmp(attributeName, "Id")){
        std::string stdtemp = attributeValue;
        uint pos = stdtemp.rfind("/", stdtemp.length()) + 1;
        std::string stdAttributeValue = stdtemp.substr(pos, stdtemp.length() - pos);
        m_pstIDPlug->m_strID = stdAttributeValue;
    }
    else if (!strcmp(attributeName, "LayoutWidth")){
        if (!strcmp(attributeValue, "FillParent")){
            m_pstLayoutParam->m_iLayoutWidth = CMarginLayoutPlug::MEASURE_MODE_FILLPARENT;
        }
        else if(!strcmp(attributeValue, "WrapContent")){
            m_pstLayoutParam->m_iLayoutWidth = CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT;
        }
        else{
            int width = atoi(attributeValue);
            m_pstLayoutParam->m_iLayoutWidth = width;
        }
    }
    else if (!strcmp(attributeName, "LayoutHeight")){
        if (!strcmp(attributeValue, "FillParent")){
            m_pstLayoutParam->m_iLayoutHeight = CMarginLayoutPlug::MEASURE_MODE_FILLPARENT;
        }
        else if(!strcmp(attributeValue, "WrapContent")){
            m_pstLayoutParam->m_iLayoutHeight = CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT;
        }
        else{
            int height = atoi(attributeValue);
            m_pstLayoutParam->m_iLayoutHeight = height;
        }
    }
    else if (!strcmp(attributeName, "LayoutTopMargin")){
        m_pstLayoutParam->m_sTopMargin = atoi(attributeValue);
    }
    else if (!strcmp(attributeName, "LayoutLeftMargin")){
        m_pstLayoutParam->m_sLeftMargin = atoi(attributeValue);
    }
    else if (!strcmp(attributeName, "LayoutRightMargin")){
        m_pstLayoutParam->m_sRightMargin = atoi(attributeValue);
    }
    else if (!strcmp(attributeName, "LayoutBottomMargin")){
        m_pstLayoutParam->m_sBottomMargin = atoi(attributeValue);
    }
    else if (!strcmp(attributeName, "PaddingLeft")){
        m_pstPaddingPlug->m_iPaddingLeft = atoi(attributeValue);
    }
    else if (!strcmp(attributeName, "PaddingTop")){
        m_pstPaddingPlug->m_iPaddingTop = atoi(attributeValue);
    }
    else if (!strcmp(attributeName, "PaddingRight")){
        m_pstPaddingPlug->m_iPaddingRight = atoi(attributeValue);
    }
    else if (!strcmp(attributeName, "PaddingBottom")){
        m_pstPaddingPlug->m_iPaddingBottom = atoi(attributeValue);
    }
    else if (!strcmp(attributeName, "Background")){
        std::string stdtemp = attributeValue;
        std::string stdAttributeValue = stdtemp.substr(10, stdtemp.length() - 10);
        std::string background = "";
        CUILib::Instance()->parseDrawableXml(stdAttributeValue, background);
        SetBackground(background);
        if(std::string::npos != background.find(".9")){
            m_bIsNineGridDrawable = true;
        }
    }
    else if (!strcmp(attributeName, "Visible")){
        if (!strcmp(attributeValue, "Gone")){
           m_pstVisiablePlug->m_Visible = CVisiablePlug::ActorVisibleType::Gone;
        }
        if (!strcmp(attributeValue, "InVisiable")){
           m_pstVisiablePlug->m_Visible = CVisiablePlug::ActorVisibleType::Invisible;
        }
    }
    else if (!strcmp(attributeName, "LayoutHorizontalGravity")){
        if (!strcmp(attributeValue, "Left")){
            m_pstUILayoutGravityPlug->m_ulGravity = (m_pstUILayoutGravityPlug->m_ulGravity & ~CUIGravity::HORIZENTAL_MASK) | CUIGravity::LEFT;
        }
        else if (!strcmp(attributeValue, "Right")){
            m_pstUILayoutGravityPlug->m_ulGravity = (m_pstUILayoutGravityPlug->m_ulGravity & ~CUIGravity::HORIZENTAL_MASK) | CUIGravity::RIGHT;
        }
        else{
            m_pstUILayoutGravityPlug->m_ulGravity = (m_pstUILayoutGravityPlug->m_ulGravity & ~CUIGravity::HORIZENTAL_MASK) | CUIGravity::CENTER_HORIZENTAL;
        }
    }
    else if (!strcmp(attributeName, "LayoutVerticalGravity")){
        if (!strcmp(attributeValue, "Top")){
            m_pstUILayoutGravityPlug->m_ulGravity = (m_pstUILayoutGravityPlug->m_ulGravity & ~CUIGravity::VERTICAL_MASK) | CUIGravity::TOP;
        }
        else if (!strcmp(attributeValue, "Bottom")){
            m_pstUILayoutGravityPlug->m_ulGravity = (m_pstUILayoutGravityPlug->m_ulGravity & ~CUIGravity::VERTICAL_MASK) | CUIGravity::BOTTOM;
        }
        else{
            m_pstUILayoutGravityPlug->m_ulGravity = (m_pstUILayoutGravityPlug->m_ulGravity & ~CUIGravity::VERTICAL_MASK) | CUIGravity::CENTER_VERTICAL;
        }
    }
    //AbsoluteLayout 适配 start
    else if (!strcmp(attributeName, "LayoutX")){
        m_pstAbsLayoutPlug->m_iLayoutX = atoi(attributeValue);
    }
    else if (!strcmp(attributeName, "LayoutY")){
        m_pstAbsLayoutPlug->m_iLayoutY = atoi(attributeValue);
    }
    //AbsoluteLayout 适配 end
    //LinearLayout 适配 start
    else if (!strcmp(attributeName, "LayoutWeight")){
        m_pstLinearLayoutPlug->m_fWeight = atof(attributeValue);
    }
    //LinearLayout 适配 end
    //RelativeLayout 适配 start
    else if (!strcmp(attributeName, "LayoutLeftOf")){
        std::string stdtemp = attributeValue;
        uint pos = stdtemp.rfind("/", stdtemp.length()) + 1;
        m_pstRelativeLayoutPlug->m_ulRule[CRelativeLayoutPlug::EnumAlign::LeftOf] = stdtemp.substr(pos, stdtemp.length() - pos);
    }
    else if (!strcmp(attributeName, "LayoutRightOf")){
        std::string stdtemp = attributeValue;
        uint pos = stdtemp.rfind("/", stdtemp.length()) + 1;
        m_pstRelativeLayoutPlug->m_ulRule[CRelativeLayoutPlug::EnumAlign::RightOf] = stdtemp.substr(pos, stdtemp.length() - pos);
    }
    else if (!strcmp(attributeName, "LayoutAbove")){
        std::string stdtemp = attributeValue;
        uint pos = stdtemp.rfind("/", stdtemp.length()) + 1;
        m_pstRelativeLayoutPlug->m_ulRule[CRelativeLayoutPlug::EnumAlign::AboveOf] = stdtemp.substr(pos, stdtemp.length() - pos);
    }
    else if (!strcmp(attributeName, "LayoutBelow")){
        std::string stdtemp = attributeValue;
        uint pos = stdtemp.rfind("/", stdtemp.length()) + 1;
        m_pstRelativeLayoutPlug->m_ulRule[CRelativeLayoutPlug::EnumAlign::BelowOf] = stdtemp.substr(pos, stdtemp.length() - pos);
    }
    else if (!strcmp(attributeName, "LayoutAlignLeft")){
        std::string stdtemp = attributeValue;
        uint pos = stdtemp.rfind("/", stdtemp.length()) + 1;
        m_pstRelativeLayoutPlug->m_ulRule[CRelativeLayoutPlug::EnumAlign::AlignLeft] = stdtemp.substr(pos, stdtemp.length() - pos);
    }
    else if (!strcmp(attributeName, "LayoutAlignRight")){
        std::string stdtemp = attributeValue;
        uint pos = stdtemp.rfind("/", stdtemp.length()) + 1;
        m_pstRelativeLayoutPlug->m_ulRule[CRelativeLayoutPlug::EnumAlign::AlignRight] = stdtemp.substr(pos, stdtemp.length() - pos);
    }
    else if (!strcmp(attributeName, "LayoutAlignTop")){
        std::string stdtemp = attributeValue;
        uint pos = stdtemp.rfind("/", stdtemp.length()) + 1;
        m_pstRelativeLayoutPlug->m_ulRule[CRelativeLayoutPlug::EnumAlign::AlignTop] = stdtemp.substr(pos, stdtemp.length() - pos);
    }
    else if (!strcmp(attributeName, "LayoutAlignBottom")){
        std::string stdtemp = attributeValue;
        uint pos = stdtemp.rfind("/", stdtemp.length()) + 1;
        m_pstRelativeLayoutPlug->m_ulRule[CRelativeLayoutPlug::EnumAlign::AlignBottom] = stdtemp.substr(pos, stdtemp.length() - pos);
    }
    else if (!strcmp(attributeName, "LayoutAlignParentLeft")){
        if (!strcmp(attributeValue, "true")){
            m_pstRelativeLayoutPlug->m_ulRuleWithParent[CRelativeLayoutPlug::EnumAlignParent::AlignParentLeft] = true;
        }
    }
    else if (!strcmp(attributeName, "LayoutAlignParentTop")){
        if (!strcmp(attributeValue, "true")){
            m_pstRelativeLayoutPlug->m_ulRuleWithParent[CRelativeLayoutPlug::EnumAlignParent::AlignParentTop] = true;
        }
    }
    else if (!strcmp(attributeName, "LayoutAlignParentRight")){
        if (!strcmp(attributeValue, "true")){
            m_pstRelativeLayoutPlug->m_ulRuleWithParent[CRelativeLayoutPlug::EnumAlignParent::AlignParentRight] = true;
        }
    }
    else if (!strcmp(attributeName, "LayoutAlignParentBottom")){
        if (!strcmp(attributeValue, "true")){
            m_pstRelativeLayoutPlug->m_ulRuleWithParent[CRelativeLayoutPlug::EnumAlignParent::AlignParentBottom] = true;
        }
    }
    else if (!strcmp(attributeName, "LayoutCenterInParent")){
        if (!strcmp(attributeValue, "true")){
            m_pstRelativeLayoutPlug->m_ulRuleWithParent[CRelativeLayoutPlug::EnumAlignParent::CenterInParent] = true;
        }
    }
    else if (!strcmp(attributeName, "LayoutCenterHorizontal")){
        if (!strcmp(attributeValue, "true")){
            m_pstRelativeLayoutPlug->m_ulRuleWithParent[CRelativeLayoutPlug::EnumAlignParent::CenterHorizontal] = true;
        }
    }
    else if (!strcmp(attributeName, "LayoutCenterVertical")){
        if (!strcmp(attributeValue, "true")){
            m_pstRelativeLayoutPlug->m_ulRuleWithParent[CRelativeLayoutPlug::EnumAlignParent::CenterVertical] = true;
        }
    }
    else if (!strcmp(attributeName, "LayoutAlignWithParentIfMissing")){
        if (!strcmp(attributeValue, "true")){
            m_pstRelativeLayoutPlug->m_ulRuleWithParent[CRelativeLayoutPlug::EnumAlignParent::AlignWithParentIfMissing] = true;
        }
    }
    //RelativeLayout 适配 end
}

bool CVisualizationStage::OnMouseEvent(Mouse_Event event)
{
     m_onlyme = false;
    if(event.mouse_type == EVENT_MOUSE_TYPE_DOWN){
            // m_onlyme = true;
            // CUILib::Instance()->SetOnlyOne(true);
            // return true;
    }else if(event.mouse_type == EVENT_MOUSE_TYPE_UP || event.mouse_type == EVENT_MOUSE_TYPE_INVALID){
        if(CUILib::Instance()->GetOnlyOne()){
            CUILib::Instance()->SetOnlyOne(false);
        }
        m_onlyme = false;
    }
    return false;
}

void CVisualizationStage::OnSizeChanged(int sNewWidth, int sNewHeight, int sNewLength, int sOldWidth, int sOldHeight, int sOldLength)
{
    return;
}

void CVisualizationStage::OnMeasure(int iWidthMeasureSpec, int iHeightMeasureSpec, int iLengthMeasureSpec)
{
    //stage的测量与actor不同，在自适应的时候不能仅以背景做值，需要先设置为fillparent，等所有子元器件都测量完，在根据子元器件的值和自身背景值计算终值。
    UIRect outRect;

    switch(m_pstLayoutParam->m_iLayoutWidth){
        case CMarginLayoutPlug::MEASURE_MODE_FILLPARENT:
        case CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT:
                outRect.m_sWidth = iWidthMeasureSpec;
            break;
        default:
                outRect.m_sWidth = m_pstLayoutParam->m_iLayoutWidth;
            break;
    }
    switch(m_pstLayoutParam->m_iLayoutHeight){
        case CMarginLayoutPlug::MEASURE_MODE_FILLPARENT:
        case CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT:
                outRect.m_sHeight = iHeightMeasureSpec;
            break;
        default:
                outRect.m_sHeight = m_pstLayoutParam->m_iLayoutHeight;
            break;
    }

    SetMeasuredDimension(outRect.m_sWidth,outRect.m_sHeight, 0);
}

void CVisualizationStage::OnLayout(bool bChanged, int sLeft, int sTop, int sDepth, int sWidth, int sHeight, int length /*= 0*/)
{
    CActor::OnLayout(bChanged, sLeft, sTop, sDepth, sWidth, sHeight, length);
}

void CVisualizationStage::OnDraw(CUICanvasInterface &stCanvas)
{
    bool onlyone = CUILib::Instance()->GetOnlyOne();
    m_IsDraw = (!onlyone || m_onlyme) && (m_pstVisiablePlug->m_Visible == CVisiablePlug::ActorVisibleType::Visible);
    // Gone状态下不描画标注线和自身矩形框
    if((!onlyone || m_onlyme)  && (m_pstVisiablePlug->m_Visible != CVisiablePlug::ActorVisibleType::Gone)){
        DrawLeftTopLine(stCanvas);
        DrawSelfRectLine(stCanvas);
    }
    if (m_IsDraw && 0 != m_BackgroundtextureId)
    {
        UIRect stOutRect;
        GetDrawRectwithPadding(stOutRect);
        if(m_bIsNineGridDrawable){
            UIRect stDrawableRect;
            GetDrawableRect(m_BackgroundtextureId, stDrawableRect);
            stCanvas.DrawNineTexture(m_BackgroundtextureId, stDrawableRect, stOutRect);
        }else{
            stCanvas.DrawTexture(m_BackgroundtextureId, stOutRect);
        }
    }
    if(m_pstVisiablePlug->m_Visible != CVisiablePlug::ActorVisibleType::Gone){
        DispatchDraw(stCanvas);
    }
}

void CVisualizationStage::GetDrawRectwithPadding(UIRect &outRect)
{
    UIRect stDrawRect;
    GetDrawRect(stDrawRect);
    outRect.m_sLeft = stDrawRect.m_sLeft + m_pstPaddingPlug->m_iPaddingLeft;
    outRect.m_sTop = stDrawRect.m_sTop + m_pstPaddingPlug->m_iPaddingTop;
    outRect.m_sWidth = stDrawRect.m_sWidth - m_pstPaddingPlug->m_iPaddingLeft - m_pstPaddingPlug->m_iPaddingRight;
    outRect.m_sHeight = stDrawRect.m_sHeight - m_pstPaddingPlug->m_iPaddingTop - m_pstPaddingPlug->m_iPaddingBottom;
}

void CVisualizationStage::GetMyselfMeasure(int& iMeasuredWidth, int& iMeasureHeight)
{
    UIRect outRect;
    //获取描画内容物尺寸
    GetDrawableRect(m_BackgroundtextureId,outRect);
    iMeasuredWidth = outRect.m_sWidth;
    iMeasureHeight = outRect.m_sHeight;
}

void CVisualizationStage::SetBackground(std::string strPath)
{
    m_BackgroundtextureId = GetDrawableTextureID(strPath);
}

void CVisualizationStage::DrawLeftTopLine(CUICanvasInterface &stCanvas)
{
    float left = stCanvas.Action().m_current_matrix->getTranslateX();
    float top = stCanvas.Action().m_current_matrix->getTranslateY();
    int size = 6;
    float vecArcPoint[size] = {0.0f};
    stCanvas.SetLineWidth(1.0f);

    vecArcPoint[0] = -left;
    stCanvas.DrawLine(vecArcPoint,size, 150, 0, 0);
    std::string temp_str = std::to_string((int16_t)left);
    temp_str += "L";
    UIRect rect(-left / 2, -15, 0, 100, 30, 0);
    stCanvas.DrawString(temp_str.c_str(), rect, 15, 15, 150, 0, 0);

    vecArcPoint[0] = 0.0f;
    vecArcPoint[1] = -top;
    stCanvas.DrawLine(vecArcPoint, size, 150, 0, 0);
    temp_str = std::to_string((int16_t)top);
    temp_str += "T";
    rect.Set(0, -top / 2, 100, 30);
    stCanvas.DrawString(temp_str.c_str(), rect, 15, 15, 150, 0, 0);
}

void CVisualizationStage::DrawSelfRectLine(CUICanvasInterface &stCanvas)
{
    int size = 15;
    float vecArcPoint[size] = {
        0.0f,0.0f,0.0f,
        0.0f,(float)GetHeight(),0.0f,
        (float)GetWidth(),(float)GetHeight(),0.0f,
        (float)GetWidth(),0.0,0.0f,
        0.0f,0.0f,0.0f,
    };
    stCanvas.SetLineWidth(1.0f);
    stCanvas.DrawLine(vecArcPoint,size, 240, 0, 0);
}
