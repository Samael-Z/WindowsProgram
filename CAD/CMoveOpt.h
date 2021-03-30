#pragma once
#include "IOpt.h"

class CMoveOpt :
    public IOpt
{
public:
    CMoveOpt(IShape* pShape, CPoint ptBeginOld, CPoint ptEndOld, CPoint ptBeginNew, CPoint ptEndNew);
    virtual void Undo() override;
    virtual void Redo() override;
private:

    IShape* m_pMoveShape;
    CPoint m_ptBenginOld;
    CPoint m_ptEndOld;
    CPoint m_ptBenginNew;
    CPoint m_ptEndNew;
};

