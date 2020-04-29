#pragma once
#include <Windows.h>
#include "comman.h"
typedef struct tagDataPackage
{
    PACKAGEHEADER m_hdr;  //数据包的包头
    char* m_pBuff;        //数据包的包体

}DATAPACKAGE,*PDATAPACKAGE;
class CUdpSocket
{
public:

    CUdpSocket();
    ~CUdpSocket();
public:
    BOOL CreateSocket(char* szIp, u_short uPort = 0);
    int RecvFrom(char* pBuff, int nSize, sockaddr_in* psi);
    int SendTO(char* pBuff, int nSize, sockaddr_in* psi);
    BOOL RecvPackage(DATAPACKAGE* pPackage, sockaddr_in* psi);
    BOOL SendPackage(DATAPACKAGE* pPackage, sockaddr_in* psi);
    void CloseSocket();
    int m_nPort = 0;
private:
    SOCKET m_SockServer = INVALID_SOCKET;
};

