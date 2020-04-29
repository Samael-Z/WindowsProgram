#pragma once


// CThreadView 对话框

class CThreadView : public CDialogEx
{
	DECLARE_DYNAMIC(CThreadView)

public:
	CThreadView(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CThreadView();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_THREAD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListThread;
	virtual BOOL OnInitDialog();
	void ShowThreadView(DWORD parentsProcessId);
};
