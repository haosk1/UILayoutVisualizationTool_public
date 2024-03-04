/**
 * frame
 *
 * 可视化元器件之FrameLayout
 **/
#ifndef __FRAME__FRAMELAYOUT_H__
#define __FRAME__FRAMELAYOUT_H__

#include "VisualizationStage.h"

class CFrameLayout : public CVisualizationStage
{
public:
  explicit CFrameLayout(){};
  virtual ~CFrameLayout(){};

  virtual void Inflate(TiXmlAttribute *attribute) override;
  virtual void SetActorAttribute(const char *attributeName, const char *attributeValue) override;

protected:
  virtual void OnMeasure(int iWidthMeasureSpec, int iHeightMeasureSpec, int iLengthMeasureSpec) override;
  virtual void OnLayout(bool bChanged, int sLeft, int sTop, int sDepth, int sWidth, int sHeight, int length) override;
  virtual void OnDraw(CUICanvasInterface &stCanvas) override;

private:
  // 属性对应start
  bool m_MeasureAllChildren = false;
  // 属性对应end
};
#endif // __FRAME__FRAMELAYOUT_H__