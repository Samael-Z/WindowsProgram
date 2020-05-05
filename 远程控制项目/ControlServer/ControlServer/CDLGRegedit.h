#pragma once


// CDLGRegedit 对话框
#include "CTcpSocket.h"
class CDLGRegedit : public CDialogEx
{
	DECLARE_DYNAMIC(CDLGRegedit)

public:
	CDLGRegedit(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDLGRegedit();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_REGIDTER };
#endif
public:
	typedef struct tagNodeInfo
	{
		HKEY m_hMainKey;
		CString m_strSubKeyPath;

		tagNodeInfo(HKEY hMainKey, CString SubKeyPath) :
			m_hMainKey(hMainKey),
			m_strSubKeyPath(SubKeyPath)

		{ }
	}NODEINFO, * PNODEINFO;

	CTcpSocket* m_Socket;
	HKEY m_HKEY_ROOT = 0;

	HTREEITEM  m_hItemSel;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTreeCtrl m_treeItem;
	CListCtrl m_listItem;
	afx_msg void OnNMDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnRegList(WPARAM lpWparam, LPARAM lpLparam);
};
