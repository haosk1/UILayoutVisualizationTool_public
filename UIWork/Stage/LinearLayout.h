/**
 * frame
 *
 * 可视化元器件之LinearLayout
 **/
#ifndef __FRAME__LINEARLAYOUT_H__
#define __FRAME__LINEARLAYOUT_H__

#include "VisualizationStage.h"
#include "AttributePlug/UIGravityPlug.h"

class CLinearLayout : public CVisualizationStage
{
public:
  explicit CLinearLayout();
  virtual ~CLinearLayout();

  virtual void Inflate(TiXmlAttribute *attribute) override;
  virtual void SetActorAttribute(const char *attributeName, const char *attributeValue) override;
  //添加属性插件
  const CUIGravityPlug *GetGravityPlug(void);
  void SetGravityPlug(const CUIGravityPlug &stParam);

protected:
  virtual void OnMeasure(int iWidthMeasureSpec, int iHeightMeasureSpec, int iLengthMeasureSpec) override;
  virtual void OnLayout(bool bChanged, int sLeft, int sTop, int sDepth, int sWidth, int sHeight, int length) override;
  virtual void OnDraw(CUICanvasInterface &stCanvas) override;

private:
  void MeasureHorizontal(int iWidthMeasureSpec, int iHeightMeasureSpec);
  void MeasureVertical(int iWidthMeasureSpec, int iHeightMeasureSpec);
  void MeasureChildWithPaddingWidthLinear(CActor *pstActor, int iWidthParentMeasureSpec, int iWidthUsed, int iHeightParentMeasureSpec, int iHeightUsed);
private:
  // 属性对应start
  enum Orientation{
    Horizontal,
    Vertical,
  };
  Orientation m_Orientation = Horizontal;
  float m_fWeightSum = 0.0;
  CUIGravityPlug* m_pstUIGravityPlug = nullptr;
  // 属性对应end
  int m_iTotalLength = 0;
};
#endif // __FRAME__LINEARLAYOUT_H__