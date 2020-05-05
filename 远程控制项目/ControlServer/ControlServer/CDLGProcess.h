#pragma once
#include "CTcpSocket.h"

// CDLGProcess 对话框

class CDLGProcess : public CDialogEx
{
	DECLARE_DYNAMIC(CDLGProcess)

public:
	CDLGProcess(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDLGProcess();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_PROCESS };
#endif
public:
	CTcpSocket* m_Socket;
	int m_nRow;			//选中行的行号；
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_lstProcess;
	afx_msg void OnNMRClickProcess(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg	LRESULT OnRecvPROLIST(WPARAM lpWparam, LPARAM lpLparam);
	afx_msg void OnThread();
	afx_msg void OnTerprocess();
	afx_msg void OnReflush();
};
