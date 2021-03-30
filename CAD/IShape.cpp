#include "pch.h"
#include "IShape.h"

IShape::IShape()
{

}

void IShape::SetBeginPt(CPoint point)
{
    m_ptBegin = point;
}

CPoint IShape::GetBeginPt() const
{
    return m_ptBegin;
}

void IShape::SetEndPt(CPoint point)
{
    m_ptEnd = point;
}

CPoint IShape::GetEndPt() const
{
    return m_ptEnd;
}




void IShape::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {	// storing code
        
        ar << m_ptBegin;
        ar << m_ptEnd;
        //画笔的变量
        ar << m_nPenStyle;
        ar << m_nPenWith;
        ar << m_clrPenColor;
        //画刷的变量
        ar << m_nBrushStyle;
        ar << m_nShadowStyle;
        ar << m_varBrushColor;
    }
    else
    {	
        ar >> m_ptBegin;
        ar >> m_ptEnd;
        //画笔的变量
        ar >> m_nPenStyle;
        ar >> m_nPenWith;
        ar >> m_clrPenColor;

        //画刷的变量
        ar >> m_nBrushStyle;
        ar >> m_nShadowStyle;
        ar >> m_varBrushColor;
    }
}
