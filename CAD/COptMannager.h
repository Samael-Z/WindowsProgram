#pragma once
#include "IOpt.h"
#include <stack>
using namespace std;
class COptMannager
{
public:
    void AddOpt(IOpt* pOpt);
    void Undo();
    void Redo();
private:
    stack<IOpt*> m_stkUndo;
    stack<IOpt*> m_stkRedo;
};

