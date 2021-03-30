#pragma once
#include "IShape.h"
class IOpt
{
public:
    virtual void Undo() = 0;
    virtual void Redo() = 0;
};

