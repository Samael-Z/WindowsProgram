#pragma once
#include "IShape.h"
class CLine :
    public IShape
{
    DECLARE_SERIAL(CLine)
public:
    CLine();
    virtual ~CLine();
    virtual void OnDraw(CDC* pDC) override;
    virtual void OnselectDraw(CDC* pDC) override;
    virtual bool ISelect(CPoint pt)  override;
    virtual void Serialize(CArchive& ar) override;
};


inline CLine::CLine()
{
}

inline CLine::~CLine()
{
}

class CLineFactory : public IFactory
{
public:
    virtual IShape* CreateShape() override;
};

