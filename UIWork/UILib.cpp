#include "UILib.h"
#include "Form/VisualizationForm.h"
#include "Stage/VisualizationStage.h"
#include "Actor/VisualizationActor.h"
#include "Actor/ImageActor.h"
#include "Actor/TextActor.h"
#include "Actor/ScrollBar.h"
#include "Stage/ScrollBarDecTurnPage.h"
#include "Stage/FrameLayout.h"
#include "Stage/AbsoluteLayout.h"
#include "Stage/LinearLayout.h"
#include "Stage/RelativeLayout.h"

CUILib *CUILib::m_Instance = nullptr;

CUILib *CUILib::Instance()
{
    if (nullptr == m_Instance)
    {
        m_Instance = new CUILib();
    }
    return m_Instance;
}
CUILib::CUILib() {}

CUILib::~CUILib() {}

bool CUILib::parseFormXml(const char *filePath, CForm **form)
{
    *form = new CVisualizationForm();
    CMarginLayoutPlug pstLayoutPlug(CUIController::Instance()->GetWindowRect().m_sWidth, CUIController::Instance()->GetWindowRect().m_sHeight);
    (*form)->SetLayoutPLug(pstLayoutPlug);
    std::string rootNote = "Resource";
    return CUIController::Instance()->parseActorXml(filePath, rootNote.c_str(), (CActor *)*form, this);
}

bool CUILib::parseFontXml(const char *filePath,
                          std::map<std::string, Font> &fontMap)
{
    if (!fontMap.empty())
    {
        fontMap.clear();
        std::map<std::string, Font> swapMap;
        fontMap.swap(swapMap);
    }

    TiXmlDocument doc;
    if (doc.LoadFile(filePath))
    {
        TiXmlElement *root = doc.RootElement();
        if (root)
        {
            // 遍历根元素的每个子元素Font
            for (TiXmlElement *element = root->FirstChildElement(); element;
                 element = element->NextSiblingElement())
            {
                Font font;
                // 每个Font的value都是Font，且只有name一个属性
                TiXmlAttribute *attribute = element->FirstAttribute();
                // printf("%s %s = %s \n",element->Value(), attribute->Name(),
                // attribute->Value());
                // 再解析Font的子元素
                for (TiXmlElement *childElement = element->FirstChildElement();
                     childElement; childElement = childElement->NextSiblingElement())
                {
                    // 每个Font的子元素的value都是Item，且只有name一个属性
                    TiXmlAttribute *childAttribute = childElement->FirstAttribute();
                    // printf("    %s %s = %s : %s \n",childElement->Value(),
                    // childAttribute->Name(), childAttribute->Value(),
                    // childElement->FirstChild()->Value());
                    if (!strcmp(childAttribute->Value(), "FontFamily"))
                    {
                        font.FontFamily = childElement->FirstChild()->Value();
                    }
                    if (!strcmp(childAttribute->Value(), "FontWidth"))
                    {
                        font.FontWidth = atoi(childElement->FirstChild()->Value());
                    }
                    if (!strcmp(childAttribute->Value(), "FontHeight"))
                    {
                        font.FontHeight = atoi(childElement->FirstChild()->Value());
                    }
                }
                fontMap.emplace(std::make_pair(attribute->Value(), font));
            }
            return true;
        }
    }
    printf("parseFontXml failed:%s \n", filePath);
    return false;
}

bool CUILib::parseStringXml(const char *filePath,
                            std::map<std::string, String> &stringMap)
{
    if (!stringMap.empty())
    {
        stringMap.clear();
        std::map<std::string, String> swapMap;
        stringMap.swap(swapMap);
    }

    TiXmlDocument doc;
    if (doc.LoadFile(filePath))
    {
        TiXmlElement *root = doc.RootElement();
        if (root)
        {
            // 遍历根元素的每个子元素String
            for (TiXmlElement *element = root->FirstChildElement(); element;
                 element = element->NextSiblingElement())
            {
                String string;
                // 每个String的value都是String，且只有name一个属性
                TiXmlAttribute *attribute = element->FirstAttribute();
                // printf("%s %s = %s \n",element->Value(), attribute->Name(),
                // attribute->Value());
                // 再解析String的子元素
                for (TiXmlElement *childElement = element->FirstChildElement();
                     childElement; childElement = childElement->NextSiblingElement())
                {
                    // 每个String的子元素都只有一个value，且没有属性
                    // printf("    %s %s = %s : %s \n",childElement->Value(),
                    // childElement->FirstChild()->Value());
                    if (!strcmp(childElement->Value(), "Chinese"))
                    {
                        string.Chinese = childElement->FirstChild()->Value();
                    }
                    if (!strcmp(childElement->Value(), "Chinese_Complex"))
                    {
                        string.Chinese_Complex = childElement->FirstChild()->Value();
                    }
                    if (!strcmp(childElement->Value(), "Chinese_Cantonese"))
                    {
                        string.Chinese_Cantonese = childElement->FirstChild()->Value();
                    }
                    if (!strcmp(childElement->Value(), "English"))
                    {
                        string.English = childElement->FirstChild()->Value();
                    }
                }
                stringMap.emplace(std::make_pair(attribute->Value(), string));
            }
            return true;
        }
    }
    printf("parseStringXml failed:%s \n", filePath);
    return false;
}

