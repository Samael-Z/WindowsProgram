
#include <iostream>
#include <windows.h>
using namespace  std;
#pragma comment(lib,"SharePath.lib")
__declspec(dllimport)  extern	char g_szSourcePath[MAX_PATH];
__declspec(dllimport)  extern	char g_szTargetPath[MAX_PATH];
__declspec(dllimport)  extern   DWORD g_dwPos;
//文件映射写文件
int main()
{
   // g_szSourcePath[MAX_PATH] = "D:\\cr39\\二阶段\\进程通信加密文件\\VM.exe" ;
  //  string strpath = "D:\\cr39\\二阶段\\进程通信加密文件\\用普通的文件操作进行大文件的加密文件的思路和实现.pdf";
   // string strTarpath = "D:\\cr39\\二阶段\\进程通信加密文件\\wode.pdf";
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

    HANDLE hFileMap = CreateFileMapping(hSoucerFile, NULL, PAGE_READWRITE, 0, 0, NULL);

    if (hFileMap == NULL)
    {
        //检查文件句柄是否成功
        return 0;
    }
    // 得到系统分配粒度
    SYSTEM_INFO SysInfo;
    GetSystemInfo(&SysInfo);
    DWORD dwGran = SysInfo.dwAllocationGranularity;
    // 得到文件尺寸
    DWORD dwFileSizeHigh;
    __int64 qwFileSize = GetFileSize(hSoucerFile, &dwFileSizeHigh);
    qwFileSize += (((__int64)dwFileSizeHigh) << 32);
    CloseHandle(hSoucerFile);
    __int64 qwFileOffset = 0;
    __int64 qwNumOfOs = 0;
    __int64 qwFileSizeOld = qwFileSize;
    while (qwFileSize >0)
    {
        DWORD dwBytesInBlock = SysInfo.dwAllocationGranularity;
        if (qwFileSize < SysInfo.dwAllocationGranularity)
        {
            dwBytesInBlock = (DWORD)qwFileSize;
        }
        PBYTE pbFile = (PBYTE)MapViewOfFile(hFileMap, FILE_MAP_READ, (DWORD)(qwFileOffset >> 32),
            (DWORD)(qwFileOffset & 0xffffffff),
            dwBytesInBlock
        );


       BOOL bWrite =  WriteFile(hTargetFile, pbFile, dwBytesInBlock, NULL, NULL);

        //文件操作
        UnmapViewOfFile(pbFile);
        //下一块偏移
        qwFileOffset += dwBytesInBlock;
        qwFileSize -= dwBytesInBlock;

        double dblHandleSize = qwFileOffset;
        double dblTotaleSize = qwFileSizeOld;
        int nPos = (int)(dblHandleSize / dblTotaleSize * 100);
        g_dwPos = nPos;

       // g_dwPos = (qwFileOffset * 100) / qwFileSizeOld;
     
    }
    CloseHandle(hTargetFile);
    CloseHandle(hFileMap);
    return 0;

   #if 0
// qwFileSize |= (((__int64)dwFileSizeHigh) << 32);
    // 关闭文件对象
    CloseHandle(hSoucerFile);
    // 偏移地址
    __int64 qwFileOffset = 0x10000;
    // 块大小
   /* DWORD dwBlockBytes = 1000 * dwGran;
    if (qwFileSize < 1000 * dwGran)
        dwBlockBytes = (DWORD)qwFileSize;*/
    //while (qwFileOffset > 0)
    while (1)
    {
        // 映射视图
        DWORD offset = 64 * 1024;
        LPBYTE lpbMapAddress = (LPBYTE)MapViewOfFile(hFileMap, FILE_MAP_READ,
            (DWORD)0,
            (DWORD)(offset & 0xFFFFFFFF),
            offset
        );


        /* LPBYTE lpbMapAddress = (LPBYTE)MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS,
             (DWORD)(0), (DWORD)(dwGran),
             dwGran);*/
        if (lpbMapAddress == NULL)
        {
           
            return 0;
        }
        // 对映射的视图进行访问
       /* for (DWORD i = 0; i < dwBlockBytes; i++)
            BYTE temp = *(lpbMapAddress + i);*/

        // 撤消文件映像
        UnmapViewOfFile(lpbMapAddress);
        // 修正参数
        //qwFileOffset += dwGran;
        ////qwFileSize -= dwBlockBytes;
        //qwFileSize -= qwFileOffset;
    
    }
    // 关闭文件映射对象句柄
    CloseHandle(hFileMap);
    return 0;
#endif
   
}






   #if 0
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
   
#endif
    


