
/*
 Gravity是Actor的属性；定义的是Actor描画内容的居中方式；对于stage来说，即是child actor的整体居中方式
 目前为止只有下面这5类元器件定义了这个属性
    EditText
	ImageActor
    TextActor
	LinearLayout
	RelativeLayout
*/
#ifndef __FRAME__UIGRAVITYPLUG_H
#define __FRAME__UIGRAVITYPLUG_H

#include "RefBase/ref_base.h"
#include "UIFW/AttributePlug/UIGravity.h"

class CUIGravityPlug : public baselib::RefBase
{
public:
    CUIGravityPlug(uint iGravity = 0)
        : m_ulGravity(iGravity)
    {}
    CUIGravityPlug(const CUIGravityPlug &stOther)
        : m_ulGravity(stOther.m_ulGravity)
    {}
    virtual ~CUIGravityPlug(){}

CUIGravityPlug &operator=(const CUIGravityPlug &stOther)
{
    if (this != &stOther)
    {
        m_ulGravity = stOther.m_ulGravity;
    }
    return *this;
}

virtual CUIGravityPlug *Clone() const
{
    return new CUIGravityPlug(*this);
}

uint m_ulGravity = (CUIGravity::LEFT | CUIGravity::TOP);
};

#endif /*#ifndef __FRAME__UIGRAVITYPLUG_H*/
