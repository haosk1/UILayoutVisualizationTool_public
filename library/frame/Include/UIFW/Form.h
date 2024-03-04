
/**
 * frame
 *
 * UIForm基类
**/

#ifndef FORM_H
#define FORM_H

#include "Actor.h"
#include "Stage.h"

class CForm : public CActor
{
public:
    explicit CForm(){};
    virtual ~CForm(){};

    virtual void Load(void);//chind do
    virtual void Unload(void);

    virtual void SetStage(CStage *layout);
    
    virtual bool DispatchMouseEvent(Mouse_Event event) override;
    virtual bool DispatchKeyEvent(Keyboard_Event event) override;
    void Measure();
    void Layout();
    
protected:
    virtual void OnDraw(CUICanvasInterface &stCanvas) override;
    virtual void DispatchDraw(CUICanvasInterface &stCanvas);

private:
    CForm(const CForm &);
    CForm &operator=(const CForm &);
    
protected:
    CStage *m_pstLayout = nullptr;//child 赋值
};

#endif /*#ifndef FORM_H*/
