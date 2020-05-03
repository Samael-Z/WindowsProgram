#include "CMyChatServer.h"

BOOL CMyChatServer::CreateServer(char* szIp, u_short nPort)
{

    BOOL bRet = m_iocpSocket.CreateIocp(szIp, nPort);
    if (!bRet)
    {
        printf("socket 创建失败, 请重启!");
        return FALSE;
    }

    m_iocpSocket.PostAccept();
    return TRUE;
}

BOOL CMyChatServer::RunServer()
{
    //接受来自客户端的数据
    while (TRUE)
    {
        //等待iocp处理完请求
        CIOCPSocket::SocketInfo* pSocketInfo = NULL;
        int nRetIoEvent = m_iocpSocket.WaitForIoEvent(pSocketInfo);

        //针对不同的请求做处理
        switch (nRetIoEvent)
        {
        case CIOCPSocket::IO_RECV:
            HandleClientData(pSocketInfo);
        default:
            break;
        }

    }
    return TRUE;
}
DWORD CMyChatServer::HandleClientData(CIOCPSocket::SocketInfo* pSocketInfo)
{
    //接受数据包
    DATAPACKAGE pkgRecv;
    BOOL bRet = RecvPackage(pSocketInfo, &pkgRecv);
    if (!bRet)
    {
        //数据包不完整, 不做处理
        return 0;
    }

    //判断数据的类型
    switch (pkgRecv.m_hdr.m_nDataType)
    {


    case DT_LOGIN:	//处理登录
    {
        //保存新客户端到链表
         m_lstClients.push_back(new ClinentInfo(pSocketInfo));
        //向客户端发送登录结果
         DATAPACKAGE package;
         package.m_hdr.m_nDataType = DT_FLUSH_NAME;
         package.m_hdr.m_nDataLen = 0;
         package.m_pBuff = NULL;
       
        for (auto ci : m_lstClients)
        {
            BOOL bRet = SendPackage(ci->m_pSocketInfo, &package);
           
        }
       

        break;
    }
    case DT_ADD_NAME:
    {
        for (auto ci : m_lstClients)
        {
            BOOL bRet = SendPackage(ci->m_pSocketInfo, &pkgRecv);
        }
        break;
    }
    case DT_DEL_NAME:
    {
        for (auto ci : m_lstClients)
        {
            BOOL bRet = SendPackage(ci->m_pSocketInfo, &pkgRecv);
        }
        break;
    }
    case DT_MSG:	//处理聊天消息
    {
        //判断是群发还是私聊
        neb::CJsonObject json;
        json.Parse(pkgRecv.m_pBuff);

        string strType;
        json.Get("type", strType);
        pkgRecv.m_hdr.m_nDataType = DT_MSG;
        if (strType == "public")//群发
        {
            //将消息转发给其他客户端
            for (auto ci : m_lstClients)
            {
              
                BOOL bRet = SendPackage(ci->m_pSocketInfo, &pkgRecv);
            }
        }
        else if (strType == "private") //私聊 
        {
          
            //将消息转发给其他客户端
            for (auto ci : m_lstClients)
            {
                BOOL bRet = SendPackage(ci->m_pSocketInfo, &pkgRecv);
            }
        }

        break;
    }

   
    case DT_LOGINOUT: //处理下线
    {
        //删除下线的客户端
        for (auto itr = m_lstClients.begin(); itr != m_lstClients.end(); ++itr)
        {
            if ((*itr)->m_pSocketInfo->m_sock == pSocketInfo->m_sock)
            {
                m_lstClients.erase(itr);
                delete pSocketInfo;
                return 0;
            }
        }

        break;
    }
    }

    //删除缓冲区
    if (pkgRecv.m_pBuff != NULL)
    {
        delete[] pkgRecv.m_pBuff;
        pkgRecv.m_pBuff = NULL;
    }
}
BOOL CMyChatServer::RecvPackage(CIOCPSocket::SocketInfo* pSocketInfo, DATAPACKAGE* pPackage)
{
    //判断缓冲区中的数据是否够一个包处理
    pSocketInfo->m_lockRecv.Lock();
    if (pSocketInfo->m_buffRecv.GetSize() >= sizeof(PACKAGEHEADER))
    {
        pSocketInfo->m_buffRecv.Peek((char*)&pPackage->m_hdr, sizeof(pPackage->m_hdr));
        if (pSocketInfo->m_buffRecv.GetSize() >= pPackage->m_hdr.m_nDataLen + sizeof(pPackage->m_hdr))
        {
            //从缓冲区中取出一个完整的数据包
            pSocketInfo->m_buffRecv.Read((char*)&pPackage->m_hdr, sizeof(pPackage->m_hdr));
            pPackage->m_pBuff = new char[pPackage->m_hdr.m_nDataLen];
            pSocketInfo->m_buffRecv.Read(pPackage->m_pBuff, pPackage->m_hdr.m_nDataLen);
            pSocketInfo->m_lockRecv.UnLock();
            return TRUE;
        }
    }
    pSocketInfo->m_lockRecv.UnLock();
    return FALSE;
}
BOOL CMyChatServer::SendPackage(CIOCPSocket::SocketInfo* pSocketInfo, DATAPACKAGE* pPackage)
{
    //将数据写入发送缓冲区中
    pSocketInfo->m_lockSend.Lock();
    pSocketInfo->m_buffSend.Write((char*)&pPackage->m_hdr, sizeof(pPackage->m_hdr));
    pSocketInfo->m_buffSend.Write(pPackage->m_pBuff, pPackage->m_hdr.m_nDataLen);
    pSocketInfo->m_lockSend.UnLock();

    //投递send请求
    m_iocpSocket.PostSend(pSocketInfo);
    return TRUE;
}
