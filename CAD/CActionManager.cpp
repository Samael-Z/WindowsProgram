#include "pch.h"
#include "CActionManager.h"

void CActionManager::undo()
{
	if (m_stkToUndo.empty())
		return;
	IAction* pAction = m_stkToUndo.top();
	m_stkToUndo.pop();
	pAction->undo();
	m_stkToRedo.push(pAction);
}

void CActionManager::redo()
{
	if (m_stkToRedo.empty())
		return;
	IAction* pAction = m_stkToRedo.top();
	m_stkToRedo.pop();
	pAction->redo();
	m_stkToUndo.push(pAction);
}

void CActionManager::InsertAction(IAction* pAction)
{
	m_stkToUndo.push(pAction);
}
