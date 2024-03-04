/**
 * frame
 *
 * 可视化元器件，是所有其他可视化actor的基类
 **/
#ifndef __FRAME__CVISUALIZATIONACTOR_H__
#define __FRAME__CVISUALIZATIONACTOR_H__

#include "UIFW/Actor.h"
#include "UILib.h"
//因为EditText、ImageActor、TextActor都包含了Gravity属性，所以放到基类CVisualizationActor中
#include "AttributePlug/UIGravityPlug.h"

class CVisualizationActor : public CActor {
public:

  explicit CVisualizationActor();
  virtual ~CVisualizationActor();

  virtual void Inflate(TiXmlAttribute *attribute) override;
  // 设置key-value属性值,子类应该重写,以便加载自己独有的属性
  virtual void SetActorAttribute(const char *attributeName,
                                 const char *attributeValue);
  //添加属性插件
  const CUIGravityPlug *GetGravityPlug(void);
  void SetGravityPlug(const CUIGravityPlug &stParam);

  virtual bool OnMouseEvent(Mouse_Event event) override;

protected:
  virtual void OnSizeChanged(int sNewWidth, int sNewHeight, int sNewLength, int sOldWidth, int sOldHeight, int sOldLength) override;
  virtual void OnMeasure(int iWidthMeasureSpec, int iHeightMeasureSpec, int iLengthMeasureSpec) override;
  virtual void OnLayout(bool bChanged, int sLeft, int sTop, int sFront, int sWidth, int sHeight, int length) override;
  virtual void OnDraw(CUICanvasInterface &stCanvas) override;

  /**
   * @brief 获取padding后的描画位置
   *
   * [I/O]|形参名|形参类型|参数含义|取值范围
   * [OUT] | stOutRect | UIRect & | 描画位置 | 设置left top width height
   *
   * @retval    返回值类型  void
   */
  virtual void GetDrawRectwithPadding(UIRect &outRect);

private:
  // 属性对应start
  // 设置背景图片，对应[Background]属性
  void SetBackground(std::string strPath);
  // 属性对应end

  // 描画标注线
  void DrawLeftTopLine(CUICanvasInterface &stCanvas);
  // 描画自身矩形框
  void DrawSelfRectLine(CUICanvasInterface &stCanvas);

protected:
  bool m_IsDraw = true;
  // 属性对应start
  CUIGravityPlug* m_pstUIGravityPlug = nullptr;
  // 属性对应end

private:
  // 属性对应start
  unsigned int m_BackgroundtextureId = 0;
  bool m_bIsNineGridDrawable = false;
  // 属性对应end
  bool m_onlyme = false;
};
#endif // __FRAME__CVISUALIZATIONACTOR_H__