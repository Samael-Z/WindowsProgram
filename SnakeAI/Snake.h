#pragma once
//Snake.h
//贪吃蛇的数据结构以及方法定义类.

#include <Windows.h>
#include <ctime>
#include <list>
using std::list;

//相关定义
//--方向宏
#define DR_UP	 0
#define DR_DOWN	 1
#define DR_LEFT	 2
#define DR_RIGHT 3
typedef INT DIRE;	//方向类型

//--速度宏
#define SP_SLOW		1
#define SP_NORMAL	2
#define SP_FAST		3
#define SP_HIGN		4
typedef INT SPEED;	//速度类型


//蛇身节点结构体,矩形的变量表示的是该节点所在的举行区域.
struct SnakeNode
{
	RECT rc;
	DIRE curDire;	//标记该节点当前的方向
};

class Snake
{
public:
	Snake();
	~Snake();
	Snake(RECT rc, POINT po, SPEED sp);//构造函数:(地图区域,节点块的大小,移动速度)
	bool SMove(DIRE dr, RECT food);//蛇身移动一步:(方向,食物的位置)返回值为是否吃到食物.
	bool GameOver();//判断游戏是否结束:

	list<SnakeNode> m_snake;//整条蛇.
	//蛇头为list中的最后一个元素.
	RECT m_rc;		//地图区域
	POINT m_po;		//蛇的节点块大小
	SPEED m_sp;		//蛇的移动速度
	CRITICAL_SECTION m_cs;
};

