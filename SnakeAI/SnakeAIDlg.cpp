
// SnakeAIDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "SnakeAI.h"
#include "SnakeAIDlg.h"
#include "afxdialogex.h"
#include <mmsystem.h>
#pragma comment( lib, "Winmm.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD WINAPI ThreadFood(LPVOID lpParameter);//产生食物线程.
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSnakeAIDlg 对话框



CSnakeAIDlg::CSnakeAIDlg(CWnd* pParent )
	: CDialogEx(IDD_SNAKEAI_DIALOG, pParent)	
{
	//初始化蛇的数据，食物的状态等等
	m_rc.left = 40;
	m_rc.right = 740;
	m_rc.top = 40;
	m_rc.bottom = 540;
	m_po.x = 20;
	m_po.y = 20;
	m_sp = SP_NORMAL;
	m_snake = Snake(m_rc, m_po, m_sp);
	m_dr = DR_RIGHT;
	m_ispc = false;
	m_foodok = true;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
}

void CSnakeAIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}



BEGIN_MESSAGE_MAP(CSnakeAIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BEGIN, &CSnakeAIDlg::OnBnClickedBegin)
	ON_BN_CLICKED(IDC_RESTART, &CSnakeAIDlg::OnBnClickedRestart)
	ON_BN_CLICKED(IDC_OPENG, &CSnakeAIDlg::OnBnClickedOpeng)
	ON_BN_CLICKED(IDC_CLOSEG, &CSnakeAIDlg::OnBnClickedCloseg)
	ON_BN_CLICKED(IDC_QUIT, &CSnakeAIDlg::OnBnClickedQuit)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CSnakeAIDlg 消息处理程序

BOOL CSnakeAIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。
	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InitializeCriticalSection(&m_cs);
	EnterCriticalSection(&m_cs);
	CRect rect(630, 280, 1435, 905);
	::SetWindowPos(GetSafeHwnd(), 0,rect.left,rect.top, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOMOVE);
	CWnd* close = GetDlgItem(IDC_CLOSEG);    ///
	close->EnableWindow(false);				////
	
	CreateGoodFood();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSnakeAIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSnakeAIDlg::OnPaint()
{
	CPaintDC dc(this);
	/*ShowSnake(dc);
	CDialogEx::OnPaint();*/
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文
		
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
		
	}
	else
	{
		ShowSnake(dc);
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSnakeAIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSnakeAIDlg::CreateFood()
{
	//使用一个随机种子，产生食物的位置
	srand((unsigned)time(0));
	int x = rand() % 25;
	int y = rand() % 15;
	m_food.left =40 + m_po.x * x;
	m_food.top = 40 + m_po.y * y;
	m_food.right = m_food.left + m_po.x;
	m_food.bottom = m_food.top + m_po.y;
	
}

void CSnakeAIDlg::CreateGoodFood()
{
	//创建的食物不允许出现在蛇的身体上
	CreateFood();
	list<SnakeNode>::iterator iter = m_snake.m_snake.begin();
	for (unsigned i = 0; i < m_snake.m_snake.size(); i++, iter++)
	{
		
		//检查食物的位置是否和蛇的位置重合
		if (((m_food.left  == iter->rc.left) &&(m_food.top  == iter->rc.top)) || 
			(m_food.right == iter->rc.right && m_food.bottom == iter->rc.bottom))
		{
			CreateFood();
			iter = m_snake.m_snake.begin();
			i = 0;
			//CreateGoodFood();
		}
	}
}

void CSnakeAIDlg::ShowSnake(CDC& dc)
{

	CBrush brushBkColor;
	//选中背景的范围，准备加载图片
	CRect rect(m_rc.left, m_rc.top, m_rc.right, m_rc.bottom);
	
	//GetClientRect(rect);
	//在背景区域画出一块矩形
	brushBkColor.CreateSolidBrush(RGB(0, 0, 50));
	dc.FillRect(rect, &brushBkColor);

	//加载背景图片
	//创建内存DC
	CDC memDC;
	//绑定内存DC
	memDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	//加载背景图片
	bitmap.LoadBitmap(IDB_BKG3);
	//选择背景图片，获取背景图片的尺寸
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	
	BITMAP bm;
	bitmap.GetBitmap(&bm);
	//在屏幕显示背景图片
	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
	//显示背景图片之后，内存DC选择原来的背景
	memDC.SelectObject(pOldBitmap);
	//删除内存DC，避免内存泄漏
	memDC.DeleteDC();

	//加载食物图标
	HICON hIconFood = ::LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_FOOD));

	dc.DrawIcon(m_food.left,m_food.top, hIconFood);

	//加载蛇的图标
	//CDC memDCSnake;
	HICON hIconSnake = ::LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_HEAD));


	
	//显示蛇的身体
	list<SnakeNode>::iterator be = m_snake.m_snake.begin();
	for (unsigned i = 0; i < m_snake.m_snake.size(); i++, be++)
	{
		//dc.FillSolidRect(&be->rc,RGB(0,205,0));
		dc.DrawIcon(be->rc.left,  be->rc.top, hIconSnake);
		
		//dc.BitBlt(be->rc.left, be->rc.top, 20, 20, &memDCSnake,20, 20, SRCCOPY);

	}

}

