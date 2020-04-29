#pragma once
#include "IShape.h"
class CEllipse :
    public IShape
{
    DECLARE_SERIAL(CEllipse)
public:
    CEllipse();
    virtual ~CEllipse();
    virtual void OnDraw(CDC* pDC) override;
    virtual bool ISelect(CPoint pt)  override;
    virtual void OnselectDraw(CDC* pDC) override;
    virtual void Serialize(CArchive& ar) override;
};

class CEllipseFactory : public IFactory
{
public:
    virtual IShape* CreateShape() override;
};
