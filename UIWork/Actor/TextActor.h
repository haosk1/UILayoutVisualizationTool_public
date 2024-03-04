/**
 * frame
 *
 * 可视化元器件之TextActor
 **/
#ifndef __FRAME__TEXTACTOR_H__
#define __FRAME__TEXTACTOR_H__

#include "VisualizationActor.h"

class CTextActor : public CVisualizationActor
{
public:
    explicit CTextActor(){};
    virtual ~CTextActor(){};

    virtual void Inflate(TiXmlAttribute *attribute) override;
    virtual void SetActorAttribute(const char *attributeName, const char *attributeValue) override;
    // 属性对应start
    // 属性对应end

protected:
    virtual void OnSizeChanged(int sNewWidth, int sNewHeight, int sNewLength, int sOldWidth, int sOldHeight, int sOldLength) override;
    virtual void OnMeasure(int iWidthMeasureSpec, int iHeightMeasureSpec, int iLengthMeasureSpec) override;
    virtual void OnDraw(CUICanvasInterface &stCanvas) override;
    
private:
    // 属性对应start
    String m_string;
    Font m_font;
    Color m_color;
    uint m_LineSpace = 0;
    bool m_SingleLine = true;
    // 属性对应end
    UIRect m_stringRect;
    uint m_uiMostTextHeight = 0;
};
#endif // __FRAME__TEXTACTOR_H__