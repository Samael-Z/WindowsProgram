#include "pch.h"
#include "CRemoveAction.h"

CRemoveAction::CRemoveAction()
{
}

CRemoveAction::CRemoveAction(IShape* pShape, CList<IShape*>* plistShapes):
    m_pShape(pShape),m_plistShapes(plistShapes)
{

}

CRemoveAction::~CRemoveAction()
{
}

void CRemoveAction::undo()
{
    m_plistShapes->AddTail(m_pShape);
}

void CRemoveAction::redo()
{
    m_plistShapes->RemoveAt(m_plistShapes->Find(m_pShape));
}
