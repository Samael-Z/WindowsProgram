#pragma once
#include "CTcpSocket.h"


// CDLGFiles 对话框

class CDLGFiles : public CDialogEx
{
	DECLARE_DYNAMIC(CDLGFiles)

public:
	CDLGFiles(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDLGFiles();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_FILE };
#endif
public:

	CTcpSocket*		GetTcpSocket();
	void			SetTcpSocket(CTcpSocket* pVal);
	void			GetDrivers();
	static DWORD WINAPI RecvPackageThreadProc(LPVOID lpParam);
	BOOL m_bStart = TRUE;
	CString m_csPath;
	CTcpSocket* m_pTcpFunSocket;
	HANDLE  m_cmdHandle;
	//void SetCmdHanle(HANDLE hwnd);
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listFiles;
	afx_msg void OnBnClickedCenter();
	CEdit m_edtPath;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkFiles(NMHDR* pNMHDR, LRESULT* pResult);
};
