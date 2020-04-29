#pragma once
#include "IAction.h"
#include <stack>
using namespace std;
class CActionManager
{
public:
	void undo();
	void redo();
	void InsertAction(IAction* pAction);
private:
	//撤销和重做数据结构
	stack<IAction*>  m_stkToUndo;  //等待被撤销
	stack<IAction*> m_stkToRedo;  //等待被重做

};

