#include "pch.h"
#include "IShape.h"
#include "Rotate.h"

IShape::IShape()
{
}

IShape::~IShape()
{
}

void IShape::MoveShape(int offsetX, int offsetY)
{
    m_ptBegin += SIZE{ offsetX, offsetY };
    m_ptEnd += SIZE{ offsetX, offsetY };
}

void IShape::RotateShape()
{
    rotate(m_ptBegin, m_ptEnd, 90.0f);
}

void IShape::Serialize(CArchive& ar)
{
    CObject::Serialize(ar);
    if (ar.IsStoring())
    {
        ar << m_ptEnd
            << m_clrBrushColor
            << m_clrPenColor
            << m_nPenStyle
            << m_nPenWidth
            << m_nShadowStyle
            << m_ptBegin
            << m_nBrushStyle;
    }
    else
    {
        ar  >> m_ptEnd
            >> m_clrBrushColor
            >> m_clrPenColor
            >> m_nPenStyle
            >> m_nPenWidth
            >> m_nShadowStyle
            >> m_ptBegin
            >> m_nBrushStyle;
    }
}

