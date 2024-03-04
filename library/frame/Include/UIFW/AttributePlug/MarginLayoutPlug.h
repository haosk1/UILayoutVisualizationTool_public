
#ifndef __FRAME__MarginLayoutPlug_H
#define __FRAME__MarginLayoutPlug_H

#include "RefBase/ref_base.h"

class CMarginLayoutPlug : public baselib::RefBase
{
public:
    typedef enum
    {
        MEASURE_MODE_FILLPARENT = -2,//填充父类
        MEASURE_MODE_WRAPCONTENT = -1,//自适应
    } MeasureMode;

    CMarginLayoutPlug(int iLayoutWidth = 0, int iLayoutHeight = 0, int iLayoutLength = 0, 
                      int sLeftMargin = 0, int sTopMargin = 0,int sFrontMargin = 0,
                      int sRightMargin = 0, int sBottomMargin = 0, int sBackMargin = 0)
        : m_iLayoutWidth(iLayoutWidth), m_iLayoutHeight(iLayoutHeight), m_iLayoutLength(iLayoutLength),
          m_sLeftMargin(sLeftMargin), m_sTopMargin(sTopMargin), m_sFrontMargin(sFrontMargin),
          m_sRightMargin(sRightMargin), m_sBottomMargin(sBottomMargin), m_sBackMargin(sBackMargin)
    {}
    CMarginLayoutPlug(const CMarginLayoutPlug &stOther)
        : m_iLayoutWidth(stOther.m_iLayoutWidth), m_iLayoutHeight(stOther.m_iLayoutHeight), m_iLayoutLength(stOther.m_iLayoutLength),
          m_sLeftMargin(stOther.m_sLeftMargin), m_sTopMargin(stOther.m_sTopMargin), m_sFrontMargin(stOther.m_sFrontMargin),
          m_sRightMargin(stOther.m_sRightMargin), m_sBottomMargin(stOther.m_sBottomMargin), m_sBackMargin(stOther.m_sBackMargin)
    {}
    virtual ~CMarginLayoutPlug(){}

CMarginLayoutPlug &operator=(const CMarginLayoutPlug &stOther)
{
    if (this != &stOther)
    {
        m_iLayoutWidth = stOther.m_iLayoutWidth;
        m_iLayoutHeight = stOther.m_iLayoutHeight;
        m_iLayoutLength = stOther.m_iLayoutLength;
        m_sLeftMargin = stOther.m_sLeftMargin;
        m_sTopMargin = stOther.m_sTopMargin;
        m_sFrontMargin = stOther.m_sFrontMargin;
        m_sRightMargin = stOther.m_sRightMargin;
        m_sBottomMargin = stOther.m_sBottomMargin;
        m_sBackMargin = stOther.m_sBackMargin;
    }
    return *this;
}

virtual CMarginLayoutPlug *Clone() const
{
    return new CMarginLayoutPlug(*this);
}

int m_iLayoutWidth;
int m_iLayoutHeight;
int m_iLayoutLength;
int m_sLeftMargin;
int m_sTopMargin;
int m_sFrontMargin;
int m_sRightMargin;
int m_sBottomMargin;
int m_sBackMargin;
};

#endif /*#ifndef __FRAME__MarginLayoutPlug_H*/
