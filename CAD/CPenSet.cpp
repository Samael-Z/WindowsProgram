// CPenSet.cpp: 实现文件
//

#include "pch.h"
#include "CAD.h"
#include "CPenSet.h"
#include "afxdialogex.h"


// CPenSet 对话框

IMPLEMENT_DYNAMIC(CPenSet, CDialogEx)

CPenSet::CPenSet(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_PEN, pParent)
	
	, m_edtWidth(1)
{

}

CPenSet::~CPenSet()
{
}

void CPenSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, CMB_PENSTYLE, m_CmbPenStyle);
	DDX_Control(pDX, CLR_COLOR, m_ClrPenColor);
	DDX_Text(pDX, EDT_WIDTH, m_edtWidth);
	DDX_Control(pDX, SC_ADJUST, m_scAdjust);
}


BEGIN_MESSAGE_MAP(CPenSet, CDialogEx)
    ON_BN_CLICKED(IDOK, &CPenSet::OnBnClickedOk)
	ON_NOTIFY(UDN_DELTAPOS, SC_ADJUST, &CPenSet::OnDeltaposAdjust)
END_MESSAGE_MAP()


// CPenSet 消息处理程序


void CPenSet::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_PenStyle = m_CmbPenStyle.GetCurSel();
	m_PenClr = m_ClrPenColor.GetColor();

    CDialogEx::OnOK();
}


BOOL CPenSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// TODO:  在此添加额外的初始化
	 /* -------  */
	 /* .......  */
	 /* _._._._  */
	 /* _.._.._  */
	m_CmbPenStyle.AddString("————————");
	m_CmbPenStyle.AddString("-------");
	m_CmbPenStyle.AddString(".......");
	m_CmbPenStyle.AddString("_._._._");
	m_CmbPenStyle.AddString("_.._.._");
	m_CmbPenStyle.AddString("		");
	m_CmbPenStyle.SetCurSel(0);
	m_CmbPenStyle.SetCurSel(m_PenStyle);
	UpdateData(TRUE);
	m_ClrPenColor.SetColor(m_PenClr);
	m_scAdjust.SetBuddy(GetDlgItem(EDT_WIDTH));
	m_scAdjust.SetRange(1, 10);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CPenSet::OnDeltaposAdjust(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码



	UpdateData(true);
	
	if (pNMUpDown->iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往下的箭头
	{
		if (m_edtWidth > 1)
		{
			m_edtWidth -= 1;
		}
		
	}
	else if (pNMUpDown->iDelta == 1) // 如果此值为1, 说明点击了Spin的往上的箭头
	{
		if (m_edtWidth < 10)
		{
			m_edtWidth += 1;
		}
		
	}
	
	UpdateData(false);



	*pResult = 0;
}
