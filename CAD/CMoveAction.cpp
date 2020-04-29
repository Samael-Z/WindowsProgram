#include "pch.h"
#include "CMoveAction.h"

CMoveAction::CMoveAction() 
{
}

CMoveAction::CMoveAction(IShape* pShape, CPoint ptnew, CPoint ptold)
    :m_pMoveShape(pShape),m_ptOld(ptold),m_ptNew(ptnew)
{

}

CMoveAction::~CMoveAction()
{
}

void CMoveAction::redo()
{
    m_pMoveShape->MoveShape(m_ptNew.x - m_ptOld.x, m_ptNew.y - m_ptOld.y);
}

void CMoveAction::undo()
{
    m_pMoveShape->MoveShape(m_ptOld.x - m_ptNew.x, m_ptOld.y - m_ptNew.y);
}
