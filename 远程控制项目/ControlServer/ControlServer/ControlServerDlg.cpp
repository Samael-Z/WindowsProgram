
// ControlServerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "ControlServer.h"
#include "ControlServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

struct ItemData
{
	CTcpSocket* m_pNewClient = nullptr;
	CFuntionDlg* m_pFunDlg = nullptr;
	CDLGFiles* m_DlgFiles = nullptr;
};

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


// CControlServerDlg 对话框



CControlServerDlg::CControlServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CONTROLSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BOOL CControlServerDlg::EnableUI(BOOL bVal)
{
	GetDlgItem(BTN_STOP)->EnableWindow(!bVal);
	GetDlgItem(LIST_CLIENTS)->EnableWindow(!bVal);
	GetDlgItem(EDT_PORT)->EnableWindow(bVal);
	GetDlgItem(BTN_START)->EnableWindow(bVal);
	GetDlgItem(BTN_REFRESH)->EnableWindow(!bVal);
	return 0;
}

void CControlServerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, LIST_CLIENTS, m_listUserClient);
}


BEGIN_MESSAGE_MAP(CControlServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(BTN_START, &CControlServerDlg::OnBnClickedStart)
	ON_BN_CLICKED(BTN_STOP, &CControlServerDlg::OnBnClickedStop)
	
	ON_NOTIFY(NM_DBLCLK, LIST_CLIENTS, &CControlServerDlg::OnNMDblclkClients)
	ON_BN_CLICKED(BTN_REFRESH, &CControlServerDlg::OnBnClickedRefresh)
	ON_MESSAGE(WM_NEWCLIENT, &CControlServerDlg::OnNewClient)
	ON_MESSAGE(WM_CLIENTOUT, &CControlServerDlg::OnClientOut)
END_MESSAGE_MAP()


// CControlServerDlg 消息处理程序

BOOL CControlServerDlg::OnInitDialog()
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
	SetDlgItemInt(EDT_PORT, 12138);
	m_listUserClient.SetExtendedStyle(m_listUserClient.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listUserClient.InsertColumn(0, "用户IP", LVCFMT_LEFT, 200);
	m_listUserClient.InsertColumn(1, "端口", LVCFMT_LEFT, 100);
	m_listUserClient.InsertColumn(2, "状态", LVCFMT_LEFT, 100);

	EnableUI(TRUE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CControlServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CControlServerDlg::OnPaint()
{
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CControlServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CControlServerDlg::OnBnClickedStart()
{
	m_tcpServer.CreateSocket();
	int nPort = GetDlgItemInt(EDT_PORT);
	if (!m_tcpServer.BindListen("0.0.0.0", nPort))
	{
		AfxMessageBox("启动服务器失败，检查端口号");
	}
	EnableUI(FALSE);
	m_bStartServer = TRUE;
	HANDLE hThread = CreateThread(NULL, 0, AcceptThreadProc, this, 0,NULL);
	CloseHandle(hThread);
	
}

DWORD WINAPI CControlServerDlg::AcceptThreadProc(LPVOID lpParam)
{
	CControlServerDlg* pThis = (CControlServerDlg*)lpParam;
	while (pThis->m_bStartServer)
	{
		CTcpSocket* pClients = new CTcpSocket();
		if (!pThis->m_tcpServer.Accept(pClients))
		{
			continue;
		}

		//向窗口发消息，处理新的客户端
		pThis->PostMessage(WM_NEWCLIENT, (WPARAM)pClients, NULL);

		

	}

	return 0;
}

void CControlServerDlg::OnBnClickedStop()
{
	EnableUI(TRUE);
}




void CControlServerDlg::OnNMDblclkClients(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	//这里弹出对话框不能用domodol 要用非模态对话框，
	////如果用模态对话框，通知只能弹出一个对话框，没有办法同时查看多个客户端的文件夹列表了
	//这里要用类指针对象，否则弹出的对话框不能持续显示

	if (!m_listUserClient.GetSelectionMark())
	{
		//m_listUserClient.SetItemText(pNMItemActivate->iItem, 2, "运行中");
		ItemData* pItemData = (ItemData*)m_listUserClient.GetItemData(pNMItemActivate->iItem);
		/*CFuntionDlg* pNewFunDlg = new CFuntionDlg();
		pNewFunDlg->SetTcpSocket(pTcpSocket);
		pNewFunDlg->Create(IDD_DIALOG1, this);*/
		pItemData->m_pFunDlg->ShowWindow(SW_SHOW);
	
	
	}
	
}





void CControlServerDlg::OnBnClickedRefresh()
{
	//AfxMessageBox("还没写");
}

LRESULT CControlServerDlg::OnNewClient(WPARAM lpWparam, LPARAM lpLparam)
{
	CTcpSocket* pClients = (CTcpSocket*)lpWparam;
	//向list空间插入用户的ip和端口信息，并且把这个客户端的信息存到这个节点上
	m_listUserClient.InsertItem(0, inet_ntoa(pClients->GetSocketAddrIn().sin_addr));
	CString csFmtPort;
	csFmtPort.Format("%d", ntohs(pClients->GetSocketAddrIn().sin_port));
	m_listUserClient.SetItemText(0, 1, csFmtPort);
	
	CFuntionDlg* pNewFunDlg = new CFuntionDlg();
	//CDLGFiles* pNewFileDlg = new CDLGFiles();
	pNewFunDlg->SetTcpSocket(pClients);
	pNewFunDlg->Create(IDD_DIALOG1, this);
	ItemData* pItemData = new ItemData;
	pItemData->m_pNewClient = pClients;
	pItemData->m_pFunDlg = pNewFunDlg;
	//pItemData->m_DlgFiles = pNewFileDlg;
	m_listUserClient.SetItemData(0, (DWORD_PTR)pItemData);
	return 0;
}

LRESULT CControlServerDlg::OnClientOut(WPARAM lpWparam, LPARAM lpLparam)
{
	CFuntionDlg* pThis = (CFuntionDlg*)lpWparam;
	for (int i = 0; i < m_listUserClient.GetItemCount(); ++i)
	{
		ItemData* pId = (ItemData*)m_listUserClient.GetItemData(i);
		if (pId->m_pFunDlg == pThis)
		{
			pId->m_pNewClient->CloseSocket();
			pId->m_pFunDlg->DestroyWindow();
			delete pId->m_pNewClient;
			delete pId->m_pFunDlg;
			delete pId;
			pId = nullptr;
			m_listUserClient.DeleteItem(i);
			break;
		}
	}
	return 0;
}
