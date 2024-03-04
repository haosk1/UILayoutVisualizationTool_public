/**
 * frame
 *
 * UI描画画布接口，提供基本描画能力
**/
#ifndef __FRAME__CUICANVASINTERFACE_H__
#define __FRAME__CUICANVASINTERFACE_H__

#include <stdio.h>
#include <vector>

#include "../gl/shader/IShaderFactory.h"

#include "UICanvasAction.h"

#define numVAOs 1
#define numVBOs 3

class UIController;
class CUICanvasInterface
{
public:
    virtual ~CUICanvasInterface(){};
    virtual void SetDefaultShader(IShaderFactory::ShaderType shaderType) = 0;
    virtual void DrawTexture(unsigned int textureId, const UIRect &stDestRect) = 0;
    virtual void DrawTexture(unsigned int textureId, const float* psPoints, unsigned long pointsSize, const float* afTexCoord, const float* afNormals = nullptr) = 0;
    virtual void DrawNineTexture(unsigned int textureId, const UIRect &stResRect, const UIRect &stDestRect) = 0;
    virtual void DrawString(const char* text, const UIRect &stDestRect, unsigned int iMostTextHeight,unsigned int textSize = 40, unsigned char ucR = 255, unsigned char ucG = 255, unsigned char ucB = 255, float zoomX = 1.0, float zoomY = 1.0) = 0;
	virtual void SetLineWidth(int iWidth) = 0;
    virtual void DrawLine(const float* psPoints, int iNum, unsigned char ucR = 255, unsigned char ucG = 255, unsigned char ucB = 255) = 0;

    virtual void ClipRound(int sX, int sY, int sZ, int sWidth, int sHeight, int sLength) = 0;
    virtual void ClipRound(const UIRect &stRect) = 0;

    CUICanvasAction& Action(){
        return *(m_CanvasActionVec.back());
    }
    //3D
    virtual void Translate(float fXOffset, float fYOffset, float fZOffset) = 0;
    virtual void Scale(float fXScale, float fYScale, float fZScale) = 0;
    virtual void Scale(float fXScale, float fYScale, float fZScale, float fPivotX, float fPivotY, float fPivotz) = 0;
    virtual void Rotate(float fDegree) = 0;
    virtual void Rotate(float fDegree, float fPivotX, float fPivotY, float fPivotZ,MatrixRouteType type) = 0;
    virtual void Alpha(unsigned char uAlpha) = 0;

    virtual int GetSaveCount(void) const = 0;
    virtual void Save() = 0;
    virtual bool Restore() = 0;
    virtual bool RestoreToCount(int iSaveCount) = 0;

protected:
    std::vector<CUICanvasAction*> m_CanvasActionVec;//动作栈
};
#endif  // __FRAME__CUICANVASINTERFACE_H__