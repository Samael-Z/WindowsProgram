#include "pch.h"
#include "InitSocket.h"

InitSocket::InitSocket()
{
    //≥ı ºªØ
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        return;
    }
    if (LOBYTE(wsaData.wVersion) != 2 ||
        HIBYTE(wsaData.wVersion) != 2) {
        WSACleanup();
        return;
    }
}

InitSocket::~InitSocket()
{
    WSACleanup();
}
InitSocket  InitSocket:: _InitSock;