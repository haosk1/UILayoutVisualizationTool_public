/**
 * frame
 *
 * 可视化元器件之FrameLayout
 **/
#ifndef __FRAME__FRAMELAYOUT_H__
#define __FRAME__FRAMELAYOUT_H__

#include "VisualizationStage.h"

class CAbsoluteLayout : public CVisualizationStage
{
public:
  explicit CAbsoluteLayout(){};
  virtual ~CAbsoluteLayout(){};

protected:
  virtual void OnMeasure(int iWidthMeasureSpec, int iHeightMeasureSpec, int iLengthMeasureSpec) override;
  virtual void OnLayout(bool bChanged, int sLeft, int sTop, int sDepth, int sWidth, int sHeight, int length) override;
};
#endif // __FRAME__FRAMELAYOUT_H__