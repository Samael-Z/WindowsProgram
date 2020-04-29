// CAlterValue.cpp: 实现文件
//

#include "pch.h"
#include "Regedit.h"
#include "CAlterValue.h"
#include "afxdialogex.h"


// CAlterValue 对话框

IMPLEMENT_DYNAMIC(CAlterValue, CDialogEx)

CAlterValue::CAlterValue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_ALTER, pParent)
{

}

CAlterValue::~CAlterValue()
{
}

void CAlterValue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAlterValue, CDialogEx)
END_MESSAGE_MAP()


// CAlterValue 消息处理程序
