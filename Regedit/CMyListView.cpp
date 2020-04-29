// CMyListView.cpp: 实现文件
//

#include "pch.h"
#include "Regedit.h"
#include "CMyListView.h"
#include "resource.h"
#include "CAlterValue.h"
// CMyListView

IMPLEMENT_DYNCREATE(CMyListView, CListView)

CMyListView::CMyListView()
{

}

CMyListView::~CMyListView()
{
}

BEGIN_MESSAGE_MAP(CMyListView, CListView)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CMyListView::OnNMRClick)
	ON_COMMAND(ID_ALTER, &CMyListView::OnAlter)
END_MESSAGE_MAP()


// CMyListView 诊断

#ifdef _DEBUG
void CMyListView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyListView 消息处理程序


void CMyListView::OnInitialUpdate()
{
    CListView::OnInitialUpdate();
    CListCtrl& list = GetListCtrl();
	list.ModifyStyle(LVS_TYPEMASK, //清掉以前的风格
		  LVS_REPORT | LVS_SINGLESEL); //设置报表风格
	list.SetExtendedStyle( list.GetExtendedStyle()| LVS_EX_FULLROWSELECT);
	int nCount = 0;
	list.InsertColumn(nCount++, _T("名称"),LVCFMT_COL_HAS_IMAGES | LVCFMT_LEFT,200 );
	list.InsertColumn(nCount++, _T("类型"), LVCFMT_LEFT, 300);
	list.InsertColumn(nCount++, _T("数据"), LVCFMT_LEFT, 300);
    // TODO: 在此添加专用代码和/或调用基类
}

void CMyListView::EnumValues(HKEY hKey, int nValueCount)
{
	GetListCtrl().DeleteAllItems();
	LONG lRet = 0;
	// 遍历值的信息
	if (nValueCount)
	{

		for (int i = 0, lRet = ERROR_SUCCESS; i < nValueCount; i++)
		{
			//值的名称
			TCHAR aryValueNameBuff[MAX_VALUE_NAME];
			DWORD nValueBuffSize = MAX_VALUE_NAME;
			aryValueNameBuff[0] = '\0';
			//值的类型
			DWORD dwValueType = 0;
			//值的数据
			BYTE aryValueValueBuff[MAXWORD];
			DWORD dwValueValueBuffSize = MAXWORD;

			//nValueBuffSize = MAX_VALUE_NAME;

			
			lRet = RegEnumValue(hKey, i,
				aryValueNameBuff,
				&nValueBuffSize,
				NULL,
				&dwValueType,
				aryValueValueBuff,
				&dwValueValueBuffSize);

			if (lRet == ERROR_SUCCESS)
			{
				//显示值的数据
				CListCtrl& list = GetListCtrl();
				
				list.InsertItem(i, aryValueNameBuff);
				int nColIdx = 1;
				list.SetItemText(i, nColIdx++, m_szRegType[int(dwValueType)]);
				list.SetItemText(i,nColIdx++,RegDataToStr(aryValueValueBuff, dwValueValueBuffSize, dwValueType));
		
			}
		}
	}
}

CString CMyListView::RegDataToStr(BYTE* lpData, DWORD lpcbData, UINT nID)
{
	CString csType = m_szRegType[nID];
	CString csData;
	if (csType == _T("REG_NONE") || csType == _T("REG_SZ") || csType == _T("REG_EXPAND_SZ") ||
		csType == _T("REG_MULTI_SZ"))
	{
		return csData = (TCHAR*)lpData;
	}
	else if (csType == _T("REG_BINARY"))
	{
		for (int i = 0; i < lpcbData; i++)
		{
			CString cStr;
			cStr.Format(_T("%02x "), lpData[i]);
			csData += cStr;

		}
		return csData;
	}
	else if (csType == _T("REG_DWORD"))
	{
		csData.Format(_T("0x%08x(%d)"), *(UINT*)lpData, *(UINT*)lpData);
		return csData;
	}
	else
	{
		csData.Format(_T("0x%016I64X(%I64u)"), *(__int64*)lpData, *(__int64*)lpData);
		return csData;
	}
}


void CMyListView::OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	CMenu MyMenu;
	MyMenu.LoadMenu(MN_RIGHT);
		
	CMenu* pSubMenu = MyMenu.GetSubMenu(0);
	POINT pt;
	GetCursorPos(&pt);
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,
		this);
}


void CMyListView::OnAlter()
{
	// TODO: 在此添加命令处理程序代码
	CAlterValue AlterValue;
	AlterValue.DoModal();
}
