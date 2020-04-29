#include "pch.h"
#include "CStar.h"
IMPLEMENT_SERIAL(CStar,CObject,1)
void CStar::OnDraw(CDC* pDC)
{
    CPen pen;
    pen.CreatePen(m_nPenStyle, m_nPenWidth, m_clrPenColor);
    HGDIOBJ hOldPen = pDC->SelectObject(&pen);

    LOGBRUSH logBrush;
    logBrush.lbStyle = m_nBrushStyle;
    logBrush.lbColor = m_clrBrushColor;
    logBrush.lbHatch = m_nShadowStyle;
    CBrush brush;
    brush.CreateBrushIndirect(&logBrush);
    HGDIOBJ pOldBrush = pDC->SelectObject(&brush);
   

    CPoint pts[5];
    pDC->SetPolyFillMode(WINDING);
    for (int i = 0; i < 5; i++)
    {
        pts[i].x = (long)(m_ptEnd.x - (m_ptEnd.x-m_ptBegin.x) * cos((i * 144 - 18) * 3.14 / 180));
        pts[i].y = (long)(m_ptEnd.y + (m_ptEnd.x - m_ptBegin.x) * sin((i * 144 - 18) * 3.14 / 180));
    }


    //根据五个定点绘制多边形（五角星的外轮廓）
    CPoint v[5] = { pts[0], pts[1], pts[2], pts[3], pts[4] };

    pDC->Polygon(v, 5);

    pDC->SelectObject(hOldPen);
    pDC->SelectObject(pOldBrush);

}

bool CStar::ISelect(CPoint pt)
{
    CPoint pts[5];
    for (int i = 0; i < 5; i++)
    {
        pts[i].x = (long)(m_ptEnd.x - (m_ptEnd.x - m_ptBegin.x) * cos((i * 144 - 18) * 3.14 / 180));
        pts[i].y = (long)(m_ptEnd.y + (m_ptEnd.x - m_ptBegin.x) * sin((i * 144 - 18) * 3.14 / 180));
    }


    //根据五个定点绘制多边形（五角星的外轮廓）
    CPoint v[5] = { pts[0], pts[1], pts[2], pts[3], pts[4] };


    CRgn rgn;
    rgn.CreatePolygonRgn(pts, 5, ALTERNATE);
    return rgn.PtInRegion(pt);
}

void CStar::OnselectDraw(CDC* pDC)
{

    CPen pen;
    pen.CreatePen(m_nPenStyle, 5, m_clrPenColor);
    HGDIOBJ hOldPen = pDC->SelectObject(&pen);
    CPoint pts[5];
    for (int i = 0; i < 5; i++)
    {
        pts[i].x = (long)(m_ptEnd.x - (m_ptEnd.x - m_ptBegin.x) * cos((i * 144 - 18) * 3.14 / 180));
        pts[i].y = (long)(m_ptEnd.y + (m_ptEnd.x - m_ptBegin.x) * sin((i * 144 - 18) * 3.14 / 180));
    }


    //根据五个定点绘制多边形（五角星的外轮廓）
    CPoint v[5] = { pts[0], pts[1], pts[2], pts[3], pts[4] };
    pDC->SelectObject(hOldPen);
}

void CStar::Serialize(CArchive& ar)
{
    IShape::Serialize(ar);
}

IShape* CStarFactory::CreateShape()
{
    return new CStar;
}
