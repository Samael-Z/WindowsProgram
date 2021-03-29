#include <iostream>
#include <windows.h>
using namespace  std;
#pragma comment(lib,"SharePath.lib")
__declspec(dllimport)  extern	char g_szSourcePath[MAX_PATH];
__declspec(dllimport)  extern	char g_szTargetPath[MAX_PATH];
__declspec(dllimport)  extern	DWORD g_dwPos;
int main()
{
    //打开源文件的文件句柄
    HANDLE hSoucerFile = CreateFile(g_szSourcePath,
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
    HANDLE hTargetFile = CreateFile(g_szTargetPath,
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
    DWORD  dwRead = 0;
  
    DWORD  dwWrite = 0;
    while (true)
    {
        //读文件
        BOOL bRead = ReadFile(hSoucerFile, aryBuff, dwPerSize, &dwRead, NULL);
        if (!bRead)
        {
            return 0;
        }
        //加密数据
        for (int i = 0; i< dwPerSize -1;++i)
        {
            aryBuff[i] = aryBuff[i] ^ 1;
        }
        //把加密后的数据写入到目的文件中
        bRead = WriteFile(hTargetFile, aryBuff, dwRead, &dwWrite, NULL);
        if (!bRead)
        {
            return 0;
        }
        //判断文件是否读取完毕
        nHanleFileSize += dwWrite;

        //显示进度
        double dblHandleSize = nHanleFileSize;
        double dblTotaleSize = nTotalFileSize;
        int nPos = (int)(dblHandleSize / dblTotaleSize * 100);
        g_dwPos = nPos;
        //如果加密完成就退出循环
        if (nHanleFileSize >= nTotalFileSize)
        {
            break;
        }
      
    }

    return 0;
}
