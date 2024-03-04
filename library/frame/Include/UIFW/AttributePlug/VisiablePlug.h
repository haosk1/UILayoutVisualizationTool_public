
#ifndef __FRAME__VisiablePlug_H
#define __FRAME__VisiablePlug_H

#include "RefBase/ref_base.h"

class CVisiablePlug : public baselib::RefBase
{
public:
    enum ActorVisibleType {
        Visible,   // 描画
        Invisible, // 不描画，在stage中有位置信息
        Gone,      // 没有实例
    };
    CVisiablePlug(ActorVisibleType VisibleType = Visible)
        : m_Visible(VisibleType)
    {}
    CVisiablePlug(const CVisiablePlug &stOther)
        : m_Visible(stOther.m_Visible)
    {}
    virtual ~CVisiablePlug(){}

    CVisiablePlug &operator=(const CVisiablePlug &stOther)
    {
        if (this != &stOther)
        {
            m_Visible = stOther.m_Visible;
        }
        return *this;
    }

    virtual CVisiablePlug *Clone() const
    {
        return new CVisiablePlug(*this);
    }

    ActorVisibleType m_Visible;
};

#endif /*#ifndef __FRAME__VisiablePlug_H*/
