#pragma once


// CProcessDlgView dialog

class CProcessDlgView : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessDlgView)

public:
	CProcessDlgView(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CProcessDlgView();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_PROCESS };
#endif
public:
	int m_nRow;//选中行号
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_listProcess;
	
	afx_msg void OnNMRClickProcess(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMoudle();
	afx_msg void OnThread();
};
