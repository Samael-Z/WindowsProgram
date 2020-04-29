#pragma once


// CMyTreeView 视图
#include "CMyListView.h"
class CMyTreeView : public CTreeView
{
	DECLARE_DYNCREATE(CMyTreeView)

protected:
	CMyTreeView();           // 动态创建所使用的受保护的构造函数
	virtual ~CMyTreeView();

private:
	typedef struct tagNodeInfo
	{
		HKEY m_hMainKey;
		CString m_strSubKeyPath;

		tagNodeInfo(HKEY hMainKey ,CString SubKeyPath) :
			m_hMainKey(hMainKey),
			m_strSubKeyPath(SubKeyPath)

		{ }
	}NODEINFO,*PNODEINFO;
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
    afx_msg void OnNMDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	void QueryKey(HKEY hKey, HTREEITEM hParentItem, PNODEINFO pParentNodInfo);
};


