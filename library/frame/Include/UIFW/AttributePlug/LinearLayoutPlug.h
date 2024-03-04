
/*
 LinearLayoutPlug是Actor专门为LinearLayout适配的属性插件
*/
#ifndef __FRAME__LinearLayoutPlug_H
#define __FRAME__LinearLayoutPlug_H

#include "RefBase/ref_base.h"

class CLinearLayoutPlug : public baselib::RefBase
{
public:
    CLinearLayoutPlug(float fWeight = 0.0)
        : m_fWeight(fWeight)
    {}
    CLinearLayoutPlug(const CLinearLayoutPlug &stOther)
        : m_fWeight(stOther.m_fWeight)
    {}
    virtual ~CLinearLayoutPlug(){}

CLinearLayoutPlug &operator=(const CLinearLayoutPlug &stOther)
{
    if (this != &stOther)
    {
        m_fWeight = stOther.m_fWeight;
    }
    return *this;
}

virtual CLinearLayoutPlug *Clone() const
{
    return new CLinearLayoutPlug(*this);
}

float m_fWeight;
};

#endif /*#ifndef __FRAME__LinearLayoutPlug_H*/
