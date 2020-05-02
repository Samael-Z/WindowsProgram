#pragma once
#include "CIocpSocket.h"
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
    BOOL    RecvPackage(CIocpSocket::SocketInfo* pSocketInfo, DATAPACKAGE* pPackage);
    BOOL    SendPackage(CIocpSocket::SocketInfo* pSocketInfo, DATAPACKAGE* pPackage);
    DWORD   HandleClientData(CIocpSocket::SocketInfo* pSocketInfo);
private:

    class ClinentInfo
    {
    public:
        ClinentInfo(CIocpSocket::SocketInfo* pSocketintfo)
            
           : m_pSocketInfo(pSocketintfo)
        {
          
        }
        CIocpSocket::SocketInfo* m_pSocketInfo;
        list<ClinentInfo*>*          m_plistClientInFo;
       

    };


    CIocpSocket m_IocpSocket;
    list<ClinentInfo*> m_listClients;
    
};