bool CUILib::parseColorXml(const char *filePath,
                           std::map<std::string, Color> &colorMap)
{
    if (!colorMap.empty())
    {
        colorMap.clear();
        std::map<std::string, Color> swapMap;
        colorMap.swap(swapMap);
    }

    TiXmlDocument doc;
    if (doc.LoadFile(filePath))
    {
        TiXmlElement *root = doc.RootElement();
        if (root)
        {
            // 遍历根元素的每个子元素Color
            for (TiXmlElement *element = root->FirstChildElement(); element;
                 element = element->NextSiblingElement())
            {
                Color color;
                // 每个Color的value都是Color，且只有name一个属性
                TiXmlAttribute *attribute = element->FirstAttribute();
                // printf("%s %s = %s \n",element->Value(), attribute->Name(),
                // attribute->Value()); 每个Color的子元素都只有一个，value就是#色值
                // printf("    %s %s = %s : %s \n",childElement->Value(),
                // childElement->FirstChild()->Value());
                std::string rgb = element->FirstChild()->Value();
                rgb = "0x" + rgb.substr(1, rgb.length() - 1); // 去掉#
                char *endptr;
                color.color_RGB = strtoul(rgb.c_str(), &endptr, 0);
                if (endptr == rgb.c_str())
                {
                    color.color_RGB = 0;
                }
                colorMap.emplace(std::make_pair(attribute->Value(), color));
            }
            return true;
        }
    }
    printf("parseColorXml failed:%s \n", filePath);
    return false;
}

bool CUILib::parseScrollBarXml(const char *filePath, CStage *ScrollBarLayout)
{
    std::string rootNote = "Resource";
    return CUIController::Instance()->parseActorXml(filePath, rootNote.c_str(), (CActor *)ScrollBarLayout, this);
}

 bool CUILib::parseDrawableXml(const std::string& pParseFileName, std::string& stdDrawablePath)
 {
    int iSuffixSize = 3;
    getFilePath(pParseFileName + ".*", stdDrawablePath);
    std::string strSuffix = stdDrawablePath.substr(stdDrawablePath.length() - iSuffixSize, iSuffixSize);
    if(strSuffix == "png"){
        return true;
    }else if (strSuffix == "xml"){
        TiXmlDocument doc;
        if (doc.LoadFile(stdDrawablePath.c_str()))
        {
            TiXmlElement *root = doc.RootElement();
            if (root)
            {
                TiXmlElement *StateListElement = root->FirstChildElement();
                // 遍历子节点
                for (TiXmlElement* itemElement = StateListElement->FirstChildElement("Item"); itemElement; itemElement = itemElement->NextSiblingElement("Item")) {
                    const char* drawable = itemElement->Attribute("Drawable");
                    if (drawable) {
                        std::string strDrawablePath = drawable;
                        strDrawablePath = strDrawablePath.substr(10, strDrawablePath.length() - 10);
                        return parseDrawableXml(strDrawablePath, stdDrawablePath);
                    }
                }
            }
        }
    }
    return false;
 }

