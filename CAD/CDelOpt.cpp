#include "pch.h"
#include "CDelOpt.h"

CDelOpt::CDelOpt(IShape* pDelShape, int nIdex, CList<IShape*>* pLstShapes):
    m_pDelShape(pDelShape),m_nIdex(nIdex),m_pLstShapes(pLstShapes)
{

}

void CDelOpt::Undo()
{

    //把从链表删除的图形再放回链表
   /* auto pos = m_pLstShapes->FindIndex(m_nIdex);
    m_pLstShapes->InsertBefore(pos, m_pDelShape);*/


    m_pLstShapes->AddTail(m_pDelShape);
}

void CDelOpt::Redo()
{
    if (m_pLstShapes->GetSize())
    {
        m_pLstShapes->RemoveAt(m_pLstShapes->Find(m_pDelShape));
    }
  
}
