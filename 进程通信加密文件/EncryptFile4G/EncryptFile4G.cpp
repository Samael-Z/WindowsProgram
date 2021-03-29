
#include <iostream>
#include <windows.h>
using namespace  std;
#pragma comment(lib,"SharePath.lib")
__declspec(dllimport)  extern	char g_szSourcePath[MAX_PATH];
__declspec(dllimport)  extern	char g_szTargetPath[MAX_PATH];
//文件映射写文件
int main()
{
   // g_szSourcePath[MAX_PATH] = "D:\\cr39\\二阶段\\进程通信加密文件\\VM.exe" ;
    string strpath = "E:\\工具包\\工具包.rar";
    string strTarpath = "D:\\cr39\\二阶段\\进程通信加密文件\\1.rar";
    //打开源文件的文件句柄
    HANDLE hSoucerFile = CreateFile(strpath.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (hSoucerFile == INVALID_HANDLE_VALUE)
    {
        //检查文件句柄是否成功
        return 0;
    }
    //创建一个新文件
    HANDLE hTargetFile = CreateFile(strTarpath.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (hTargetFile == INVALID_HANDLE_VALUE)
    {
        //检查文件句柄是否成功
        return 0;
    }
    //每次读取64K
    const DWORD dwPerSize = 0x10000;
    char aryBuff[dwPerSize] = { 0 };

    //获取文件的大小
    __int64 nTotalFileSize = 0;
    GetFileSizeEx(hSoucerFile, (PLARGE_INTEGER)&nTotalFileSize);

    //已经处理的数据的大小
    __int64 nHanleFileSize = 0;
     HANDLE hFileMap = CreateFileMapping(hSoucerFile, NULL, PAGE_READWRITE, 0, 0, NULL);

     if (hFileMap == NULL)
     {
         //检查文件句柄是否成功
         return 0;
     }

    while (nHanleFileSize <= nTotalFileSize)
    {
        //创建映射文件对象

      
        //映射文件视图
       LPVOID  MapView = MapViewOfFile(hFileMap, FILE_MAP_WRITE, 0, 0, 0);
       DWORD dw =  GetLastError();
    }
   
    return 0;
}


