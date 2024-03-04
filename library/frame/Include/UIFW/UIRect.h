/**
 * frame
 *
 * UI矩形框定义
**/
#ifndef __UIRECT_H__
#define __UIRECT_H__
#include "UIDef.h"
#include "UIPoint.h"

class UIRect
{
public:
    int m_sLeft, m_sTop, m_sFront, m_sWidth, m_sHeight, m_sLength;

    UIRect(void) : m_sLeft(0), m_sTop(0), m_sFront(0), m_sWidth(0), m_sHeight(0), m_sLength(0)
    {
    }

    UIRect(int sLeft, int sTop, int sFront, int sWidth, int sHeight, int sLength)
        : m_sLeft(sLeft), m_sTop(sTop), m_sFront(sFront), m_sWidth(sWidth), m_sHeight(sHeight), m_sLength(sLength)
    {
    }

    UIRect(const UIRect &rhs)
        : m_sLeft(rhs.m_sLeft), m_sTop(rhs.m_sTop), m_sFront(rhs.m_sFront), m_sWidth(rhs.m_sWidth), m_sHeight(rhs.m_sHeight), m_sLength(rhs.m_sLength)
    {
    }

    bool IsValid(void) const
    {
        return m_sWidth > 0 && m_sHeight > 0;
    }

    int GetRight(void) const
    {
        int sRight = m_sLeft + m_sWidth;
        return sRight;
    }

    int GetBottom(void) const
    {
        int sBottom = m_sTop + m_sHeight;
        return sBottom;
    }

    void Set(int sLeft, int sTop, int sWidth, int sHeight)
    {
        m_sLeft = sLeft;
        m_sTop = sTop;
        m_sWidth = sWidth;
        m_sHeight = sHeight;
    }

    void Set(int sLeft, int sTop, int sFront, int sWidth, int sHeight, int sLength)
    {
        m_sLeft = sLeft;
        m_sTop = sTop;
        m_sWidth = sWidth;
        m_sHeight = sHeight;
        m_sLength = sLength;
        m_sFront = sFront;
    }

    void Set(const UIRect &stOther)
    {
        Set(stOther.m_sLeft, stOther.m_sTop, stOther.m_sFront, stOther.m_sWidth, stOther.m_sHeight, stOther.m_sLength);
    }
    
    //取交集
    bool Intersect(const UIRect &rhs, UIRect &stResult)
    {
        int sTempLeft = UI_MAX(m_sLeft, rhs.m_sLeft);
        int sTempTop = UI_MAX(m_sTop, rhs.m_sTop);
        int sTempFront = UI_MAX(m_sFront, rhs.m_sFront);
        int sTempRight = UI_MIN(GetRight(), rhs.GetRight());
        int sTempBottom = UI_MIN(GetBottom(), rhs.GetBottom());
        int sTempLength = UI_MIN(m_sLength, rhs.m_sLength);

        if (sTempLeft < sTempRight && sTempTop < sTempBottom)
        {
            stResult.Set(sTempLeft, sTempTop, sTempFront, sTempRight - sTempLeft, sTempBottom - sTempTop, sTempLength);
            return true;
        }

        return false;
    }
    
    //取并集
    UIRect &Unite(const UIRect &rhs)
    {
        int sOriginRight = GetRight();
        int sOriginBottom = GetBottom();
        m_sLeft = m_sLeft < rhs.m_sLeft ? m_sLeft : rhs.m_sLeft;
        m_sTop = m_sTop < rhs.m_sTop ? m_sTop : rhs.m_sTop;
        m_sFront = UI_MIN(m_sFront, rhs.m_sFront);
        int sRight = sOriginRight > rhs.GetRight() ? sOriginRight : rhs.GetRight();
        int sBottom = sOriginBottom > rhs.GetBottom() ? sOriginBottom : rhs.GetBottom();
        m_sWidth = sRight - m_sLeft;
        m_sHeight = sBottom - m_sTop;
        m_sLength = UI_MAX(m_sLength, rhs.m_sLength);
        return *this;
    }

    bool Contains(int iX, int iY) const
    {
        return IsValid() && (iX >= m_sLeft) && (iX < GetRight()) && (iY >= m_sTop) && (iY < GetBottom());
    }

    bool Contains(const UIPoint &stPoint) const
    {
        return Contains(stPoint.m_iX, stPoint.m_iY);
    }

    void Clear(void)
    {
        m_sLeft = 0;
        m_sTop = 0;
        m_sWidth = 0;
        m_sHeight = 0;
        m_sLength = 0;
        m_sFront = 0;
    }
    
};
#endif  // __UIRECT_H__