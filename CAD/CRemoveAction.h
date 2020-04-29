#pragma once
#include "IAction.h"
class CRemoveAction :
    public IAction
{
public:
    CRemoveAction();
    CRemoveAction(IShape* pShape, CList<IShape*>* plistShapes);
    virtual ~CRemoveAction();
    virtual void undo() override;
    virtual void redo() override;
private:
    IShape* m_pShape;
    CList<IShape*>* m_plistShapes;
};

