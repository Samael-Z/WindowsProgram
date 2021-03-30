#pragma once


// CadLeftTreeView 视图
#include <afxcview.h>
class CadLeftTreeView : public CTreeView
{
	DECLARE_DYNCREATE(CadLeftTreeView)

protected:
	CadLeftTreeView();           // 动态创建所使用的受保护的构造函数
	virtual ~CadLeftTreeView();

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
};


