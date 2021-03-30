#include "pch.h"
#include "CRotateOpt.h"

CRotateOpt::CRotateOpt(IShape* pShape, CPoint ptBeginOld, CPoint ptEndOld, CPoint ptBeginNew, CPoint ptEndNew) :
    m_pRotateShape(pShape),
    m_ptBenginOld(ptBeginOld),
    m_ptEndOld(ptEndOld),
    m_ptBenginNew(ptBeginNew),
    m_ptEndNew(ptEndNew)
{

}

void CRotateOpt::Undo()
{
    m_pRotateShape->SetBeginPt(m_ptBenginOld);
    m_pRotateShape->SetEndPt(m_ptEndOld);
}

void CRotateOpt::Redo()
{
    m_pRotateShape->SetBeginPt(m_ptBenginNew);
    m_pRotateShape->SetEndPt(m_ptEndNew);
}
