#include "pch.h"
#include "CLine.h"
IMPLEMENT_SERIAL(CLine,CObject,1)
void CLine::OnDraw(CDC* pDC)
{
    CPen pen;
    pen.CreatePen(m_nPenStyle, m_nPenWidth, m_clrPenColor);
    HGDIOBJ hOldPen = pDC->SelectObject(&pen);
    pDC->MoveTo(m_ptBegin);
    pDC->LineTo(m_ptEnd);
    pDC->SelectObject(hOldPen);
}

void CLine::OnselectDraw(CDC* pDC)
{
    CPen pen;
    pen.CreatePen(m_nPenStyle, 5, m_clrPenColor);
    HGDIOBJ hOldPen = pDC->SelectObject(&pen);
    pDC->MoveTo(m_ptBegin);
    pDC->LineTo(m_ptEnd);
    pDC->SelectObject(hOldPen);

}

bool CLine::ISelect(CPoint pt)
{
    const int nCount = 4;
    POINT aryPoint[nCount];
    const int nOffset = 10;
    aryPoint[0].x = m_ptBegin.x + nOffset;
    aryPoint[0].y = m_ptBegin.y - nOffset;

    aryPoint[1].x = m_ptEnd.x + nOffset;
    aryPoint[1].y = m_ptEnd.y - nOffset;

    aryPoint[2].x = m_ptEnd.x - nOffset;
    aryPoint[2].y = m_ptEnd.y + nOffset;

    aryPoint[3].x = m_ptBegin.x - nOffset;
    aryPoint[3].y = m_ptBegin.y + nOffset;
    CRgn rgn;
    rgn.CreatePolygonRgn(aryPoint, nCount, ALTERNATE);
    return rgn.PtInRegion(pt);
}

void CLine::Serialize(CArchive& ar)
{
    IShape::Serialize(ar);
}

IShape* CLineFactory::CreateShape()
{
    return new CLine;
}
