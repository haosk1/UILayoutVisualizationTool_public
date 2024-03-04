/**
 * UIWork类型定义
**/
#ifndef __UIWORKDEF_H__
#define __UIWORKDEF_H__

typedef enum
{
    EVENT_ACTIVE_TYPE_INVALID,//无效消息
    EVENT_ACTIVE_TYPE_VALID,//有效消息
} liz;

struct Font
{
    Font():FontFamily(""),FontWidth(0),FontHeight(0){};
    std::string FontFamily;
    unsigned int FontWidth;
    unsigned int FontHeight;
};

struct String
{
    String():Chinese(""),Chinese_Complex(""),Chinese_Cantonese(""),English(""){};
    std::string Chinese;
    std::string Chinese_Complex;
    std::string Chinese_Cantonese;
    std::string English;
};

struct Color
{
    Color():color_RGB(0x00){};
    unsigned int color_RGB;
};


#endif // __UIWORKDEF_H__