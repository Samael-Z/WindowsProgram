
// SnakeAIDlg.h: 头文件
//

#pragma once

#include "Bfs.h"
#include "Snake.h"
// CSnakeAIDlg 对话框
class CSnakeAIDlg : public CDialogEx
{
// 构造
public:
	CSnakeAIDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SNAKEAI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	void CreateFood();			//产生一个食物.
	void CreateGoodFood();		//产生一个不会与蛇身重复的食物.
	void ShowSnake(CDC& dc);	//显示蛇身.
	void SetDire();				//设置方向.
	bool IsSafe();				//判断当前走位是否安全..
	DIRE GetSafeDire();			//得到当前状态的安全方向.

private:

	Snake m_snake;	//蛇
	RECT m_rc;		//地图
	RECT m_food;	//食物
	POINT m_po;		//蛇节点块的大小
	SPEED m_sp;		//蛇的速度
	DIRE m_dr;		//方向类型	
	UINT_PTR m_tim;	//定时器id暂停判断
	UINT_PTR  m_Music;
	bool m_ispc;	//标记现在是否开启智能模式
	Bfs m_bfs;		//智能寻路类.
	CRITICAL_SECTION m_cs;
public:
	bool m_foodok;//食物是否产生.(另开一个线程单独产生食物,防止卡顿)
	afx_msg void OnBnClickedBegin();
	afx_msg void OnBnClickedRestart();
	afx_msg void OnBnClickedOpeng();
	afx_msg void OnBnClickedCloseg();
	afx_msg void OnBnClickedQuit();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
