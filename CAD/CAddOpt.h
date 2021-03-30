#pragma once
#include "IOpt.h"
class CAddOpt :
    public IOpt
{
public:
    CAddOpt(IShape* pAddShape, CList<IShape*>* pLstAddShapes);
    void Undo() override;

    void Redo() override;
private:
    IShape* m_pAddShape;
    CList<IShape*>* m_pLstAddShapes;

};

