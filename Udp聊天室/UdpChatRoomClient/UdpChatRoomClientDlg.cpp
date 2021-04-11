
// UdpChatRoomClientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "UdpChatRoomClient.h"
#include "UdpChatRoomClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD  WINAPI  RecvPkgProc(LPVOID  lpParament);
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


// CUdpChatRoomClientDlg 对话框



CUdpChatRoomClientDlg::CUdpChatRoomClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UDPCHATROOMCLIENT_DIALOG, pParent)
	, _StrNick(_T(""))
	, _PrivateTo(_T(""))
	, _StrInput(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUdpChatRoomClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, LB_ONLINUSER, _LBOnlineUsers);
	DDX_Text(pDX, EDT_NICK, _StrNick);
	DDX_Control(pDX, LB_SHOWMSG, _LBShowMsg);
	DDX_Text(pDX, IDC_EDIT3, _PrivateTo);
	DDX_Control(pDX, RB_PUBLIC, _RbPublicChat);
	DDX_Text(pDX, EDT_SENDDATA, _StrInput);
	DDX_Control(pDX, RB_PRIVATE, _rbPrivate);
}

BEGIN_MESSAGE_MAP(CUdpChatRoomClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BTN_LOGIN, &CUdpChatRoomClientDlg::OnBnClickedLogin)
	ON_BN_CLICKED(BTN_LOGOUT, &CUdpChatRoomClientDlg::OnBnClickedLogout)
	ON_MESSAGE(WM_HANDLEPKG, &CUdpChatRoomClientDlg::OnHandlepkg)
	ON_LBN_DBLCLK(LB_ONLINUSER, &CUdpChatRoomClientDlg::OnDblclkLbOnlinuser)
END_MESSAGE_MAP()


// CUdpChatRoomClientDlg 消息处理程序

BOOL CUdpChatRoomClientDlg::OnInitDialog()
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

	//把服务器的IP和端口赋值下
	_ServerInfo._dwIp = inet_addr("127.0.0.1");
	_ServerInfo._wPort = htons(0x9527);

	_RbPublicChat.SetCheck(TRUE);

   
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CUdpChatRoomClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUdpChatRoomClientDlg::OnPaint()
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
HCURSOR CUdpChatRoomClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUdpChatRoomClientDlg::OnBnClickedLogin()
{
	// TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
   

	if (_StrNick.GetLength() == 0)
	{
		AfxMessageBox("请输入昵称!");
		return;
	}

	//初始化socket资源
    _SocketSelf = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (_SocketSelf == INVALID_SOCKET)
    {
        AfxMessageBox("创建sock失败");
    }

    _HRecv = CreateThread(NULL, 0, RecvPkgProc, this, 0, 0);
    if (_HRecv == NULL)
    {
        AfxMessageBox("接收数据包线程失败");
        CloseHandle(_HRecv);
    }


	//上线就发送一个上线包

	
	PACKAGE  pkgLogin;
	memset(&pkgLogin, 0, sizeof(pkgLogin));
	pkgLogin._nCommand = C2S_LOGIN;
	_SelfInfo._dwIp = inet_addr("127.0.0.1");
	_SelfInfo._wPort = 0;
	memcpy(_SelfInfo._szNick, _StrNick.GetBuffer(), _StrNick.GetLength() + 1);

	pkgLogin._ClientSend = _SelfInfo;

	BOOL bRet =  SentTo(_SocketSelf, _ServerInfo, pkgLogin);
	if (!bRet)
	{
		AfxMessageBox("上线包发送失败");
	}

	//拿到自己的端口号
    sockaddr_in si;
    int nLen = sizeof(si);
    getsockname(_SocketSelf, (sockaddr*)&si, &nLen);
    _SelfInfo._wPort = si.sin_port;

	//登陆成功在设置成不可选
    SetDlgItemText(ST_SELF, _StrNick);
    GetDlgItem(BTN_LOGIN)->EnableWindow(FALSE);
    GetDlgItem(BTN_LOGOUT)->EnableWindow(TRUE);
}


void CUdpChatRoomClientDlg::OnBnClickedLogout()
{
	// TODO: 在此添加控件通知处理程序代码
	
	GetDlgItem(BTN_LOGIN)->EnableWindow(TRUE);
	GetDlgItem(BTN_LOGOUT)->EnableWindow(FALSE);

	PACKAGE  pkgLogout;
	memset(&pkgLogout, 0, sizeof(pkgLogout));

	pkgLogout._nCommand = C2S_LOGOUT;
	pkgLogout._ClientSend = _SelfInfo;

    BOOL  bRet =  SentTo(_SocketSelf, _ServerInfo, pkgLogout);
	if (bRet)
	{
        //应该是发完下线包在挂起线程
		//清理资源
		TerminateThread(_HRecv, 0);
		_HRecv = NULL;
		closesocket(_SocketSelf);
		_LBOnlineUsers.ResetContent();
	}
	
}
DWORD  WINAPI  RecvPkgProc(LPVOID  lpParament)
{
	CUdpChatRoomClientDlg* pThis = (CUdpChatRoomClientDlg*)lpParament;
	//开一个线程去收数据包
	
	while (TRUE)
	{
		PACKAGE  RecvPkg;
		memset(&RecvPkg, 0, sizeof(RecvPkg));
		sockaddr_in  si = { 0 };
		int  nSilen = sizeof(si);
		int nRet = recvfrom(pThis->_SocketSelf, (char*)&RecvPkg, sizeof(RecvPkg), 0, (sockaddr*)&si, &nSilen);
		if (nRet == 0 || nRet == SOCKET_ERROR)
		{
			continue;
		}

		//只做收数据的活，处理数据包交给别人，
		//非ui线程不操作界面
	//	AfxMessageBox("收到数据包");
		pThis->_Lock.Lock();
		pThis->_PackageInfo.push_back(RecvPkg);
		pThis->_Lock.Unlock();
		pThis->PostMessage(WM_HANDLEPKG);

	}

	return 0;
}

