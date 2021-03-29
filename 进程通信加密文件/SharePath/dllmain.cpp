// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#pragma data_seg("SHAREPATH") //共享空间的开始
__declspec(dllexport) char g_szSourcePath[MAX_PATH] = { 0 };//源文件路径
__declspec(dllexport) char g_szTargetPath[MAX_PATH] = { 0 };//目的文件路径
__declspec(dllexport) DWORD g_dwPos = 0;  //进度条的步长，共享段的变量要初始化
//这种是直接在共享段里面声明导出变量的，正常应该新建两个.h和.cpp文件，头文件声明变量，实现文件实现变量
#pragma data_seg()//共享空间的结束
#pragma comment(linker, "/SECTION:SHAREPATH,RWS")

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

