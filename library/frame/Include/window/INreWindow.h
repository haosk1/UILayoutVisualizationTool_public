/**
 * frame
 *
 * 窗口刷新类接口
**/

#ifndef __FRAME__INREWINDOW_H__
#define __FRAME__INREWINDOW_H__

namespace frame {

class IOnRenderCallback {
public:
    virtual ~IOnRenderCallback(){};
    virtual void OnRender() = 0;
};

class INreWindow
{
public:
    virtual ~INreWindow(){};
/**
 * @brief 设置是否刷新屏幕
 */
    virtual void Active(bool bActive) = 0;
    virtual void SetRenderListener(IOnRenderCallback* callback) = 0;
};

} // namespace frame 

#endif // __FRAME__INREWINDOW_H__
