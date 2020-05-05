// CDLGFiles.cpp: 实现文件
//

#include "pch.h"
#include "ControlServer.h"
#include "CDLGFiles.h"
#include "afxdialogex.h"

#include "ControlServerDlg.h"
#include "CMyChatServer.h"

// CDLGFiles 对话框

IMPLEMENT_DYNAMIC(CDLGFiles, CDialogEx)

CDLGFiles::CDLGFiles(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_FILE, pParent)
{

}

CDLGFiles::~CDLGFiles()
{
}

void CDLGFiles::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, LIST_FILES, m_listFiles);
    DDX_Control(pDX, EDT_FILEPAYH, m_edtPath);
}


BEGIN_MESSAGE_MAP(CDLGFiles, CDialogEx)
    ON_BN_CLICKED(BTN_CENTER, &CDLGFiles::OnBnClickedCenter)
    ON_NOTIFY(NM_DBLCLK, LIST_FILES, &CDLGFiles::OnNMDblclkFiles)
END_MESSAGE_MAP()


// CDLGFiles 消息处理程序





BOOL CDLGFiles::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
    m_listFiles.SetExtendedStyle(m_listFiles.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    m_listFiles.InsertColumn(0, "文件名", LVCFMT_LEFT, 100);
    m_listFiles.InsertColumn(1, "文件类型", LVCFMT_LEFT, 100);

    //创建一个线程接收客户端的数据
   HANDLE hThreadRecv = CreateThread(NULL, 0, RecvPackageThreadProc, this, 0, NULL);
    CloseHandle(hThreadRecv);
   //发送命令，获取盘符
  GetDrivers();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


CTcpSocket* CDLGFiles::GetTcpSocket()
{
    return m_pTcpFunSocket;
}

void CDLGFiles::SetTcpSocket(CTcpSocket* pVal)
{
    m_pTcpFunSocket = pVal;
}

void CDLGFiles::GetDrivers()
{
    DATAPACKAGE pkgDrivers;
    pkgDrivers.m_hdr.m_nDataLen = 0;
    pkgDrivers.m_hdr.m_nDataType = DT_GETDRIVERS;
    pkgDrivers.m_pBuff = NULL;
    m_pTcpFunSocket->SendPackage(&pkgDrivers);
}

DWORD __stdcall CDLGFiles::RecvPackageThreadProc(LPVOID lpParam)
{
    CDLGFiles* pFilesDlg = (CDLGFiles*)lpParam;
   
    while (pFilesDlg->m_bStart)
    {
        DATAPACKAGE pkgDrivers = { 0 };
        if (!pFilesDlg->m_pTcpFunSocket->RecvPackage(&pkgDrivers))
        {
           
            pFilesDlg->GetParent()->PostMessage(WM_CLIENTOUT, (WPARAM)pFilesDlg, NULL);
            AfxMessageBox("客户端退出");
            return 0;
        }
        switch (pkgDrivers.m_hdr.m_nDataType)
        {
        case DT_DRIVERS:
        {
           
            //获取盘符命令
            CJsonObject json;
            json.Parse(pkgDrivers.m_pBuff);
            CJsonObject jsonAry;
            json.Get("Drivers", jsonAry);
            for (int  i = 0; i < jsonAry.GetArraySize(); ++i)
            {
                string strDrivers;
                jsonAry.Get(i, strDrivers);
                pFilesDlg->m_listFiles.InsertItem(0, strDrivers.c_str());
                pFilesDlg->m_listFiles.SetItemText(0, 1, "文件夹");
              
            }

            break;
        }
        case DT_DIR:
        {
            
            CJsonObject jsonPath;
            jsonPath.Parse(pkgDrivers.m_pBuff);
            CJsonObject jsonFile;
            CJsonObject jsonFoder;
            jsonPath.Get("文件夹", jsonFoder);
            jsonPath.Get("文件", jsonFile);
            string strfiles = jsonFile.ToString();
            string strfoders = jsonFoder.ToString();
            if (strfiles.empty() && strfoders.empty())
            {
                AfxMessageBox("空文件");
                break;
            }
            //遍历添加文件夹类型的文件名
            pFilesDlg->m_listFiles.DeleteAllItems();
            for (int i = 0; i < jsonFoder.GetArraySize(); ++i)
            {
                string strFoder;
                jsonFoder.Get(i, strFoder);
                pFilesDlg->m_listFiles.InsertItem(0, strFoder.c_str());
                pFilesDlg->m_listFiles.SetItemText(0, 1, "文件夹");

            }
            //遍历添加文件类型的文件名
            for (int j = 0; j < jsonFile.GetArraySize(); ++j)
            {

                string strFile;
                jsonFile.Get(j, strFile);
                pFilesDlg->m_listFiles.InsertItem(0, strFile.c_str());
               
                pFilesDlg->m_listFiles.SetItemText(0, 1, "文件");
            }
            pFilesDlg->m_listFiles.InsertItem(0, "..");
            //pFunDlg->m_listFiles.SetItemData(0,)
            break;
        }
        case DT_CMD:
        {
            PDATAPACKAGE pkgCmd = new DATAPACKAGE;
            pkgCmd->m_hdr.m_nDataLen = pkgDrivers.m_hdr.m_nDataLen;
            pkgCmd->m_hdr.m_nDataType = pkgDrivers.m_hdr.m_nDataType;
            pkgCmd->m_pBuff = new char[pkgDrivers.m_hdr.m_nDataLen];
            memcpy(pkgCmd->m_pBuff, pkgDrivers.m_pBuff, pkgDrivers.m_hdr.m_nDataLen);
           
            BOOL bRet = ::PostMessage(pFilesDlg->m_pTcpFunSocket->m_Cmdhwnd, WM_CMD, (WPARAM)pkgCmd, NULL);
            
            int n = 0;
            break;
        }
        case DT_PROCESS:
        {
            PDATAPACKAGE pkgProcess = new DATAPACKAGE;
            pkgProcess->m_hdr.m_nDataLen = pkgDrivers.m_hdr.m_nDataLen;
            pkgProcess->m_hdr.m_nDataType = pkgDrivers.m_hdr.m_nDataType;
            pkgProcess->m_pBuff = new char[pkgDrivers.m_hdr.m_nDataLen];
            memcpy(pkgProcess->m_pBuff, pkgDrivers.m_pBuff, pkgDrivers.m_hdr.m_nDataLen);

            BOOL bRet = ::PostMessage(pFilesDlg->m_pTcpFunSocket->m_Processhwnd, WM_PROCESS, (WPARAM)pkgProcess, NULL);

            break;
        }
        case DT_THREAD:
        {
            PDATAPACKAGE pkgProcess = new DATAPACKAGE;
            pkgProcess->m_hdr.m_nDataLen = pkgDrivers.m_hdr.m_nDataLen;
            pkgProcess->m_hdr.m_nDataType = pkgDrivers.m_hdr.m_nDataType;
            pkgProcess->m_pBuff = new char[pkgDrivers.m_hdr.m_nDataLen];
            memcpy(pkgProcess->m_pBuff, pkgDrivers.m_pBuff, pkgDrivers.m_hdr.m_nDataLen);

            BOOL bRet = ::PostMessage(pFilesDlg->m_pTcpFunSocket->m_Processhwnd, WM_PROCESS, (WPARAM)pkgProcess, NULL);

            break;
        }
        case DT_REG:
        {
            PDATAPACKAGE pkgProcess = new DATAPACKAGE;
            pkgProcess->m_hdr.m_nDataLen = pkgDrivers.m_hdr.m_nDataLen;
            pkgProcess->m_hdr.m_nDataType = pkgDrivers.m_hdr.m_nDataType;
            pkgProcess->m_pBuff = new char[pkgDrivers.m_hdr.m_nDataLen];
            memcpy(pkgProcess->m_pBuff, pkgDrivers.m_pBuff, pkgDrivers.m_hdr.m_nDataLen);

            BOOL bRet = ::PostMessage(pFilesDlg->m_pTcpFunSocket->m_Reghwnd, WM_GETREGLST, (WPARAM)pkgProcess, NULL);

            break;
        }
        default:
            break;
        }
        //如果缓冲区的数据不为空，删除缓冲区
        if (pkgDrivers.m_pBuff!= NULL)
        {
            delete[] pkgDrivers.m_pBuff;
        }
    }
    return 0;
}






void CDLGFiles::OnNMDblclkFiles(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    *pResult = 0;

     DATAPACKAGE pkg;
    //如果点击了文件提示用户
    CString csIsempty = m_listFiles.GetItemText(pNMItemActivate->iItem, 1);
    if (csIsempty == "文件")
    {
        AfxMessageBox("不支持打开文件,请点击文件夹");
        return;
    }

   // CString csBack = m_listFiles.GetItemText(pNMItemActivate->iItem, 0);
    //得到list空间上的路径 组合文件夹的路径，像客户端发送
    CString csPath = m_listFiles.GetItemText(pNMItemActivate->iItem,0);
    if (csPath == "..")
    {
     /**************************************
     /  构造路径：每次获取list控件上的文件名，与盘符进行加\\的路径拼接
     /返回上一层目录思路：把路径的最后一个文件名删除，就可以得到上一级目录的路径
     /先删除路径后面的最后一个\\,在获得最后一个\\的索引值
     /然后根据索引值截断字符串就得到一个上一级的路径
     /问题：无法返回根目录，因为没有保存根目录所有盘符
     ******************************************/
        m_csPath.TrimRight('\\');
        int nIndex = m_csPath.ReverseFind('\\');
        if (nIndex != -1)
        {
            m_csPath = m_csPath.Left(nIndex);
        }
        m_csPath = m_csPath  + '\\';
        if (m_csPath == '\\')
        {
            AfxMessageBox("无法返回根目录");
            return;
        }
        pkg.m_hdr.m_nDataLen = m_csPath.GetLength() + 1;
        pkg.m_hdr.m_nDataType = DT_GETDIR;
        pkg.m_pBuff = m_csPath.GetBuffer(0);
        m_pTcpFunSocket->SendPackage(&pkg);
        return;
    }
    m_csPath = m_csPath + csPath + '\\';
   
    pkg.m_hdr.m_nDataLen = m_csPath.GetLength() +1;
    pkg.m_hdr.m_nDataType = DT_GETDIR;
    pkg.m_pBuff = m_csPath.GetBuffer(0);
    m_pTcpFunSocket->SendPackage(&pkg);

}
void CDLGFiles::OnBnClickedCenter()
{
    //获取edit空间上用户输入的路径，向客户端发送
    CString csMarkPath;
    GetDlgItemText(EDT_FILEPAYH, csMarkPath);
    m_csPath = csMarkPath;
    DATAPACKAGE pkgMark;
    pkgMark.m_hdr.m_nDataLen = csMarkPath.GetLength() + 1;
    pkgMark.m_hdr.m_nDataType = DT_GETDIR;
    pkgMark.m_pBuff = csMarkPath.GetBuffer(0);
    m_pTcpFunSocket->SendPackage(&pkgMark);
}