
// CopyFileDlg.h : header file
//

#pragma once

#pragma comment(lib,"SharePath.lib")
__declspec(dllimport)  extern	char g_szSourcePath[MAX_PATH];
__declspec(dllimport)  extern	char g_szTargetPath[MAX_PATH];
__declspec(dllimport)  extern	DWORD g_dwPos;
// CCopyFileDlg dialog
class CCopyFileDlg : public CDialogEx
{
// Construction
public:
	CCopyFileDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COPYFILE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedEncrypt();
	CString _SourcePath;
	CString _TargetPath;
	CProgressCtrl _pcRate;
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
