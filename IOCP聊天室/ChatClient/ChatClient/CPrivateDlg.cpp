// CPrivateDlg.cpp: 实现文件
//

#include "pch.h"
#include "ChatClient.h"
#include "CPrivateDlg.h"
#include "afxdialogex.h"


// CPrivateDlg 对话框

IMPLEMENT_DYNAMIC(CPrivateDlg, CDialogEx)

CPrivateDlg::CPrivateDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_PRIVATE, pParent)
{

}

CPrivateDlg::~CPrivateDlg()
{
}



CString CPrivateDlg::GetPrivateMessage() const
{
	return m_csPrivateMsg;
}

void CPrivateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPrivateDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPrivateDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPrivateDlg 消息处理程序


BOOL CPrivateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CPrivateDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(EDT_PRIMSG, m_csPrivateMsg);
	CDialogEx::OnOK();
}
