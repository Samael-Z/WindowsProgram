// CRegTreeView.cpp: 实现文件
//

#include "pch.h"
#include "ControlServer.h"
#include "CRegTreeView.h"
#include "afxdialogex.h"


// CRegTreeView 对话框

IMPLEMENT_DYNAMIC(CRegTreeView, CDialogEx)

CRegTreeView::CRegTreeView(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_REGIDTER, pParent)
{

}

CRegTreeView::~CRegTreeView()
{
}

void CRegTreeView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRegTreeView, CDialogEx)
END_MESSAGE_MAP()


// CRegTreeView 消息处理程序
