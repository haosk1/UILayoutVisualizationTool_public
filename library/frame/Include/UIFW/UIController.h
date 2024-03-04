/**
 * frame
 *
 * UI控制器
 **/
#ifndef __FRAME__CUICONTRLLER_H__
#define __FRAME__CUICONTRLLER_H__

#include <vector>
#include <fstream>
#include <sstream>

#include "Animator/AnimatorManager.h"
#include "CUIPhysicalLayer.h"
#include "tinyxml/tinyxml.h"
class UIInitCallback
{
public:
    virtual ~UIInitCallback(){};
    virtual void onUIInit() = 0;
};

class XMLParseCallback
{
public:
    virtual ~XMLParseCallback(){};
    virtual CActor *onGetChildActorFromChildElement(const char *childElementValue) = 0;
    virtual bool onLinkParentAndChild(const char *parentValue, CActor *parent, CActor *child) = 0;
};

class InputCharCallback
{
public:
    virtual ~InputCharCallback(){};
    virtual void onInputCharCallback(unsigned int iChar) = 0;
};

class CUIController
{
public:
    static CUIController *Instance();
    virtual ~CUIController();
    static void AddUIInitCallback(UIInitCallback *callback);

    void Init(int width, int height);

    void SetWindowSize(int width, int height);
    UIRect GetWindowRect();

    CUIPhysicalLayer *GetUIPhysicalLayer();
    CAnimatorManager *GetAnimatorManager();

    ulong getCurrentTime(); // 毫秒

    std::vector<float> GetBezierPoints(UIPoint start, UIPoint end, UIPoint controller);

	bool parseOBJ(const char *filePath, std::vector<float>& verts, std::vector<float>& textureCoords, std::vector<float>& normals, unsigned long& vertsNumber);

    bool parseActorXml(const char *filePath, const char *rootNote, CActor *rootActor, XMLParseCallback *callback);

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
     * [IN] | limitRect | const UIRect& | 字符串最大范围矩形框 | 非空 | 如果是单行文字，请将limitRect.m_sHeight设为-1;
     *
     * @retval    返回值类型  bool | true:函数调用成功；false：函数调用失败
     */
    bool getTextSize(const std::string &text, uint textSize, uint lineSpace, const UIRect &limitRect, UIRect &outSize, uint &outMostTextHeight);

    /**
     * @brief 获取图片尺寸
     *
     * [I/O]|形参名|形参类型|参数含义|取值范围
     * [IN] | textureId | unsigned int | 纹理ID | 非0
     * [OUT] | outRect | UIRect& | 图片尺寸(宽高)数据 | 非0
     *
     * @retval    返回值类型  void
     */
    bool GetDrawableRect(unsigned int textureId, UIRect &outRect);

    void AddInputCharCallback(InputCharCallback* pInputCharCallback);
    void RemoveInputCharCallback(InputCharCallback* pInputCharCallback);
    const std::vector<InputCharCallback*> GetInputCharCallbacks();

    /**
     * @brief 获取在投影矩阵中的空间坐标对应的归一化坐标
     *
     * [I/O]|形参名|形参类型|参数含义|取值范围
     * [IN] | screenPoint | const UIPointWithZ & | 空间点坐标
     * [IN] | projection_matrix | const UImat4 & | 投影矩阵
     *
     * @retval    UIPointFWithZ  归一化坐标
     */
    UIPointFWithZ GetCollectDirection(const UIPointWithZ &screenPoint, const UImat4 &projection_matrix);

    /**
     * @brief 获取屏幕点击与平面的交点
     *
     * [I/O]|形参名|形参类型|参数含义|取值范围
     * [IN] | screenPoint | const UIPoint & | 屏幕坐标
     * [IN] | point_on_plane | const UIPointFWithZ & | 平面任一点坐标
     * [IN] | ormal | const UIPointFWithZ & | 平面法向量
     * [OUT] | intersection | UIPointFWithZ & | 交点坐标
     *
     * @retval    bool  是否有交点
     */
    bool GetIntersection(const UIPoint &screenPoint, const UIPointFWithZ &point_on_plane, const UIPointFWithZ &normal, UIPointFWithZ& intersection);

private:
    explicit CUIController();
    bool ParseXmlElement(TiXmlElement *rootElement, CActor *parentActor, XMLParseCallback *callback);

    static CUIController* m_Instance;
    static std::vector<UIInitCallback*> m_UIInitCallback_vec;
    CUIPhysicalLayer* m_UIPhysicalLayer;
    CAnimatorManager* m_AnimatorManager;

    std::vector<InputCharCallback*> m_vecpInputCharCallback;

    int m_WindowWidth = 0;
    int m_WindowHeight = 0;
};
#endif // __FRAME__CUICONTRLLER_H__