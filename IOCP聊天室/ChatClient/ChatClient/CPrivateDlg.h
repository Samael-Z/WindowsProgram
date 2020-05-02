#pragma once


// CPrivateDlg 对话框

class CPrivateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPrivateDlg)

public:
	CPrivateDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPrivateDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_PRIVATE };
#endif
public:
	CString m_csPrivateMsg;
	CString GetPrivateMessage() const;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
