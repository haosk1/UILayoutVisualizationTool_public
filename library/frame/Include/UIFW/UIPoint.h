/**
 * frame
 *
 * UI点定义
**/
#ifndef __UIPOINT_H__
#define __UIPOINT_H__

class UIPoint
{
public:
    UIPoint(){};
    UIPoint(int ix, int iy){
        this->m_iX = ix;
        this->m_iY = iy;
    }
    int m_iX = 0, m_iY = 0;
    
};

class UIPointF
{
public:
    UIPointF(){};
    UIPointF(float x, float y){
        this->m_X = x;
        this->m_Y = y;
    }
    float m_X = 0, m_Y = 0;
    
};

class UIPointWithZ
{
public:
    UIPointWithZ(){};
    UIPointWithZ(int ix, int iy, int iZ){
        this->m_iX = ix;
        this->m_iY = iy;
        this->m_iZ = iZ;
    }
    int m_iX = 0, m_iY = 0, m_iZ = 0;
    
};

class UIPointFWithZ
{
public:
    UIPointFWithZ(){};
    UIPointFWithZ(float fx, float fy, float fZ){
        this->m_fX = fx;
        this->m_fY = fy;
        this->m_fZ = fZ;
    }
    float m_fX = 0, m_fY = 0, m_fZ = 0;
    
};
#endif  // __UIPOINT_H__