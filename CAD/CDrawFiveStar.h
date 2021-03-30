#pragma once
#include "IShape.h"
class CDrawFiveStar :
    public IShape
{
    DECLARE_SERIAL(CDrawFiveStar)
public:
    CDrawFiveStar();
    virtual void OnDraw(CDC* pDC)  override;

    virtual BOOL IsSelect(CPoint pt) override;

    virtual void OnSelectDraw(CDC* pDC) override;
    void FiveStar(CDC* pDC);
    virtual void Serialize(CArchive& ar);
};

class  CDrawFiveStarFactory:public IShapeFactory
{
public:
	
    virtual IShape* CreateIShape() override;
};


//#define RECTANGLE 1
//#define PENTAGRAM 2
#define PI 3.14159265358979323846
#define ANGLE_TO_RADIAN(angle) (angle * PI / 180)
#define RADIAN_TO_ANGLE(radian) (radian * 180 / PI)

