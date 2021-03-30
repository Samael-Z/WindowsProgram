
// PipeCmdDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "PipeCmd.h"
#include "PipeCmdDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HANDLE g_hRead;
HANDLE g_hWrite;

HANDLE g_hCmdWrite;
HANDLE g_hCmdRead;

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


// CPipeCmdDlg 对话框



CPipeCmdDlg::CPipeCmdDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PIPECMD_DIALOG, pParent)
	, _edtShow(_T(""))
	, _edtInputCmd(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPipeCmdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, EDT_SHOW, _edtShow);
	DDX_Text(pDX, EDT_INPUTCMD, _edtInputCmd);
	DDX_Control(pDX, EDT_SHOW, _edtShowSet);
}

BEGIN_MESSAGE_MAP(CPipeCmdDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPipeCmdDlg 消息处理程序




BOOL CPipeCmdDlg::OnInitDialog()
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
    HMODULE hMod = LoadLibrary("MySafeSkin.dll");
    if (hMod)
    {
        typedef int (WINAPI* pSkin)(void);
        pSkin  skin;
        skin = (pSkin)GetProcAddress(hMod, "MySafeSkin");
        skin();
    }


	//创建匿名管道

	SECURITY_ATTRIBUTES  sa{ sizeof(SECURITY_ATTRIBUTES),NULL,TRUE };
	//创建一条管道  cmd子进程写数据，本进程读数据
	BOOL bRet = CreatePipe(&g_hRead, &g_hCmdWrite, &sa, NULL);
	if (!bRet)
	{
		AfxMessageBox("CreatePipe1 is failed");
		
	}
	//创建一条管道	本进程写数据 cmd子进程读数据，
    bRet = CreatePipe(&g_hCmdRead, &g_hWrite, &sa, NULL);
    if (!bRet)
    {
        AfxMessageBox("CreatePipe2 is failed");

    }
	// 两条管道实现了两个进程相互通信  ，匿名管道是单向的
	//创建子进程
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
	si.dwFlags |= STARTF_USESTDHANDLES;
	si.hStdInput = g_hCmdRead;
	si.hStdOutput = g_hCmdWrite;
	si.hStdError = g_hCmdRead;
    ZeroMemory(&pi, sizeof(pi));

    // Start the child process. 
    if (!CreateProcess(NULL, // No module name (use command line). 
        "cmd.exe",			// Command line. 
        NULL,				// Process handle not inheritable. 
        NULL,				// Thread handle not inheritable. 
        TRUE,				// Set handle inheritance to FALSE. 
		CREATE_NO_WINDOW,					// No creation flags. 
        NULL,				// Use parent's environment block. 
        NULL,				// Use parent's starting directory. 
        &si,				// Pointer to STARTUPINFO structure.
        &pi)				// Pointer to PROCESS_INFORMATION structure.
        )
    {
		AfxMessageBox("CreateProcess failed.");
    }

	_CmdhProcess = pi.hProcess;
   // CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);



	//读写操作

	//设置定时器一直等待读数据
	SetTimer(1, 1500, NULL);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPipeCmdDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPipeCmdDlg::OnPaint()
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
HCURSOR CPipeCmdDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CPipeCmdDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		ThisWriteData();
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CPipeCmdDlg::OnTimer(UINT_PTR nIDEvent)
{
	// 不断的在定时器里面检查是否有数据要读
	if (nIDEvent == 1)
	{
		ReceiveData();
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CPipeCmdDlg::ThisWriteData()
{
	GetDlgItemText(EDT_INPUTCMD, _edtInputCmd);
	_edtInputCmd += "\r\n";
	DWORD dwBytesToWrite;
	//响应回车消息 ，用来向cmd管道写入命令
	if (!WriteFile(g_hWrite, _edtInputCmd.GetBuffer(), _edtInputCmd.GetLength(), &dwBytesToWrite, NULL))
	{
		AfxMessageBox("写入数据失败");
		CloseHandle(g_hWrite);
		CloseHandle(g_hRead);
		CloseHandle(g_hCmdRead);
		CloseHandle(g_hCmdWrite);
		return;
	}
	 


}

void CPipeCmdDlg::ReceiveData()
{
	
	DWORD dwBytesToPeek;
	if (PeekNamedPipe(g_hRead,NULL,NULL,NULL, &dwBytesToPeek,NULL) && dwBytesToPeek >0)
	{
		char* pBuff = new char[dwBytesToPeek + 1]{ 0 };

        DWORD  dwBytesToRead;
       if (!ReadFile(g_hRead, pBuff, dwBytesToPeek + 1, &dwBytesToRead, NULL))
       {
		   AfxMessageBox("ReadFile is failed");
       }
	   _edtShow += pBuff;
	   delete[] pBuff;
		SetDlgItemText(EDT_SHOW, _edtShow);

        int dwLen = _edtShow.GetLength();

       _edtShowSet.SetSel(dwLen, dwLen, FALSE);

	}

}


void CPipeCmdDlg::OnClose()
{
	TerminateProcess(_CmdhProcess, 300);
	CDialogEx::OnClose();
}
