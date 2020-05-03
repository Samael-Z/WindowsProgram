#pragma once
#include "IOCPSocket.h"
#include <list>
#include "CJsonObject.hpp"
#include <string>
#include "CTcpSocket.h"
using namespace neb;
using namespace std;
class CMyChatServer
{
public:
    BOOL CreateServer(char* szIp, u_short nPort);
    BOOL RunServer();
private:
    DWORD HandleClientData(CIOCPSocket::SocketInfo* pSocketInfo);
    BOOL RecvPackage(CIOCPSocket::SocketInfo* pSocketInfo, DATAPACKAGE* pPackage);
    BOOL SendPackage(CIOCPSocket::SocketInfo* pSocketInfo, DATAPACKAGE* pPackage);
private:

    class ClinentInfo
    {
    public:
        ClinentInfo(CIOCPSocket::SocketInfo* pSocketInfo) :
            m_pSocketInfo(pSocketInfo)
        { }
        CIOCPSocket::SocketInfo* m_pSocketInfo;
    };
       
    

    CIOCPSocket m_iocpSocket;

    list<ClinentInfo*> m_lstClients;
    
};

