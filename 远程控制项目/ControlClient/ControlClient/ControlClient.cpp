// ControlClient.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
#include <afx.h>
#include <iostream>
#include "CTcpSocket.h"
#include "CJsonObject.hpp"
#include <string>
#include <TlHelp32.h>
#include <stdlib.h>
                   
#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

using namespace std;
using namespace neb;
CJsonObject GetDrivers();
CJsonObject GetFolder(CString strfile);
CJsonObject EnumProcess();
CJsonObject EnumThread(DWORD dwId);
void ExitProcess(DWORD dwProcessId);
void CreatePipePro();
char* GetCmdData(CString csCmd);

HANDLE m_hParentsWrite;  
HANDLE m_hParentsRead;
HANDLE m_hChildWrite;
HANDLE m_hChildRead;

typedef struct tagNodeInfo
{
    HKEY m_hMainKey;
    CString m_strSubKeyPath;

    tagNodeInfo(HKEY hMainKey, CString SubKeyPath) :
        m_hMainKey(hMainKey),
        m_strSubKeyPath(SubKeyPath)

    { }
}NODEINFO, * PNODEINFO;
CJsonObject  GetRegKey(HKEY mainhKey,CString csPath);
CJsonObject QueryKey(HKEY hKey);


int main()
{
    
    //PNODEINFO pp = new NODEINFO(HKEY_CURRENT_USER, "");
    
   
    //GetRegKey(HKEY_CURRENT_USER,"");
    CreatePipePro();
  
  
    CTcpSocket tcpClients;
    tcpClients.CreateSocket();
    if (!tcpClients.Connect("127.0.0.1",12138))
    {
        printf("链接服务器失败\r\n");
        return 0;
    }
    else
    {
        printf("链接服务器成功，正在等待服务器命令\r\n");
    }
    //接收数据包
    while (true)
    {
        DATAPACKAGE pkgRecv;
        if (!tcpClients.RecvPackage(&pkgRecv))
        {
            printf("链接中断\r\n");
            break;
        }
        switch (pkgRecv.m_hdr.m_nDataType)
        {
        case DT_GETDRIVERS:
        {

            CJsonObject json = GetDrivers();
            string strJson = json.ToFormattedString();
            DATAPACKAGE pkgDrivers;
            pkgDrivers.m_hdr.m_nDataLen = strJson.size() + 1;
            pkgDrivers.m_hdr.m_nDataType = DT_DRIVERS;
            pkgDrivers.m_pBuff = (char *)strJson.c_str();
            tcpClients.SendPackage(&pkgDrivers);
            break;
        }
        case DT_GETDIR:
        {
            CJsonObject jsonPath = GetFolder(pkgRecv.m_pBuff);
            string strPath = jsonPath.ToFormattedString();
            DATAPACKAGE pgkPath;
            pgkPath.m_hdr.m_nDataLen = strPath.size() + 1;
            pgkPath.m_hdr.m_nDataType = DT_DIR;
            pgkPath.m_pBuff = (char*)strPath.c_str();
            tcpClients.SendPackage(&pgkPath);
            break;
        }
        case DT_GETCMD:
        {
            CString csCmdData = GetCmdData(pkgRecv.m_pBuff);
            //printf("%s\r\n", csCmdData.GetBuffer(0));
            DATAPACKAGE pkgCmd;
            pkgCmd.m_hdr.m_nDataType = DT_CMD;
            pkgCmd.m_hdr.m_nDataLen = csCmdData.GetLength() + 1;
            pkgCmd.m_pBuff = csCmdData.GetBuffer(0);
            tcpClients.SendPackage(&pkgCmd);
            break;
        }
        case DT_GETPROCESS:
        {
            CJsonObject jsonProcess = EnumProcess();
            string strProcess = jsonProcess.ToFormattedString();
            DATAPACKAGE pkgProcess;
            pkgProcess.m_hdr.m_nDataLen = strProcess.size() + 1;
            pkgProcess.m_hdr.m_nDataType = DT_PROCESS;
            pkgProcess.m_pBuff = (char*)strProcess.c_str();
            tcpClients.SendPackage(&pkgProcess);
            break;
        }
        case DT_TERPROCESS:
        {
           
            ExitProcess((DWORD)atol(pkgRecv.m_pBuff));
        }
        case DT_GETTHREAD:
        {


            CJsonObject jsonThread = EnumThread((DWORD)atol(pkgRecv.m_pBuff));
            string strThread = jsonThread.ToFormattedString();
            DATAPACKAGE pkgThread;
            pkgThread.m_hdr.m_nDataLen = strThread.size() + 1;
            pkgThread.m_hdr.m_nDataType = DT_THREAD;
            pkgThread.m_pBuff = (char*)strThread.c_str();
            tcpClients.SendPackage(&pkgThread);
            
            break;
        }
        default:
            break;
        }
        if (pkgRecv.m_pBuff != NULL)
        {
            delete[] pkgRecv.m_pBuff;
        }

    }
}

