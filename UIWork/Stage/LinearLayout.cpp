#include "LinearLayout.h"

CLinearLayout::CLinearLayout()
{
  if(nullptr == m_pstUIGravityPlug){
    m_pstUIGravityPlug = new CUIGravityPlug();
  }
}
CLinearLayout::~CLinearLayout()
{
  if(nullptr != m_pstUIGravityPlug){
    delete m_pstUIGravityPlug;
    m_pstUIGravityPlug = NULL;
  }
}

void CLinearLayout::Inflate(TiXmlAttribute *attribute)
{
  CVisualizationStage::Inflate(attribute);
}

void CLinearLayout::SetActorAttribute(const char *attributeName,
                                              const char *attributeValue)
{
  CVisualizationStage::SetActorAttribute(attributeName, attributeValue);
  // 处理自己的 SetActorAttribute
  if (!strcmp(attributeName, "Orientation")){
    if (!strcmp(attributeValue, "Vertical")){
      m_Orientation = Orientation::Vertical;
    }
  }
  else if (!strcmp(attributeName, "WeightSum")){
    m_fWeightSum = atof(attributeValue);
  }
  else if (!strcmp(attributeName, "HorizontalGravity")){
    if (!strcmp(attributeValue, "Left")){
      m_pstUIGravityPlug->m_ulGravity = (m_pstUIGravityPlug->m_ulGravity & ~CUIGravity::HORIZENTAL_MASK) | CUIGravity::LEFT;
    }
    else if (!strcmp(attributeValue, "Right")){
      m_pstUIGravityPlug->m_ulGravity = (m_pstUIGravityPlug->m_ulGravity & ~CUIGravity::HORIZENTAL_MASK) | CUIGravity::RIGHT;
    }
    else{
      m_pstUIGravityPlug->m_ulGravity = (m_pstUIGravityPlug->m_ulGravity & ~CUIGravity::HORIZENTAL_MASK) | CUIGravity::CENTER_HORIZENTAL;
    }
  }
  else if (!strcmp(attributeName, "VerticalGravity")){
    if (!strcmp(attributeValue, "Top")){
      m_pstUIGravityPlug->m_ulGravity = (m_pstUIGravityPlug->m_ulGravity & ~CUIGravity::VERTICAL_MASK) | CUIGravity::TOP;
    }
    else if (!strcmp(attributeValue, "Bottom")){
      m_pstUIGravityPlug->m_ulGravity = (m_pstUIGravityPlug->m_ulGravity & ~CUIGravity::VERTICAL_MASK) | CUIGravity::BOTTOM;
    }
    else{
      m_pstUIGravityPlug->m_ulGravity = (m_pstUIGravityPlug->m_ulGravity & ~CUIGravity::VERTICAL_MASK) | CUIGravity::CENTER_VERTICAL;
    }
  }
}

const CUIGravityPlug *CLinearLayout::GetGravityPlug(void)
{
  return m_pstUIGravityPlug;
}

void CLinearLayout::SetGravityPlug(const CUIGravityPlug &stParam)
{
  if (m_pstUIGravityPlug != &stParam)
  {
    if (m_pstUIGravityPlug)
    {
      delete m_pstUIGravityPlug;
    }
    m_pstUIGravityPlug = stParam.Clone();
  }
}

/*
  m_fWeight是子元器件的属性：
    当横向LinearLayout宽度大于子元器件总体宽度，或者纵向LinearLayout高度大于子元器件总体高度时，
    会按照各个子元器件的LayoutWeight占总体Weight的比例向各个子元器件分配剩余空间。
  WeightSum是linearlayout的属性
    LinearLayout的总体Weight大小，如果为0.0f，那么在分配剩余空间时会自动计算各个子元器件的weight的总和作为总体Weight大小。
  例如：
  WeightSum = "1.0"
*/
void CLinearLayout::OnMeasure(int iWidthMeasureSpec, int iHeightMeasureSpec, int iLengthMeasureSpec)
{
  //老样子，先测量自己
  CVisualizationStage::OnMeasure(iWidthMeasureSpec, iHeightMeasureSpec, iLengthMeasureSpec);
  
  if (m_Orientation == Orientation::Horizontal)
  {
    MeasureHorizontal(iWidthMeasureSpec, iHeightMeasureSpec);
  }
  else
  {
    MeasureVertical(iWidthMeasureSpec, iHeightMeasureSpec);
  }
}

