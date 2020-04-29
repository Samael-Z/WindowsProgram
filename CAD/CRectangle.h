#pragma once
#include "IShape.h"
class CRectangle :
    public IShape
{
    DECLARE_SERIAL(CRectangle)
public:
    CRectangle();
    virtual ~CRectangle();
    virtual void OnDraw(CDC* pDC) override;
    virtual bool ISelect(CPoint pt)  override;
    virtual void OnselectDraw(CDC* pDC) override;
    virtual void Serialize(CArchive& ar) override;
};
inline CRectangle::CRectangle()
{
}
inline CRectangle::~CRectangle()
{
}
class CRectangleFactory : public IFactory
{
public:
    virtual IShape* CreateShape() override;
};