void CSnakeAIDlg::SetDire()
{
	//初始化一个15x2 5的矩阵，里面全部置为false
	//false表示能走，true表示不能走
	XY size;
	size.x = 15;
	size.y = 25;
	bool maze[15][25];
	memset(maze, 0, sizeof(maze));
	//得到蛇的长度，把蛇的位置设置为true，即不能走的位置
	list<SnakeNode>::iterator iter = m_snake.m_snake.begin();
	for (unsigned i = 0; i < m_snake.m_snake.size(); i++, iter++)
	{
		XY curpo;
		curpo.y = (iter->rc.left -40) / m_po.x;
		curpo.x = (iter->rc.top - 40) / m_po.y;
		maze[curpo.x][curpo.y] = 1;
	}

	RECT rect = m_snake.m_snake.back().rc;
	XY st;
	//计算出找出蛇头的位置
	st.y = (rect.left - 40) / m_po.x;
	st.x = (rect.top - 40) / m_po.y;
	XY en;
	//计算出食物的位置，x，y分别是CreateFood的随机种子产生的x，y
	en.y = (m_food.left- 40) / m_po.x;
	en.x = (m_food.top - 40) / m_po.y;
	m_bfs.InitBfs((bool**)maze, size);
	m_bfs.EetBfs(st, en);
	if (m_bfs.m_que.empty())
	{
		//如果找不到路，在产生一个食物

		//AfxMessageBox("没有找到一条可以找到食物的路，游戏结束");

		HANDLE hThreadFDEx = ::CreateThread(NULL, 0, ThreadFood, this, 0, NULL);
		::CloseHandle(hThreadFDEx);
		SetDire();
	}
	
}

bool CSnakeAIDlg::IsSafe()
{

	//虚拟的路径虚拟的蛇..
	queue<XY> tque = m_bfs.m_que;
	Snake tsna(m_snake);
	while (!tque.empty())//队列不为空
	{
		XY txy = tque.front();
		tque.pop();
		SnakeNode node;
		node.rc.left =txy.y * 20 + 40;
		node.rc.top = txy.x * 20 + 40;
		node.rc.right = node.rc.left + 20;
		node.rc.bottom = node.rc.top + 20;
		node.curDire = DR_LEFT;				//随便初始化一个方向即可..

		tsna.m_snake.push_back(node);
		tsna.m_snake.pop_front();
	}

	XY size;
	size.x = 15;
	size.y = 25;
	bool maze[15][25];
	memset(maze, 0, sizeof(maze));
	list<SnakeNode>::iterator iter = ++tsna.m_snake.begin();
	for (unsigned i = 1; i < tsna.m_snake.size() - 1; i++, iter++)
	{
		XY curpo;
		curpo.y = (iter->rc.left - 40)/ 20;
		curpo.x = (iter->rc.top - 40) / 20;
		maze[curpo.x][curpo.y] = 1;
	}

	RECT rect = tsna.m_snake.back().rc;
	RECT rect2 = tsna.m_snake.front().rc;
	XY st;
	st.y = (rect.left - 40) / m_po.x;
	st.x = (rect.top - 40) / m_po.y;
	XY en;
	en.y = (rect2.left - 40) / m_po.x;    ///
	en.x = (rect2.top - 40) / m_po.y;      ///
	m_bfs.InitBfs((bool**)maze, size);
	if (m_bfs.CalcBfs(st, en))
		return true;
	return false;
}

