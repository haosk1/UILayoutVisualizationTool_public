/**
 * frame
 *
 * UICanvas使用的矩阵
**/
#ifndef UI_CANVAS_MATRIX_H
#define UI_CANVAS_MATRIX_H

#include "UIMatrix.h"

enum
{
    UI_GL_MODEVIEW,
    UI_GL_PROJECTION,
    UI_GL_TEXTURE,
};

class UIController;
class CUICanvasAction
{
  public:
    CUICanvasAction();
    ~CUICanvasAction();

    void UIGLMatrixMode(int mode);
    void UIGLFrustum(float fleft, float fright, float fbottom, float ftop, float fnear, float ffar,
                     bool bPerspective = false);

    void UIGLLoadIdentity();
    void UIGLMultMatrix(const UImat4 &pIn);
    void UIGLLoadMatrix(const UImat4 &pIn);
    void UIGLTranslatef(float x, float y, float z);
    void UIGLRotatef(float angle, float x, float y, float z);
    void UIGLScalef(float x, float y, float z);

  public:
    UImat4 m_projection_matrix;//投影矩阵
    UImat4 m_modelview_matrix;//视图矩阵
    UImat4 m_texture_matrix;//纹理矩阵
    UImat4 *m_current_matrix;//矩阵控制器

    UIRect m_ClipRect;//剪裁区域
    unsigned char m_ucAlpha = 255;//色值

  private:
    CUICanvasAction(const CUICanvasAction &);
    CUICanvasAction &operator=(const CUICanvasAction &);
};

#endif //UI_CANVAS_MATRIX_H
