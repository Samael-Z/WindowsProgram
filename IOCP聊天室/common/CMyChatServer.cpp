#include "CMyChatServer.h"

BOOL CMyChatServer::CreateServer(char* szIp, u_short nPort)
{

    BOOL bRet = m_TcpSocket.CreateSocket();
    if (!bRet)
    {
        return FALSE;
    }
    bRet = m_TcpSocket.BindListen(szIp, nPort);
    if (!bRet)
    {
        return FALSE;
    }
    return TRUE;
}

BOOL CMyChatServer::RunServer()
{
    //接受链接
    while (true)
    {
        CTcpSocket* pTcpsocket = new CTcpSocket();
        ClinentInfo* pCI = new ClinentInfo(clock(), pTcpsocket, &m_listClients);
        BOOL bRet = m_TcpSocket.Accept(pCI->m_pTcpSocketClients);
        if (!bRet)
        {
            break;
        }
        printf("IP: %s  Port: %d  连接到服务器\r\n", inet_ntoa(pCI->m_pTcpSocketClients->GetSocketAddrIn().sin_addr),
            ntohs(pCI->m_pTcpSocketClients->GetSocketAddrIn().sin_port));
       //m_ClientAddr.push_back(pCI->m_pTcpSocketClients->GetSocketAddrIn());
       m_listClients.push_back(pCI);
        HANDLE hThread = CreateThread(NULL, 0, HandleClientThreadProc, (LPVOID)pCI, 0, NULL);
        CloseHandle(hThread);
    }
   
    return TRUE;
}
DWORD __stdcall CMyChatServer::HandleClientThreadProc(LPVOID lpParam)
{

    ClinentInfo* pCI = (ClinentInfo*)lpParam;
    while (true)
    {
        
        DATAPACKAGE pkgRecv;
       
        BOOL bRet = pCI->m_pTcpSocketClients->RecvPackage(&pkgRecv);
        if (!bRet)
        {
            printf("接收数据包失败");
        }


        //判断数据的类型
        switch (pkgRecv.m_hdr.m_nDataType)
        {
        case DT_LOGIN:
        {
            pCI->m_clockHeartTime = clock();
            //向客户端发送登陆结果
            DATAPACKAGE packageLogin;
            packageLogin.m_hdr.m_nDataType = DT_LOGIN_OK;
            packageLogin.m_hdr.m_nDataLen = pkgRecv.m_hdr.m_nDataLen;
            packageLogin.m_pBuff = pkgRecv.m_pBuff;

            /*for (auto itr = pCI->m_pClientAddr->begin(); itr != pCI->m_pClientAddr->end(); ++itr )
            {

            }*/
            pCI->m_pTcpSocketClients->SendPackage(&packageLogin);
            

            break;
        }

        case DT_MSG:
        {
            //json解析，判断类型是public还是private
            CJsonObject json;
            json.Parse(pkgRecv.m_pBuff);
            string strType;
            json.Get("type", strType);
            if (strType == "public") //群发
            {

                for (auto ci : *(pCI->m_plistClientInFo))
                {
                    DATAPACKAGE packageMsg;
                    packageMsg.m_hdr.m_nDataType = DT_MSG;
                    packageMsg.m_hdr.m_nDataLen = pkgRecv.m_hdr.m_nDataLen;
                    packageMsg.m_pBuff = pkgRecv.m_pBuff;
                    bRet = ci->m_pTcpSocketClients->SendPackage(&packageMsg);
                }
            }
            if (strType == "private") //私聊
            {
                for (auto ci : *(pCI->m_plistClientInFo))
                {
                    DATAPACKAGE packageMsg;
                    packageMsg.m_hdr.m_nDataType = DT_MSG;
                    packageMsg.m_hdr.m_nDataLen = pkgRecv.m_hdr.m_nDataLen;
                    packageMsg.m_pBuff = pkgRecv.m_pBuff;
                    bRet = ci->m_pTcpSocketClients->SendPackage(&packageMsg);
                }
            }
            if (pkgRecv.m_pBuff != NULL)
            {
                delete[] pkgRecv.m_pBuff;
            }
            break;
        }
        //接收客户端退出数据包
        case DT_LOGINOUT:
        {
            for (auto itr = pCI->m_plistClientInFo->begin(); itr != pCI->m_plistClientInFo->end(); ++itr)
            {
                if ((*itr) == pCI)
                {
                   
                    printf("ip:%s 端口号：%d 下线了  \r\n",
                        inet_ntoa(pCI->m_pTcpSocketClients->GetSocketAddrIn().sin_addr),
                        ntohs(pCI->m_pTcpSocketClients->GetSocketAddrIn().sin_port));
                    pCI->m_plistClientInFo->erase(itr);
                    delete pCI;
                    break;
                }
            }

            break;
        }

        }

    }
}