DIRE CSnakeAIDlg::GetSafeDire()
{
	//得到当前地图
	XY size;
	size.x = 15;
	size.y = 25;
	bool maze[15][25];
	memset(maze, 0, sizeof(maze));
	list<SnakeNode>::iterator iter = m_snake.m_snake.begin();
	for (unsigned i = 0; i < m_snake.m_snake.size(); i++, iter++)
	{
		XY curpo;
		curpo.y = (iter->rc.left - 40) / m_po.x;
		curpo.x = (iter->rc.top - 40) / m_po.y;
		maze[curpo.x][curpo.y] = 1;
	}
	//得到蛇头和蛇尾.
	RECT rect = m_snake.m_snake.back().rc;
	//RECT rect2=m_snake.m_snake.front().rc;
	XY st;
	st.y = (rect.left - 40) / 20;
	st.x = (rect.top - 40) / 20;
	
	////XY en;
	//en.y=rect2.left/m_po.x;
	//en.x=(rect2.top-50)/m_po.y;

	XY next = st;
	next.x++;
	if (next.x < 15 && maze[next.x][next.y] == 0)
	{
		return DR_DOWN;
	}
	next = st;
	next.y++;
	if (next.y < 25 && maze[next.x][next.y] == 0)
	{
		return DR_RIGHT;
	}
	next = st;
	next.x--;
	if (next.x >= 0 && maze[next.x][next.y] == 0)
	{
		return DR_UP;
	}
	next = st;
	next.y--;
	if (next.y >= 0 && maze[next.x][next.y] == 0)
	{
		return DR_LEFT;
	}
	return DR_UP;
}



