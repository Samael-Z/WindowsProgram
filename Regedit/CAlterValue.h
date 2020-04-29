#pragma once


// CAlterValue 对话框

class CAlterValue : public CDialogEx
{
	DECLARE_DYNAMIC(CAlterValue)

public:
	CAlterValue(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAlterValue();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_ALTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
