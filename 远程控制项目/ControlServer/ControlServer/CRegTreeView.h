#pragma once


// CRegTreeView 对话框

class CRegTreeView : public CDialogEx
{
	DECLARE_DYNAMIC(CRegTreeView)

public:
	CRegTreeView(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CRegTreeView();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_REGIDTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
