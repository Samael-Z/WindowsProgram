
// ChatClientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "ChatClient.h"
#include "ChatClientDlg.h"
#include "afxdialogex.h"
#include "CPrivateDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CChatClientDlg 对话框



CChatClientDlg::CChatClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, LISTCTRL_USER, m_listUser);

}



BEGIN_MESSAGE_MAP(CChatClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(BTN_LOGIN, &CChatClientDlg::OnBnClickedLogin)
	ON_BN_CLICKED(BTN_SEND, &CChatClientDlg::OnBnClickedSend)
	ON_WM_CLOSE()
    ON_BN_CLICKED(BTN_LOGOUT, &CChatClientDlg::OnBnClickedLogout)
	ON_WM_TIMER()
	ON_NOTIFY(NM_DBLCLK, LISTCTRL_USER, &CChatClientDlg::OnNMDblclkUser)
END_MESSAGE_MAP()


// CChatClientDlg 消息处理程序

BOOL CChatClientDlg::OnInitDialog()
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
	m_listUser.InsertColumn(0, "用户名", LVCFMT_LEFT,70);
	m_listUser.InsertColumn(1, "端口号", LVCFMT_LEFT, 70);
	m_listUser.SetExtendedStyle(m_listUser.GetExtendedStyle() |
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	EnableSendUI(FALSE);
	
	//m_listUser.InsertItem(0, "xiaoliang");
	SetDlgItemInt(EDT_PORT, 9527);
	SetDlgItemText(EDT_USERNAME, "小亮");
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CChatClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CChatClientDlg::OnPaint()
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
HCURSOR CChatClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

DWORD WINAPI CChatClientDlg::ThreadProc(LPVOID lpParam)
{
	CChatClientDlg* pThis = (CChatClientDlg*)lpParam;
	while (true)
	{
		//接收数据包
		DATAPACKAGE package;
		if (!pThis->m_TcpSocketClients.RecvPackage(&package))
		{
			continue;
		}
		//判断数据类型
		switch (package.m_hdr.m_nDataType)
		{
		case DT_LOGIN:
		{
			break;
		}
		case DT_MSG:
		{
			//解析json字符串
			CJsonObject jsonMsg;
			jsonMsg.Parse(package.m_pBuff);
			std::string strMsgType;
			jsonMsg.Get("type", strMsgType);
			if (strMsgType == "public")  //群发
			{
				//将收到的数据显示到UI
				//拼接json字符串显示输出用户名+消息内容
				std::string strMsg;
				std::string strUserName;
				jsonMsg.Get("msg", strMsg);
				jsonMsg.Get("name", strUserName);
				std::string strToShow = strUserName + " 说 " + strMsg;
				CString csShowText;
				pThis->GetDlgItemText(EDT_SHOWMSG, csShowText);
				csShowText += "\r\n";
				csShowText += strToShow.c_str();
				pThis->SetDlgItemText(EDT_SHOWMSG, csShowText);
			}
			if (strMsgType == "private") //私聊
			{
				std::string strMsg;
				std::string strUserName;
				jsonMsg.Get("msg", strMsg);
				jsonMsg.Get("name", strUserName);
				std::string strUser = pThis->m_csUserName.GetBuffer(0);
				std::string strToShow = "（私聊）"+strUser + "对" + strUserName + " 说 " + strMsg;

				CString csShowText;
				pThis->GetDlgItemText(EDT_SHOWMSG, csShowText);
				csShowText += "\r\n";
				csShowText += strToShow.c_str();
				pThis->SetDlgItemText(EDT_SHOWMSG, csShowText);
			}
			
			

			break;
		}
		case DT_LOGIN_OK:
		{
			CString csUserName = package.m_pBuff;
			
			pThis->m_listUser.InsertItem(pThis->m_InsertRow, csUserName);
			//CString csUserPort;
			/*csUserPort.Format("%d", pThis->m_SocketClients.m_nPort);
			pThis->m_listUser.SetItemText(pThis->m_InsertRow++, 1, csUserPort);*/
			pThis->EnableSendUI(TRUE);
			
			break;
		}
		//释放缓冲区
		if (package.m_pBuff!= NULL)
		{
			delete[] package.m_pBuff;
		}

		}
	}
	return 0;
}

void CChatClientDlg::EnableSendUI(BOOL bEnable)
{
	//登陆成功，用户名，端口，上线控件变灰
	GetDlgItem(EDT_USERNAME)->EnableWindow(!bEnable);
	GetDlgItem(EDT_PORT)->EnableWindow(!bEnable);
	GetDlgItem(BTN_LOGIN)->EnableWindow(!bEnable);

	//登陆成功，发送，下线，用户列表，聊天框控件显示正常颜色
	GetDlgItem(EDT_SHOWMSG)->EnableWindow(bEnable);
	GetDlgItem(BTN_LOGOUT)->EnableWindow(bEnable);
	GetDlgItem(LISTCTRL_USER)->EnableWindow(bEnable);
	GetDlgItem(BTN_SEND)->EnableWindow(bEnable);
	GetDlgItem(EDT_SEND)->EnableWindow(bEnable);
}

void CChatClientDlg::OnBnClickedLogin()
{

	CString csUserName;
	GetDlgItemText(EDT_USERNAME, csUserName);
	int nPort = GetDlgItemInt(EDT_PORT);
	if (!m_TcpSocketClients.CreateSocket())
	{
		AfxMessageBox("创建socket失败");
	 }
	if (!m_TcpSocketClients.Connect("127.0.0.1", nPort))
	{
		AfxMessageBox("链接服务器失败");
	}

	DATAPACKAGE package;
	package.m_hdr.m_nDataLen = csUserName.GetLength() + 1;
	package.m_hdr.m_nDataType = DT_LOGIN;
	package.m_pBuff = csUserName.GetBuffer(0);
	
	
	if (!m_TcpSocketClients.SendPackage(&package))
	{
		AfxMessageBox("登录数据包发送失败");
	}
	HANDLE hThread = CreateThread(NULL, 0, ThreadProc, this, 0, NULL);
	CloseHandle(hThread);
	
	
}


void CChatClientDlg::OnBnClickedSend()
{
	//获取消息编辑框内容
	CString csText;
	GetDlgItemText(EDT_SEND, csText);
	//获取用户名
	
	GetDlgItemText(EDT_USERNAME, m_csUserName);
	//定义一个json对象
	//生成调用对象的函数生成键值对
	//把键值对压缩成一行数据
	CJsonObject JsonSend;
	JsonSend.Add("type","public");
	JsonSend.Add("name", m_csUserName.GetBuffer(0));
	JsonSend.Add("msg", csText.GetBuffer(0));
	std::string strText = JsonSend.ToString();

	//发送数据
	DATAPACKAGE package;
	package.m_hdr.m_nDataLen = strText.size() + 1;
	package.m_hdr.m_nDataType = DT_MSG;
	package.m_pBuff = (char*)strText.c_str();

	if (!m_TcpSocketClients.SendPackage(&package))
	{
		AfxMessageBox("消息数据包发送失败");
	};

}


void CChatClientDlg::OnClose()
{
	
	DATAPACKAGE packageOut;
	packageOut.m_hdr.m_nDataLen = 0;
	packageOut.m_hdr.m_nDataType  = DT_LOGINOUT;
	packageOut.m_pBuff = NULL;
	if (!m_TcpSocketClients.SendPackage(&packageOut))
	{
		AfxMessageBox("关闭数据包发送失败");
	}
	m_TcpSocketClients.CloseSocket();
	//CDialogEx::OnClose();
}


void CChatClientDlg::OnBnClickedLogout()
{
	DATAPACKAGE packageOut;
	packageOut.m_hdr.m_nDataLen = 0;
	packageOut.m_hdr.m_nDataType = DT_LOGINOUT;
	packageOut.m_pBuff = NULL;
	if (!m_TcpSocketClients.SendPackage(&packageOut))
	{
		AfxMessageBox("下线数据包发送失败");
	}
	m_TcpSocketClients.CloseSocket();
	
	EnableSendUI(FALSE);
}


void CChatClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	/*DATAPACKAGE packageOut;
	packageOut.m_hdr.m_nDataLen = 0;
	packageOut.m_hdr.m_nDataType = DT_HEART;
	packageOut.m_pBuff = NULL;
	m_SocketClients.SendPackage(&packageOut, &m_siServer);*/

	CDialogEx::OnTimer(nIDEvent);
}


void CChatClientDlg::OnNMDblclkUser(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	
	if (!m_listUser.GetSelectionMark())
	{
		CPrivateDlg dlg;
		if (dlg.DoModal() == IDOK)
		{
			CString csPrivate = dlg.GetPrivateMessage();
			 CString csUserName = m_listUser.GetItemText(pNMItemActivate->iItem, pNMItemActivate->iSubItem);

			CJsonObject JsonSend;
			JsonSend.Add("type", "private");
			JsonSend.Add("name", csUserName.GetBuffer(0));
			JsonSend.Add("msg", csPrivate.GetBuffer(0));
			std::string strText = JsonSend.ToString();

			//发送数据
			DATAPACKAGE package;
			package.m_hdr.m_nDataLen = strText.size() + 1;
			package.m_hdr.m_nDataType = DT_MSG;
			package.m_pBuff = (char*)strText.c_str();

			if (!m_TcpSocketClients.SendPackage(&package))
			{
				AfxMessageBox("私聊消息数据包发送失败");
			}
			


		}
		
	}
	*pResult = 0;
}
