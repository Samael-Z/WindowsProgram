#pragma once
#include <Windows.h>
#include "comman.h"
typedef struct tagDataPackage
{
    PACKAGEHEADER m_hdr;  //数据包的包头
    char* m_pBuff = NULL;        //数据包的包体

}DATAPACKAGE, * PDATAPACKAGE;






class CTcpSocket
{
public:
    BOOL CreateSocket();
    BOOL BindListen(char* szIp, u_short nPort);
    BOOL Accept(CTcpSocket* pTcpSocket);
    BOOL Recv(char* pBuff, int* pnLen);
    BOOL Send(char* pBuff, int* pnLen);
    BOOL Connect(char* szIp, u_short nPort);
    BOOL RecvPackage(DATAPACKAGE* pPackage);
    BOOL SendPackage(DATAPACKAGE* pPackage);

    void CloseSocket();
    
    const sockaddr_in& GetSocketAddrIn() const;
private:
    SOCKET m_socket = INVALID_SOCKET;
    //SOCKET m_socketClients = INVALID_SOCKET;
    sockaddr_in m_siClientSocket;
public:
    HWND   m_Cmdhwnd;
    HWND   m_Processhwnd;
    HWND   m_Reghwnd;

};

