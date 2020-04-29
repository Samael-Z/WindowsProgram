// CDrawSetting.cpp: 实现文件
//

#include "pch.h"
#include "CAD.h"
#include "CDrawSetting.h"
#include "afxdialogex.h"


// CDrawSetting 对话框

IMPLEMENT_DYNAMIC(CDrawSetting, CDialogEx)

CDrawSetting::CDrawSetting(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_SETTING, pParent)
    //, m_edtPenWidth(0)
{

}

CDrawSetting::~CDrawSetting()
{
}

void CDrawSetting::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, CMB_PENSTYLE, m_cmdPenstyle);
    DDX_Control(pDX, IDB_PENCOLOR, m_btnPenColor);
    DDX_Text(pDX, EDT_PENWID, m_nPenWidth);
    DDX_Control(pDX, CMB_BRUSESTYLE, m_cmbBrushStyle);
    DDX_Control(pDX, IDB_BRUSECOLOR, m_btnBrushColor);
    DDX_Control(pDX, CMB_SDWSTYLE, m_cmdShadowStyle);
}


BEGIN_MESSAGE_MAP(CDrawSetting, CDialogEx)
    ON_BN_CLICKED(IDOK, &CDrawSetting::OnBnClickedOk)
END_MESSAGE_MAP()


// CDrawSetting 消息处理程序


BOOL CDrawSetting::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    
    m_cmdPenstyle.AddString(_T("———————————————"));
    m_cmdPenstyle.AddString(_T("-----------------------------------"));
    m_cmdPenstyle.AddString(_T(".........................................."));
    m_cmdPenstyle.AddString(_T("_._._._ _._._.__._._._ _._._.__._."));
    m_cmdPenstyle.AddString(_T("_.._..__.._..__.._.._..__.._..__..__.."));
    m_cmdPenstyle.AddString(_T("                          "));
    m_cmdPenstyle.SetCurSel(m_nPenStyle);
    m_btnPenColor.SetColor(m_clrPenColor);
    m_cmdShadowStyle.AddString(_T("-----------------------------------"));
    m_cmdShadowStyle.AddString(_T("|||||||||||||||||||||||||||||||||||"));
    m_cmdShadowStyle.AddString(_T(" \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ "));
    m_cmdShadowStyle.AddString(_T("/////////////////////////////////////////"));
    m_cmdShadowStyle.AddString(_T("+++++++++++++++++++++++++++++"));
    m_cmdShadowStyle.AddString(_T("x x x x x x x x x x x x x x x x x x x  "));
    m_cmdShadowStyle.SetCurSel(m_nShadowStyle);

    m_cmbBrushStyle.AddString(_T("BS_SOLID 实心刷子"));
    m_cmbBrushStyle.AddString(_T("BS_NULL  空心刷子"));
    //m_cmbBrushStyle.AddString(_T("BS_HOLLOW 空刷子"));
    m_cmbBrushStyle.AddString(_T("BS_HATCHED 阴影刷子"));
    m_cmbBrushStyle.AddString(_T("BS_PATTERN 位图刷子"));
    m_cmbBrushStyle.SetCurSel(m_nBrushStyle);
    m_btnBrushColor.SetColor(m_clrBrushColor);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}


void CDrawSetting::OnBnClickedOk()
{

    //获取画笔风格
    m_nPenStyle = m_cmdPenstyle.GetCurSel();
    //获取画笔宽度
    UpdateData(TRUE);
    //获取画笔颜色
    m_clrPenColor = m_btnPenColor.GetColor();
    //获取画刷风格
    m_nBrushStyle = m_cmbBrushStyle.GetCurSel();
    //获取画刷颜色
    m_clrBrushColor = m_btnBrushColor.GetColor();
    //获取阴影风格
    m_nShadowStyle = m_cmdShadowStyle.GetCurSel();


    CDialogEx::OnOK();
}
