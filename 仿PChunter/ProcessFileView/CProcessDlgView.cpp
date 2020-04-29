// CProcessDlgView.cpp : implementation file
//

#include "pch.h"
#include "ProcessFileView.h"
#include "CProcessDlgView.h"
#include "afxdialogex.h"
#include <TlHelp32.h>

// CProcessDlgView dialog

IMPLEMENT_DYNAMIC(CProcessDlgView, CDialogEx)

CProcessDlgView::CProcessDlgView(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_PROCESS, pParent)
{

}

CProcessDlgView::~CProcessDlgView()
{
}

void CProcessDlgView::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, LIST_PROCESS, m_listProcess);
}


BEGIN_MESSAGE_MAP(CProcessDlgView, CDialogEx)
    ON_WM_RBUTTONUP()
    ON_WM_RBUTTONDOWN()
    ON_NOTIFY(NM_RCLICK, LIST_PROCESS, &CProcessDlgView::OnNMRClickProcess)
    ON_COMMAND(BTN_MOUDLE, &CProcessDlgView::OnMoudle)
    ON_COMMAND(BTN_THREAD, &CProcessDlgView::OnThread)
END_MESSAGE_MAP()


// CProcessDlgView message handlers


BOOL CProcessDlgView::OnInitDialog()
{
    CDialogEx::OnInitDialog();

   
    m_listProcess.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | 
        m_listProcess.GetExtendedStyle());
    int nIndex = 0;
    m_listProcess.SetTextBkColor(RGB(0, 255, 0));
    m_listProcess.InsertColumn(nIndex++, "映像名称",LVCFMT_LEFT, 200);
    m_listProcess.InsertColumn(nIndex++, "进程ID", LVCFMT_LEFT,200);
    m_listProcess.InsertColumn(nIndex++, "父进程ID", LVCFMT_LEFT,200);
    m_listProcess.InsertColumn(nIndex++, "映像路径", LVCFMT_LEFT,300);

    HANDLE hSnapProcess = CreateToolhelp32Snapshot(
        TH32CS_SNAPPROCESS,
        0); //对于遍历进程, 此参数会被忽略

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(pe);
    if (Process32First(hSnapProcess, &pe))
    {
        int Row = 0;
        CString  strID;
        do
        {

           m_listProcess.InsertItem( Row, pe.szExeFile);
           int col = 1;
         

            strID.Format("%d", pe.th32ProcessID);
            m_listProcess.SetItemText(Row, col++, strID);


            strID.Format("%d", pe.th32ParentProcessID);
            m_listProcess.SetItemText(Row++, col++, strID);


            //list控件显示有问题，不显示第二列和第三列
        } while (Process32Next(hSnapProcess, &pe));
    }


    return TRUE;  

}






void CProcessDlgView::OnNMRClickProcess(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    *pResult = 0;

    CMenu mnProcess;
    mnProcess.LoadMenu(MENU_PROCESS);
    CMenu* SubMenu = mnProcess.GetSubMenu(0);
    m_nRow = m_listProcess.GetSelectionMark();
    if (m_nRow)
    {
        POINT pt;
        GetCursorPos(&pt);
        SubMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this, NULL);
    }
    


}


void CProcessDlgView::OnMoudle()
{
    CString strProcessId;
    strProcessId = m_listProcess.GetItemText(m_nRow, 0);
    strProcessId = m_listProcess.GetItemText(m_nRow, 1);
    strProcessId = m_listProcess.GetItemText(m_nRow, 2);
    int n = 0;

}


void CProcessDlgView::OnThread()
{
    CString strProcessId;
    strProcessId = m_listProcess.GetItemText(m_nRow, 0);
    strProcessId = m_listProcess.GetItemText(m_nRow, 1);
    strProcessId = m_listProcess.GetItemText(m_nRow, 2);
    
}
