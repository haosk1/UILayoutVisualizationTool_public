/**
 * frame
 *
 * 可视化stage，是所有其他可视化stage的基类
 **/
#ifndef __FRAME__CVISUALIZATIONSTAGE_H__
#define __FRAME__CVISUALIZATIONSTAGE_H__

#include "UIFW/Stage.h"
#include "UILib.h"
class CVisualizationStage : public CStage
{
public:
    explicit CVisualizationStage(){};
    virtual ~CVisualizationStage(){};

    virtual void Inflate(TiXmlAttribute *attribute) override;
    // 设置key-value属性值,子类应该重写,以便加载自己独有的属性
    virtual void SetActorAttribute(const char *attributeName, const char *attributeValue);

    //添加属性插件

    virtual bool OnMouseEvent(Mouse_Event event) override;

protected:
    virtual void OnSizeChanged(int sNewWidth, int sNewHeight, int sNewLength, int sOldWidth, int sOldHeight, int sOldLength) override;
    virtual void OnMeasure(int iWidthMeasureSpec, int iHeightMeasureSpec, int iLengthMeasureSpec) override;
    virtual void OnLayout(bool bChanged, int sLeft, int sTop, int sDepth, int sWidth, int sHeight, int length) override;
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
    //获取stage自身测量值（不包含子元器件）
    void GetMyselfMeasure(int& iMeasuredWidth, int& iMeasureHeight);

private:
    // 属性对应start
    // 设置背景图片，对应[Background]属性
    void SetBackground(std::string strPath);
    // 属性对应end

    //可视化划线
    void DrawLeftTopLine(CUICanvasInterface &stCanvas);
    void DrawSelfRectLine(CUICanvasInterface &stCanvas);

protected:
    bool m_IsDraw = true;
    // 属性对应start
    // 属性对应end

private:
    // 属性对应start
    unsigned int m_BackgroundtextureId = 0;
    bool m_bIsNineGridDrawable = false;
    // 属性对应end
    bool m_onlyme = false;
};
#endif // __FRAME__CVISUALIZATIONSTAGE_H__