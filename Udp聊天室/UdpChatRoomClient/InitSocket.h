#pragma once
#include <Winsock2.h>
#include <Windows.h>
#pragma  comment(lib,"Ws2_32.lib")
#include <list>
#include <String>
using namespace  std;
class InitSocket
{
public:
    InitSocket();
    ~InitSocket();

private:
    static InitSocket  _InitSock;
};

