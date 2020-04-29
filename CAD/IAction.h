#pragma once
#include "IShape.h"
class IAction
{
public:
    IAction();
    virtual ~IAction();
public:
    virtual void undo() = 0;
    virtual void redo() = 0;
    
};

