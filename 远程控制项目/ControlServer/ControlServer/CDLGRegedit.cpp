// CDLGRegedit.cpp: 实现文件
//

#include "pch.h"
#include "ControlServer.h"
#include "CDLGRegedit.h"
#include "afxdialogex.h"
#include "ControlServerDlg.h"
#include "CJsonObject.hpp"
using namespace neb;
using namespace std;
// CDLGRegedit 对话框

IMPLEMENT_DYNAMIC(CDLGRegedit, CDialogEx)

CDLGRegedit::CDLGRegedit(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_REGIDTER, pParent)
{

}

CDLGRegedit::~CDLGRegedit()
{
}

void CDLGRegedit::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, TREE_LIST, m_treeItem);
    DDX_Control(pDX, LIST_ITEM, m_listItem);
}


BEGIN_MESSAGE_MAP(CDLGRegedit, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, TREE_LIST, &CDLGRegedit::OnNMDblclkList)
	ON_MESSAGE(WM_GETREGLST, &CDLGRegedit::OnRegList)
END_MESSAGE_MAP()


// CDLGRegedit 消息处理程序


BOOL CDLGRegedit::OnInitDialog()
{
    CDialogEx::OnInitDialog();
	m_Socket->m_Reghwnd = GetSafeHwnd();
    //初始化列表控件，给每一列一个表头
    m_listItem.SetExtendedStyle(m_listItem.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    m_listItem.InsertColumn(0, "名称", LVCFMT_LEFT, 100);
    m_listItem.InsertColumn(1, "类型", LVCFMT_LEFT, 100);
    m_listItem.InsertColumn(2, "数据", LVCFMT_LEFT, 100);


    //初始化树控件
	m_treeItem.ModifyStyle(0, TVS_FULLROWSELECT | TVS_HASLINES |
		TVS_LINESATROOT | TVS_HASBUTTONS);
	HTREEITEM hItem = m_treeItem.InsertItem(_T("HKEY_CLASSES_ROOT"));
	PNODEINFO pNodeInfo = new NODEINFO(HKEY_CLASSES_ROOT, NULL);
	m_treeItem.SetItemData(hItem, (DWORD_PTR)pNodeInfo);

	hItem = m_treeItem.InsertItem(_T("HKEY_CURRENT_USER"));
	pNodeInfo = new NODEINFO(HKEY_CURRENT_USER, NULL);
	m_treeItem.SetItemData(hItem, (DWORD_PTR)pNodeInfo);

	hItem = m_treeItem.InsertItem(_T("HKEY_LOCAL_MACHINE"));
	pNodeInfo = new NODEINFO(HKEY_LOCAL_MACHINE, NULL);
	m_treeItem.SetItemData(hItem, (DWORD_PTR)pNodeInfo);

	hItem = m_treeItem.InsertItem(_T("HKEY_USERS"));
	pNodeInfo = new NODEINFO(HKEY_USERS, NULL);
	m_treeItem.SetItemData(hItem, (DWORD_PTR)pNodeInfo);

	hItem = m_treeItem.InsertItem(_T("HKEY_CURRENT_CONFIG"));
	pNodeInfo = new NODEINFO(HKEY_CURRENT_CONFIG, NULL);
	m_treeItem.SetItemData(hItem, (DWORD_PTR)pNodeInfo);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}


void CDLGRegedit::OnNMDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	//如果当前键名没有子项，那么就遍历这个键
	m_hItemSel = m_treeItem.GetSelectedItem();
	PNODEINFO pNodeInfo = (PNODEINFO)m_treeItem.GetItemData(m_hItemSel);
	//如果有子节点就不插入新的子节点了
	if (!m_treeItem.ItemHasChildren(m_hItemSel))
	{
		
		DATAPACKAGE pkgReg;
		pkgReg.m_pBuff = pNodeInfo->m_strSubKeyPath.GetBuffer(0);
		pkgReg.m_hdr.m_nDataLen = pNodeInfo->m_strSubKeyPath.GetLength() + 1;

		if (pNodeInfo->m_hMainKey == HKEY_CLASSES_ROOT)
		{
			m_HKEY_ROOT = HKEY_CLASSES_ROOT;
			pkgReg.m_hdr.m_nDataType = DT_GETREG_ROOT;
			
		}
		if (pNodeInfo->m_hMainKey == HKEY_CURRENT_USER)
		{
			m_HKEY_ROOT = HKEY_CURRENT_USER;
			pkgReg.m_hdr.m_nDataType = DT_GETREG_CUR_USER;
		}
		if (pNodeInfo->m_hMainKey == HKEY_LOCAL_MACHINE)
		{
			m_HKEY_ROOT = HKEY_LOCAL_MACHINE;
			pkgReg.m_hdr.m_nDataType = DT_GETREG_LOC_MACH;;
			
		}
		if (pNodeInfo->m_hMainKey == HKEY_USERS)
		{
			m_HKEY_ROOT = HKEY_USERS;
			pkgReg.m_hdr.m_nDataType = DT_GETREG_USERS;
		
		}
		if (pNodeInfo->m_hMainKey == HKEY_CURRENT_CONFIG)
		{
			m_HKEY_ROOT = HKEY_CURRENT_CONFIG;
			pkgReg.m_hdr.m_nDataType = DT_GETREG_CUR_CONFIG;
		}
		m_Socket->SendPackage(&pkgReg);
	}

}

LRESULT CDLGRegedit::OnRegList(WPARAM lpWparam, LPARAM lpLparam)
{


	PDATAPACKAGE pkgData = (PDATAPACKAGE)lpWparam;

	
	CJsonObject json;
	json.Parse(pkgData->m_pBuff);
	CJsonObject jsonThread;
	string strData;
	json.Get("statu", strData);
	if (strData.c_str() == "0")
	{
		return 0;
	}
	else
	{
		for (int i = 0; i < json.GetArraySize(); i++)
		{
			//m_treeItem.InsertItem(,)
		}
	}
	return 0;
}
