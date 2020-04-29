#pragma once
#include <afxwin.h>
#include "IShape.h"
#include "IAction.h"
class CMoveAction :public IAction
{
public:
    CMoveAction();
    CMoveAction(IShape * pShape,CPoint ptnew,CPoint ptold );
    ~CMoveAction();
    virtual void redo() override;
    virtual void undo() override;
private:
    CPoint m_ptOld;   //移动后的坐标
    CPoint m_ptNew;   //移动前的坐标
    IShape* m_pMoveShape; //移动的图形
};

