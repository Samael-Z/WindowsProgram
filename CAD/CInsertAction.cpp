#include "pch.h"
#include "CInsertAction.h"

CInsertAction::CInsertAction()
{
}

CInsertAction::CInsertAction(IShape* pShape, CList<IShape*>* plistShapes):
    m_pShape(pShape),m_plistShapes(plistShapes)
{
}

CInsertAction::~CInsertAction()
{
}

void CInsertAction::undo()
{
    m_plistShapes->RemoveAt(m_plistShapes->Find(m_pShape));
}

void CInsertAction::redo()
{
    m_plistShapes->AddTail(m_pShape);
}
