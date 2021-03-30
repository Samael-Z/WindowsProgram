#pragma once
#include "IShape.h"
class CDrawRectangle :
    public IShape
{
    DECLARE_SERIAL(CDrawRectangle)
public:
    CDrawRectangle();
   virtual void OnDraw(CDC* pDC)  override;
   virtual BOOL IsSelect(CPoint pt)  override;
   virtual void OnSelectDraw(CDC* pDC) override;
   virtual void Serialize(CArchive& ar);
};

class CDrawRectangleFactory: public IShapeFactory
{
public:

    virtual IShape* CreateIShape() override;
};

