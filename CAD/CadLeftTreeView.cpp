// CadLeftTreeView.cpp: 实现文件
//

#include "pch.h"
#include "CAD.h"
#include "CadLeftTreeView.h"
#include "CADDoc.h"

// CadLeftTreeView

IMPLEMENT_DYNCREATE(CadLeftTreeView, CTreeView)

CadLeftTreeView::CadLeftTreeView()
{

}

CadLeftTreeView::~CadLeftTreeView()
{
}

BEGIN_MESSAGE_MAP(CadLeftTreeView, CTreeView)
    ON_NOTIFY_REFLECT(NM_DBLCLK, &CadLeftTreeView::OnNMDblclk)
END_MESSAGE_MAP()


// CadLeftTreeView 诊断

#ifdef _DEBUG
void CadLeftTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CadLeftTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// CadLeftTreeView 消息处理程序


void CadLeftTreeView::OnInitialUpdate()
{
    CTreeView::OnInitialUpdate();

    auto& treectrLeft = GetTreeCtrl();

    treectrLeft.ModifyStyle(0, TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT);
   
    HTREEITEM Clines =  treectrLeft.InsertItem("直线");
   
    HTREEITEM CTangle = treectrLeft.InsertItem("矩形");
  
    HTREEITEM CEliip = treectrLeft.InsertItem("椭圆");
  
    HTREEITEM CStar = treectrLeft.InsertItem("五角星");

    HTREEITEM CPen = treectrLeft.InsertItem("画笔设置");

    HTREEITEM CBrush = treectrLeft.InsertItem("画刷设置");
 
   
}


void CadLeftTreeView::OnNMDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
    // TODO: 在此添加控件通知处理程序代码
    auto& treectrLeft = GetTreeCtrl();

    HTREEITEM hItem = treectrLeft.GetSelectedItem();
    
   CString  csText = treectrLeft.GetItemText(hItem);
  
   ((CCADDoc*)GetDocument())->m_TreeSelect = csText;

   GetDocument()->UpdateAllViews(NULL);
    *pResult = 0;
}
