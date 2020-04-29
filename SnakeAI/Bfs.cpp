#include "pch.h"
#include "Bfs.h"

int dir[4][2] = { {-1,0},{1,0},{0,-1},{0,1} };
Bfs::Bfs( )
	
{
	//临界区
	InitializeCriticalSection(&m_cs);
	EnterCriticalSection(&m_cs);
	m_size.x = 15;
	m_size.y = 25;
	m_chess = new bool* [m_size.x];
	m_visit = new bool* [m_size.x];
	m_parent = new XY * [m_size.x];

	for (int i = 0; i < m_size.x; i++)
	{
		m_chess[i] = new bool[m_size.y];
		m_visit[i] = new bool[m_size.y];
		m_parent[i] = new XY[m_size.y];
	}
	
}
Bfs::~Bfs()
{

	LeaveCriticalSection(&m_cs);
	DeleteCriticalSection(&m_cs);
	for (int i = 0; i < m_size.x; i++)
	{
		delete[] m_chess[i];
		delete[] m_visit[i];
		delete[] m_parent[i];
	}

	delete[] m_chess;
	delete[] m_visit;
	delete[] m_parent;
	m_chess = nullptr;
	m_visit = nullptr;
	m_parent = nullptr;
				
}
void Bfs::InitBfs(bool** chess, XY size)
{

	
	for (int i = 0; i < m_size.x; i++)
	{
		for (int j = 0; j < m_size.y; j++)
		{
			m_chess[i][j] = *((bool*)chess + m_size.y * i + j);
			m_visit[i][j] = false;
			m_parent[i][j].x = -1;
			m_parent[i][j].y = -1;
		}
	}
	while (!m_que.empty())
		m_que.pop();
}

bool Bfs::CalcBfs(XY st, XY en)
{
	//声明一个队列
	queue<XY> temque;
	//访问过的点
	m_visit[st.x][st.y] = true;
	temque.push(st);
	XY head, next;
	int quesize = 0;
	//如果队列不为空，遍历队列
	while (!temque.empty())
	{
		quesize = temque.size();
		while (quesize--)
		{
			head = temque.front();
		
			temque.pop();
			if (head.x == en.x && head.y == en.y)
			{
		
				return true;//已经达到目的了.
			}
			for (int i = 0; i < 4; i++)
			{
				next.x = head.x + dir[i][0];
				next.y = head.y + dir[i][1];
				if (next.x < 0 || (next.x > (m_size.x - 1)) ||
					next.y < 0 || (next.y > (m_size.y - 1)) ||
					m_chess[next.x][next.y])
					continue;
				if (!m_visit[next.x][next.y])
				{
					m_visit[next.x][next.y] = TRUE;
					temque.push(next);

					m_parent[next.x][next.y].x = head.x;
					m_parent[next.x][next.y].y = head.y;

					
				}
			}
		}
	}
	return false;
}

void Bfs::CalcQue(XY en)
{
	
	if (en.x != -1 && en.y != -1)
	{
		CalcQue(m_parent[en.x][en.y]);
		m_que.push(en);

	}
}

//st----蛇头坐标
//en---食物坐标
void Bfs::EetBfs(XY st, XY en)
{
	CalcBfs(st, en);

	CalcQue(en);


	m_que.pop();//弹出没用的起始点..

	
}
