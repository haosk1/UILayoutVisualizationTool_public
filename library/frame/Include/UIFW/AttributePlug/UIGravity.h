
#ifndef __FRAME__UIGRAVITY_H
#define __FRAME__UIGRAVITY_H

#include "../UIRect.h"

class CUIGravity
{
  public:
    enum
    {
        NONE = 0x000000000,
        TOP = 0x00000001,
        CENTER_VERTICAL = 0x00000002,
        BOTTOM = 0x00000003,
        VERTICAL_MASK = 0x00000003,
        LEFT = 0x00000004,
        CENTER_HORIZENTAL = 0x00000008,
        RIGHT = 0x0000000C,
        HORIZENTAL_MASK = 0x0000000C,
    };
    
    static void Apply(unsigned int ulGravity, const UIRect &stContainerRect, 
                      int sWidth, int sHeight, int sLength, 
                      short sLeftMargin, short sTopMargin, short sFrontMargin, short sRightMargin, short sBottomMargin, short sBackMargin, 
                      UIRect &stOutRect)
    {
        ApplyHorizontal(ulGravity, sWidth, stContainerRect.m_sLeft, stContainerRect.m_sWidth, sLeftMargin, sRightMargin, stOutRect.m_sLeft);

        ApplyVertical(ulGravity, sHeight, stContainerRect.m_sTop, stContainerRect.m_sHeight, sTopMargin, sBottomMargin, stOutRect.m_sTop);

        stOutRect.m_sFront = stContainerRect.m_sFront + sFrontMargin;

        stOutRect.m_sWidth = sWidth;
        stOutRect.m_sHeight = sHeight;
        stOutRect.m_sLength = sLength;
    }

    static void ApplyHorizontal(unsigned int ulGravity, short sWidth, short sContainerLeft, short sContainerWidth,
                                short sLeftMargin, short sRightMargin, int &sOutLeft)
    {
        switch (ulGravity & HORIZENTAL_MASK)
        {
        case LEFT:
            sOutLeft = sContainerLeft + sLeftMargin;
            break;
        case CENTER_HORIZENTAL:
            sOutLeft = sContainerLeft + sLeftMargin + (sContainerWidth - sWidth - sLeftMargin - sRightMargin) / 2;
            break;
        case RIGHT:
            sOutLeft = sContainerLeft + sContainerWidth - sWidth - sRightMargin;
            break;
        default:
            sOutLeft = sContainerLeft + sLeftMargin;
            break;
        }
    }

    static void ApplyVertical(unsigned int ulGravity, short sHeight, short sContainerTop, short sContainerHeight,
                              short sTopMargin, short sBottomMargin, int &sOutTop)
    {
        switch (ulGravity & VERTICAL_MASK)
        {
        case TOP:
            sOutTop = sContainerTop + sTopMargin;
            break;
        case CENTER_VERTICAL:
            sOutTop = sContainerTop + sTopMargin + (sContainerHeight - sHeight - sTopMargin - sBottomMargin) / 2;
            break;
        case BOTTOM:
            sOutTop = sContainerTop + sContainerHeight - sHeight - sBottomMargin;
            break;
        default:
            sOutTop = sContainerTop + sTopMargin;
            break;
        }
    }
};

#endif /*#ifndef UIGRAVITY_H*/
