#include "pch.h"
#include "CDrawEllipse.h"
IMPLEMENT_SERIAL(CDrawEllipse, CObject, 1)
CDrawEllipse::CDrawEllipse()
{

}

void CDrawEllipse::OnDraw(CDC* pDC)
{
    //创建画笔的mfc类只有这个一个创建画笔的函数
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
    pDC->Ellipse(&rc);
}

BOOL CDrawEllipse::IsSelect(CPoint pt)
{
    CRgn  rgnRect;
    rgnRect.CreateEllipticRgn(GetBeginPt().x, GetBeginPt().y, GetEndPt().x, GetEndPt().y);
    return rgnRect.PtInRegion(pt);
    
}

void CDrawEllipse::OnSelectDraw(CDC* pDC)
{
    CPen pen;
    pen.CreatePen(GetPenStyle(), GetPenWith() + 5, GetClrPenColor());
    CPen* cOldpen = pDC->SelectObject(&pen);

    CRect rc(GetBeginPt(), GetEndPt());
    pDC->Ellipse(&rc);
    pDC->SelectObject(cOldpen);
}

IShape* CDrawEllipseFactory::CreateIShape()
{
    
    return new CDrawEllipse;
}


void CDrawEllipse::Serialize(CArchive& ar)
{
    IShape::Serialize(ar);
}
