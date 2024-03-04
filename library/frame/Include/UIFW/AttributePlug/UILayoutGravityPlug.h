
/*
 LayoutGravity是staged定义的属性，由child Actor设置；定义的是child在stage中的居中方式。
*/
#ifndef __FRAME__UILayoutGravity_H
#define __FRAME__UILayoutGravity_H

#include "RefBase/ref_base.h"
#include "UIGravity.h"

class CUILayoutGravityPlug : public baselib::RefBase
{
public:
    CUILayoutGravityPlug(uint iGravity = 0)
        : m_ulGravity(iGravity)
    {}
    CUILayoutGravityPlug(const CUILayoutGravityPlug &stOther)
        : m_ulGravity(stOther.m_ulGravity)
    {}
    virtual ~CUILayoutGravityPlug(){}

CUILayoutGravityPlug &operator=(const CUILayoutGravityPlug &stOther)
{
    if (this != &stOther)
    {
        m_ulGravity = stOther.m_ulGravity;
    }
    return *this;
}

virtual CUILayoutGravityPlug *Clone() const
{
    return new CUILayoutGravityPlug(*this);
}

uint m_ulGravity = (CUIGravity::LEFT | CUIGravity::TOP);
};

#endif /*#ifndef __FRAME__UILayoutGravity_H*/
