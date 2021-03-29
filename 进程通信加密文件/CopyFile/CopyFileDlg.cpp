
// CopyFileDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "CopyFile.h"
#include "CopyFileDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CCopyFileDlg dialog



CCopyFileDlg::CCopyFileDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COPYFILE_DIALOG, pParent)
	, _SourcePath(_T(""))
	, _TargetPath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCopyFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, EBC_SOURCE, _SourcePath);
	DDX_Text(pDX, EBC_TARGET, _TargetPath);
	DDX_Control(pDX, PC_RATE, _pcRate);
}

BEGIN_MESSAGE_MAP(CCopyFileDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BTN_ENCRYPT, &CCopyFileDlg::OnBnClickedEncrypt)
	ON_WM_COPYDATA()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CCopyFileDlg message handlers

BOOL CCopyFileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

    HMODULE hModule = LoadLibrary("MySafeSkin.dll");
    if (hModule)
    {
        typedef  int  (WINAPI* pMySafeSkin)(void);
        pMySafeSkin MySafeSkin;
        MySafeSkin = (pMySafeSkin)GetProcAddress(hModule, "MySafeSkin");
        MySafeSkin();
    }
	_pcRate.SetRange(0, 100);
	_pcRate.SetStep(1);
	_pcRate.SetPos(20);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCopyFileDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCopyFileDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCopyFileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCopyFileDlg::OnBnClickedEncrypt()
{
	// 加密文件 把路径更新到Cstring上
	//memcpy把路径拷贝共享内存上
	UpdateData(TRUE);
	memcpy(g_szSourcePath,_SourcePath.GetString(), _SourcePath.GetLength()+1);
	memcpy(g_szTargetPath, _TargetPath.GetString(), _TargetPath.GetLength() + 1);
	//启动了另一个进程，开始拷贝文件
	//创建子进程,开始拷贝文件
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    LPTSTR szCmdline = TEXT("EncryptFile.exe");

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Start the child process. 
    if (!CreateProcess(NULL,   // No module name (use command line)
        szCmdline,      // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        CREATE_NO_WINDOW,// 新的控制台进程没有控制台窗口
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi)           // Pointer to PROCESS_INFORMATION structure
        )
    {
        AfxMessageBox("CreateProcess failed");
        return;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

	//开启定时器
    _pcRate.SetPos(0);
    SetTimer(1, 100, NULL);

}


BOOL CCopyFileDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	

	return CDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}


void CCopyFileDlg::OnTimer(UINT_PTR nIDEvent)
{
	//if (nIDEvent == 1)
	//{
        // 实时设置进度
        _pcRate.SetPos(g_dwPos);
        if (g_dwPos >= 100)
        {
            KillTimer(1);
            AfxMessageBox("加密完成!");
        }

	//}

	CDialogEx::OnTimer(nIDEvent);
}
