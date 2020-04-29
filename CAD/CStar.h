#pragma once
#include "IShape.h"

class CStar :
    public IShape
{
    DECLARE_SERIAL(CStar)
public:
    virtual void OnDraw(CDC* pDC) override;
    virtual bool ISelect(CPoint pt) override;
    virtual void OnselectDraw(CDC* pDC) override;
    virtual void Serialize(CArchive& ar) override;
};
class CStarFactory :public IFactory
{
public:
    virtual IShape* CreateShape() override;
};



