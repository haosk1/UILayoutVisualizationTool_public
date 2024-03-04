
#ifndef __FRAME__IDPlug_H
#define __FRAME__IDPlug_H

#include "RefBase/ref_base.h"
#include <string>

class CIDPlug : public baselib::RefBase
{
public:
    CIDPlug(std::string strID = "")
        : m_strID(strID)
    {}
    CIDPlug(const CIDPlug &stOther)
        : m_strID(stOther.m_strID)
    {}
    virtual ~CIDPlug(){}

    CIDPlug &operator=(const CIDPlug &stOther)
    {
        if (this != &stOther)
        {
            m_strID = stOther.m_strID;
        }
        return *this;
    }

    virtual CIDPlug *Clone() const
    {
        return new CIDPlug(*this);
    }

    std::string m_strID;
};

#endif /*#ifndef __FRAME__IDPlug_H*/
