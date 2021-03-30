#include "pch.h"
#include "CDrawRectangle.h"
IMPLEMENT_SERIAL(CDrawRectangle, CObject, 1);
CDrawRectangle::CDrawRectangle()
{

}

void CDrawRectangle::OnDraw(CDC* pDC)
{

    CPen ShapePen;
    ShapePen.CreatePen(GetPenStyle(), GetPenWith(), GetClrPenColor());
    pDC->SelectObject(&ShapePen);


    CBrush  ShapeBrush;
    LOGBRUSH   lbBrushStyle;
    lbBrushStyle.lbColor = GetClrBrushColor();
    lbBrushStyle.lbHatch = GetShadowStyle();
    lbBrushStyle.lbStyle = GetBrushStyle();
    ShapeBrush.CreateBrushIndirect(&lbBrushStyle);
    pDC->SelectObject(&ShapeBrush);

    CRect rc(GetBeginPt(), GetEndPt());
    pDC->Rectangle(&rc);
}

BOOL CDrawRectangle::IsSelect(CPoint pt)
{

    CRgn  rgnRect;
    rgnRect.CreateRectRgn(GetBeginPt().x, GetBeginPt().y, GetEndPt().x, GetEndPt().y);
    return rgnRect.PtInRegion(pt);

}

void CDrawRectangle::OnSelectDraw(CDC* pDC)
{

    CPen pen;
    pen.CreatePen(GetPenStyle(), GetPenWith() + 5, GetClrPenColor());
    CPen* cOldpen = pDC->SelectObject(&pen);

    CRect rc(GetBeginPt(), GetEndPt());
    pDC->Rectangle(&rc);
    pDC->SelectObject(cOldpen);
}



IShape* CDrawRectangleFactory::CreateIShape()
{
    return new CDrawRectangle;
}


void CDrawRectangle::Serialize(CArchive& ar)
{
    IShape::Serialize(ar);
}
