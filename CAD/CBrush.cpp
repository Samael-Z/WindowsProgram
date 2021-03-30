// CBrush.cpp: 实现文件
//

#include "pch.h"
#include "CAD.h"
#include "CBrush.h"
#include "afxdialogex.h"


// CBrush 对话框

IMPLEMENT_DYNAMIC(CBrushSet, CDialogEx)

CBrushSet::CBrushSet(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_BRUSH, pParent)
{

}

CBrushSet::~CBrushSet()
{
}

void CBrushSet::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO1, m_cmbBrushStyle);
    DDX_Control(pDX, IDC_COMBO2, m_cmbShadowStyle);
    DDX_Control(pDX, IDC_MFCCOLORBUTTON1, m_clrBrushColor);
}


BEGIN_MESSAGE_MAP(CBrushSet, CDialogEx)
    ON_BN_CLICKED(IDOK, &CBrushSet::OnBnClickedOk)
END_MESSAGE_MAP()


// CBrush 消息处理程序


BOOL CBrushSet::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
       /* ----- */
       /* ||||| */
       /* \\\\\ */
       /* ///// */
       /* +++++ */
       /* xxxxx */
    m_cmbBrushStyle.AddString("实心刷子");
    m_cmbBrushStyle.AddString("空心刷子");
    m_cmbBrushStyle.AddString("阴影刷子");
    m_cmbBrushStyle.SetCurSel(0);
    m_cmbShadowStyle.AddString("-----");
    m_cmbShadowStyle.AddString("|||||");
    m_cmbShadowStyle.AddString("\\\\\\\\\\\\\\\\\\ ");
    m_cmbShadowStyle.AddString("/////");
    m_cmbShadowStyle.AddString(" +++++");
    m_cmbShadowStyle.AddString(" xxxxx");
    m_cmbShadowStyle.SetCurSel(0);
    
     m_cmbBrushStyle.SetCurSel(m_nBrushStyle);
     m_cmbShadowStyle.SetCurSel(m_nShadowStyle);
     m_clrBrushColor.SetColor(m_varBrushColor);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}


void CBrushSet::OnBnClickedOk()
{
    
    m_nBrushStyle = m_cmbBrushStyle.GetCurSel();
    m_nShadowStyle = m_cmbShadowStyle.GetCurSel();
    m_varBrushColor = m_clrBrushColor.GetColor();


    CDialogEx::OnOK();
}
