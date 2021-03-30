#pragma once
#include "IOpt.h"

class CRotateOpt :
    public IOpt
{

public:
    CRotateOpt(IShape* pShape, CPoint ptBeginOld, CPoint ptEndOld, CPoint ptBeginNew, CPoint ptEndNew);
    virtual void Undo() override;


    virtual void Redo() override;

private:
    IShape* m_pRotateShape;
    CPoint m_ptBenginOld;
    CPoint m_ptEndOld;
    CPoint m_ptBenginNew;
    CPoint m_ptEndNew;

};