CActor *CUILib::onGetChildActorFromChildElement(const char *childElementValue)
{
    CActor *childActor = nullptr;
    std::string value = childElementValue;
    if (value == "ActorFullScreenAnimation" || value == "Function")
    {
        childActor = new CFrameLayout();
    }
    else if (value == "MXUIScreenRelativeLayout" || value == "RelativeLayout")
    {
        childActor = new CRelativeLayout();
    }
    else if (value == "LinearLayout")
    {
        childActor = new CLinearLayout();
    }
    else if (value == "SmoothListActorEx" || value == "TransparencyActor")
    {
        childActor = new CVisualizationActor();
    }
    else if (value == "ImageButton" || value == "ImageActor" || value == "AutomaticImageActor")
    {
        childActor = new CImageActor();
    }
    else if (value == "EditBox" || value == "TextActor" || value == "Button")
    {
        childActor = new CTextActor();
    }
    else if (value == "ScrollBarTrack" || value == "DropDownScrollBarTrack")
    {
        childActor = new CScrollBar();
    }
    else if (value == "SmoothListScrollBar")
    {
        childActor = new CScrollBarDecTurnPage();
    }
    return childActor;
}

bool CUILib::onLinkParentAndChild(const char *parentValue, CActor *parent, CActor *child)
{
    if (nullptr == parentValue || nullptr == parent || nullptr == child)
    {
        return false;
    }
    if (parent->IsStage())
    {
        ((CStage *)parent)->AddActor(child);
    }
    else if (!strcmp(parentValue, "Resource") && !parent->IsStage())
    {
        ((CForm *)parent)->SetStage((CStage *)child);
    }
    else
    {
        return false;
    }
    return true;
}

void CUILib::SetCharacterLibraryPath(const char *path)
{
    return CUIController::Instance()->SetCharacterLibraryPath(path);
}

bool CUILib::getTextSize(const std::string &text, uint textSize, uint lineSpace,
                         const UIRect &limitRect, UIRect &outSize, uint &outMostTextHeight)
{
    return CUIController::Instance()->getTextSize(text, textSize, lineSpace,
                                                  limitRect, outSize, outMostTextHeight);
}

void CUILib::SetOnlyOne(bool onlyone) { m_onlyOne = onlyone; }

bool CUILib::GetOnlyOne() { return m_onlyOne; }

void CUILib::SetFontMap(const std::map<std::string, Font> &fontMap)
{
    if (!m_fontMap.empty())
    {
        m_fontMap.clear();
        std::map<std::string, Font> swapMap;
        m_fontMap.swap(swapMap);
    }
    m_fontMap.insert(fontMap.begin(), fontMap.end());
}

void CUILib::GetFont(const std::string &key, Font &font)
{
    std::map<std::__cxx11::string, Font>::iterator iter = m_fontMap.find(key);
    if (iter != m_fontMap.end())
    {
        font = iter->second;
    }else{
        getFontFromXml(key, font);
    }
    return;
}

void CUILib::GetFontMap(std::map<std::string, Font> &fontMap)
{
    if (!fontMap.empty())
    {
        fontMap.clear();
        std::map<std::string, Font> swapMap;
        fontMap.swap(swapMap);
    }
    fontMap.insert(m_fontMap.begin(), m_fontMap.end());
}

void CUILib::SetStringMap(const std::map<std::string, String> &stringMap)
{
    if (!m_stringMap.empty())
    {
        m_stringMap.clear();
        std::map<std::string, String> swapMap;
        m_stringMap.swap(swapMap);
    }
    m_stringMap.insert(stringMap.begin(), stringMap.end());
}

void CUILib::GetString(const std::string &key, String &string)
{
    std::map<std::string, String>::iterator iter = m_stringMap.find(key);
    if (iter != m_stringMap.end())
    {
        string = iter->second;
    }
    return;
}

void CUILib::GetStringMap(std::map<std::string, String> &stringMap)
{
    if (!stringMap.empty())
    {
        stringMap.clear();
        std::map<std::string, String> swapMap;
        stringMap.swap(swapMap);
    }
    stringMap.insert(m_stringMap.begin(), m_stringMap.end());
}

void CUILib::SetColorMap(const std::map<std::string, Color> &colorMap)
{
    if (!m_colorMap.empty())
    {
        m_colorMap.clear();
        std::map<std::string, Color> swapMap;
        m_colorMap.swap(swapMap);
    }
    m_colorMap.insert(colorMap.begin(), colorMap.end());
}

void CUILib::GetColor(const std::string &key, Color &color)
{
    std::map<std::string, Color>::iterator iter = m_colorMap.find(key);
    if (iter != m_colorMap.end())
    {
        color = iter->second;
    }else{
        getColorFromXml(key, color);
    }
    return;
}

