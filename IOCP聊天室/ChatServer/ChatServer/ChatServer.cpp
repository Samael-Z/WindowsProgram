// ChatServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include "CMyChatServer.h"
int main()
{
   
    CMyChatServer tcpServer;
    tcpServer.CreateServer("127.0.0.1",12138);
    tcpServer.RunServer();


    return 0;
}

