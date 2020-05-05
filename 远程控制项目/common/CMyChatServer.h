#pragma once
#include <list>
#include "CJsonObject.hpp"
#include <string>
#include <time.h>
#include "CTcpSocket.h"
using namespace neb;
using namespace std;
class CMyChatServer
{
public:
    BOOL CreateServer(char* szIp, u_short nPort);
    BOOL RunServer();
private:
    static DWORD WINAPI HandleClientThreadProc(LPVOID lpParam);
private:

    class ClinentInfo
    {
    public:
        ClinentInfo(clock_t ct, CTcpSocket* ptcpsock, list<ClinentInfo*>* plistinfo)
            :m_clockHeartTime(ct),
            m_pTcpSocketClients(ptcpsock),
            m_plistClientInFo(plistinfo)
        {

        }
        clock_t                     m_clockHeartTime;  //上一次发送心跳包时间
        CTcpSocket*                 m_pTcpSocketClients;
        list<ClinentInfo*>*          m_plistClientInFo;
    };


    CTcpSocket m_TcpSocket;
    list<ClinentInfo*> m_listClients;
};

