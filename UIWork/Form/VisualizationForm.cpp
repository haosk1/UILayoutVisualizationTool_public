#include "VisualizationForm.h"

bool CVisualizationForm::OnMouseEvent(Mouse_Event event)
{
    if(event.mouse_button == EVENT_MOUSE_BUTTON_RIGHT && event.mouse_type == EVENT_MOUSE_TYPE_DOWN){
        UIPoint point(event.iPosX, event.iPosY);
        UIRect stHitRect;
        GetHitRect(stHitRect);
        if (stHitRect.Contains(point))
        {
            m_IsCompare = true;
            return true;
        }
    }else if(event.mouse_button == EVENT_MOUSE_BUTTON_RIGHT && (event.mouse_type == EVENT_MOUSE_TYPE_UP || event.mouse_type == EVENT_MOUSE_TYPE_INVALID)){
        m_IsCompare = false;
        return true;
    }
    return false;
}

void CVisualizationForm::DispatchDraw(CUICanvasInterface &stCanvas)
{
    if (nullptr != m_pstLayout)
    {
        int iSaveCount = stCanvas.GetSaveCount();
        stCanvas.Save();
        if(m_IsCompare){
            stCanvas.Alpha(125);
        }
        m_pstLayout->Draw(stCanvas);
        stCanvas.RestoreToCount(iSaveCount);
    }
}