// CMyTreeView.cpp: 实现文件
//

#include "pch.h"
#include "Regedit.h"
#include "CMyTreeView.h"


// CMyTreeView
#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383
IMPLEMENT_DYNCREATE(CMyTreeView, CTreeView)

CMyTreeView::CMyTreeView()
{

}

CMyTreeView::~CMyTreeView()
{
}

BEGIN_MESSAGE_MAP(CMyTreeView, CTreeView)
    ON_NOTIFY_REFLECT(NM_DBLCLK, &CMyTreeView::OnNMDblclk)
END_MESSAGE_MAP()


// CMyTreeView 诊断

#ifdef _DEBUG
void CMyTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyTreeView 消息处理程序


void CMyTreeView::OnInitialUpdate()
{
    CTreeView::OnInitialUpdate();
    CTreeCtrl& tree = GetTreeCtrl();
    tree.ModifyStyle(0, TVS_FULLROWSELECT | TVS_HASLINES | 
        TVS_LINESATROOT | TVS_HASBUTTONS);
	HTREEITEM hItem = tree.InsertItem(_T("HKEY_CLASSES_ROOT"));
	PNODEINFO pNodeInfo = new NODEINFO(HKEY_CLASSES_ROOT,NULL);
	tree.SetItemData(hItem, (DWORD_PTR)pNodeInfo);

	hItem = tree.InsertItem(_T("HKEY_CURRENT_USER"));
	pNodeInfo = new NODEINFO(HKEY_CURRENT_USER, NULL);
	tree.SetItemData(hItem, (DWORD_PTR)pNodeInfo);

	hItem = tree.InsertItem(_T("HKEY_LOCAL_MACHINE"));
	pNodeInfo = new NODEINFO(HKEY_LOCAL_MACHINE, NULL);
	tree.SetItemData(hItem, (DWORD_PTR)pNodeInfo);

	hItem = tree.InsertItem(_T("HKEY_USERS"));
	pNodeInfo = new NODEINFO(HKEY_USERS, NULL);
	tree.SetItemData(hItem, (DWORD_PTR)pNodeInfo);

	hItem = tree.InsertItem(_T("HKEY_CURRENT_CONFIG"));
	pNodeInfo = new NODEINFO(HKEY_CURRENT_CONFIG, NULL);
	tree.SetItemData(hItem, (DWORD_PTR)pNodeInfo);

    // TODO: 在此添加专用代码和/或调用基类
}


void CMyTreeView::OnNMDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
    // TODO: 在此添加控件通知处理程序代码
    *pResult = 0;
    CTreeCtrl& tree = GetTreeCtrl();
    HTREEITEM  hItemSel = tree.GetSelectedItem();
	PNODEINFO pNodeInfo =  (PNODEINFO)tree.GetItemData(hItemSel);
	//如果有子节点就不插入新的子节点了
	if (!tree.ItemHasChildren(hItemSel))
	{
		HKEY hKey = NULL;
		LONG lRet =	RegOpenKey(pNodeInfo->m_hMainKey,pNodeInfo->m_strSubKeyPath , &hKey);
		if (! lRet == ERROR_SUCCESS)
		{
			AfxMessageBox(_T("请提升管理员权限，不提别用了！"));
			return;
		}
		QueryKey(hKey,hItemSel,pNodeInfo);
	}
	
}
void CMyTreeView::QueryKey(HKEY hKey, HTREEITEM hParentItem, PNODEINFO pParentNodInfo)
{
	TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
	DWORD    cbName;                   // size of name string 
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
	DWORD    cchClassName = MAX_PATH;  // size of class string 
	DWORD    cSubKeys = 0;               // number of subkeys 
	DWORD    cbMaxSubKey;              // longest subkey size 
	DWORD    cchMaxClass;              // longest class string 
	DWORD    nCountOfValues;              // number of values for key 
	DWORD    cchMaxValue;          // longest value name 
	DWORD    cbMaxValueData;       // longest value data 
	DWORD    cbSecurityDescriptor; // size of security descriptor 
	FILETIME ftLastWriteTime;      // last write time 

	DWORD i, retCode;

	TCHAR  achValue[MAX_VALUE_NAME];
	DWORD cchValue = MAX_VALUE_NAME;

	// Get the class name and the value count. 
	retCode = RegQueryInfoKey(
		hKey,                    // key handle 
		achClass,                // buffer for class name 
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,               // 子键的个数
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&nCountOfValues,                // 键所对应的值的个数
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime);       // last write time 

								 // Enumerate the subkeys, until RegEnumKeyEx fails.

	//遍历键的信息
	if (cSubKeys)
	{

		for (i = 0; i < cSubKeys; i++)
		{
			cbName = MAX_KEY_LENGTH;
			retCode = RegEnumKeyEx(hKey, i,
				achKey,
				&cbName,
				NULL,
				NULL,
				NULL,
				&ftLastWriteTime);
			if (retCode == ERROR_SUCCESS)
			{
				//将遍历的控件插入到树控件中
				PNODEINFO pNodeInfo = new NODEINFO(pParentNodInfo->m_hMainKey,
					pParentNodInfo->m_strSubKeyPath  + achKey + _T("\\"));
				HTREEITEM hSubItem = GetTreeCtrl().InsertItem(
					achKey,
					hParentItem
				);
				GetTreeCtrl().SetItemData(hSubItem, (DWORD_PTR)pNodeInfo);

			}
		}
	}

	POSITION pos = GetDocument()->GetFirstViewPosition();
	while (pos)
	{
		CView* pView = GetDocument()->GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CMyListView)))
		{
			CMyListView* pListView = (CMyListView*)pView;
			pListView->EnumValues(hKey,nCountOfValues);
			break;
		}
	}
}