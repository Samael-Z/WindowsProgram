// CFileDlgView.cpp : implementation file
//

#include "pch.h"
#include "ProcessFileView.h"
#include "CFileDlgView.h"
#include "afxdialogex.h"


// CFileDlgView dialog

IMPLEMENT_DYNAMIC(CFileDlgView, CDialogEx)

CFileDlgView::CFileDlgView(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_FILE, pParent)
{

}

CFileDlgView::~CFileDlgView()
{
}

void CFileDlgView::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, LIST_FILE, m_listFile);
    DDX_Control(pDX, IDC_EDIT1, m_editPath);
}


BEGIN_MESSAGE_MAP(CFileDlgView, CDialogEx)
    ON_NOTIFY(NM_DBLCLK, LIST_FILE, &CFileDlgView::OnNMDblclkFile)
    ON_BN_CLICKED(BTN_RETRUE, &CFileDlgView::OnBnClickedRetrue)
    ON_BN_CLICKED(BTN_CERTERN, &CFileDlgView::OnBnClickedCertern)
END_MESSAGE_MAP()


// CFileDlgView message handlers


BOOL CFileDlgView::OnInitDialog()
{
    CDialogEx::OnInitDialog();
   // m_listFile.ModifyStyle()
    m_listFile.SetExtendedStyle(LVS_EX_FULLROWSELECT |
        LVS_EX_GRIDLINES | m_listFile.GetExtendedStyle());
   
    int nIndex = 0;
    m_listFile.SetBkColor(RGB(0, 255, 0));
    m_listFile.InsertColumn(nIndex++, "文件名", LVCFMT_LEFT, 200);
    m_listFile.InsertColumn(nIndex++, "大小", LVCFMT_LEFT, 200);
    m_listFile.InsertColumn(nIndex++, "文件类型", LVCFMT_LEFT, 200);
   
    int Row = 0;
    
    TCHAR szDriverBuffer[512] = { 0 };
    CString csDriver;
    DWORD dwLength = GetLogicalDriveStrings(IN 512, OUT szDriverBuffer);
    // 执行后，在szDriverBuffer中存储的盘符数据格式：
    // C://<nul>D://<nul><nul>

    TCHAR* lpDriverBuffer = szDriverBuffer;
    while (*lpDriverBuffer != NULL)
    {

        m_listFile.InsertItem(Row++, lpDriverBuffer);
        m_listFile.SetBkColor(RGB(0, 255, 0));
        csDriver = lpDriverBuffer;
        lpDriverBuffer = lpDriverBuffer + csDriver.GetLength() + 1;

        //cout<<csDriver<<endl;
      //  AfxMessageBox(lpDriverBuffer);
    }
   
   
     int n = m_listFile.GetItemCount();
     for (int i = 0; i < n; i++)
     {
         
     }
    

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}


void CFileDlgView::OnNMDblclkFile(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    *pResult = 0;
    CString strCurSelect;
    strCurSelect = m_listFile.GetItemText(pNMItemActivate->iItem, 2);
    if (strCurSelect == "文件")
    {
        return;
    }
    CFileFind finder;
    CString strPath;
    CString oldStrPath;
    strPath = m_listFile.GetItemText(pNMItemActivate->iItem, 0);
    oldStrPath = strPath;
    strPath += "\\";
    m_strPath = m_strPath + strPath;

    BOOL bRet = finder.FindFile(m_strPath + "\\*.*"); //查找所有,即是遍历
    int nRow = 0;
    m_listFile.DeleteAllItems();
    while (bRet)
    {
       // 
        bRet = finder.FindNextFile();

        CString str;
        str = finder.GetFileName();
       
        m_listFile.InsertItem(nRow, str);
        
        BOOL bIs = finder.IsArchived();
        bIs = finder.IsDirectory();
        if (bIs)
        {
            m_listFile.SetItemText(nRow++, 2, "文件夹");
        }
        else
        {
            m_listFile.SetItemText(nRow++, 2, "文件");
        }
        
        
        
    }
}



void CFileDlgView::OnBnClickedRetrue()
{
    m_listFile.DeleteAllItems();
    int Row = 0;
    TCHAR szDriverBuffer[512] = { 0 };
    CString csDriver;
    DWORD dwLength = GetLogicalDriveStrings(IN 512, OUT szDriverBuffer);
    TCHAR* lpDriverBuffer = szDriverBuffer;
    while (*lpDriverBuffer != NULL)
    {

        m_listFile.InsertItem(Row++, lpDriverBuffer);
        m_listFile.SetBkColor(RGB(0, 255, 0));
        csDriver = lpDriverBuffer;
        lpDriverBuffer = lpDriverBuffer + csDriver.GetLength() + 1;
    }
    m_strPath.Empty();
}


void CFileDlgView::OnBnClickedCertern()
{
    // TODO: 在此添加控件通知处理程序代码
    CString strEXE = "explorer.exe ";
    CString str;
    GetDlgItemText(IDC_EDIT1, str);
    strEXE += str;
    UINT n = WinExec(strEXE,SW_SHOWNORMAL);
   
}
