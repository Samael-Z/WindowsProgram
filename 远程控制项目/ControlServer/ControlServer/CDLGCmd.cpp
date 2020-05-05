// CDLGCmd.cpp: 实现文件
//

#include "pch.h"
#include "ControlServer.h"
#include "CDLGCmd.h"
#include "afxdialogex.h"
#include "ControlServerDlg.h"

// CDLGCmd 对话框

IMPLEMENT_DYNAMIC(CDLGCmd, CDialogEx)

CDLGCmd::CDLGCmd(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_CMD, pParent)
{

}

CDLGCmd::~CDLGCmd()
{
}



void CDLGCmd::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, EDT_INPUT, m_edtCmd);
    DDX_Control(pDX, EDT_SHOWCMD, m_edtCmdShow);
}


BEGIN_MESSAGE_MAP(CDLGCmd, CDialogEx)
    ON_BN_CLICKED(BTN_CERTAIN, &CDLGCmd::OnBnClickedCertain)
    ON_MESSAGE(WM_CMD, &CDLGCmd::OnRecvCMD)
END_MESSAGE_MAP()


// CDLGCmd 消息处理程序


void CDLGCmd::OnBnClickedCertain()
{
    
	CString  strWrite;
	GetDlgItemText(EDT_INPUT, strWrite);
	strWrite += "\r\n";


    DATAPACKAGE  pkgCmdDir;
    pkgCmdDir.m_hdr.m_nDataType = DT_GETCMD;
    pkgCmdDir.m_hdr.m_nDataLen = strWrite.GetLength()+1;
    pkgCmdDir.m_pBuff = strWrite.GetBuffer(0);
    m_pSocket->SendPackage(&pkgCmdDir);

}
LRESULT CDLGCmd::OnRecvCMD(WPARAM lpWparam, LPARAM lpLparam)
{
    PDATAPACKAGE pkgCmd = (PDATAPACKAGE)lpWparam;
    m_edtCmdShow.SetSel(-1);
    m_edtCmdShow.ReplaceSel(pkgCmd->m_pBuff);
    return 0;
}


BOOL CDLGCmd::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    m_pSocket->m_Cmdhwnd = GetSafeHwnd();

    DATAPACKAGE  pkgCmdDir;
    pkgCmdDir.m_hdr.m_nDataType = DT_GETCMD;
    pkgCmdDir.m_hdr.m_nDataLen = 0;
    pkgCmdDir.m_pBuff = NULL;
    m_pSocket->SendPackage(&pkgCmdDir);
    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}
