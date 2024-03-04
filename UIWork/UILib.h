/**
 * UILib
 **/
#ifndef __CUILIB_H__
#define __CUILIB_H__

#include <map>
#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>
#include <iostream>
#include <glob.h>

#include "UIFW/UIController.h"
#include "UIFW/tinyxml/tinyxml.h"
#include "UIWorkDef.h"
class CUILib : public XMLParseCallback
{
public:
    static CUILib *Instance();
    virtual ~CUILib();

    // 实现XMLParseCallback
    virtual CActor *onGetChildActorFromChildElement(const char *childElementValue);
    virtual bool onLinkParentAndChild(const char *parentValue, CActor *parent, CActor *child);

    // 解析Form xml文件
    bool parseFormXml(const char *filePath, CForm **form);
    // 解析Font xml文件
    bool parseFontXml(const char *filePath, std::map<std::string, Font> &fontMap);
    // 解析String xml文件
    bool parseStringXml(const char *filePath, std::map<std::string, String> &stringMap);
    // 解析color xml文件
    bool parseColorXml(const char *filePath, std::map<std::string, Color> &colorMap);
    // 解析ScrollBar xml文件
    bool parseScrollBarXml(const char *filePath, CStage *ScrollBarLayout);

    // 解析Drawable xml文件
    bool parseDrawableXml(const std::string& pParseFileName, std::string& stdDrawablePath);

    /**
     * @brief 设置字体库
     *
     * [I/O]|形参名|形参类型|参数含义|取值范围
     * [IN] | path | const char& | 字体库路径 | 非空
     *
     * @retval    返回值类型  void
     */
    void SetCharacterLibraryPath(const char *path);

    /**
     * @brief 获取文言字符串实际描画宽高
     *
     * [I/O]|形参名|形参类型|参数含义|取值范围
     * [OUT] | outSize | UIRect & | 返回的字符串rect | 非空
     * [OUT] | outMostTextHeight | uint & | 返回的字符串中最高字符的高度值 | 非空
     * [IN] | text | const std::string& | 字符串 | 非空
     * [IN] | textSize | uint | 字符size | uint
     * [IN] | lineSpace | uint | 行间距 | uint
     * [IN] | limitRect | const UIRect& | 字符串最大范围矩形框 | 非空 |
     * 如果是单行文字，请将limitRect.m_sHeight设为-1;
     *
     * @retval    返回值类型  bool | true:函数调用成功；false：函数调用失败
     */
    bool getTextSize(const std::string &text, uint textSize, uint lineSpace,
                     const UIRect &limitRect, UIRect &outSize, uint &outMostTextHeight);

    void SetOnlyOne(bool onlyone);
    bool GetOnlyOne();

    void SetFontMap(const std::map<std::string, Font> &fontMap);
    void GetFont(const std::string &key, Font &font);
    void GetFontMap(std::map<std::string, Font> &fontMap);

    void SetStringMap(const std::map<std::string, String> &stringMap);
    void GetString(const std::string &key, String &string);
    void GetStringMap(std::map<std::string, String> &stringMap);

    void SetColorMap(const std::map<std::string, Color> &colorMap);
    void GetColor(const std::string &key, Color &color);
    void GetColorMap(std::map<std::string, Color> &colorMap);

    bool getFilePath(const std::string& strFileName, std::string& files);
    // 从xml文件中递归获取color值
    bool getColorFromXml(const std::string& pParseFileName, Color &color);
    // 从xml文件中递归获取Font值
    bool getFontFromXml(const std::string& pParseFileName, Font &font);

private:
    explicit CUILib();
    static CUILib *m_Instance;

    bool m_onlyOne = false;
    std::map<std::string, Font> m_fontMap;
    std::map<std::string, String> m_stringMap;
    std::map<std::string, Color> m_colorMap;
};
#endif // __CUILIB_H__