#pragma once
#include "IAction.h"

class CInsertAction :
    public IAction
{
public:
    CInsertAction();
    CInsertAction(IShape* pShape, CList<IShape*>* plistShapes);
    virtual ~CInsertAction();
    virtual void undo() override;
    virtual void redo() override;
private:
    IShape* m_pShape;
    CList<IShape*>* m_plistShapes;
};

