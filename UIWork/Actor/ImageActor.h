/**
 * frame
 *
 * 可视化元器件之ImageActor
 **/
#ifndef __FRAME__IMAGEACTOR_H__
#define __FRAME__IMAGEACTOR_H__

#include "VisualizationActor.h"

class CImageActor : public CVisualizationActor
{
public:
    explicit CImageActor();
    virtual ~CImageActor(){};

    virtual void Inflate(TiXmlAttribute *attribute) override;
    virtual void SetActorAttribute(const char *attributeName, const char *attributeValue) override;
    // 属性对应start
    // 设置前景图片，对应[Background]属性
    void SetDrawbale(std::string strPath);
    // 属性对应end

protected:
    virtual void OnSizeChanged(int sNewWidth, int sNewHeight, int sNewLength, int sOldWidth, int sOldHeight, int sOldLength) override;
    virtual void OnMeasure(int iWidthMeasureSpec, int iHeightMeasureSpec, int iLengthMeasureSpec) override;
    virtual void OnDraw(CUICanvasInterface &stCanvas) override;

private:
    // 属性对应start
    unsigned int m_DrawabletextureId = 0;
    bool m_bIsNineGridDrawable = false;
    // 属性对应end
};
#endif // __FRAME__TEXTACTOR_H__