/**
 * frame
 *
 * UI物理层，UI事件和描画的入口
**/
#ifndef __FRAME__CUIPHYSICALLAYER_H__
#define __FRAME__CUIPHYSICALLAYER_H__

#include <map>

#include "event/IEventManager.h"
#include "window/INreWindow.h"

#include "UICanvasInterface.h"
#include "Form.h"



using namespace frame;
class UIController;
class CUIPhysicalLayer : public IEventCallback, public IOnRenderCallback
{
public:
    CUIPhysicalLayer(){};
    bool LoadForm(CForm* form, unsigned long formID);
    bool UnloadForm(unsigned long formID);
    bool UnloadForm(CForm* form);
    //for Interface IEventCallback
    virtual ~CUIPhysicalLayer();
    virtual void DispatchMouseEvent(Mouse_Event event) override;
    virtual void DispatchKeyEvent(Keyboard_Event event) override;
    virtual void DispatchInputCharEvent(Input_Char_Event event) override;
    virtual void onEndJourney() override;
    //for Interface IOnRenderCallback
    virtual void OnRender() override;

    void hardwareRender();

    UIRect GetViewRect(void) const;
    void GetViewRect(UIRect &stRect) const;

    void Invalidate(const UIRect &stRect);
private:
    void Draw(CUICanvasInterface &stCanvas);

    std::map<unsigned long, CForm*> m_stFormMap;

    //动画相关start
    UIRect m_stDirtyRect;
    //动画相关end
};
#endif  // __FRAME__CUIPHYSICALLAYER_H__