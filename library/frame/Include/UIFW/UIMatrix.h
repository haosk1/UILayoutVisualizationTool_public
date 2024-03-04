/**
 * frame
 *
 * UI矩阵
**/
#ifndef __UIMATRIX_H__
#define __UIMATRIX_H__

#include <math.h>
#include <string.h>

#include "UIPoint.h"
#include "UIRect.h"

typedef enum
{
    X,
    Y,
    Z
} MatrixRouteType;
typedef struct
{
    float v11, v12, v13;
    float v21, v22, v23;
    float v31, v32, v33;
} Matrix33_t;

//33矩阵适用于2D
class CUIMatrix3
{
public:
    
public:
    CUIMatrix3();
    CUIMatrix3(const Matrix33_t &stNativeMatrix);
    CUIMatrix3(const CUIMatrix3 &stOther);
    ~CUIMatrix3();

    bool operator==(const CUIMatrix3 &stMatrix);
    bool operator!=(const CUIMatrix3 &stMatrix);
    CUIMatrix3 &operator=(const CUIMatrix3 &stMatrix);

    bool IsIdentity() const;
    bool IsOnlyTranslation(void) const;
    const Matrix33_t &GetMatrix(void) const;
    void GetMatrix(Matrix33_t &stNativeMatrix) const;
    void SetMatrix(const Matrix33_t &stNativeMatrix);
    void SetMatrix(const CUIMatrix3 &stMatrix);

    void LoadIdentity(void);
    void Translate(float fX, float fY);
    void Rotate(float fAngle);
    void Rotate(float fAngle, float fPivotX, float fPivotY);
    void Scale(float fX, float fY);
    void Scale(float fX, float fY, float fPivotX, float fPivotY);
    void Multiply(const CUIMatrix3 &stMatrix); // stMatrix * this
    bool Invert(CUIMatrix3 &stInverMatrix) const;

    void MapPoints(const UIPoint astSrc[], UIPoint astDst[], int iCount) const;
    void MapPoints(UIPoint astPoint[], int iCount) const;
    void MapXY(int sSrcX, int sSrcY, int &sDstX, int &sDstY) const;
    void MapRect(const UIRect &stSrc, UIRect &stDst) const; //求变换后图形的外接矩形
    void MapRect(UIRect &stSrc) const;

private:
    inline void AdjointMatrix(CUIMatrix3 &stAdjointMatrix) const;
    inline float Determinant(const CUIMatrix3 &stAdjointMatrix) const;
    inline void MultiScalar(float fScalar);

private:
    Matrix33_t m_stNativeMatrix;
};

//44矩阵适用于3D
class CUIMatrix4
{
  public:
    float data[16];

    enum Entry
    {
        kScaleX = 0,
        kSkewY = 1,
        kPerspective0 = 3,
        kSkewX = 4,
        kScaleY = 5,
        kPerspective1 = 7,
        kScaleZ = 10,
        kTranslateX = 12,
        kTranslateY = 13,
        kTranslateZ = 14,
        kPerspective2 = 15
    };

    enum Type
    {
        kTypeIdentity = 0x0,
        kTypeTranslate = 0x1,
        kTypeScale = 0x2,
        kTypeAffine = 0x4,
        kTypePerspective = 0x8,
        kTypeRectToRect = 0x10,
        kTypeUnknown = 0x20,
    };

    static const int sGeometryMask = 0xf;

    CUIMatrix4()
    {
        loadIdentity();
    }

    CUIMatrix4 &operator=(const CUIMatrix4 &stMatrix)
    {
        if (this != &stMatrix)
        {
            memcpy(data, stMatrix.data, sizeof(data));
            mType = stMatrix.mType;
        }
        return *this;
    }

    float operator[](int index) const
    {
        return data[index];
    }

    float &operator[](int index)
    {
        mType = kTypeUnknown;
        return data[index];
    }

    friend bool operator==(const CUIMatrix4 &a, const CUIMatrix4 &b)
    {
        return (0 == memcmp(&a.data[0], &b.data[0], 16 * sizeof(float)));
    }

    friend bool operator!=(const CUIMatrix4 &a, const CUIMatrix4 &b)
    {
        return !(a == b);
    }

    static const CUIMatrix4 &identity();

    void loadIdentity();
    void load(const float *v);
    void load(CUIMatrix4 &v);

    int getType();

    void loadInverse(const CUIMatrix4 &v);
    void loadTranslate(float x, float y, float z);
    void loadScale(float sx, float sy, float sz);
    void loadSkew(float sx, float sy);
    void loadRotate(float angle, MatrixRouteType type);
    void loadRotate(float angle, float x, float y, float z);
    void loadMultiply(const CUIMatrix4 &u, const CUIMatrix4 &v);
    void loadOrtho(float fleft, float fright, float fbottom, float ftop, float fnear, float ffar);
    void loadFrustum(float fleft, float fright, float fbottom, float ftop, float fnear, float ffar);
    void loadPrspective(float fleft, float fright, float fbottom, float ftop, float fnear, float ffar, float fovy = 1.0472f);

    void multiply(const CUIMatrix4 &v);
    void multiply(float v);
    void translate(float x, float y, float z = 0);
    void scale(float sx, float sy, float sz);
    void skew(float sx, float sy);
    void rotate(float angle, float x, float y, float z);
    void rotate(float angle, MatrixRouteType type);

    bool isSimple();
    bool isPureTranslate();
    bool isIdentity();
    bool isPerspective();
    bool rectToRect();
    bool positiveScale();
    bool changesBounds();

    void copyTo(float *v) const;

    float mapZ(const UIPointWithZ &orig) const;
    void mapPoint3d(UIPointWithZ &vec) const;
    void mapPoint(float &x, float &y); // 2d only

    float getTranslateX() const;
    float getTranslateY() const;

  private:
    uint mType;

    inline float getMat4V(int i, int j) const
    {
        return data[i * 4 + j];
    }

    inline void setMat4V(int i, int j, float v)
    {
        data[i * 4 + j] = v;
    }

    int getGeometryType();
};

typedef CUIMatrix4 UImat4;
#endif  // __UIMATRIX_H__