
/*
 RelativeLayoutPlug是Actor专门为RelativeLayout适配的属性插件
*/
#ifndef __FRAME__RelativeLayoutPlug_H
#define __FRAME__RelativeLayoutPlug_H

#include "RefBase/ref_base.h"
#include <string.h>
#include "UIFW/UIDef.h"

class CRelativeLayoutPlug : public baselib::RefBase
{
public:
    enum EnumAlign
    {
        LeftOf = 0,
        RightOf,
        AboveOf,
        BelowOf,
        AlignLeft,
        AlignRight,
        AlignTop,
        AlignBottom,

        AlignCount,
    };
    enum EnumAlignParent
    {
        AlignParentLeft = 0,
        AlignParentRight,
        AlignParentTop,
        AlignParentBottom,
        CenterInParent,
        CenterHorizontal,
        CenterVertical,
        AlignWithParentIfMissing,

        AlignParentCount,
    };

    CRelativeLayoutPlug(int iPreLeft = -1, int iPreTop = -1, int iPreRight = -1, int iPreBottom = -1)
        : m_iPreLeft(iPreLeft), m_iPreTop(iPreTop), m_iPreRight(iPreRight), m_iPreBottom(iPreBottom)
    {
        ClearRule();
        ClearRuleWithParent();
    }
    CRelativeLayoutPlug(const CRelativeLayoutPlug &stOther)
        :m_iPreLeft(stOther.m_iPreLeft), m_iPreTop(stOther.m_iPreTop),m_iPreRight(stOther.m_iPreRight), m_iPreBottom(stOther.m_iPreBottom)
    {
        for (ulong iCount = 0; iCount < ARRAY_MAX(m_ulRule); ++iCount)
        {
            m_ulRule[iCount] = stOther.m_ulRule[iCount];
        }
        for (ulong iCount = 0; iCount < ARRAY_MAX(m_ulRuleWithParent); ++iCount)
        {
            m_ulRuleWithParent[iCount] = stOther.m_ulRuleWithParent[iCount];
        }
    }
    virtual ~CRelativeLayoutPlug(){}

    void ClearRule(void)
    {
        for (ulong iCount = 0; iCount < ARRAY_MAX(m_ulRule); ++iCount)
        {
            m_ulRule[iCount] = "";
        }
    }

     void ClearRuleWithParent(void)
    {
        for (ulong iCount = 0; iCount < ARRAY_MAX(m_ulRuleWithParent); ++iCount)
        {
            m_ulRuleWithParent[iCount] = false;
        }
    }

    void AddRule(ulong ulAlign, std::string strAnchor)
    {
        if (ulAlign < ARRAY_MAX(m_ulRule))
        {
            m_ulRule[ulAlign] = strAnchor;
        }
    }
    void AddRuleWithParent(ulong ulAlign, bool bAnchor)
    {
        if (ulAlign < ARRAY_MAX(m_ulRuleWithParent))
        {
            m_ulRuleWithParent[ulAlign] = bAnchor;
        }
    }

    CRelativeLayoutPlug &operator=(const CRelativeLayoutPlug &stOther)
    {
        if (this != &stOther)
        {
            m_iPreLeft = stOther.m_iPreLeft;
            m_iPreTop = stOther.m_iPreTop;
            m_iPreRight = stOther.m_iPreRight;
            m_iPreBottom = stOther.m_iPreBottom;
            for (ulong iCount = 0; iCount < ARRAY_MAX(m_ulRule); ++iCount)
            {
                m_ulRule[iCount] = stOther.m_ulRule[iCount];
            }
            for (ulong iCount = 0; iCount < ARRAY_MAX(m_ulRuleWithParent); ++iCount)
            {
                m_ulRuleWithParent[iCount] = stOther.m_ulRuleWithParent[iCount];
            }
        }
        return *this;
    }

    virtual CRelativeLayoutPlug *Clone() const
    {
        return new CRelativeLayoutPlug(*this);
    }

    //测量时距离Relativelayout边界的距离,也就是说relativelayout子元器件的位置是在测量时计算好的
    int m_iPreLeft;
    int m_iPreTop;
    int m_iPreRight;
    int m_iPreBottom;

    std::string m_ulRule[AlignCount];
    bool m_ulRuleWithParent[AlignParentCount];
};

#endif /*#ifndef __FRAME__RelativeLayoutPlug_H*/
