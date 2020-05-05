#pragma once
#include "CDLGFiles.h"

#include <string>
#include "CDLGCmd.h"

#include "CDLGProcess.h"
#include "CDLGRegedit.h"
// CFuntionDlg 对话框

class CFuntionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFuntionDlg)

public:
	CFuntionDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CFuntionDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif
public:
	
	CDLGCmd			m_DlgCmd;
	CDLGFiles		m_DlgFiles;
	CDLGProcess		m_DlgProcess;
	CDLGRegedit		m_DLgRegedit;
	void			SetTcpSocket(CTcpSocket* pVal);
	CTcpSocket* m_pTcpFunSocket;
private:
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	//afx_msg void OnNMDblclkFiles(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnBnClickedCenter();
	//afx_msg void OnClose();
    CTabCtrl m_tabFun;
	afx_msg void OnTcnSelchangeFuntion(NMHDR* pNMHDR, LRESULT* pResult);
};
