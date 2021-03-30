
// PipeCmdDlg.h: 头文件
//

#pragma once



// CPipeCmdDlg 对话框
class CPipeCmdDlg : public CDialogEx
{
// 构造
public:
	CPipeCmdDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PIPECMD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString _edtShow;
	CString _edtInputCmd;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void  ThisWriteData();
	void  ReceiveData();
	afx_msg void OnClose();
	HANDLE _CmdhProcess;
	CEdit _edtShowSet;
};