void CLinearLayout::MeasureHorizontal(int iWidthMeasureSpec, int iHeightMeasureSpec)
{
  m_iTotalLength = 0;//子元器件已使用的测量宽度
  int iMaxHeight = 0;//子元器件使用的最大测量高度，后续测量layout自适应高度值使用
  float fTotalWeight = 0.0f;//子元器件m_fWeight总和
  bool bFillParent = false;//是否layout的高度值是自适应，并且有子元器件高度值是fillparent的

  int iChildCount = GetChildCount();
  /*
    第一次轮询，4个目的：
    1.明确bFillParent是否为true，以便后续判断是否后续测量完自适应的layout高度值后，再重新测量高度为fillparent的子元器高度。
    2.计算出fTotalWeight：子元器件m_fWeight累加值，以便后续判断是否需要分配剩余空间
    3.计算出m_iTotalLength：子元器件使用的总测量值（=子元器件自身测量值 + 子元器件margin + layout的pading值）；m_iTotalLength和m_sMeasureWidth的差值是可以分配的剩余空间
    4.完成子元器件基础部分的测量，所谓基础部分就是两种情况：
      （1）如果layout自适应，需要测量，否则无法确认剩余空间，也就无法分配weight。这里说明下具体情况：
        [1].layout自适应，子元器件有正常值（宽度值非0），这种情况测量是不需要解释的：子元器件可以测量
        [2].layout自适应，子元器件宽度值是0，即完全由weight分配；这种情况下也需要测量，因为不测量就不知道剩余空间是多少，所以需要先按照子元器件[自适应]来测量，测量后的剩余空间根据weight分配。
      （2）子元器件的宽度值不为0。为0说明只使用剩余空间weight分配，所以不能测量，否则都需要测量
  */
  for (int iCount = 0; iCount < iChildCount; ++iCount)
  {
    CActor *pstActor = GetChildAt(iCount);
    if (pstActor == NULL ||  pstActor->GetVisiablePlug()->m_Visible == CVisiablePlug::Gone)
    {
      continue;
    }

    const CMarginLayoutPlug* pChildMarginLayoutPlug = pstActor->GetLayoutPlug();

    //累加子元器件m_fWeight
    fTotalWeight += pstActor->GetLinearLayoutPlug()->m_fWeight;
    if(m_pstLayoutParam->m_iLayoutHeight == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT && !bFillParent){
      //自身height为自适应并且有子元器件的高度为fillparent
      bFillParent = (pChildMarginLayoutPlug->m_iLayoutHeight == CMarginLayoutPlug::MEASURE_MODE_FILLPARENT);
    }
  
    //因为还无法计算子元器件的占比宽度，所以只能先将margin加上
    m_iTotalLength += (pChildMarginLayoutPlug->m_sLeftMargin + pChildMarginLayoutPlug->m_sRightMargin);
    //测量基础部分，判断原因见上【第一次轮询，4个目的】
    if((m_pstLayoutParam->m_iLayoutWidth == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT) || (pChildMarginLayoutPlug->m_iLayoutWidth != 0)){
      bool bChangeWidth = false;
      //如果子元器件只使用的是m_fWeight
      if(pChildMarginLayoutPlug->m_iLayoutWidth == 0 && pstActor->GetLinearLayoutPlug()->m_fWeight > 0.0f){
        //先把子元器件的m_sWidth设成自适应，以便测量子元器件宽高能正常让其自适应，而不是0
        const_cast<CMarginLayoutPlug*>(pChildMarginLayoutPlug)->m_iLayoutWidth = CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT;
        bChangeWidth = true;
      }
      //测量子元器件宽高,m_iTotalLength含义是前面actor已使用的宽（高）度值
      MeasureChildWithPaddingWidthLinear(pstActor, m_sMeasureWidth, m_iTotalLength, m_sMeasureHeight, 0);
      //如果将子元器件的m_sWidth设成自适应了，那（bChangeWidth）需要改回来
      if(bChangeWidth){
        const_cast<CMarginLayoutPlug*>(pChildMarginLayoutPlug)->m_iLayoutWidth = 0;
      }

      //m_iTotalLength += 子元器件的测量宽度
      int iWidth = 0;
      int iHeight = 0;
      int iLength = 0;
      pstActor->GetMeasuredDimension(iWidth,iHeight, iLength);
      m_iTotalLength += iWidth;
      //如果layout高度自适应，需要计算子元器件的最大高度值
      if(m_pstLayoutParam->m_iLayoutHeight == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT){
        int iTemp = pChildMarginLayoutPlug->m_sTopMargin + pChildMarginLayoutPlug->m_sBottomMargin;
        //如果layout高度自适应，不能使用高度为fillparent的子元器件测量高度，否则就必是layout的parent高度了
        if(pChildMarginLayoutPlug->m_iLayoutHeight != CMarginLayoutPlug::MEASURE_MODE_FILLPARENT){
          iTemp += iHeight;
        }
        iMaxHeight = UI_MAX(iMaxHeight, iTemp);
      }
    }    
  }

  //m_iTotalLength计算完毕；是目前为止子元器件已使用的宽度值，只差Weight相关计算了。
  m_iTotalLength += (m_pstPaddingPlug->m_iPaddingLeft + m_pstPaddingPlug->m_iPaddingRight);
  //完成layout的宽度测量
  int iMyselfMeasureWidth = 0;
  int iMyselfMeasureHeight = 0;
  GetMyselfMeasure(iMyselfMeasureWidth, iMyselfMeasureHeight);
  if(m_pstLayoutParam->m_iLayoutWidth == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT){
    int iwidth = UI_MAX(m_iTotalLength, iMyselfMeasureWidth);
		iwidth = UI_MIN(iwidth, iWidthMeasureSpec);
		SetMeasuredDimension(iwidth,m_sMeasureHeight, 0);
  }

  //layout测量值和子元器件已使用的宽度值有差值，说明有Weight富余空间：iDelta
  int iDelta = m_sMeasureWidth - m_iTotalLength;
  //判断是否有富余空间给子元器件使用Weight，并且子元器件使用了Weight计算宽度值
  if((iDelta != 0) && (fTotalWeight > 0.0f)){
    m_iTotalLength = 0;
    fTotalWeight = m_fWeightSum > 0.0f ? m_fWeightSum : fTotalWeight;//如果layout定义了总值使用layout定义的，否则使用子元器件加等的。
    /*
      第二次轮询，1个目的：
        对剩余空间按照子元器件的weight值分配并完成测量；测量分两种：
          1.之前已经测量了基础部分，那么就是将基础测量值 + 分配值完成测量
          2.之前没有测量过，即全部使用weight分配的子元器，那么就完全使用分配值完成测量
    */
    for (int iCount = 0; iCount < iChildCount; ++iCount)
    {
      CActor *pstActor = GetChildAt(iCount);
      if (pstActor == NULL ||  pstActor->GetVisiablePlug()->m_Visible == CVisiablePlug::Gone)
      {
        continue;
      }
      const CLinearLayoutPlug* pChildLinearLayoutPlug = pstActor->GetLinearLayoutPlug();
      const CMarginLayoutPlug* pChildLayout = pstActor->GetLayoutPlug();
      int iChildWidthMeasureSpec = 0;
      int iChildHeightMeasure = 0;
      int iChildLengthMeasure = 0;
      //说明这个子元器件要使用Weight富余空间
      if(pChildLinearLayoutPlug->m_fWeight > 0.0f){
        int iused = static_cast<int>(iDelta * (pChildLinearLayoutPlug->m_fWeight / fTotalWeight));//获取到该子元器件要使用的富余空间
        // 如果子元器件完成过基础测量
        if((m_pstLayoutParam->m_iLayoutWidth == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT) || (pChildLayout->m_iLayoutWidth != 0)){
          pstActor->GetMeasuredDimension(iChildWidthMeasureSpec, iChildHeightMeasure, iChildLengthMeasure);
          pstActor->Measure(iChildWidthMeasureSpec + iused, iChildHeightMeasure, 0);
        }
        //子元器件没有完成过基础测量，只使用的是m_fWeight
        else{
          //获取子元器件测量高度允许值；因为这个子元器件没有测量过，所以不能使用GetMeasuredDimension
          int iChildHeightMeasureSpec = GetChildMeasureSpec(pstActor, m_sMeasureHeight, pChildLayout->m_iLayoutHeight);
          int iOldWidth = pChildLayout->m_iLayoutWidth;
          const_cast<CMarginLayoutPlug*>(pChildLayout)->m_iLayoutWidth = iused;
          pstActor->Measure(iused, iChildHeightMeasureSpec, 0);
          const_cast<CMarginLayoutPlug*>(pChildLayout)->m_iLayoutWidth = iOldWidth;
        }   
      }
      //如果layout高度自适应，需要计算子元器件的最大高度值
      pstActor->GetMeasuredDimension(iChildWidthMeasureSpec, iChildHeightMeasure, iChildLengthMeasure);
      if(m_pstLayoutParam->m_iLayoutHeight == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT){
          int iTemp = pChildLayout->m_sTopMargin + pChildLayout->m_sBottomMargin;
          //如果layout高度自适应，不能使用高度为fillparent的子元器件测量高度，否则就必是layout的parent高度了
          if(pChildLayout->m_iLayoutHeight != CMarginLayoutPlug::MEASURE_MODE_FILLPARENT){
            iTemp += iChildHeightMeasure;
          }
          iMaxHeight = UI_MAX(iMaxHeight, iTemp);
      }
      m_iTotalLength += (iChildWidthMeasureSpec + pChildLayout->m_sLeftMargin + pChildLayout->m_sRightMargin);
    }
    m_iTotalLength += (m_pstPaddingPlug->m_iPaddingLeft + m_pstPaddingPlug->m_iPaddingRight);
  }

  //完成layout的高度测量
  if(m_pstLayoutParam->m_iLayoutHeight == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT){
    iMaxHeight += (m_pstPaddingPlug->m_iPaddingTop + m_pstPaddingPlug->m_iPaddingBottom);
    int iHeight = UI_MAX(iMaxHeight, iMyselfMeasureHeight);
		iHeight = UI_MIN(iHeight, iHeightMeasureSpec);
		SetMeasuredDimension(m_sMeasureWidth,iHeight, 0);
    //如果有子元器件高度值是fillparent，需要重新测量其高度值
    if(bFillParent){
      for (int iCount = 0; iCount < iChildCount; ++iCount)
      {
        CActor *pstActor = GetChildAt(iCount);
        if (pstActor == NULL ||  pstActor->GetVisiablePlug()->m_Visible == CVisiablePlug::Gone)
        {
          continue;
        }
        const CMarginLayoutPlug* pChildLayout = pstActor->GetLayoutPlug();
        if(pChildLayout->m_iLayoutHeight == CMarginLayoutPlug::MEASURE_MODE_FILLPARENT){
          int iOldWidth = pChildLayout->m_iLayoutWidth;
          int iChildWidth = 0;
          int iChildHeight = 0;
          int iChildLength = 0;
          pstActor->GetMeasuredDimension(iChildWidth, iChildHeight, iChildLength);
          const_cast<CMarginLayoutPlug*>(pChildLayout)->m_iLayoutWidth = iChildWidth;
          MeasureChildWithMargin(pstActor, m_sMeasureWidth, m_sMeasureHeight, 0);
          const_cast<CMarginLayoutPlug*>(pChildLayout)->m_iLayoutWidth = iOldWidth;
        }
      }
    }
  }
}

