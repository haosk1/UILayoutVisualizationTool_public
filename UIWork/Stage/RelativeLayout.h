/**
 * frame
 *
 * 可视化元器件之RelativeLayout
 **/
#ifndef __FRAME__RELATIVELAYOUT_H__
#define __FRAME__RELATIVELAYOUT_H__

#include "VisualizationStage.h"
#include "AttributePlug/UIGravityPlug.h"

class CRelativeLayout : public CVisualizationStage
{
public:
  explicit CRelativeLayout();
  virtual ~CRelativeLayout();

  virtual void Inflate(TiXmlAttribute *attribute) override;
  virtual void SetActorAttribute(const char *attributeName, const char *attributeValue) override;
  //添加属性插件
  const CUIGravityPlug *GetGravityPlug(void);
  void SetGravityPlug(const CUIGravityPlug &stParam);
  //from father
  virtual bool AddActor(CActor *pstActor);
  virtual bool UpdateActorLayout(CActor *pstActor, const UIRect &stParam);
  virtual bool RemoveActor(CActor *pstActor);

protected:
  virtual void OnMeasure(int iWidthMeasureSpec, int iHeightMeasureSpec, int iLengthMeasureSpec) override;
  virtual void OnLayout(bool bChanged, int sLeft, int sTop, int sDepth, int sWidth, int sHeight, int length) override;
  virtual void OnDraw(CUICanvasInterface &stCanvas) override;

private:
  class CRelativeLayoutNode
  {
    public:
      CActor *m_pstActor = nullptr;
      std::list<CRelativeLayoutNode*> m_pstActorWhichMeRelyList;
      std::list<CRelativeLayoutNode*> m_pstActorWhichRelyMeList;
  };
  //整理所有子元器件
  void SortChildren();
  //初始化所有子元器件和依赖元器件集合
  void InitAllChildAndRelayMap();
  //按照定位优先级（依赖关系）将所有子元器件排序
  void GetSortedActorFromAllChildAndReyMap(std::vector<CActor*> &stSortedActorVec, 
                                            const std::vector<CRelativeLayoutNode*> &stAllNode, const std::map<std::string, CRelativeLayoutNode*> &stRelyMap, bool bIsHorizontal);
  //从所有子元器件和可依赖元器件集合中获取依赖链表根元器件集合
  void FindRootActorList(std::list<CRelativeLayoutNode*> &stRootList, const std::vector<CRelativeLayoutNode*> &stAllNode, const std::map<std::string, CRelativeLayoutNode*> &stRelyMap, bool bIsHorizontal);
  //计算横向pre数据
  void ApplyHorizontalSizeRule(CActor* pstActor, int iMyWidth);
  //计算纵向pre数据
  void ApplyVerticalSizeRule(CActor* pstActor, int iMyHeight);
  //根据依赖项获取依赖的子元器件
  CActor* GetRelayActor(CActor* pstActor, ulong ulAlignRule);
  //根据pre等值获取子元器件的可测量空间
  int GetRelativeChildMeasureSpec(int iChildStart, int iChildEnd, int iChildSize,
                                                 int iStartMargin, int iEndMargin, int iStartPadding,
                                                 int iEndPadding, int iParentSize);
  //检查并重新计算m_iPre_值
  void PositionChildHorizontal(CActor *pstChild, int iParentWidth, bool bParentWrapContent);
  //检查并重新计算m_iPre_值
  void PositionChildVertical(CActor *pstChild, int iParentHeight, bool bParentWrapContent);

private:
  CUIGravityPlug* m_pstUIGravityPlug = nullptr;
  bool m_bDirtyGraph = true;//子元器件是否有变动，有变动需要重新排序定位

  std::vector<CActor*> m_stHorizontalSortedChildVec;//排序后的横向依赖子元器件集合
  std::vector<CActor*> m_stVerticalSortedChildVec;//排序后的纵向依赖子元器件集合
  std::vector<CRelativeLayoutNode*> m_stAllNode;//所有子元器件，以依赖形式的链表
  typedef std::map<std::string, CRelativeLayoutNode*> RelyMap;//只有有id的子元器件才有可能被依赖
  RelyMap m_stRelyMap;//可被依赖的子元器件列表

};
#endif // __FRAME__RELATIVELAYOUT_H__