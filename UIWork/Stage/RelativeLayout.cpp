#include "RelativeLayout.h"

CRelativeLayout::CRelativeLayout()
{
  if(nullptr == m_pstUIGravityPlug){
    m_pstUIGravityPlug = new CUIGravityPlug();
  }
}
CRelativeLayout::~CRelativeLayout()
{
  if(nullptr != m_pstUIGravityPlug){
    delete m_pstUIGravityPlug;
    m_pstUIGravityPlug = NULL;
  }
}

void CRelativeLayout::Inflate(TiXmlAttribute *attribute)
{
  CVisualizationStage::Inflate(attribute);
}

void CRelativeLayout::SetActorAttribute(const char *attributeName,
                                              const char *attributeValue)
{
  CVisualizationStage::SetActorAttribute(attributeName, attributeValue);
  // 处理自己的 SetActorAttribute
  if (!strcmp(attributeName, "HorizontalGravity")){
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

const CUIGravityPlug *CRelativeLayout::GetGravityPlug(void)
{
  return m_pstUIGravityPlug;
}

void CRelativeLayout::SetGravityPlug(const CUIGravityPlug &stParam)
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

bool CRelativeLayout::AddActor(CActor *pstActor)
{
  bool bResult = CVisualizationStage::AddActor(pstActor);
  if (bResult)
  {
      m_bDirtyGraph = true;
  }
  return bResult;
}

bool CRelativeLayout::UpdateActorLayout(CActor *pstActor, const UIRect &stParam)
{
  bool bResult = CVisualizationStage::UpdateActorLayout(pstActor, stParam);
  if (bResult)
  {
      m_bDirtyGraph = true;
  }
  return bResult;
}

bool CRelativeLayout::RemoveActor(CActor *pstActor)
{
  bool bResult = CStage::RemoveActor(pstActor);
  if (bResult)
  {
      m_bDirtyGraph = true;
  }
  return bResult;
}

void CRelativeLayout::OnMeasure(int iWidthMeasureSpec, int iHeightMeasureSpec, int iLengthMeasureSpec)
{
  //老样子，先测量自己
  CVisualizationStage::OnMeasure(iWidthMeasureSpec, iHeightMeasureSpec, iLengthMeasureSpec);
  //如果子元器件有变动，需要重新整理所有子元器件；否则不需要
  if (m_bDirtyGraph)
  {
    m_bDirtyGraph = false;
    //整理所有子元器件
    SortChildren();
  }
  int iWidth = 0;
  int iHeight = 0;

  //轮询所有横向定位排序子元器件集合，根据依赖关系，依次测量
  int iSortedCount = static_cast<int>(m_stHorizontalSortedChildVec.size());
  for (int iCount = 0; iCount < iSortedCount; ++iCount)
  {
    CActor *pstActor = m_stHorizontalSortedChildVec[iCount];
    if (pstActor->GetVisiablePlug()->m_Visible == CVisiablePlug::ActorVisibleType::Gone)
    {
      continue;
    }
    //计算并保存到Relativelayout边界的距离，即pre_。
    //因为这个方法是根据RelayActor测量出的当前actor距离layout的值，所以需要从头到位有序测量
    ApplyHorizontalSizeRule(pstActor, m_sMeasureWidth);
    //根据pre_等值获取子元器件测量的允许值。
    int iChildWidthMeasureSpec = GetRelativeChildMeasureSpec(
      pstActor->GetRelativeLayoutPlug()->m_iPreLeft, pstActor->GetRelativeLayoutPlug()->m_iPreRight, pstActor->GetLayoutPlug()->m_iLayoutWidth,
      pstActor->GetLayoutPlug()->m_sLeftMargin, pstActor->GetLayoutPlug()->m_sRightMargin, 
      GetPaddingPlug()->m_iPaddingLeft, GetPaddingPlug()->m_iPaddingRight, m_sMeasureWidth);
    //测量子元器件
    pstActor->Measure(iChildWidthMeasureSpec, m_sMeasureHeight, 0);
    //检查并重新计算m_iPre_值
    PositionChildHorizontal(pstActor, m_sMeasureWidth, GetLayoutPlug()->m_iLayoutWidth == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT);
  }

  iSortedCount = static_cast<int>(m_stVerticalSortedChildVec.size());
  for (int iCount = 0; iCount < iSortedCount; ++iCount)
  {
    CActor *pstActor = m_stVerticalSortedChildVec[iCount];
    if (pstActor->GetVisiablePlug()->m_Visible == CVisiablePlug::ActorVisibleType::Gone)
    {
      continue;
    }
    //计算并保存到Relativelayout边界的距离，即pre_
    //因为这个方法是根据RelayActor测量出的当前actor距离layout的值，所以需要从头到位有序测量
    ApplyVerticalSizeRule(pstActor, m_sMeasureHeight);
    //根据pre_等值获取子元器件测量的允许值。
    int iChildWidthMeasureSpec = GetRelativeChildMeasureSpec(
      pstActor->GetRelativeLayoutPlug()->m_iPreLeft, pstActor->GetRelativeLayoutPlug()->m_iPreRight, pstActor->GetLayoutPlug()->m_iLayoutWidth,
      pstActor->GetLayoutPlug()->m_sLeftMargin, pstActor->GetLayoutPlug()->m_sRightMargin, 
      GetPaddingPlug()->m_iPaddingLeft, GetPaddingPlug()->m_iPaddingRight, m_sMeasureWidth);
    int iChildHeightMeasureSpec = GetRelativeChildMeasureSpec(
      pstActor->GetRelativeLayoutPlug()->m_iPreTop, pstActor->GetRelativeLayoutPlug()->m_iPreBottom, pstActor->GetLayoutPlug()->m_iLayoutHeight,
      pstActor->GetLayoutPlug()->m_sTopMargin, pstActor->GetLayoutPlug()->m_sBottomMargin, 
      GetPaddingPlug()->m_iPaddingTop, GetPaddingPlug()->m_iPaddingBottom,m_sMeasureHeight);
    //测量子元器件
    pstActor->Measure(iChildWidthMeasureSpec, iChildHeightMeasureSpec, 0);
    //检查并重新计算m_iPre_值
    PositionChildVertical(pstActor, m_sMeasureHeight, GetLayoutPlug()->m_iLayoutHeight == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT);
    //计算子元器件最大可用值，用于到时候测量自适应的layout
    iWidth = UI_MAX(iWidth, pstActor->GetRelativeLayoutPlug()->m_iPreRight + pstActor->GetLayoutPlug()->m_sRightMargin);
    iHeight = UI_MAX(iHeight, pstActor->GetRelativeLayoutPlug()->m_iPreBottom + pstActor->GetLayoutPlug()->m_sBottomMargin);
  }

  //计算单纯由所有Actor组成区域的大小
  int iContentWidth = iWidth - GetPaddingPlug()->m_iPaddingLeft;
  int iContentHeight = iHeight - GetPaddingPlug()->m_iPaddingTop;

  int iChildCount = GetChildCount();
  //如果layout宽或高自适应
  if (GetLayoutPlug()->m_iLayoutWidth == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT 
    || GetLayoutPlug()->m_iLayoutHeight == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT)
  {
    // RelativeLayout的Size不是确定值时，使用Actor组成区域的大小，计算所有居中Actor的位置
    for (int iCount = 0; iCount < iChildCount; ++iCount)
    {
      CActor *pstActor = GetChildAt(iCount);
      if (pstActor->GetVisiablePlug()->m_Visible == CVisiablePlug::ActorVisibleType::Gone)
      {
        continue;
      }
      int iChildMeasuredWidth = 0;
      int iChildMeasuredHeight = 0;
      int iChildMeasuredLength = 0;
      pstActor->GetMeasuredDimension(iChildMeasuredWidth, iChildMeasuredHeight, iChildMeasuredLength);
      CRelativeLayoutPlug* pstCRelativeLayoutPlug = const_cast<CRelativeLayoutPlug*>(pstActor->GetRelativeLayoutPlug());
      //layout宽度自适应
      if (GetLayoutPlug()->m_iLayoutWidth == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT 
        && (pstActor->GetRelativeLayoutPlug()->m_ulRuleWithParent[CRelativeLayoutPlug::EnumAlignParent::CenterInParent]
        || pstActor->GetRelativeLayoutPlug()->m_ulRuleWithParent[CRelativeLayoutPlug::EnumAlignParent::CenterHorizontal]))
      {
        pstCRelativeLayoutPlug->m_iPreLeft = (iContentWidth - iChildMeasuredWidth) / 2;
        pstCRelativeLayoutPlug->m_iPreLeft = UI_MAX(pstCRelativeLayoutPlug->m_iPreLeft, 0);
        pstCRelativeLayoutPlug->m_iPreRight = pstCRelativeLayoutPlug->m_iPreLeft + iChildMeasuredWidth;
      }
      //layout高度自适应
      if (GetLayoutPlug()->m_iLayoutHeight == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT
        && (pstActor->GetRelativeLayoutPlug()->m_ulRuleWithParent[CRelativeLayoutPlug::EnumAlignParent::CenterInParent]
        || pstActor->GetRelativeLayoutPlug()->m_ulRuleWithParent[CRelativeLayoutPlug::EnumAlignParent::CenterVertical]))
      {
        pstCRelativeLayoutPlug->m_iPreTop = (iContentHeight - iChildMeasuredHeight) / 2;
        pstCRelativeLayoutPlug->m_iPreTop = UI_MAX(pstCRelativeLayoutPlug->m_iPreTop, 0);
        pstCRelativeLayoutPlug->m_iPreBottom = pstCRelativeLayoutPlug->m_iPreTop + iChildMeasuredHeight;
      }
    }
    
    int iMyselfMeasureWidth = 0;
    int iMyselfMeasureHeight = 0;
    GetMyselfMeasure(iMyselfMeasureWidth, iMyselfMeasureHeight);
    //如果layout自适应，需要根据所有Actor组成区域的大小设置layout测量值
    if (GetLayoutPlug()->m_iLayoutWidth == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT)
    {
      iWidth += GetPaddingPlug()->m_iPaddingRight;
      iWidth = UI_MAX(iWidth, iMyselfMeasureWidth);
		  iWidth = UI_MIN(iWidth, iWidthMeasureSpec);
      SetMeasuredDimension(iWidth,m_sMeasureHeight, 0);
    }
    if(GetLayoutPlug()->m_iLayoutHeight == CMarginLayoutPlug::MEASURE_MODE_WRAPCONTENT){
      iHeight += GetPaddingPlug()->m_iPaddingBottom;
      iHeight = UI_MAX(iHeight, iMyselfMeasureHeight);
		  iHeight = UI_MIN(iHeight, iHeightMeasureSpec);
      SetMeasuredDimension(m_sMeasureWidth,iHeight, 0);
    }
  }

  // RelativeLayout的默认对齐方式是左对齐和顶端对齐
  if (m_pstUIGravityPlug->m_ulGravity == CUIGravity::NONE || m_pstUIGravityPlug->m_ulGravity == (CUIGravity::LEFT | CUIGravity::TOP))
  {
    return;
  }

  //如果RelativeLayout不是左对齐和顶端对齐，则按照对齐方式调整所有Actor的位置。
  //注意Actor的m_sPreLeft、m_sPreTop、m_sPreRight、m_sPreBottom等都已经将Padding计算在内，所以这里
  //不可以再将Padding计算在内。
  UIRect stContainerRect(GetPaddingPlug()->m_iPaddingLeft, GetPaddingPlug()->m_iPaddingTop, 0,
                        iWidth - GetPaddingPlug()->m_iPaddingLeft - GetPaddingPlug()->m_iPaddingRight, 
                        iHeight - GetPaddingPlug()->m_iPaddingTop - GetPaddingPlug()->m_iPaddingBottom,
                        0);
  UIRect stContentRect;

  CUIGravity::Apply(m_pstUIGravityPlug->m_ulGravity, stContainerRect, iContentWidth, iContentHeight, 0, 0, 0, 0, 0, 0, 0, stContentRect);
  short sHorizontalOffset = stContentRect.m_sLeft;
  short sVerticalOffset = stContentRect.m_sTop;

  if (!sHorizontalOffset && !sVerticalOffset)
  {
    return;
  }
  {
    for (int iCount = 0; iCount < iChildCount; ++iCount)
    {
      CActor *pstActor = GetChildAt(iCount);
      if (pstActor->GetVisiablePlug()->m_Visible == CVisiablePlug::ActorVisibleType::Gone)
      {
        continue;
      }
      CRelativeLayoutPlug* pstCRelativeLayoutPlug = const_cast<CRelativeLayoutPlug*>(pstActor->GetRelativeLayoutPlug());
      pstCRelativeLayoutPlug->m_iPreLeft += sHorizontalOffset;
      pstCRelativeLayoutPlug->m_iPreRight += sHorizontalOffset;
      pstCRelativeLayoutPlug->m_iPreTop += sVerticalOffset;
      pstCRelativeLayoutPlug->m_iPreBottom += sVerticalOffset;
    }
  }
}

void CRelativeLayout::SortChildren()
{
  //先初始化所有子元器件和依赖元器件集合
  InitAllChildAndRelayMap();
  //按照定位优先级排序；排一遍横向的，再排一遍纵向的，这两个里面可能有重合的子元器件
  m_stHorizontalSortedChildVec.clear();
  m_stVerticalSortedChildVec.clear();
  GetSortedActorFromAllChildAndReyMap(m_stHorizontalSortedChildVec, m_stAllNode, m_stRelyMap, true);
  GetSortedActorFromAllChildAndReyMap(m_stVerticalSortedChildVec, m_stAllNode, m_stRelyMap, false);
}

void CRelativeLayout::InitAllChildAndRelayMap()
{
  //初始化
  std::vector<CRelativeLayoutNode*>::iterator Iter = m_stAllNode.begin();
  std::vector<CRelativeLayoutNode*>::iterator IterEnd = m_stAllNode.end();
  //注意所有成员集合中放的都是指针，所以只需要清理m_stAllNode中的内容就可以了。
  for (; Iter != IterEnd; ++Iter)
  {
    if(nullptr != *Iter){
      delete *Iter;
    }
  }
  m_stAllNode.clear();
  m_stRelyMap.clear();

  //将所有子元器件以未初始化的链表形式放到m_stAllNode中待使用；有ID的子元器件放到m_stRelyMap中待使用。
  int iChildCount = GetChildCount();
  for (int iCount = 0; iCount < iChildCount; ++iCount)
  {
      CActor *pstActor = GetChildAt(iCount);
      CRelativeLayoutNode *pstNode = new CRelativeLayoutNode();
      pstNode->m_pstActor = pstActor;
      m_stAllNode.push_back(pstNode);
      if (!pstActor->GetIDPlug()->m_strID.empty())
      {
          m_stRelyMap.insert(std::make_pair(pstActor->GetIDPlug()->m_strID, pstNode));
      }
  }
}

void CRelativeLayout::GetSortedActorFromAllChildAndReyMap(std::vector<CActor*> &stSortedActorVec, 
  const std::vector<CRelativeLayoutNode*> &stAllNode, const std::map<std::string, CRelativeLayoutNode*> &stRelyMap, bool bIsHorizontal)
{
  std::list<CRelativeLayoutNode*> stRootList;
  //从所有子元器件和可依赖元器件集合中获取依赖链表根元器件集合
  FindRootActorList(stRootList, stAllNode, stRelyMap, bIsHorizontal);

  //轮询获取依赖链表根元器件集合，
  while (!stRootList.empty())
  {
    CRelativeLayoutNode *pstNode = stRootList.front();
    stSortedActorVec.push_back(pstNode->m_pstActor);
    //轮询所有依赖这个根元器件的子元器件
    std::list<CRelativeLayoutNode *>::iterator Iter = pstNode->m_pstActorWhichRelyMeList.begin();
    std::list<CRelativeLayoutNode *>::iterator IterEnd = pstNode->m_pstActorWhichRelyMeList.end();
    for (; Iter != IterEnd; ++Iter)
    {
      //如果这个子元器件确实依赖该根元器件
      std::list<CRelativeLayoutNode *>::iterator FindIter =
          find((*Iter)->m_pstActorWhichMeRelyList.begin(), (*Iter)->m_pstActorWhichMeRelyList.end(), pstNode);
      if (FindIter != (*Iter)->m_pstActorWhichMeRelyList.end())
      {
        //将该根元器件从这个子元器件依赖列表中删除
        (*Iter)->m_pstActorWhichMeRelyList.erase(FindIter);
        /*
          如果这个子元器件只依赖这个根元器件，那么将这个子元器件也放在根元器件集合中，
          注意这个时候已经删除了这个子元器件的依赖，所以这个子元器件现在确实是[根元器件了]，只不过是二级，排在所有一级根元器件之后
          这样层层轮询完毕后，所有元器件按照依赖层级排了序，所谓依赖层级就是什么也不依赖、只依赖一级根元器件的、只依赖1、2级根元器件的，以此类推，这也是定位优先级
        */
        if ((*Iter)->m_pstActorWhichMeRelyList.empty())
        {
          stRootList.push_back(*Iter);
        }
      }
    }
    stRootList.erase(stRootList.begin());
  }
}

void CRelativeLayout::FindRootActorList(std::list<CRelativeLayoutNode*> &stRootList, const std::vector<CRelativeLayoutNode*> &stAllNode, const std::map<std::string, CRelativeLayoutNode*> &stRelyMap, bool bIsHorizontal)
{
  if (stAllNode.empty())
  {
      return;
  }

  //获取排序用到的的依赖属性列表
  std::vector<int> stAlignVec;
  if (bIsHorizontal)
  {
    int aiHoriAlign[] = {CRelativeLayoutPlug::EnumAlign::LeftOf, CRelativeLayoutPlug::EnumAlign::RightOf,CRelativeLayoutPlug::EnumAlign:: AlignLeft, CRelativeLayoutPlug::EnumAlign::AlignRight};
    stAlignVec.assign(aiHoriAlign, aiHoriAlign + ARRAY_MAX(aiHoriAlign));
  }
  else
  {
    int aiVertAlign[] = {CRelativeLayoutPlug::EnumAlign::AboveOf, CRelativeLayoutPlug::EnumAlign::BelowOf, CRelativeLayoutPlug::EnumAlign::AlignTop, CRelativeLayoutPlug::EnumAlign::AlignBottom};
    stAlignVec.assign(aiVertAlign, aiVertAlign + ARRAY_MAX(aiVertAlign));
  }
  //初始化所有链表形式存放的子元器件
  std::vector<CRelativeLayoutNode*>::const_iterator Iter = stAllNode.begin();
  std::vector<CRelativeLayoutNode*>::const_iterator IterEnd = stAllNode.end();
  for (; Iter != IterEnd; ++Iter)
  {
      (*Iter)->m_pstActorWhichMeRelyList.clear();
      (*Iter)->m_pstActorWhichRelyMeList.clear();
  }

  //轮询所有子元器件的所有指定方向依赖，将整个依赖链表补完
  Iter = stAllNode.begin();
  IterEnd = stAllNode.end();
  for (; Iter != IterEnd; ++Iter)
  {
    std::vector<int>::iterator AlignIter = stAlignVec.begin();
    std::vector<int>::iterator AlignIterEnd = stAlignVec.end();
    for (; AlignIter != AlignIterEnd; ++AlignIter)
    {
      CActor *pstActor = (*Iter)->m_pstActor;
      //没有此项依赖
      if (pstActor->GetRelativeLayoutPlug()->m_ulRule[*AlignIter].empty())
      {
          continue;
      }
      //有依赖的话从可被依赖的子元器件列表中寻找依赖的子元器件并补充链表
      RelyMap::const_iterator FindIter = stRelyMap.find(pstActor->GetRelativeLayoutPlug()->m_ulRule[*AlignIter]);
      //没找到或者依赖了自己
      if (FindIter == m_stRelyMap.end() || FindIter->second->m_pstActor == pstActor)
      {
          continue;
      }
      //找到了，补充依赖链表
      (*Iter)->m_pstActorWhichMeRelyList.push_back(FindIter->second);
      FindIter->second->m_pstActorWhichRelyMeList.push_back(*Iter);
    }
  }

  //获取依赖链表根元器件集合
  Iter = stAllNode.begin();
  IterEnd = stAllNode.end();
  for (; Iter != IterEnd; ++Iter)
  {
    //如果该子元器件不依赖任何其他子元器件，那么它就是一个[根]元器件
    if ((*Iter)->m_pstActorWhichMeRelyList.empty())
    {
        stRootList.push_back(*Iter);
    }
  }
}

void CRelativeLayout::ApplyHorizontalSizeRule(CActor* pstActor, int iMyWidth)
{ 
  //因为这个方法是根据RelayActor测量出的当前actor距离layout的值，所以需要从头到位有序测量
  CActor* pstRelayActor = nullptr;
  CRelativeLayoutPlug* pstCRelativeLayoutPlug = const_cast<CRelativeLayoutPlug*>(pstActor->GetRelativeLayoutPlug());
  pstCRelativeLayoutPlug->m_iPreLeft = -1;
  pstCRelativeLayoutPlug->m_iPreRight = -1;

  //给m_iPreRight赋值，代表目前该子元器件距离layout的right值
  pstRelayActor = GetRelayActor(pstActor, CRelativeLayoutPlug::EnumAlign::LeftOf);
  if (pstRelayActor)
  {
    pstCRelativeLayoutPlug->m_iPreRight =
      pstRelayActor->GetRelativeLayoutPlug()->m_iPreLeft - pstRelayActor->GetLayoutPlug()->m_sLeftMargin - pstActor->GetLayoutPlug()->m_sRightMargin;
  }
  else if (pstCRelativeLayoutPlug->m_ulRuleWithParent[CRelativeLayoutPlug::EnumAlignParent::AlignWithParentIfMissing] 
            && !pstCRelativeLayoutPlug->m_ulRule[CRelativeLayoutPlug::EnumAlign::LeftOf].empty())
  {
    if (iMyWidth >= 0)
    {
      pstCRelativeLayoutPlug->m_iPreRight = iMyWidth - GetPaddingPlug()->m_iPaddingRight -  pstActor->GetLayoutPlug()->m_sRightMargin;
    }
  }
  //给m_iPreLeft赋值，代表目前该子元器件距离layout的left值
  pstRelayActor = GetRelayActor(pstActor, CRelativeLayoutPlug::EnumAlign::RightOf);
  if (pstRelayActor)
  {
    pstCRelativeLayoutPlug->m_iPreLeft =
      pstRelayActor->GetRelativeLayoutPlug()->m_iPreRight + pstRelayActor->GetLayoutPlug()->m_sRightMargin + pstActor->GetLayoutPlug()->m_sLeftMargin;
  }
  else if (pstCRelativeLayoutPlug->m_ulRuleWithParent[CRelativeLayoutPlug::EnumAlignParent::AlignWithParentIfMissing] 
            && !pstCRelativeLayoutPlug->m_ulRule[CRelativeLayoutPlug::EnumAlign::RightOf].empty())
  {
    pstCRelativeLayoutPlug->m_iPreLeft = GetPaddingPlug()->m_iPaddingRight + pstActor->GetLayoutPlug()->m_sLeftMargin;
  }

  //给m_iPreLeft赋值，代表目前该子元器件距离layout的left值，因为LayoutAlign的优先级大于LayoutOf，所以后计算LayoutAlign
  pstRelayActor = GetRelayActor(pstActor, CRelativeLayoutPlug::EnumAlign::AlignLeft);
  if (pstRelayActor)
  {
    pstCRelativeLayoutPlug->m_iPreLeft = pstRelayActor->GetRelativeLayoutPlug()->m_iPreLeft + pstActor->GetLayoutPlug()->m_sLeftMargin;
  }
  else if (pstCRelativeLayoutPlug->m_ulRuleWithParent[CRelativeLayoutPlug::EnumAlignParent::AlignWithParentIfMissing] 
            && !pstCRelativeLayoutPlug->m_ulRule[CRelativeLayoutPlug::EnumAlign::AlignLeft].empty())
  {
    pstCRelativeLayoutPlug->m_iPreLeft = GetPaddingPlug()->m_iPaddingLeft + pstActor->GetLayoutPlug()->m_sLeftMargin;
  }
  //给m_iPreRight赋值，代表目前该子元器件距离layout的right值,因为LayoutAlign的优先级大于LayoutOf，所以后计算LayoutAlign
  pstRelayActor = GetRelayActor(pstActor, CRelativeLayoutPlug::EnumAlign::AlignRight);
  if (pstRelayActor)
  {
    pstCRelativeLayoutPlug->m_iPreRight = pstRelayActor->GetRelativeLayoutPlug()->m_iPreRight - pstActor->GetLayoutPlug()->m_sRightMargin;
  }
  else if (pstCRelativeLayoutPlug->m_ulRuleWithParent[CRelativeLayoutPlug::EnumAlignParent::AlignWithParentIfMissing] 
            && !pstCRelativeLayoutPlug->m_ulRule[CRelativeLayoutPlug::EnumAlign::AlignRight].empty())
  {
    if (iMyWidth >= 0)
    {
      pstCRelativeLayoutPlug->m_iPreRight = iMyWidth - GetPaddingPlug()->m_iPaddingRight - pstActor->GetLayoutPlug()->m_sRightMargin;
    }
  }

  //给m_iPreLeft赋值，代表目前该子元器件距离layout的left值，因为LayoutAlignParent的优先级大于LayoutOf和LayoutAlign，所以最后计算
  if (pstCRelativeLayoutPlug->m_ulRuleWithParent[CRelativeLayoutPlug::EnumAlignParent::AlignParentLeft])
  {
    pstCRelativeLayoutPlug->m_iPreLeft = GetPaddingPlug()->m_iPaddingLeft + pstActor->GetLayoutPlug()->m_sLeftMargin;
  }
  //给m_iPreRight赋值，代表目前该子元器件距离layout的right值,因为LayoutAlignParent的优先级大于LayoutOf和LayoutAlign，所以最后计算
  if (pstCRelativeLayoutPlug->m_ulRuleWithParent[CRelativeLayoutPlug::EnumAlignParent::AlignParentRight])
  {
    if (iMyWidth >= 0)
    {
      pstCRelativeLayoutPlug->m_iPreRight = iMyWidth - GetPaddingPlug()->m_iPaddingRight - pstActor->GetLayoutPlug()->m_sRightMargin;
    }
  }
}

void CRelativeLayout::ApplyVerticalSizeRule(CActor* pstActor, int iMyHeight)
{
  //因为这个方法是根据RelayActor测量出的当前actor距离layout的值，所以需要从头到位有序测量
  CActor* pstRelayActor = nullptr;
  CRelativeLayoutPlug* pstCRelativeLayoutPlug = const_cast<CRelativeLayoutPlug*>(pstActor->GetRelativeLayoutPlug());
  pstCRelativeLayoutPlug->m_iPreTop = -1;
  pstCRelativeLayoutPlug->m_iPreBottom = -1;

  //给m_iPreBottom赋值，代表目前该子元器件距离layout的Bottom值
  pstRelayActor = GetRelayActor(pstActor, CRelativeLayoutPlug::EnumAlign::AboveOf);
  if (pstRelayActor)
  {
    pstCRelativeLayoutPlug->m_iPreBottom =
      pstRelayActor->GetRelativeLayoutPlug()->m_iPreTop - pstRelayActor->GetLayoutPlug()->m_sTopMargin - pstActor->GetLayoutPlug()->m_sBottomMargin;
  }
  else if (pstCRelativeLayoutPlug->m_ulRuleWithParent[CRelativeLayoutPlug::EnumAlignParent::AlignWithParentIfMissing] 
            && !pstCRelativeLayoutPlug->m_ulRule[CRelativeLayoutPlug::EnumAlign::AboveOf].empty())
  {
    if (iMyHeight >= 0)
    {
      pstCRelativeLayoutPlug->m_iPreBottom = iMyHeight - GetPaddingPlug()->m_iPaddingBottom -  pstActor->GetLayoutPlug()->m_sBottomMargin;
    }
  }
  //给m_iPreTop赋值，代表目前该子元器件距离layout的Top值
  pstRelayActor = GetRelayActor(pstActor, CRelativeLayoutPlug::EnumAlign::BelowOf);
  if (pstRelayActor)
  {
    pstCRelativeLayoutPlug->m_iPreTop =
      pstRelayActor->GetRelativeLayoutPlug()->m_iPreBottom + pstRelayActor->GetLayoutPlug()->m_sBottomMargin + pstActor->GetLayoutPlug()->m_sTopMargin;
  }
  else if (pstCRelativeLayoutPlug->m_ulRuleWithParent[CRelativeLayoutPlug::EnumAlignParent::AlignWithParentIfMissing] 
            && !pstCRelativeLayoutPlug->m_ulRule[CRelativeLayoutPlug::EnumAlign::BelowOf].empty())
  {
    pstCRelativeLayoutPlug->m_iPreTop = GetPaddingPlug()->m_iPaddingTop + pstActor->GetLayoutPlug()->m_sTopMargin;
  }

  //给m_iPreTop赋值，代表目前该子元器件距离layout的Top值，因为LayoutAlign的优先级大于LayoutOf，所以后计算LayoutAlign
  pstRelayActor = GetRelayActor(pstActor, CRelativeLayoutPlug::EnumAlign::AlignTop);
  if (pstRelayActor)
  {
    pstCRelativeLayoutPlug->m_iPreTop = pstRelayActor->GetRelativeLayoutPlug()->m_iPreTop + pstActor->GetLayoutPlug()->m_sTopMargin;
  }
  else if (pstCRelativeLayoutPlug->m_ulRuleWithParent[CRelativeLayoutPlug::EnumAlignParent::AlignWithParentIfMissing] 
            && !pstCRelativeLayoutPlug->m_ulRule[CRelativeLayoutPlug::EnumAlign::AlignTop].empty())
  {
    pstCRelativeLayoutPlug->m_iPreTop = GetPaddingPlug()->m_iPaddingTop + pstActor->GetLayoutPlug()->m_sTopMargin;
  }
  //给m_iPreBottom赋值，代表目前该子元器件距离layout的Bottom值,因为LayoutAlign的优先级大于LayoutOf，所以后计算LayoutAlign
  pstRelayActor = GetRelayActor(pstActor, CRelativeLayoutPlug::EnumAlign::AlignBottom);
  if (pstRelayActor)
  {
    pstCRelativeLayoutPlug->m_iPreBottom = pstRelayActor->GetRelativeLayoutPlug()->m_iPreBottom - pstActor->GetLayoutPlug()->m_sBottomMargin;
  }
  else if (pstCRelativeLayoutPlug->m_ulRuleWithParent[CRelativeLayoutPlug::EnumAlignParent::AlignWithParentIfMissing] 
            && !pstCRelativeLayoutPlug->m_ulRule[CRelativeLayoutPlug::EnumAlign::AlignBottom].empty())
  {
    if (iMyHeight >= 0)
    {
      pstCRelativeLayoutPlug->m_iPreBottom = iMyHeight - GetPaddingPlug()->m_iPaddingBottom - pstActor->GetLayoutPlug()->m_sBottomMargin;
    }
  }

  //给m_iPreTop赋值，代表目前该子元器件距离layout的Top值，因为LayoutAlignParent的优先级大于LayoutOf和LayoutAlign，所以最后计算
  if (pstCRelativeLayoutPlug->m_ulRuleWithParent[CRelativeLayoutPlug::EnumAlignParent::AlignParentTop])
  {
    pstCRelativeLayoutPlug->m_iPreTop = GetPaddingPlug()->m_iPaddingTop + pstActor->GetLayoutPlug()->m_sTopMargin;
  }
  //给m_iPreBottom赋值，代表目前该子元器件距离layout的Bottom值,因为LayoutAlignParent的优先级大于LayoutOf和LayoutAlign，所以最后计算
  if (pstCRelativeLayoutPlug->m_ulRuleWithParent[CRelativeLayoutPlug::EnumAlignParent::AlignParentBottom])
  {
    if (iMyHeight >= 0)
    {
      pstCRelativeLayoutPlug->m_iPreBottom = iMyHeight - GetPaddingPlug()->m_iPaddingBottom - pstActor->GetLayoutPlug()->m_sBottomMargin;
    }
  }
}

CActor *CRelativeLayout::GetRelayActor(CActor* pstActor, ulong ulAlignRule)
{
  if (ulAlignRule >= ARRAY_MAX(pstActor->GetRelativeLayoutPlug()->m_ulRule))
  {
      return NULL;
  }

  CActor *pstRelayActor = NULL;
  do
  {
    std::string strID = pstActor->GetRelativeLayoutPlug()->m_ulRule[ulAlignRule];
    if (strID.empty())
    {
      return NULL;
    }

    RelyMap::iterator FindIter = m_stRelyMap.find(strID);
    if (FindIter == m_stRelyMap.end())
    {
      //依赖的元器件不是RelativeLayout的Child
      return NULL;
    }

    pstRelayActor = FindIter->second->m_pstActor;
    //如果依赖的子元器件是Gone，那就使用依赖的子元器件的依赖
    pstActor = pstRelayActor;
  } while (pstRelayActor->GetVisiablePlug()->m_Visible == CVisiablePlug::ActorVisibleType::Gone);

  return pstRelayActor;
}

int CRelativeLayout::GetRelativeChildMeasureSpec(int iChildStart, int iChildEnd, int iChildSize,
                                                 int iStartMargin, int iEndMargin, int iStartPadding,
                                                 int iEndPadding, int iParentSize)
{
  int iTempStart = iChildStart;
  int iTempEnd = iChildEnd;

  //如果start位置小于0，将start位置设置为layoutPadiding + 自身margin
  if (iTempStart < 0)
  {
    iTempStart = iStartPadding + iStartMargin;
  }
  //如果end位置小于0，将end位置设置为layoutPadiding - layoutPadding - 自身margin
  if (iTempEnd < 0)
  {
    iTempEnd = iParentSize - iEndPadding - iEndMargin;
  }
  //自身最大值
  int iMaxAvailable = iTempEnd - iTempStart;
  //如果自身pre有值，也就是说有依赖并计算完毕，使用计算的值
  if (iChildStart >= 0 && iChildEnd >= 0)
  {
    if (iMaxAvailable >= 0)
    {
      return iMaxAvailable;
    }
  }
  //如果没有计算pre值，或者说没有依赖，使用layout测量值
  return iParentSize;
}

void CRelativeLayout::PositionChildHorizontal(CActor *pstChild, int iParentWidth, bool bParentWrapContent)
{
  CRelativeLayoutPlug* pstCChildRelativeLayoutPlug = const_cast<CRelativeLayoutPlug*>(pstChild->GetRelativeLayoutPlug());
  int iChildMeasuredWidth = 0;
  int iChildMeasuredHeight = 0;
  int iChildMeasuredLength = 0;
  pstChild->GetMeasuredDimension(iChildMeasuredWidth, iChildMeasuredHeight, iChildMeasuredLength);
  //m_iPreLeft有问题，m_iPreRight没问题，需要重新根据m_sPreRight和元器件测量宽度计算
  if (pstCChildRelativeLayoutPlug->m_iPreLeft < 0 && pstCChildRelativeLayoutPlug->m_iPreRight >= 0)
  {
    pstCChildRelativeLayoutPlug->m_iPreLeft = pstCChildRelativeLayoutPlug->m_iPreRight - iChildMeasuredWidth;
  }
  //m_iPreRight有问题，m_iPreLeft没问题，需要重新根据m_sPreLeft和元器件测量宽度计算
  else if (pstCChildRelativeLayoutPlug->m_iPreLeft >= 0 && pstCChildRelativeLayoutPlug->m_iPreRight < 0)
  {
    pstCChildRelativeLayoutPlug->m_iPreRight = pstCChildRelativeLayoutPlug->m_iPreLeft + iChildMeasuredWidth;
  }
  //如果m_iPreRight和m_iPreLeft都有问题，那可以根据parent的padding和child的margin计算
  else if (pstCChildRelativeLayoutPlug->m_iPreLeft < 0 && pstCChildRelativeLayoutPlug->m_iPreRight < 0)
  {
    if (!bParentWrapContent)
    {
      // RelativeLayout的Width为确定值时，可以计算出水平居中的Actor的PreLeft和PreRight。
      //而该Actor就可以作为其他Actor在水平方向上的位置Anchor（位置基点）。
      if (pstCChildRelativeLayoutPlug->m_ulRuleWithParent[CRelativeLayoutPlug::EnumAlignParent::CenterInParent]
        || pstCChildRelativeLayoutPlug->m_ulRuleWithParent[CRelativeLayoutPlug::EnumAlignParent::CenterHorizontal])
      {
        pstCChildRelativeLayoutPlug->m_iPreLeft = (iParentWidth - iChildMeasuredWidth) / 2;
        pstCChildRelativeLayoutPlug->m_iPreLeft = UI_MAX(pstCChildRelativeLayoutPlug->m_iPreLeft, 0);
        pstCChildRelativeLayoutPlug->m_iPreRight = pstCChildRelativeLayoutPlug->m_iPreLeft + iChildMeasuredWidth;
        return;
      }
    }
    pstCChildRelativeLayoutPlug->m_iPreLeft = GetPaddingPlug()->m_iPaddingLeft + pstChild->GetLayoutPlug()->m_sLeftMargin;
    pstCChildRelativeLayoutPlug->m_iPreRight = pstCChildRelativeLayoutPlug->m_iPreLeft + iChildMeasuredWidth;
  }
  else if (pstCChildRelativeLayoutPlug->m_iPreLeft >= pstCChildRelativeLayoutPlug->m_iPreRight)
  {
    //出问题了，这left在right右边了。。
  }
}

void CRelativeLayout::PositionChildVertical(CActor *pstChild, int iParentHeight, bool bParentWrapContent)
{
  CRelativeLayoutPlug* pstCChildRelativeLayoutPlug = const_cast<CRelativeLayoutPlug*>(pstChild->GetRelativeLayoutPlug());
  int iChildMeasuredWidth = 0;
  int iChildMeasuredHeight = 0;
  int iChildMeasuredLength = 0;
  pstChild->GetMeasuredDimension(iChildMeasuredWidth, iChildMeasuredHeight, iChildMeasuredLength);
  //m_iPreTop有问题，m_iPreBottom没问题，需要重新根据m_iPreBottom和元器件测量宽度计算
  if (pstCChildRelativeLayoutPlug->m_iPreTop < 0 && pstCChildRelativeLayoutPlug->m_iPreBottom >= 0)
  {
    pstCChildRelativeLayoutPlug->m_iPreTop = pstCChildRelativeLayoutPlug->m_iPreBottom - iChildMeasuredHeight;
  }
  //m_iPreBottom有问题，m_iPreTop没问题，需要重新根据m_iPreBottom和元器件测量宽度计算
  else if (pstCChildRelativeLayoutPlug->m_iPreTop >= 0 && pstCChildRelativeLayoutPlug->m_iPreBottom < 0)
  {
    pstCChildRelativeLayoutPlug->m_iPreBottom = pstCChildRelativeLayoutPlug->m_iPreTop + iChildMeasuredHeight;
  }
  //如果m_iPreTop和m_iPreBottom都有问题，那可以根据parent的padding和child的margin计算
  else if (pstCChildRelativeLayoutPlug->m_iPreTop < 0 && pstCChildRelativeLayoutPlug->m_iPreBottom < 0)
  {
    if (!bParentWrapContent)
    {
      // RelativeLayout的height为确定值时，可以计算出水平居中的Actor的PreTop和PreBottom。
      //而该Actor就可以作为其他Actor在竖直方向上的位置Anchor（位置基点）。 
      if (pstCChildRelativeLayoutPlug->m_ulRuleWithParent[CRelativeLayoutPlug::EnumAlignParent::CenterInParent]
        || pstCChildRelativeLayoutPlug->m_ulRuleWithParent[CRelativeLayoutPlug::EnumAlignParent::CenterVertical])
      {
        pstCChildRelativeLayoutPlug->m_iPreTop = (iParentHeight - iChildMeasuredHeight) / 2;
        pstCChildRelativeLayoutPlug->m_iPreTop = UI_MAX(pstCChildRelativeLayoutPlug->m_iPreTop, 0);
        pstCChildRelativeLayoutPlug->m_iPreBottom = pstCChildRelativeLayoutPlug->m_iPreTop + iChildMeasuredHeight;
        return;
      }
    }
    pstCChildRelativeLayoutPlug->m_iPreTop = GetPaddingPlug()->m_iPaddingTop + pstChild->GetLayoutPlug()->m_sTopMargin;
    pstCChildRelativeLayoutPlug->m_iPreBottom = pstCChildRelativeLayoutPlug->m_iPreTop + iChildMeasuredHeight;
  }
  else if (pstCChildRelativeLayoutPlug->m_iPreTop >= pstCChildRelativeLayoutPlug->m_iPreBottom)
  {
    //出问题了，这top在bottom下边了。。
  }
}

void CRelativeLayout::OnLayout(bool bChanged, int sLeft, int sTop, int sDepth, int sWidth, int sHeight, int length)
{
  (void)bChanged;
  (void)sLeft;
  (void)sTop;
  (void)sWidth;
  (void)sHeight;

  int iChildCount = GetChildCount();
  for (int iCount = 0; iCount < iChildCount; ++iCount)
  {
    CActor *pstActor = GetChildAt(iCount);
    if (pstActor && (pstActor->GetVisiablePlug()->m_Visible != CVisiablePlug::Gone))
    {
      const CRelativeLayoutPlug* pstCChildRelativeLayoutPlug = pstActor->GetRelativeLayoutPlug();
      pstActor->Layout(pstCChildRelativeLayoutPlug->m_iPreLeft, pstCChildRelativeLayoutPlug->m_iPreTop, sDepth,
                        pstCChildRelativeLayoutPlug->m_iPreRight - pstCChildRelativeLayoutPlug->m_iPreLeft,
                        pstCChildRelativeLayoutPlug->m_iPreBottom - pstCChildRelativeLayoutPlug->m_iPreTop, length);
    }
  }
}

void CRelativeLayout::OnDraw(CUICanvasInterface &stCanvas)
{
  CVisualizationStage::OnDraw(stCanvas);
}