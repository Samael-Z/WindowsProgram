// CDLGProcess.cpp: 实现文件
//

#include "pch.h"
#include "ControlServer.h"
#include "CDLGProcess.h"
#include "afxdialogex.h"
#include "CJsonObject.hpp"
#include "ControlServerDlg.h"
using   namespace neb;
using   namespace std;
// CDLGPrcess 对话框

IMPLEMENT_DYNAMIC(CDLGProcess, CDialogEx)

CDLGProcess::CDLGProcess(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_PROCESS, pParent)
{

}

CDLGProcess::~CDLGProcess()
{
}

void CDLGProcess::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, LST_PROCESS, m_lstProcess);
}


BEGIN_MESSAGE_MAP(CDLGProcess, CDialogEx)
    ON_NOTIFY(NM_RCLICK, LST_PROCESS, &CDLGProcess::OnNMRClickProcess)
    ON_COMMAND(BTN_THREAD, &CDLGProcess::OnThread)
    ON_COMMAND(BTN_TERPROCESS, &CDLGProcess::OnTerprocess)
    ON_MESSAGE(WM_PROCESS, &CDLGProcess::OnRecvPROLIST)
    ON_COMMAND(BTN_REFLUSH, &CDLGProcess::OnReflush)
END_MESSAGE_MAP()


// CDLGProcess 消息处理程序


BOOL CDLGProcess::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    m_Socket->m_Processhwnd = GetSafeHwnd();
    m_lstProcess.SetExtendedStyle(m_lstProcess.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    m_lstProcess.InsertColumn(0, "映像名称",LVCFMT_LEFT,150);
    m_lstProcess.InsertColumn(1, "进程ID", LVCFMT_LEFT, 150);
    m_lstProcess.InsertColumn(2, "父进程ID", LVCFMT_LEFT, 150);
    DATAPACKAGE pkgPro;
    pkgPro.m_hdr.m_nDataLen = 0;
    pkgPro.m_hdr.m_nDataType = DT_GETPROCESS;
    pkgPro.m_pBuff = NULL;
    m_Socket->SendPackage(&pkgPro);
    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}


void CDLGProcess::OnNMRClickProcess(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    *pResult = 0;
    CMenu mnProcess;
    mnProcess.LoadMenu(MENU_PROCESS);
    CMenu* SubMenu = mnProcess.GetSubMenu(0);
    m_nRow = m_lstProcess.GetSelectionMark();
    if (m_nRow)
    {
        POINT pt;
        GetCursorPos(&pt);
        SubMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this, NULL);
    }
}

LRESULT CDLGProcess::OnRecvPROLIST(WPARAM lpWparam, LPARAM lpLparam)
{
    PDATAPACKAGE pkgData = (PDATAPACKAGE)lpWparam;

    m_lstProcess.DeleteAllItems();
    CJsonObject json;
    json.Parse(pkgData->m_pBuff);
    CJsonObject jsonThread;
    if (json.Get("ThreadId", jsonThread))
    {
        for (int j = 0; j < jsonThread.GetArraySize(); ++j)
        {
            string strThreadId;
            jsonThread.Get(j, strThreadId);
            m_lstProcess.InsertItem(0, strThreadId.c_str());
        }
        return 0;
    }
    for (int i = 0; i < json.GetArraySize(); i++)
    {
        CJsonObject js;
        string strfile;
        json.Get(i, js);
        js.Get("exe", strfile);
        m_lstProcess.InsertItem(0, strfile.c_str());
        
        js.Get("id", strfile);
        m_lstProcess.SetItemText(0, 1, strfile.c_str());
        js.Get("parid", strfile);
        m_lstProcess.SetItemText(0, 2, strfile.c_str());

    }
    return 0;
}


void CDLGProcess::OnThread()
{
    // 遍历线程
    CString ProcessId = m_lstProcess.GetItemText(m_nRow, 1);
    DATAPACKAGE pkgThread;
    pkgThread.m_hdr.m_nDataLen = ProcessId.GetLength() + 1;
    pkgThread.m_hdr.m_nDataType = DT_GETTHREAD;
    pkgThread.m_pBuff = ProcessId.GetBuffer(0);
    m_Socket->SendPackage(&pkgThread);
}


void CDLGProcess::OnTerprocess()
{
    // 结束进程
    CString ProcessId = m_lstProcess.GetItemText(m_nRow, 1);
    DATAPACKAGE pkgExit;
    pkgExit.m_hdr.m_nDataLen = ProcessId.GetLength() + 1;
    pkgExit.m_hdr.m_nDataType = DT_TERPROCESS;
    pkgExit.m_pBuff = ProcessId.GetBuffer(0);
    m_Socket->SendPackage(&pkgExit);


    DATAPACKAGE pkgPro;
    pkgPro.m_hdr.m_nDataLen = 0;
    pkgPro.m_hdr.m_nDataType = DT_GETPROCESS;
    pkgPro.m_pBuff = NULL;
    m_Socket->SendPackage(&pkgPro);
}


void CDLGProcess::OnReflush()
{
    DATAPACKAGE pkgPro;
    pkgPro.m_hdr.m_nDataLen = 0;
    pkgPro.m_hdr.m_nDataType = DT_GETPROCESS;
    pkgPro.m_pBuff = NULL;
    m_Socket->SendPackage(&pkgPro);
}
