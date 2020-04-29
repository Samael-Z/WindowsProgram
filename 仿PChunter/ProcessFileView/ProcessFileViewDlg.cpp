
// ProcessFileViewDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ProcessFileView.h"
#include "ProcessFileViewDlg.h"
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


// CProcessFileViewDlg dialog



CProcessFileViewDlg::CProcessFileViewDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROCESSFILEVIEW_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProcessFileViewDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, TAB_PROFILE, m_TabCtrl);
}

BEGIN_MESSAGE_MAP(CProcessFileViewDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, TAB_PROFILE, &CProcessFileViewDlg::OnSelchangeTabProfile)
END_MESSAGE_MAP()


// CProcessFileViewDlg message handlers

BOOL CProcessFileViewDlg::OnInitDialog()
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
	int nIndex = 0;
	m_TabCtrl.InsertItem(nIndex++, "进程查看视图");
	m_TabCtrl.InsertItem(nIndex, "文件查看视图");
	m_FileView.Create(DLG_FILE, &m_TabCtrl);
	m_ProcessView.Create(DLG_PROCESS, &m_TabCtrl);

	m_ProcessView.ShowWindow(SW_SHOW);
	CRect re;
	m_TabCtrl.GetClientRect(&re);
	re.top +=20;
	//re.left += 10;
	//re.right -= 30;
	//re.bottom -= 50;
	
	m_FileView.MoveWindow(&re);
	m_ProcessView.MoveWindow(&re);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CProcessFileViewDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CProcessFileViewDlg::OnPaint()
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
HCURSOR CProcessFileViewDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CProcessFileViewDlg::OnSelchangeTabProfile(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	int nIndex = m_TabCtrl.GetCurSel();
	m_FileView.ShowWindow(SW_HIDE);
	m_ProcessView.ShowWindow(SW_HIDE);
	switch (nIndex)
	{
	case 0:
		m_ProcessView.ShowWindow(SW_SHOW);
		break;
	case 1:
		m_FileView.ShowWindow(SW_SHOWNORMAL);
		break;
	default:
		break;
	}
}
