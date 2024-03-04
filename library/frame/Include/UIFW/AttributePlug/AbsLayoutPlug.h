
/*
 AbsLayoutPlug是Actor专门为AbsoluteLayout适配的属性插件
*/
#ifndef __FRAME__AbsLayoutPlug_H
#define __FRAME__AbsLayoutPlug_H

#include "RefBase/ref_base.h"

class CAbsLayoutPlug : public baselib::RefBase
{
public:
    CAbsLayoutPlug(int iLayoutX = 0, int iLayoutY = 0)
        : m_iLayoutX(iLayoutX),m_iLayoutY(iLayoutY)
    {}
    CAbsLayoutPlug(const CAbsLayoutPlug &stOther)
        : m_iLayoutX(stOther.m_iLayoutX),m_iLayoutY(stOther.m_iLayoutY)
    {}
    virtual ~CAbsLayoutPlug(){}

CAbsLayoutPlug &operator=(const CAbsLayoutPlug &stOther)
{
    if (this != &stOther)
    {
        m_iLayoutX = stOther.m_iLayoutX;
        m_iLayoutY = stOther.m_iLayoutY;
    }
    return *this;
}

virtual CAbsLayoutPlug *Clone() const
{
    return new CAbsLayoutPlug(*this);
}

int m_iLayoutX;
int m_iLayoutY;
};

#endif /*#ifndef __FRAME__AbsLayoutPlug_H*/