void CLinearLayout::MeasureVertical(int iWidthMeasureSpec, int iHeightMeasureSpec)
{
  m_iTotalLength = 0;//子元器件已使用的测量宽度
  int iMaxWidth = 0;//子元器件使用的最大测量宽度，后续测量layout自适应宽度值使用
  float fTotalWeight = 0.0f;//子元器件m_fWeight总和
  bool bFillParent = false;//是否layout的高度值是自适应，并且有子元器件高度值是fillparent的

  int iChildCount = GetChildCount();
  /*
    第一次轮询，4个目的：
    1.明确bFillParent是否为true，以便后续判断是否后续测量完自适应的layout宽度值后，再重新测量宽度为fillparent的子元器宽度。
    2.计算出fTotalWeight：子元器件m_fWeight累加值，以便后续判断是否需要分配剩余空间
    3.计算出m_iTotalLength：子元器件使用的总测量值（=子元器件自身测量值 + 子元器件margin + layout的pading值）；m_iTotalLength和m_sMeasureWidth的差值是可以分配的剩余空间
    4.完成子元器件基础部分的测量，所谓基础部分就是两种情况：
      （1）如果layout自适应，需要测量，否则无法确认剩余空间，也就无法分配weight。这里说明下具体情况：
        [1].layout自适应，子元器件有正常值（高度值非0），这种情况测量是不需要解释的：子元器件可以测量
        [2].layout自适应，子元器件高度值是0，即完全由weight分配；这种情况下也需要测量，因为不测量就不知道剩余空间是多少，所以需要先按照子元器件[自适应]来测量，测量后的剩余空间根据weight分配。
      （2）子元器件的高度值不为0。为0说明只使用剩余空间weight分配，所以不能测量，否则都需要测量
  */
  for (int iCount = 0; iCount < iChildCount; ++iCount)
  {
    CActor *pstActor = GetChildAt(iCount);
    if (pstActor == NULL ||  pstActor->GetVisiablePlug()->m_Visible == CVisiablePlug::Gone)
    {
      continue;
    }

    const CMarginLayoutPlug* pChildMarginLayoutPlug = pstActor->GetLayoutPlug();

    //累加子元器件m_fWeight
    fTotalWeight += pstActor->GetLinearLayoutPlug()->m_fWeight;
    if(m_pstLayoutParam->m_iLayoutWidth == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT && !bFillParent){
      //自身height为自适应并且有子元器件的高度为fillparent
      bFillParent = (pChildMarginLayoutPlug->m_iLayoutWidth == CMarginLayoutPlug::MEASURE_MODE_FILLPARENT);
    }
  
    //因为还无法计算子元器件的占比高度，所以只能先将margin加上
    m_iTotalLength += (pChildMarginLayoutPlug->m_sTopMargin + pChildMarginLayoutPlug->m_sBottomMargin);
    //测量基础部分，判断原因见上【第一次轮询，4个目的】
    if((m_pstLayoutParam->m_iLayoutHeight == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT) || (pChildMarginLayoutPlug->m_iLayoutHeight != 0)){
      bool bChangeWidth = false;
      //如果子元器件只使用的是m_fWeight
      if(pChildMarginLayoutPlug->m_iLayoutHeight == 0 && pstActor->GetLinearLayoutPlug()->m_fWeight > 0.0f){
        //先把子元器件的m_sHeight设成自适应，以便测量子元器件高高能正常让其自适应，而不是0
        const_cast<CMarginLayoutPlug*>(pChildMarginLayoutPlug)->m_iLayoutHeight = CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT;
        bChangeWidth = true;
      }
      //测量子元器件宽高,m_iTotalLength含义是前面actor已使用的宽（高）度值
      MeasureChildWithPaddingWidthLinear(pstActor, m_sMeasureWidth, 0, m_sMeasureHeight, m_iTotalLength);
      //如果将子元器件的m_sHeight设成自适应了，那（bChangeWidth）需要改回来
      if(bChangeWidth){
        const_cast<CMarginLayoutPlug*>(pChildMarginLayoutPlug)->m_iLayoutHeight = 0;
      }

      //m_iTotalLength += 子元器件的测量高度
      int iWidth = 0;
      int iHeight = 0;
      int iLength = 0;
      pstActor->GetMeasuredDimension(iWidth,iHeight,iLength);
      m_iTotalLength += iHeight;
      //如果layout宽度自适应，需要计算子元器件的最大宽度值
      if(m_pstLayoutParam->m_iLayoutWidth == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT){
        int iTemp = pChildMarginLayoutPlug->m_sLeftMargin + pChildMarginLayoutPlug->m_sRightMargin;
        //如果layout宽度自适应，不能使用宽度为fillparent的子元器件测量高度，否则就必是layout的parent宽度度了
        if(pChildMarginLayoutPlug->m_iLayoutWidth != CMarginLayoutPlug::MEASURE_MODE_FILLPARENT){
          iTemp += iWidth;
        }
        iMaxWidth = UI_MAX(iMaxWidth, iTemp);
      }
    }    
  }

  //m_iTotalLength计算完毕；是目前为止子元器件已使用的高度值，只差Weight相关计算了。
  m_iTotalLength += (m_pstPaddingPlug->m_iPaddingTop + m_pstPaddingPlug->m_iPaddingBottom);
  int iMyselfMeasureWidth = 0;
  int iMyselfMeasureHeight = 0;
  GetMyselfMeasure(iMyselfMeasureWidth, iMyselfMeasureHeight);
  //完成layout的高度测量
  if(m_pstLayoutParam->m_iLayoutHeight == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT){
    int iHeight = UI_MAX(m_iTotalLength, iMyselfMeasureHeight);
		iHeight = UI_MIN(iHeight, iHeightMeasureSpec);
		SetMeasuredDimension(m_sMeasureWidth,iHeight, 0);
  }

  //layout测量值和子元器件已使用的高度值有差值，说明有Weight富余空间：iDelta
  int iDelta = m_sMeasureHeight - m_iTotalLength;
  //判断是否有富余空间给子元器件使用Weight，并且子元器件使用了Weight计算高度值
  if((iDelta != 0) && (fTotalWeight > 0.0f)){
    m_iTotalLength = 0;
    fTotalWeight = m_fWeightSum > 0.0f ? m_fWeightSum : fTotalWeight;//如果layout定义了总值使用layout定义的，否则使用子元器件加等的。
    /*
      第二次轮询，1个目的：
        对剩余空间按照子元器件的weight值分配并完成测量；测量分两种：
          1.之前已经测量了基础部分，那么就是将基础测量值 + 分配值完成测量
          2.之前没有测量过，即全部使用weight分配的子元器，那么就完全使用分配值完成测量
    */
    for (int iCount = 0; iCount < iChildCount; ++iCount)
    {
      CActor *pstActor = GetChildAt(iCount);
      if (pstActor == NULL ||  pstActor->GetVisiablePlug()->m_Visible == CVisiablePlug::Gone)
      {
        continue;
      }
      const CLinearLayoutPlug* pChildLinearLayoutPlug = pstActor->GetLinearLayoutPlug();
      const CMarginLayoutPlug* pChildLayout = pstActor->GetLayoutPlug();
      int iChildWidthMeasureSpec = 0;
      int iChildHeightMeasure = 0;
      int iChildLengthMeasure = 0;
      //说明这个子元器件要使用Weight富余空间
      if(pChildLinearLayoutPlug->m_fWeight > 0.0f){
        int iused = static_cast<int>(iDelta * (pChildLinearLayoutPlug->m_fWeight / fTotalWeight));//获取到该子元器件要使用的富余空间
        
        // 如果子元器件完成过基础测量
        if((m_pstLayoutParam->m_iLayoutHeight == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT) || (pChildLayout->m_iLayoutHeight != 0)){
          pstActor->GetMeasuredDimension(iChildWidthMeasureSpec, iChildHeightMeasure, iChildLengthMeasure);
          pstActor->Measure(iChildWidthMeasureSpec, iChildHeightMeasure + iused, 0);
        }
        //子元器件没有完成过基础测量，只使用的是m_fWeight
        else{
          //获取子元器件测量宽度允许值；因为这个子元器件没有测量过，所以不能使用GetMeasuredDimension
          int iChildWidthMeasureSpec = GetChildMeasureSpec(pstActor, m_sMeasureHeight, pChildLayout->m_iLayoutWidth);
          int iOldHeight = pChildLayout->m_iLayoutHeight;
          const_cast<CMarginLayoutPlug*>(pChildLayout)->m_iLayoutHeight = iused;
          pstActor->Measure(iChildWidthMeasureSpec, iused, 0);
          const_cast<CMarginLayoutPlug*>(pChildLayout)->m_iLayoutHeight = iOldHeight;
        }   
      }
      //如果layout宽度自适应，需要计算子元器件的最大高度值
      pstActor->GetMeasuredDimension(iChildWidthMeasureSpec, iChildHeightMeasure, iChildLengthMeasure);
      if(m_pstLayoutParam->m_iLayoutWidth == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT){
          int iTemp = pChildLayout->m_sLeftMargin + pChildLayout->m_sRightMargin;
          //如果layout宽度自适应，不能使用宽度为fillparent的子元器件测量宽度，否则就必是layout的parent宽度了
          if(pChildLayout->m_iLayoutHeight != CMarginLayoutPlug::MEASURE_MODE_FILLPARENT){
            iTemp += iChildWidthMeasureSpec;
          }
          iMaxWidth = UI_MAX(iMaxWidth, iTemp);
      }
      m_iTotalLength += (iChildHeightMeasure + pChildLayout->m_sTopMargin + pChildLayout->m_sBottomMargin);
    }
    m_iTotalLength += (m_pstPaddingPlug->m_iPaddingTop + m_pstPaddingPlug->m_iPaddingBottom);
  }

  //完成layout的宽度测量
  if(m_pstLayoutParam->m_iLayoutWidth == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT){
    iMaxWidth += (m_pstPaddingPlug->m_iPaddingLeft + m_pstPaddingPlug->m_iPaddingTop);
    int iWidth = UI_MAX(iMaxWidth, iMyselfMeasureWidth);
		iWidth = UI_MIN(iWidth, iWidthMeasureSpec);
		SetMeasuredDimension(iWidth,m_sMeasureHeight, 0);
    //如果有子元器件宽度值是fillparent，需要重新测量其宽度值
    if(bFillParent){
      for (int iCount = 0; iCount < iChildCount; ++iCount)
      {
        CActor *pstActor = GetChildAt(iCount);
        if (pstActor == NULL ||  pstActor->GetVisiablePlug()->m_Visible == CVisiablePlug::Gone)
        {
          continue;
        }
        const CMarginLayoutPlug* pChildLayout = pstActor->GetLayoutPlug();
        if(pChildLayout->m_iLayoutWidth == CMarginLayoutPlug::MEASURE_MODE_FILLPARENT){
          int iOldHeight = pChildLayout->m_iLayoutHeight;
          int iChildWidth = 0;
          int iChildHeight = 0;
          int iChildLength = 0;
          pstActor->GetMeasuredDimension(iChildWidth, iChildHeight, iChildLength);
          const_cast<CMarginLayoutPlug*>(pChildLayout)->m_iLayoutHeight = iChildHeight;
          MeasureChildWithMargin(pstActor, m_sMeasureWidth, m_sMeasureHeight, 0);
          const_cast<CMarginLayoutPlug*>(pChildLayout)->m_iLayoutHeight = iOldHeight;
        }
      }
    }
  }
}

