#include "pch.h"
#include "CAddOpt.h"

CAddOpt::CAddOpt(IShape* pAddShape, CList<IShape*>* pAddLstShapes):
   m_pAddShape(pAddShape) , m_pLstAddShapes(pAddLstShapes)
{

}

void CAddOpt::Undo()
{
    m_pLstAddShapes->RemoveAt(m_pLstAddShapes->Find(m_pAddShape));
}

void CAddOpt::Redo()
{
   
    m_pLstAddShapes->AddTail(m_pAddShape);
}
