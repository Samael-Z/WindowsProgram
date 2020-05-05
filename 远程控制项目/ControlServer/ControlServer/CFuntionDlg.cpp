// CFuntionDlg.cpp: 实现文件
//

#include "pch.h"
#include "ControlServer.h"
#include "CFuntionDlg.h"
#include "afxdialogex.h"
#include "CJsonObject.hpp"
#include <string>
#include "ControlServerDlg.h"

using namespace std;
using namespace neb;

// CFuntionDlg 对话框

IMPLEMENT_DYNAMIC(CFuntionDlg, CDialogEx)

CFuntionDlg::CFuntionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

    
}

CFuntionDlg::~CFuntionDlg()
{
}



void CFuntionDlg::SetTcpSocket(CTcpSocket* pVal)
{
    m_pTcpFunSocket = pVal;
}

void CFuntionDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
   // DDX_Control(pDX, LIST_FILES, m_listFiles);
    DDX_Control(pDX, TAB_FUNTION, m_tabFun);
}


BEGIN_MESSAGE_MAP(CFuntionDlg, CDialogEx)
  
    ON_NOTIFY(TCN_SELCHANGE, TAB_FUNTION, &CFuntionDlg::OnTcnSelchangeFuntion)
END_MESSAGE_MAP()


// CFuntionDlg 消息处理程序


BOOL CFuntionDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    m_DlgFiles.SetTcpSocket(m_pTcpFunSocket);
    m_DlgCmd.m_pSocket = m_pTcpFunSocket;
    m_DlgProcess.m_Socket = m_pTcpFunSocket;
    m_DLgRegedit.m_Socket = m_pTcpFunSocket;
    int nIdex = 0;
    m_tabFun.InsertItem(nIdex++, "文件查看");
    m_tabFun.InsertItem(nIdex++, "CMD命令");
    m_tabFun.InsertItem(nIdex++, "进程查看");
    m_tabFun.InsertItem(nIdex++, "注册表查看");


    m_DlgCmd.Create(DLG_CMD, &m_tabFun);
    
    m_DlgProcess.Create(DLG_PROCESS, &m_tabFun);
    m_DLgRegedit.Create(DLG_REGIDTER, &m_tabFun);
    m_DlgFiles.Create(DLG_FILE, &m_tabFun);

    m_DlgFiles.ShowWindow(SW_SHOW);
    CRect rc;
 
    m_tabFun.GetClientRect(&rc);
    rc.top += 20;
    rc.left += 10;
    
    m_DlgCmd.MoveWindow(&rc);
    m_DlgFiles.MoveWindow(&rc);
    m_DlgProcess.MoveWindow(&rc);
    m_DLgRegedit.MoveWindow(&rc);


    ////初始化list空间
    //m_listFiles.InsertColumn(0, "文件名", LVCFMT_LEFT, 150);
    //m_listFiles.InsertColumn(1, "文件类型", LVCFMT_LEFT, 150);
    //m_listFiles.SetExtendedStyle(m_listFiles.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    //
    ////创建一个线程接收客户端的数据
    //HANDLE hThreadRecv = CreateThread(NULL, 0, RecvPackageThreadProc, this, 0, NULL);
    //CloseHandle(hThreadRecv);
    ////发送命令，获取盘符
    //GetDrivers();
    
    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}


//void CFuntionDlg::OnNMDblclkFiles(NMHDR* pNMHDR, LRESULT* pResult)
//{
//    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//  
//    *pResult = 0;
//    DATAPACKAGE pkg;
//    //如果点击了文件提示用户
//    CString csIsempty = m_listFiles.GetItemText(pNMItemActivate->iItem, 1);
//    if (csIsempty == "文件")
//    {
//        AfxMessageBox("不支持打开文件,请点击文件夹");
//        return;
//    }
//
//   // CString csBack = m_listFiles.GetItemText(pNMItemActivate->iItem, 0);
//    //得到list空间上的路径 组合文件夹的路径，像客户端发送
//    CString csPath = m_listFiles.GetItemText(pNMItemActivate->iItem,0);
//    if (csPath == "..")
//    {
//     /**************************************
//     /  构造路径：每次获取list控件上的文件名，与盘符进行加\\的路径拼接
//     /返回上一层目录思路：把路径的最后一个文件名删除，就可以得到上一级目录的路径
//     /先删除路径后面的最后一个\\,在获得最后一个\\的索引值
//     /然后根据索引值截断字符串就得到一个上一级的路径
//     /问题：无法返回根目录，因为没有保存根目录所有盘符
//     ******************************************/
//        m_csPath.TrimRight('\\');
//        int nIndex = m_csPath.ReverseFind('\\');
//        if (nIndex != -1)
//        {
//            m_csPath = m_csPath.Left(nIndex);
//        }
//        m_csPath = m_csPath  + '\\';
//        if (m_csPath == '\\')
//        {
//            AfxMessageBox("无法返回根目录");
//            return;
//        }
//        pkg.m_hdr.m_nDataLen = m_csPath.GetLength() + 1;
//        pkg.m_hdr.m_nDataType = DT_GETDIR;
//        pkg.m_pBuff = m_csPath.GetBuffer(0);
//        m_pTcpFunSocket->SendPackage(&pkg);
//        return;
//    }
//    m_csPath = m_csPath + csPath + '\\';
//   
//    pkg.m_hdr.m_nDataLen = m_csPath.GetLength() +1;
//    pkg.m_hdr.m_nDataType = DT_GETDIR;
//    pkg.m_pBuff = m_csPath.GetBuffer(0);
//    m_pTcpFunSocket->SendPackage(&pkg);
//
//}


//void CFuntionDlg::OnBnClickedCenter()
//{
//    //获取edit空间上用户输入的路径，向客户端发送
//    CString csMarkPath;
//    GetDlgItemText(EDT_FILEPAYH, csMarkPath);
//    m_csPath = csMarkPath;
//    DATAPACKAGE pkgMark;
//    pkgMark.m_hdr.m_nDataLen = csMarkPath.GetLength() + 1;
//    pkgMark.m_hdr.m_nDataType = DT_GETDIR;
//    pkgMark.m_pBuff = csMarkPath.GetBuffer(0);
//    m_pTcpFunSocket->SendPackage(&pkgMark);
//}
//
//
//void CFuntionDlg::OnClose()
//{
//    ShowWindow(SW_HIDE);
//}


void CFuntionDlg::OnTcnSelchangeFuntion(NMHDR* pNMHDR, LRESULT* pResult)
{
    // TODO: 在此添加控件通知处理程序代码
    *pResult = 0;
    int nIndex = m_tabFun.GetCurSel();

    m_DlgCmd.ShowWindow(SW_HIDE);
    m_DlgFiles.ShowWindow(SW_HIDE);
    m_DlgProcess.ShowWindow(SW_HIDE);
    m_DLgRegedit.ShowWindow(SW_HIDE);
    switch (nIndex)
    {
    case 0:
    {
        m_DlgFiles.ShowWindow(SW_SHOW);
        break;
    }
    case 1:
    {
        m_DlgCmd.ShowWindow(SW_SHOW);
        break;
    }
    case 2:
    {
        m_DlgProcess.ShowWindow(SW_SHOW);
        break;
    }
    case 3:
    {
        m_DLgRegedit.ShowWindow(SW_SHOW);
        break;
    }
    default:
        break;
    }
}