void CSnakeAIDlg::OnBnClickedBegin()
{
	CWnd* begin = GetDlgItem(IDC_BEGIN);
	begin->EnableWindow(false);
	m_tim = SetTimer(1, 100, NULL);
	m_dr = DR_RIGHT;//初始化方向变量


	PlaySound("bgm.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	CString strX, strY, strXY, stri;
	strX.Format("%d", m_rc.right - m_rc.left);
	strY.Format("%d", m_rc.bottom - m_rc.top);
	//stri.Format("%d", i);

	strXY = " snake  现在是手动操作游戏模块，请手动操作贪吃蛇的移动方向  定时器开启" ;
	OutputDebugString(strXY);
	
}


void CSnakeAIDlg::OnBnClickedRestart()
{
	CWnd* begin = GetDlgItem(IDC_BEGIN);
	begin->EnableWindow(true);
	KillTimer(m_tim);
	// TODO: 在此添加控件通知处理程序代码
}


void CSnakeAIDlg::OnBnClickedOpeng()
{
	//当用户点击这个按钮把这个按钮变成灰色，
	//并且切换其他按钮的颜色
	CWnd* close = GetDlgItem(IDC_CLOSEG);
	CWnd* open = GetDlgItem(IDC_OPENG);
	close->EnableWindow(true);
	open->EnableWindow(false);
	//TRUE表示智能模式开启
	m_ispc = true;
	
}


void CSnakeAIDlg::OnBnClickedCloseg()
{
	CWnd* close = GetDlgItem(IDC_CLOSEG);
	CWnd* open = GetDlgItem(IDC_OPENG);
	close->EnableWindow(false);
	open->EnableWindow(true);
	m_ispc = false;
	// TODO: 在此添加控件通知处理程序代码
}


void CSnakeAIDlg::OnBnClickedQuit()
{
	LeaveCriticalSection(&m_cs);
	DeleteCriticalSection(&m_cs);
	CDialogEx::OnCancel();
	// TODO: 在此添加控件通知处理程序代码
}


void CSnakeAIDlg::OnTimer(UINT_PTR nIDEvent)
{
	
	if (nIDEvent == m_tim)
	{

		//SetDlgItemText(IDB_BKG3, (LPCTSTR)m_snake.m_nScore);
		//判定游戏结束
		//判定规则，蛇头触碰到游戏边界或者触碰到自己身体
		if (m_snake.GameOver())
		{
			//结束定时器
			KillTimer(m_tim);
			//弹出游戏结束的弹窗
			MessageBox("游戏结束!\n点击确定重新开始游戏.", "状态");
			//清理蛇的数据，准备开始下一局游戏
			m_snake.m_snake.clear();
			m_snake = Snake(m_rc, m_po, m_sp);

			CWnd* begin = GetDlgItem(IDC_BEGIN);
			begin->EnableWindow(true);
			CreateGoodFood();
			CDialogEx::OnTimer(nIDEvent);
			return;
		}
		if (m_ispc)
		{
			//
			SetDire();
			
			XY cur = m_bfs.m_que.front();
			
			m_bfs.m_que.pop();

			RECT rc = m_snake.m_snake.back().rc;
			XY head;
			head.y = (rc.left - 40) / m_po.x;
			head.x = (rc.top - 40) / m_po.y;
			if (cur.x == head.x)
			{
				if (cur.y - head.y == 1)
					m_dr = DR_RIGHT;
				else if (cur.y - head.y == -1)
					m_dr = DR_LEFT;
			}
			else if (cur.y == head.y)
			{
				if (cur.x - head.x == 1)
					m_dr = DR_DOWN;
				else if (cur.x - head.x == -1)
					m_dr = DR_UP;
			}
			else
			{
				m_dr = GetSafeDire();
			}
			if (!IsSafe())
			{
				/*KillTimer(m_tim);
				MessageBox(L"不安全!",L"警告");*/
				m_dr = GetSafeDire();
			}
		}

		if (m_foodok && m_snake.SMove(m_dr, m_food))
		{
			/*int Score = 0;
			CString str;
			Score++;
			str.Format("[ZXA] %d \r\n", Score);
			OutputDebugString(str);*/
			m_foodok = false;//食物已经被吃了.
			//CreateGoodFood();
			//SetDire();
			//InvalidateRect(&m_rc,NULL);
			
		}

		//产生食物..

		HANDLE hThreadFD = ::CreateThread(NULL, 0, ThreadFood, this, 0, NULL);
		::CloseHandle(hThreadFD);

		InvalidateRect(&m_rc);
	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CSnakeAIDlg::OnEraseBkgnd(CDC* pDC)
{
	
	return CDialogEx::OnEraseBkgnd(pDC);
}


BOOL CSnakeAIDlg::PreTranslateMessage(MSG* pMsg)
{
	if (m_ispc)//不处理消息..
		return CDialog::PreTranslateMessage(pMsg);
	//获取四个方向键
	//转义键盘消息，控制蛇的走位
	switch (pMsg->wParam)
	{
	case VK_UP:
		m_dr = DR_UP;
		return true;
		break;
	case VK_DOWN:
		m_dr = DR_DOWN;
		return true;
		break;
	case VK_LEFT:
		m_dr = DR_LEFT;
		return true;
		break;
	case VK_RIGHT:
		m_dr = DR_RIGHT;
		return true;
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);

}
DWORD WINAPI ThreadFood(LPVOID lpParameter)
{
	CSnakeAIDlg* dlg = (CSnakeAIDlg*)lpParameter;
	//创建一个食物的线程
	if (!dlg->m_foodok)
	{
		dlg->CreateGoodFood();
		dlg->m_foodok = true;
	}
	return 0;
}

int CSnakeAIDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	/*PlaySound(MAKEINTRESOURCE(IDR_WAVE2), 
		AfxGetResourceHandle(), SND_ASYNC | SND_RESOURCE | SND_NODEFAULT | SND_LOOP);*/
	return 0;
}
