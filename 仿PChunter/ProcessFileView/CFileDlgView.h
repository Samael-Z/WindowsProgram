#pragma once


// CFileDlgView dialog

class CFileDlgView : public CDialogEx
{
	DECLARE_DYNAMIC(CFileDlgView)

public:
	CFileDlgView(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CFileDlgView();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_FILE };
#endif
public:
	CString m_strPath;
	CString m_OldStrPath;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_listFile;
	afx_msg void OnNMDblclkFile(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedRetrue();
	CEdit m_editPath;
	afx_msg void OnBnClickedCertern();
};
