#include "pch.h"
#include "CRectangle.h"
IMPLEMENT_SERIAL(CRectangle,CObject,1)
void CRectangle::OnDraw(CDC* pDC)
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

    pDC->Rectangle(m_ptBegin.x, m_ptBegin.y, m_ptEnd.x, m_ptEnd.y);
    pDC->SelectObject(hOldPen);
    pDC->SelectObject(pOldBrush);
}

bool CRectangle::ISelect(CPoint pt)
{
    CRect rect1(m_ptBegin, m_ptEnd);
    CRect rect2(m_ptEnd, m_ptBegin);
    if (m_ptEnd.x >m_ptBegin.x)
    {
        return rect1.PtInRect(pt);
    }
    else
    {
        return rect2.PtInRect(pt);

    }
   
}

void CRectangle::OnselectDraw(CDC* pDC)
{
    CPen pen;
    pen.CreatePen(m_nPenStyle, 5, m_clrPenColor);
    HGDIOBJ hOldPen = pDC->SelectObject(&pen);
    pDC->Rectangle(m_ptBegin.x, m_ptBegin.y, m_ptEnd.x, m_ptEnd.y);
    pDC->SelectObject(hOldPen);
}

void CRectangle::Serialize(CArchive& ar)
{
    IShape::Serialize(ar);
}

IShape* CRectangleFactory::CreateShape()
{
    return new CRectangle;
}
