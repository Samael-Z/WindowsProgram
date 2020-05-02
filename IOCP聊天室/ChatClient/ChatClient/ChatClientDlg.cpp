
// ChatClientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "ChatClient.h"
#include "ChatClientDlg.h"
#include "afxdialogex.h"

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
	ON_BN_CLICKED(BTN_SENDTO, &CChatClientDlg::OnBnClickedSendto)
	ON_NOTIFY(NM_CLICK, LISTCTRL_USER, &CChatClientDlg::OnNMClickUser)
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

	m_listUser.SetExtendedStyle(m_listUser.GetExtendedStyle() |
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	EnableSendUI(FALSE);
	

	SetDlgItemInt(EDT_PORT, 12138);
	SetDlgItemText(EDT_USERNAME, "小亮");
	HANDLE hThread = CreateThread(NULL, 0, ThreadProc, this, 0, NULL);
	CloseHandle(hThread);
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
		case DT_MSG :
		{
			//解析json字符串
			neb::CJsonObject json;
			json.Parse(package.m_pBuff);

			std::string strType;
			json.Get("type", strType);

			//消息
			std::string strMSg;
			json.Get("msg", strMSg);

			//昵称
			std::string strUsername;
			json.Get("name", strUsername);

			//显示数据
			CString csText;
			pThis->GetDlgItemText(EDT_SHOWMSG, csText);

			if (strType == "public")//群发
			{
				//构造成 XXX 说: XXX的格式
				std::string strToShow = strUsername + " 说: " + strMSg;
				csText += "\r\n";
				csText += strToShow.c_str();
			}
			else if (strType == "private") //私聊 
			{
				std::string strToUsername;
				json.Get("to", strToUsername);

				if (!strcmp(strToUsername.c_str(), pThis->m_csUserName))
				{
					std::string strToShow = strToUsername + " 对你说: " + strMSg;
					csText += "\r\n";
					csText += strToShow.c_str();
				}

			}
			pThis->SetDlgItemText(EDT_SHOWMSG, csText);

			break;
		}
		case DT_FLUSH_NAME:
		{
			pThis->m_listUser.DeleteAllItems();
			DATAPACKAGE package;
			package.m_hdr.m_nDataLen = pThis->m_csUserName.GetLength() + 1;
			package.m_hdr.m_nDataType = DT_ADD_NAME;
			package.m_pBuff = pThis->m_csUserName.GetBuffer(0);
			pThis->m_TcpSocketClients.SendPackage(&package);
			pThis->EnableSendUI(TRUE);
			break;
		}
		case DT_ADD_NAME:
		{
			pThis->m_listUser.InsertItem(0, package.m_pBuff);
			break;
		}
		case DT_DEL_NAME:
		{
			CString szName;
			int nCount = pThis->m_listUser.GetItemCount();
			for (int i = 0; i < nCount; i++)
			{
				szName = pThis->m_listUser.GetItemText(i, 0);
				if (szName == package.m_pBuff)
				{
					pThis->m_listUser.DeleteItem(i);
					break;
				}
			}
			break;
		}
		case DT_LOGIN_OK:
		{
			
			//pThis->EnableSendUI(TRUE);
			
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
	m_csUserName = csUserName;
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
	
	DATAPACKAGE package;

	package.m_hdr.m_nDataLen = m_csUserName.GetLength() + 1;
	package.m_hdr.m_nDataType = DT_DEL_NAME;
	package.m_pBuff = m_csUserName.GetBuffer(0);
	m_TcpSocketClients.SendPackage(&package);

	package.m_hdr.m_nDataLen = 0;
	package.m_hdr.m_nDataType = DT_LOGINOUT;
	package.m_pBuff = NULL;
	m_TcpSocketClients.SendPackage(&package);


	m_TcpSocketClients.CloseSocket();
	CDialogEx::OnClose();
}


void CChatClientDlg::OnBnClickedLogout()
{
	DATAPACKAGE package;

	package.m_hdr.m_nDataLen = m_csUserName.GetLength() + 1;
	package.m_hdr.m_nDataType = DT_DEL_NAME;
	package.m_pBuff = m_csUserName.GetBuffer(0);
	m_TcpSocketClients.SendPackage(&package);

	package.m_hdr.m_nDataLen = 0;
	package.m_hdr.m_nDataType = DT_LOGINOUT;
	package.m_pBuff = NULL;
	m_TcpSocketClients.SendPackage(&package);

	m_TcpSocketClients.CloseSocket();
	
	EnableSendUI(FALSE);
}


void CChatClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	

	CDialogEx::OnTimer(nIDEvent);
}


void CChatClientDlg::OnNMDblclkUser(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	
	//if (!m_listUser.GetSelectionMark())
	//{
	//	CPrivateDlg dlg;
	//	if (dlg.DoModal() == IDOK)
	//	{
	//		CString csPrivate = dlg.GetPrivateMessage();
	//		 CString csUserName = m_listUser.GetItemText(pNMItemActivate->iItem, pNMItemActivate->iSubItem);

	//		CJsonObject JsonSend;
	//		JsonSend.Add("type", "private");
	//		JsonSend.Add("name", csUserName.GetBuffer(0));
	//		JsonSend.Add("msg", csPrivate.GetBuffer(0));
	//		std::string strText = JsonSend.ToString();

	//		//发送数据
	//		DATAPACKAGE package;
	//		package.m_hdr.m_nDataLen = strText.size() + 1;
	//		package.m_hdr.m_nDataType = DT_MSG;
	//		package.m_pBuff = (char*)strText.c_str();

	//		if (!m_TcpSocketClients.SendPackage(&package))
	//		{
	//			AfxMessageBox("私聊消息数据包发送失败");
	//		}
	//		


	//	}
		
	//}
	*pResult = 0;
}


void CChatClientDlg::OnBnClickedSendto()
{
	// TODO: 在此添加控件通知处理程序代码
	CString csText;
	GetDlgItemText(EDT_SEND, csText);

	//构造json字符串
// 	{"type":"public", "name" : "昵称", "msg" : "我是消息"} --群发
// 	{ "type":"private", "name" : "昵称", "msg" : "我是消息", "to" : "私聊对象" } --私聊
	CString csUsername;

	GetDlgItemText(EDT_USERNAME, csUsername);
	m_csUserName = csUsername;

	neb::CJsonObject jsonSend;
	jsonSend.Add("type", "private");
	jsonSend.Add("name", csUsername.GetBuffer(0));
	jsonSend.Add("msg", csText.GetBuffer(0));
	jsonSend.Add("to", m_csToUsername.GetBuffer(0));
	std::string strText = jsonSend.ToString();

	//发送
	DATAPACKAGE package;
	package.m_hdr.m_nDataLen = strText.size() + 1;
	package.m_hdr.m_nDataType = DT_MSG;
	package.m_pBuff = (char*)strText.c_str();
	m_TcpSocketClients.SendPackage(&package);
}


void CChatClientDlg::OnNMClickUser(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int nItem = pNMItemActivate->iItem;
	if (nItem != -1)
	{
		m_csToUsername = m_listUser.GetItemText(nItem, 0);
	}
}
