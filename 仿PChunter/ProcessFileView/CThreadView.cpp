// CThreadView.cpp: 实现文件
//

#include "pch.h"
#include "ProcessFileView.h"
#include "CThreadView.h"
#include "afxdialogex.h"

#include <TlHelp32.h>
// CThreadView 对话框

IMPLEMENT_DYNAMIC(CThreadView, CDialogEx)

CThreadView::CThreadView(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_THREAD, pParent)
{

}

CThreadView::~CThreadView()
{
}

void CThreadView::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, LIST_THREAD, m_ListThread);
}


BEGIN_MESSAGE_MAP(CThreadView, CDialogEx)
END_MESSAGE_MAP()


// CThreadView 消息处理程序


BOOL CThreadView::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    int nIndex = 0;
	m_ListThread.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES |
		m_ListThread.GetExtendedStyle());
    m_ListThread.InsertColumn(nIndex++, "线程ID", LVCFMT_LEFT, 200);
    m_ListThread.InsertColumn(nIndex++, "创建该线程的进程ID", LVCFMT_LEFT, 200);
    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}

void CThreadView::ShowThreadView(DWORD parentsProcessId)
{

	HANDLE hThread = CreateToolhelp32Snapshot(
		TH32CS_SNAPTHREAD,
		parentsProcessId);
	THREADENTRY32  te;
	te.dwSize = sizeof(te);

	if (Thread32First(hThread, &te))
	{
		int nRow = 0;
		CString strID;
		do
		{
			
			strID.Format("%d", te.th32ThreadID);
			m_ListThread.InsertItem(nRow, strID);
			strID.Format("%d", te.th32OwnerProcessID);
			m_ListThread.SetItemText(nRow++, 1, strID);


		} while (Thread32Next(hThread, &te));
	}
}
