#include "pch.h"
#include "CMoveOpt.h"

CMoveOpt::CMoveOpt(IShape* pShape, CPoint ptBeginOld, CPoint ptEndOld, CPoint ptBeginNew, CPoint ptEndNew):
    m_pMoveShape(pShape),
    m_ptBenginOld(ptBeginOld),
    m_ptEndOld(ptEndOld),
    m_ptBenginNew(ptBeginNew),
    m_ptEndNew(ptEndNew)
{

}

void CMoveOpt::Undo()
{
    m_pMoveShape->SetBeginPt(m_ptBenginOld);
    m_pMoveShape->SetEndPt(m_ptEndOld);
}

void CMoveOpt::Redo()
{
    m_pMoveShape->SetBeginPt(m_ptBenginNew);
    m_pMoveShape->SetEndPt(m_ptEndNew);

}
