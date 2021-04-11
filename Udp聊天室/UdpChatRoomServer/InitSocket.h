#pragma once
#include <Winsock2.h>
#include <Windows.h>
#pragma  comment(lib,"Ws2_32.lib")
class InitSocket
{
public:
    InitSocket();
    ~InitSocket();

private:
    static InitSocket  _InitSock;
};