void CUILib::GetColorMap(std::map<std::string, Color> &colorMap)
{
    if (!colorMap.empty())
    {
        colorMap.clear();
        std::map<std::string, Color> swapMap;
        colorMap.swap(swapMap);
    }
    colorMap.insert(m_colorMap.begin(), m_colorMap.end());
}


bool CUILib::getColorFromXml(const std::string& pParseFileName, Color &color)
{
    std::string strColorFilePath = "";
    int iSuffixSize = 3;
    getFilePath(pParseFileName + ".*", strColorFilePath);
    if ((!strColorFilePath.empty()) && ("xml" == strColorFilePath.substr(strColorFilePath.length() - iSuffixSize, iSuffixSize))){
        TiXmlDocument doc;
        if (doc.LoadFile(strColorFilePath.c_str()))
        {
            TiXmlElement *root = doc.RootElement();
            if (root)
            {
                TiXmlElement *StateListElement = root->FirstChildElement();
                // 遍历子节点
                for (TiXmlElement* itemElement = StateListElement->FirstChildElement("Item"); itemElement; itemElement = itemElement->NextSiblingElement("Item")) {
                    const char* pstrColor = itemElement->Attribute("Color");
                    if (pstrColor) {
                        std::string strColor = pstrColor;
                        if("#" == strColor.substr(0, 1)){
                            std::string rgb = "0x" + strColor.substr(1, strColor.length() - 1); // 去掉#
                            char *endptr;
                            color.color_RGB = strtoul(rgb.c_str(), &endptr, 0);
                            if (endptr == rgb.c_str())
                            {
                                color.color_RGB = 0;
                            }
                            m_colorMap.emplace(std::make_pair(pParseFileName, color));
                            return true;
                        }else if("@" == strColor.substr(0, 1)){
                            return getColorFromXml(strColor.substr(7, strColor.length() - 7), color);// 7:@Color/
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool CUILib::getFontFromXml(const std::string& pParseFileName, Font &font)
{
    std::string strFilePath = "";
    int iSuffixSize = 3;
    getFilePath(pParseFileName + ".*", strFilePath);
    if ((!strFilePath.empty()) && ("xml" == strFilePath.substr(strFilePath.length() - iSuffixSize, iSuffixSize))){
        TiXmlDocument doc;
        if (doc.LoadFile(strFilePath.c_str()))
        {
            TiXmlElement *root = doc.RootElement();
            if (root)
            {
                TiXmlElement *StateListElement = root->FirstChildElement();
                // 遍历子节点
                for (TiXmlElement* itemElement = StateListElement->FirstChildElement("Item"); itemElement; itemElement = itemElement->NextSiblingElement("Item")) {
                    const char* pstrFont = itemElement->Attribute("Font");
                    if (pstrFont) {
                        std::string strFont = pstrFont;
                        if("@" == strFont.substr(0, 1)){
                            GetFont(strFont.substr(6, strFont.length() - 6), font);// 6:@Font/
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

std::string m_Path = "./frame/UI/res";
bool CUILib::getFilePath(const std::string& strFileName, std::string& files) 
{
    DIR* dir;
    struct dirent* entry;
    bool isFound = false;
    bool isglobed = false;

    if ((dir = opendir(m_Path.c_str())) != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
        
            if (entry->d_type == DT_DIR) {
                m_Path = m_Path + "/" + entry->d_name;
                bool ret = getFilePath(strFileName, files); // 递归遍历子目录
                if(ret)
                {
                    isFound = true;
                    break;
                }
            }
            else if(!isglobed){
                isglobed = true;
                // 使用 glob() 函数匹配文件
                glob_t glob_result;
                std::string strFileAllPath = m_Path + "/" + strFileName;
                glob(strFileAllPath.c_str(), GLOB_NOSORT, nullptr, &glob_result);
                // 将匹配到的文件添加到结果中
                if(glob_result.gl_pathc > 0){;
                    files = glob_result.gl_pathv[0];
                    globfree(&glob_result);
                    isFound = true;
                    break;
                }
                // for (int i = 0; i < glob_result.gl_pathc; i++) {
                //     files.push_back(glob_result.gl_pathv[i]);
                // }
                // 释放 glob 结构
                globfree(&glob_result);
            }
        }
        closedir(dir);
    }

    if("./frame/UI/res" != m_Path && std::string::npos != m_Path.find_last_of("/")){
        m_Path = m_Path.substr(0, m_Path.find_last_of("/"));
    }
    return isFound;
}