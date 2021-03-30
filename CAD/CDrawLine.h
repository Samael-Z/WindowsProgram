#pragma once
#include "IShape.h"
class CDrawLine :
    public IShape
{
    DECLARE_SERIAL(CDrawLine)
public:
    CDrawLine();
    virtual void OnDraw(CDC* pDC)  override;
    virtual BOOL IsSelect(CPoint pt) override;
    virtual void OnSelectDraw(CDC* pDC) override;

    virtual void Serialize(CArchive& ar);
};

class CDrawLineFactory:public IShapeFactory
{
public:

    virtual IShape* CreateIShape() override;
};