afx_msg LRESULT CUdpChatRoomClientDlg::OnHandlepkg(WPARAM wParam, LPARAM lParam)
{

   

    //依次取出数据包，避免丢包或者拿错包
	//用了list保存数据包，在用临界区同步
	_Lock.Lock();
	auto  Pkg = _PackageInfo.front();
	_PackageInfo.pop_front();
	_Lock.Unlock();

	switch (Pkg._nCommand)
	{

	case  S2C_LOGIN:
	{
		OnS2CLogin(Pkg);
	}
	break;
	case  S2C_LOGOUT:
	{
		OnS2CLogout(Pkg);
	}
	break;
	case  S2C_GROUP:
	{
		OnS2CGroup(Pkg);
	}
	break;
	case  S2C_PRIVATE:
	{

		OnS2CPrivate(Pkg);
		
	}
	break;
	}
	return 0;
}


void CUdpChatRoomClientDlg::OnS2CLogin(PACKAGE pkg)
{
    CString strFmt;
    strFmt.Format("(%s:%d)%s",
        inet_ntoa(*(in_addr*)&pkg._ClientSend._dwIp),
        ntohs(pkg._ClientSend._wPort),
        pkg._ClientSend._szNick);
    
	_LBOnlineUsers.AddString(strFmt);
}

void CUdpChatRoomClientDlg::OnS2CLogout(PACKAGE pkg)
{

    CString strFmt;
    strFmt.Format("(%s:%d)%s",
        inet_ntoa(*(in_addr*)&pkg._ClientSend._dwIp),
        ntohs(pkg._ClientSend._wPort),
        pkg._ClientSend._szNick);
	//listbox 查找一条信息
    int nIdx = _LBOnlineUsers.FindString(-1, strFmt);
	_LBOnlineUsers.DeleteString(nIdx);
}

void CUdpChatRoomClientDlg::OnS2CGroup(PACKAGE pkg)
{
	CString  strFmt;
	strFmt.Format("%s说：%s", pkg._ClientSend._szNick, pkg._szMsg);
	_LBShowMsg.AddString(strFmt);
	_LBShowMsg.SetCurSel(_LBShowMsg.GetCount() - 1);
}

void CUdpChatRoomClientDlg::OnS2CPrivate(PACKAGE pkg)
{
    CString  strFmt;
     strFmt.Format("%s对你说：%s", pkg._ClientSend._szNick, pkg._szMsg);
     _LBShowMsg.AddString(strFmt);
	 _LBShowMsg.SetCurSel(_LBShowMsg.GetCount() - 1);
}


void CUdpChatRoomClientDlg::OnDblclkLbOnlinuser()
{
	// TODO: 在此添加控件通知处理程序代码
	int nPos = _LBOnlineUsers.GetCurSel();
	_LBOnlineUsers.GetText(nPos, _StrSelPrivate);
	_PrivateTo = _StrSelPrivate;

	_rbPrivate.SetCheck(TRUE);
	_RbPublicChat.SetCheck(FALSE);
	UpdateData(FALSE);
	
}

void CUdpChatRoomClientDlg::SendChatMsg()
{


	UpdateData(TRUE);
    PACKAGE  pkgMsg;
    memset(&pkgMsg, 0, sizeof(pkgMsg));
	if (_RbPublicChat.GetCheck())
	{
		//公聊
		pkgMsg._nCommand = C2S_GROUP;
		memcpy(pkgMsg._szMsg, _StrInput.GetBuffer(), _StrInput.GetLength() + 1);
		pkgMsg._ClientSend = _SelfInfo;
		SentTo(_SocketSelf, _ServerInfo, pkgMsg);
	}
	else
	{
		//私聊
		pkgMsg._nCommand = C2S_PRIVATE;
        memcpy(pkgMsg._szMsg, _StrInput.GetBuffer(), _StrInput.GetLength() + 1);
        pkgMsg._ClientSend = _SelfInfo;
		string  strSep;
		strSep = _StrSelPrivate;
		//从listbox获取用户信息的字符串，然后转换格式
        int nSickPos = strSep.find(")");
        string Sick = strSep.substr(nSickPos + 1);
        int nPortPos = strSep.find(":");
        string port = strSep.substr(nPortPos + 1, nSickPos - nPortPos - 1);
        string ip = strSep.substr(1, nPortPos - 1);
		//拿到ip地址端口和昵称
        DWORD dwip = inet_addr(ip.c_str());
        WORD  dPort = atoi(port.c_str());
		pkgMsg._uiTo._dwIp = dwip;
		pkgMsg._uiTo._wPort = htons(dPort);
		memcpy(pkgMsg._uiTo._szNick, Sick.c_str(), Sick.size() + 1);
		
		SentTo(_SocketSelf, _ServerInfo, pkgMsg);
	}
	

}


BOOL CUdpChatRoomClientDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if (pMsg->message == WM_KEYDOWN&& pMsg->wParam == VK_RETURN)
	{
		//重写回车消息
		SendChatMsg();
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
