
#ifndef __FRAME__PaddingPlug_H
#define __FRAME__PaddingPlug_H

#include "RefBase/ref_base.h"

class CPaddingPlug : public baselib::RefBase
{
public:
    CPaddingPlug(int iPaddingLeft = 0, int iPaddingTop = 0, int iPaddingFront = 0, 
                 int iPaddingRight = 0,int iPaddingBottom = 0, int iPaddingBack = 0)
        : m_iPaddingLeft(iPaddingLeft), m_iPaddingTop(iPaddingTop), m_iPaddingFront(iPaddingFront),
          m_iPaddingRight(iPaddingRight), m_iPaddingBottom(iPaddingBottom), m_iPaddingBack(iPaddingBack)
    {}
    CPaddingPlug(const CPaddingPlug &stOther)
        : m_iPaddingLeft(stOther.m_iPaddingLeft), m_iPaddingTop(stOther.m_iPaddingTop), m_iPaddingFront(stOther.m_iPaddingFront),
          m_iPaddingRight(stOther.m_iPaddingRight), m_iPaddingBottom(stOther.m_iPaddingBottom), m_iPaddingBack(stOther.m_iPaddingBack)
    {}
    virtual ~CPaddingPlug(){}

CPaddingPlug &operator=(const CPaddingPlug &stOther)
{
    if (this != &stOther)
    {
        m_iPaddingLeft = stOther.m_iPaddingLeft;
        m_iPaddingTop = stOther.m_iPaddingTop;
        m_iPaddingFront = stOther.m_iPaddingFront;
        m_iPaddingRight = stOther.m_iPaddingRight;
        m_iPaddingBottom = stOther.m_iPaddingBottom;
        m_iPaddingBack = stOther.m_iPaddingBack;
    }
    return *this;
}

virtual CPaddingPlug *Clone() const
{
    return new CPaddingPlug(*this);
}

int m_iPaddingLeft;
int m_iPaddingTop;
int m_iPaddingFront;
int m_iPaddingRight;
int m_iPaddingBottom;
int m_iPaddingBack;
};

#endif /*#ifndef __FRAME__PaddingPlug_H*/
