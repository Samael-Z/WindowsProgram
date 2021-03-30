#pragma once


// CPenSet 对话框

class CPenSet : public CDialogEx
{
	DECLARE_DYNAMIC(CPenSet)

public:
	CPenSet(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPenSet();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_PEN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

	
	CComboBox m_CmbPenStyle;
	CMFCColorButton m_ClrPenColor;
	int m_edtWidth;
	int m_PenStyle;
	COLORREF m_PenClr;
	CSpinButtonCtrl m_scAdjust;
	afx_msg void OnDeltaposAdjust(NMHDR* pNMHDR, LRESULT* pResult);
};
