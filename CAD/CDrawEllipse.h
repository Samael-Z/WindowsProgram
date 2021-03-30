#pragma once
#include "IShape.h"
class CDrawEllipse :
    public IShape
{
    DECLARE_SERIAL(CDrawEllipse)
public:
    CDrawEllipse();
    virtual void OnDraw(CDC* pDC)  override;
  
    virtual BOOL IsSelect(CPoint pt) override;
    virtual void OnSelectDraw(CDC* pDC) override;
    virtual void Serialize(CArchive& ar);
};

class CDrawEllipseFactory:public IShapeFactory
{
public:

    virtual IShape* CreateIShape() override;
};

