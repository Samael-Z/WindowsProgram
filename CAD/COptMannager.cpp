#include "pch.h"
#include "COptMannager.h"

void COptMannager::AddOpt(IOpt* pOpt)
{
    m_stkUndo.push(pOpt);
}

void COptMannager::Undo()
{
    if (m_stkUndo.empty())
    {
        return;
    }
    auto pOpt = m_stkUndo.top();
    m_stkUndo.pop();

    pOpt->Undo();

    m_stkRedo.push(pOpt);

}

void COptMannager::Redo()
{
    if (m_stkRedo.empty())
    {
        return;
    }
    auto pOpt = m_stkRedo.top();
    m_stkRedo.pop();
    pOpt->Redo();

    m_stkUndo.push(pOpt);
}