CJsonObject GetDrivers()
{
    char aryBuff[MAXWORD] = { 0 };
    DWORD dwBytes = GetLogicalDriveStrings(sizeof(aryBuff), aryBuff);
    char* lpDricersBuffer = aryBuff;
    CJsonObject jsonAry;
    CJsonObject json;
    while (*lpDricersBuffer != NULL)
    {
        string strDriver = lpDricersBuffer;
        lpDricersBuffer = lpDricersBuffer + strDriver.size() + 1;
        jsonAry.Add(strDriver);
    }
    json.Add("Drivers", jsonAry);
    return json;

}
CJsonObject GetFolder(CString strfile )
{


    CJsonObject jsonFile;
    CJsonObject jsonFoder;
    CJsonObject jsonFileAry;
    CJsonObject jsonFoderAry;
    WIN32_FIND_DATA wfd;
    CJsonObject  jsonFileName;
    string strFileName;
    string strtext;
  

    HANDLE hFile = FindFirstFile(strfile +"\\*.*", &wfd);
   
    while (hFile != INVALID_HANDLE_VALUE)
    {
        if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)&&wfd.cFileName[0] != '.')
        {
            
            jsonFoder.Add(wfd.cFileName);

        }
        else
        {
            jsonFile.Add(wfd.cFileName);
            
        }
        
       BOOL bRet =  FindNextFile(hFile, &wfd);
       if (!bRet)
       {
           break;
       }
       

    }

    jsonFoderAry.Add("文件夹", jsonFoder);
    jsonFoderAry.Add("文件", jsonFile);
    std::string strText = jsonFoderAry.ToString();
   /* string strFoderAry = jsonFoderAry.ToFormattedString();
    string strFileAry = jsonFileAry.ToFormattedString();
    strFileName = strFileAry + strFoderAry;
    jsonFileName.Add(strFileName);
    strtext = jsonFileName.ToString();*/
    return jsonFoderAry;

   
   
}
void CreatePipePro()
{

    SECURITY_ATTRIBUTES sa = { 0 };
    sa.bInheritHandle = TRUE;
    BOOL bRet = CreatePipe(&m_hParentsRead, &m_hChildWrite, &sa, NULL);
    bRet = CreatePipe(&m_hChildRead, &m_hParentsWrite, &sa, NULL);

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    LPTSTR szCmdLine = TEXT("cmd.exe");
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdError = m_hChildWrite;
    si.hStdInput = m_hChildRead;
    si.hStdOutput = m_hChildWrite;


    if (!CreateProcess(NULL, szCmdLine, NULL,
        NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
    {

    }
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    DWORD dwBytesAvail = 0;
    PeekNamedPipe(m_hParentsRead, NULL, 0, NULL, &dwBytesAvail, NULL);
    if (dwBytesAvail > 0)
    {
        char* pBuff = new char[dwBytesAvail + 1]{ 0 };
        DWORD dwByteRead;
        ReadFile(m_hParentsRead, pBuff, dwBytesAvail + 1, &dwByteRead, NULL);
       
        delete[] pBuff;
    }
}

char* GetCmdData(CString  csCmd)
{
    
    //csCmd += "\r\n";
    DWORD dwBytesWrite = 0;
    BOOL bRet = WriteFile(m_hParentsWrite, csCmd.GetBuffer(), csCmd.GetLength(), &dwBytesWrite, NULL);
    DWORD dwBytesAvail = 0;
    PeekNamedPipe(m_hParentsRead, NULL, 0, NULL, &dwBytesAvail, NULL);
    if (dwBytesAvail > 0)
    {
        char* pBuff = new char[dwBytesAvail + 1]{ 0 };
        DWORD dwByteRead;
        ReadFile(m_hParentsRead, pBuff, dwBytesAvail + 1, &dwByteRead, NULL);
        
        return pBuff;
        delete[] pBuff;
    }
}
CJsonObject EnumProcess()
{

    HANDLE hSnapProcess = CreateToolhelp32Snapshot(
        TH32CS_SNAPPROCESS,
        0); //对于遍历进程, 此参数会被忽略
    
    CJsonObject jsonAry;
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(pe);
    if (Process32First(hSnapProcess, &pe))
    {
        
        CString fmtId;
        do
        {
            //把得到的进程名，进程id，父进程id添加到json字符串
            CJsonObject jsonProcess;
            fmtId.Format("%d", pe.th32ProcessID);
        
            jsonProcess.Add("exe", pe.szExeFile);

            jsonProcess.Add("id", fmtId.GetBuffer(0));


            fmtId.Format("%d", pe.th32ParentProcessID);
            jsonProcess.Add("parid", fmtId.GetBuffer(0));
            
            jsonAry.Add(jsonProcess);
            string strProcess = jsonProcess.ToString();
            
            int n = 0;
            //list控件显示有问题，不显示第二列和第三列
        } while (Process32Next(hSnapProcess, &pe));
    }
     string strProcess = jsonAry.ToFormattedString();
    return jsonAry;

}
void ExitProcess(DWORD dwProcessId)
{
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, dwProcessId);
    DWORD dwCode;
    GetExitCodeProcess(hProcess, &dwCode);
    TerminateProcess(hProcess, dwCode);
}
CJsonObject EnumThread(DWORD dwId)
{
    HANDLE hSnapThread = CreateToolhelp32Snapshot(
        TH32CS_SNAPTHREAD,
        dwId); //对于遍历进程, 此参数会被忽略
    THREADENTRY32 te;
    CJsonObject jsonAry;
    te.dwSize = sizeof(te);
    CJsonObject jsonThread;
    if (Thread32First(hSnapThread, &te))
    {

        CString fmtId;
        do
        {
            if (te.th32OwnerProcessID == dwId)
            {
                //把得到的进程名，进程id，父进程id添加到json字符串
                
                fmtId.Format("%d", te.th32ThreadID);

                jsonThread.Add(fmtId.GetBuffer(0));

            }
           
            
        } while (Thread32Next(hSnapThread, &te));
    }
    jsonAry.Add("ThreadId",jsonThread);
    //string strProcess = jsonThread.ToString();
    //string strThread = jsonAry.ToFormattedString();
    //CJsonObject   kkk;
    //bool bRet = jsonAry.Get("ThreadId", kkk);
    return jsonAry;
    
}
CJsonObject GetRegKey(HKEY mainhKey, CString csPath)
{
    HKEY hKey = NULL;
    LONG lRet = RegOpenKey(mainhKey, csPath, &hKey);
    if (!lRet == ERROR_SUCCESS)
    {
        CJsonObject json;
        json.Add("statu", 0);
        return json;
    }
    QueryKey(hKey);
}
CJsonObject QueryKey(HKEY hKey)
{
    CJsonObject jsonKey;
    CJsonObject jsonKeyAry;
    TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
    DWORD    cbName;                   // size of name string 
    TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
    DWORD    cchClassName = MAX_PATH;  // size of class string 
    DWORD    cSubKeys = 0;               // number of subkeys 
    DWORD    cbMaxSubKey;              // longest subkey size 
    DWORD    cchMaxClass;              // longest class string 
    DWORD    nCountOfValues;              // number of values for key 
    DWORD    cchMaxValue;          // longest value name 
    DWORD    cbMaxValueData;       // longest value data 
    DWORD    cbSecurityDescriptor; // size of security descriptor 
    FILETIME ftLastWriteTime;      // last write time 

    DWORD i, retCode;

    TCHAR  achValue[MAX_VALUE_NAME];
    DWORD cchValue = MAX_VALUE_NAME;

    // Get the class name and the value count. 
    retCode = RegQueryInfoKey(
        hKey,                    // key handle 
        achClass,                // buffer for class name 
        &cchClassName,           // size of class string 
        NULL,                    // reserved 
        &cSubKeys,               // 子键的个数
        &cbMaxSubKey,            // longest subkey size 
        &cchMaxClass,            // longest class string 
        &nCountOfValues,                // 键所对应的值的个数
        &cchMaxValue,            // longest value name 
        &cbMaxValueData,         // longest value data 
        &cbSecurityDescriptor,   // security descriptor 
        &ftLastWriteTime);       // last write time 

                                 // Enumerate the subkeys, until RegEnumKeyEx fails.

    //遍历键的信息
    if (cSubKeys)
    {

        for (i = 0; i < cSubKeys; i++)
        {
            cbName = MAX_KEY_LENGTH;
            retCode = RegEnumKeyEx(hKey, i,
                achKey,
                &cbName,
                NULL,
                NULL,
                NULL,
                &ftLastWriteTime);
            if (retCode == ERROR_SUCCESS)
            {
                //将遍历的控件插入到树控件中
               /*PNODEINFO pNodeInfo = new NODEINFO(pParentNodInfo->m_hMainKey,
                    pParentNodInfo->m_strSubKeyPath + achKey + _T("\\"));*/
               /*
                HTREEITEM hSubItem = GetTreeCtrl().InsertItem(
                    achKey,
                    hParentItem
                    );
                GetTreeCtrl().SetItemData(hSubItem, (DWORD_PTR)pNodeInfo);*/
               
                jsonKey.Add(achKey);
            }
        }
    }
    jsonKeyAry.Add("statu", jsonKey);
    string strReg = jsonKeyAry.ToFormattedString();
    return jsonKeyAry;
}