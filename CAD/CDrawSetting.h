#pragma once


// CDrawSetting 对话框

class CDrawSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CDrawSetting)

public:
	CDrawSetting(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDrawSetting();
public:
	int GetPenStyle() const { return m_nPenStyle; }
	int GetPenWidth() const { return m_nPenWidth; }
	int GetBrushStyle() const { return m_nBrushStyle; }
	int GetShadowStyle() const { return m_nShadowStyle; }
	void SetPenStyle(int val) { m_nPenStyle = val; }
	void SetPenWidt(int val) { m_nPenWidth = val; }
	void SetBrushStyle(int val) { m_nBrushStyle = val; }
	void SetShadowStyle(int val) { m_nShadowStyle = val; }
	void SetPenColor(COLORREF val) { m_clrPenColor =val; }
	void SetBrushColor(COLORREF val) { m_clrBrushColor = val; }
	COLORREF GetPenColor() const { return m_clrPenColor; }
	COLORREF GetBrushColor() const { return m_clrBrushColor; }

private:
	int m_nPenStyle;
	int m_nPenWidth;
	COLORREF m_clrPenColor;
	int m_nBrushStyle;
	int m_nShadowStyle;
	COLORREF m_clrBrushColor;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_SETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cmdPenstyle;
	CMFCColorButton m_btnPenColor;
	virtual BOOL OnInitDialog();
	//int m_edtPenWidth;
	afx_msg void OnBnClickedOk();
	CComboBox m_cmbBrushStyle;
	CMFCColorButton m_btnBrushColor;
	CComboBox m_cmdShadowStyle;
};
