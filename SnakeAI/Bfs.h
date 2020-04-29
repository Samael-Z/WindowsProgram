#pragma once
//Bfs.h
//贪吃蛇基本寻路算法.

#ifndef BFS_H_H
#define BFS_H_H

#include <queue>
using std::queue;

struct XY
{
	int x;
	int y;
};

class Bfs
{
public:
	Bfs();									//初始化地图大小								
	~Bfs();
	void InitBfs(bool** chess, XY size);	//初始化地图区域的值.
	bool CalcBfs(XY st, XY en);				//计算Bfs路径.
	void EetBfs(XY st, XY en);				//得到Bfs路径.
	void CalcQue(XY en);					//计算队列
	queue<XY> m_que;
private:
	bool** m_chess;		//用矩阵表示的图.
	bool** m_visit;		//节点是否被访问过.
	XY** m_parent;		//每个访问过的节点的父节点.
	XY m_size;			//图的大小.
	CRITICAL_SECTION m_cs;
};

#endif //BFS_H_H