void CLinearLayout::MeasureChildWithPaddingWidthLinear(CActor *pstActor, int iWidthParentMeasureSpec, int iWidthUsed, int iHeightParentMeasureSpec, int iHeightUsed)
{
  const CMarginLayoutPlug *childPstLayoutParam = pstActor->GetLayoutPlug();
  const CPaddingPlug *ParentPstPaddingPlug = GetPaddingPlug();
  int marginHorizontal = 
      ParentPstPaddingPlug->m_iPaddingLeft + ParentPstPaddingPlug->m_iPaddingRight + iWidthUsed;
  int marginVerticalGravity = 
      ParentPstPaddingPlug->m_iPaddingTop + ParentPstPaddingPlug->m_iPaddingBottom + iHeightUsed;

  int iWidthChildMeasureSpec = GetChildMeasureSpec(pstActor, iWidthParentMeasureSpec - marginHorizontal, childPstLayoutParam->m_iLayoutWidth);
  int iHeightChildMeasureSpec = GetChildMeasureSpec(pstActor, iHeightParentMeasureSpec - marginVerticalGravity, childPstLayoutParam->m_iLayoutHeight);
  pstActor->Measure(iWidthChildMeasureSpec, iHeightChildMeasureSpec, 0);
}

void CLinearLayout::OnLayout(bool bChanged, int sLeft, int sTop, int sDepth, int sWidth, int sHeight, int length /*= 0*/)
{
  (void)sLeft;
  (void)sTop;
  (void)bChanged;

  const CPaddingPlug *ParentPstPaddingPlug = GetPaddingPlug();
  UIRect stParentRect(ParentPstPaddingPlug->m_iPaddingLeft, ParentPstPaddingPlug->m_iPaddingTop, 0,
                      sWidth - ParentPstPaddingPlug->m_iPaddingLeft - ParentPstPaddingPlug->m_iPaddingRight,
                      sHeight - ParentPstPaddingPlug->m_iPaddingTop - ParentPstPaddingPlug->m_iPaddingBottom,
                      0);

  UIRect stAllChildrenRect;
  int iAllchidrenWidth = stParentRect.m_sWidth;
  int iAllchidrenHeight = stParentRect.m_sHeight;
  //由于m_iTotalLength中包含了m_sPadding，在计算所有子元器件水平方向的居中时，要减去Padding的值。
  if (m_Orientation == Orientation::Horizontal){
    iAllchidrenWidth = m_iTotalLength - ParentPstPaddingPlug->m_iPaddingLeft - ParentPstPaddingPlug->m_iPaddingRight;
  }else{
    iAllchidrenHeight = m_iTotalLength - ParentPstPaddingPlug->m_iPaddingTop - ParentPstPaddingPlug->m_iPaddingBottom;
  }
  //这里需要根据layout的[m_pstUIGravityPlug]计算子元器件整体的位置:stAllChildrenRect
  CUIGravity::Apply(m_pstUIGravityPlug->m_ulGravity, stParentRect, iAllchidrenWidth, iAllchidrenHeight, 0, 0, 0, 0, 0, 0, 0, stAllChildrenRect);

  int iChildMeasureWidth = 0;
  int iChildMeasureHeight = 0;
  int iChildMeasureLength = 0;
  int iChildPassed = (m_Orientation == Orientation::Horizontal) ? stAllChildrenRect.m_sLeft : stAllChildrenRect.m_sTop;//iChildPassed即前面子元器件占用的距离
  int iChildCount = GetChildCount();
  for (int i = 0; i < iChildCount; ++i)
  {
    CActor *pstActor = GetChildAt(i);
    const CVisiablePlug* pstVisiablePlug = pstActor->GetVisiablePlug();
    if (pstActor && (pstVisiablePlug->m_Visible != CVisiablePlug::Gone))
    {
      const CMarginLayoutPlug *pstChildLayoutPlug = pstActor->GetLayoutPlug();
      uint childGravity = CUIGravity::NONE;
      //横向layout去掉水平方向的居中方式，纵向去掉竖直方向的。
      int enumPassedGravity = (m_Orientation == Orientation::Horizontal) ? CUIGravity::HORIZENTAL_MASK : CUIGravity::VERTICAL_MASK;
      childGravity = pstActor->GetUILayoutGravityPlug()->m_ulGravity & ~enumPassedGravity;
      //横向layout如果子元器件没有设置竖直方向的居中方式，那就使用layout的[VerticalGravity];纵向如果子元器件没有设置水平方向的，使用layoutd的[HorizontalGravity]
      int enumUsedGravity = (m_Orientation == Orientation::Horizontal) ? CUIGravity::VERTICAL_MASK : CUIGravity::HORIZENTAL_MASK;
      if((childGravity & enumUsedGravity) == CUIGravity::NONE){
        childGravity = m_pstUIGravityPlug->m_ulGravity & ~enumPassedGravity;
      }
      
      pstActor->GetMeasuredDimension(iChildMeasureWidth, iChildMeasureHeight, iChildMeasureLength);
      UIRect stOutRect;
      CUIGravity::Apply(childGravity, stAllChildrenRect, 
                      iChildMeasureWidth, iChildMeasureHeight, 0,
                      pstChildLayoutPlug->m_sLeftMargin, pstChildLayoutPlug->m_sTopMargin, 0,
                      pstChildLayoutPlug->m_sRightMargin, pstChildLayoutPlug->m_sBottomMargin, 0,
                      stOutRect);
      if (m_Orientation == Orientation::Horizontal)
      {
        iChildPassed += (stOutRect.m_sLeft - stAllChildrenRect.m_sLeft);
        pstActor->Layout(iChildPassed, stOutRect.m_sTop, sDepth, stOutRect.m_sWidth, stOutRect.m_sHeight, length);
        iChildPassed += (stOutRect.m_sWidth + pstChildLayoutPlug->m_sRightMargin);
      }else{
        iChildPassed += (stOutRect.m_sTop - stAllChildrenRect.m_sTop);
        pstActor->Layout(stOutRect.m_sLeft, iChildPassed, sDepth, stOutRect.m_sWidth, stOutRect.m_sHeight, length);
        iChildPassed += (stOutRect.m_sHeight + pstChildLayoutPlug->m_sBottomMargin);
      }
    }
  }
}

void CLinearLayout::OnDraw(CUICanvasInterface &stCanvas)
{
  CVisualizationStage::OnDraw(stCanvas);
}