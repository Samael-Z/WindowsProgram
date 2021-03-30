#pragma once


// CBrush 对话框

class CBrushSet : public CDialogEx
{
	DECLARE_DYNAMIC(CBrushSet)

public:
	CBrushSet(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBrushSet();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_BRUSH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_cmbBrushStyle;
	CComboBox m_cmbShadowStyle;
	CMFCColorButton m_clrBrushColor;
	afx_msg void OnBnClickedOk();
public:
	int			m_nBrushStyle;
	int			m_nShadowStyle;
	COLORREF	m_varBrushColor;
};
