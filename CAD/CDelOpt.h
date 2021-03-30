#pragma once
#include "IOpt.h"
class CDelOpt :
    public IOpt
{

public:
    CDelOpt(IShape* pDelShape, int nIdex, CList<IShape*>* pLstShapes);
    void Undo() override;


    void Redo() override;
private:
    IShape* m_pDelShape;
    int m_nIdex;
    CList<IShape*>* m_pLstShapes;

};

