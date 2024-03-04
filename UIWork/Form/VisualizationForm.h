
/**
 * frame
 *
 * 可视化form
**/

#ifndef __FRAME__CVISUALIZATIONFORM_H__
#define __FRAME__CVISUALIZATIONFORM_H__

#include <vector>

#include "UIFW/Form.h"

class CVisualizationForm : public CForm
{
public:
    explicit CVisualizationForm(){};
    virtual ~CVisualizationForm(){};

    virtual bool OnMouseEvent(Mouse_Event event);

protected:
    virtual void DispatchDraw(CUICanvasInterface &stCanvas);

private:
    CVisualizationForm(const CVisualizationForm &);
    CVisualizationForm &operator=(const CVisualizationForm &);

    bool m_IsCompare = false;
};

#endif /*#ifndef __FRAME__CVISUALIZATIONFORM_H__*/
