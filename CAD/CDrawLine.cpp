#include "pch.h"
#include "CDrawLine.h"
IMPLEMENT_SERIAL(CDrawLine, CObject, 1);
CDrawLine::CDrawLine()
{

}

void CDrawLine::OnDraw(CDC* pDC)
{

    CPen ShapePen;
    ShapePen.CreatePen(GetPenStyle(), GetPenWith(), GetClrPenColor());
    pDC->SelectObject(&ShapePen);

    pDC->MoveTo(GetBeginPt());
    pDC->LineTo(GetEndPt());
}

BOOL CDrawLine::IsSelect(CPoint pt)
{

    const int nCount = 4;
    POINT aryPoint[nCount];
    const int nOffset = 10;
    aryPoint[0].x = GetBeginPt().x + nOffset;
    aryPoint[0].y = GetBeginPt().y - nOffset;

    aryPoint[1].x = GetEndPt().x + nOffset;
    aryPoint[1].y = GetEndPt().y - nOffset;

    aryPoint[2].x = GetEndPt().x - nOffset;
    aryPoint[2].y = GetEndPt().y + nOffset;

    aryPoint[3].x = GetBeginPt().x - nOffset;
    aryPoint[3].y = GetBeginPt().y + nOffset;
    CRgn rgn;
    rgn.CreatePolygonRgn(aryPoint, nCount, WINDING);
    return rgn.PtInRegion(pt);
}

void CDrawLine::OnSelectDraw(CDC* pDC)
{
    CPen ShapePen;
    ShapePen.CreatePen(GetPenStyle(), GetPenWith()+5, GetClrPenColor());
    pDC->SelectObject(&ShapePen);

    pDC->MoveTo(GetBeginPt());
    pDC->LineTo(GetEndPt());
}

IShape* CDrawLineFactory::CreateIShape()
{
    return new CDrawLine;
}


void CDrawLine::Serialize(CArchive& ar)
{
    IShape::Serialize(ar);
}
