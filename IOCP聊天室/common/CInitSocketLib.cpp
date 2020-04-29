
#include "CInitSocketLib.h"

CInitSocketLib::CInitSocketLib()
{
    //初始化协议版本号
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

CInitSocketLib::~CInitSocketLib()
{
    //清除协议
    WSACleanup();
}
CInitSocketLib CInitSocketLib::m_socket;