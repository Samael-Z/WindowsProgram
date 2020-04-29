// TonyTeacher.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <queue>
#include <time.h>
//#include <Windows.h>
#include <string>
#include <afx.h>
using namespace std;
queue<CString> g_Customers;
DWORD WINAPI ThreadProc(LPVOID lpParamater)
{
    HANDLE hEvent = (HANDLE)lpParamater;
    BOOL bRet = TRUE;
    while (bRet)
    {
        if (g_Customers.empty())
        {
            //Sleep(3000);
            system("cls");
            printf("所有顾客都已经理发结束,明天再见！\r\n");
            break;
        }
        WaitForSingleObject(hEvent, INFINITE);
        CString strWork = g_Customers.front();
        g_Customers.pop();
        printf("Tony老师%d号，正在为%s理发....\r\n\r\n", GetCurrentThreadId(), strWork);
        Sleep(1000);
        printf("Tony老师%d号理发结束，正在准备为下一位顾客理发！\r\n\r\n",GetCurrentThreadId());
        SetEvent(hEvent);
    }

    return 0;
}
int main()
{
    
       
        
       

    srand((unsigned)time(0));
    int nCustomer = rand() % 100 ;
    int nBarber = rand() % 4; 
    printf("    ★★★★★★★★Tony老师的理发店★★★★★★★★★★\r\n\r\n");
    printf("    ★★★★★★★★今天一共来了%d位顾客★★★★★★★\r\n\r\n", nCustomer);
    printf("    ★★★★★★★★今天有%d位理发师上班★★★★★★★★\r\n\r\n", nBarber);
    if (!nBarber)
    {
        printf("    不好意思今天休息，请明天在来！");
        return 0;
    }
    else
    {
        printf("    正在为顾客打扫房间，请稍等。。。。\r\n");
    }
    Sleep(3000);
    system("cls");
    CString strCust;
    CString strCustId;
    for (int i = 1; i < nCustomer; i++)
    {
        strCustId.Format("%d", i);
        strCust = "顾客" + strCustId;
        
        g_Customers.push(strCust);
       /* strCust = g_Customers.front();
        g_Customers.pop();
        printf("%s", strCust);*/
    }
    printf("顾客已经排好队伍，理发师请就位\r\n");
    printf("开始工作.....\r\n");
    Sleep(1000);
    HANDLE hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
    HANDLE aryThreads[4] = { NULL };
    for (int i = 0; i < nBarber; i++)
    {
        aryThreads[i] = CreateThread(NULL, 0, ThreadProc, hEvent, 0, NULL);
    }

    WaitForMultipleObjects(nBarber, aryThreads, TRUE, INFINITE);
   
    
    return 0;
}


