#include "CIocpSocket.h"

CIocpSocket::CIocpSocket()
{
}

CIocpSocket::~CIocpSocket()
{
}
BOOL CIocpSocket::CreateIocp(char* szIp, u_short nProt)
{
    m_sockServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in siServer;
    siServer.sin_family = AF_INET;
    siServer.sin_addr.S_un.S_addr = inet_addr(szIp);
    siServer.sin_port = htons(nProt);
    int nRet = bind(m_sockServer, (sockaddr*)&siServer, sizeof(siServer));
    if (nRet == SOCKET_ERROR)
    {
        return FALSE;
    }
    nRet = listen(m_sockServer, SOMAXCONN /*5*/);
    if (nRet == SOCKET_ERROR)
    {
        return FALSE;
    }
    //创建IOCP对象
    //第四个参数填0，自动根据计算机的硬件匹配适合的线程个数
    m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
    if (m_hIocp == NULL)
    {
       
        return FALSE;
    }

    //把socket注册给IOCP,把服务端的socket在IOCP的队列里面关联起来
    //这个API的返回值和第此调用时候的一样
    CreateIoCompletionPort((HANDLE)m_sockServer, m_hIocp, NULL, 0);
    return TRUE;
}
//向iocp投递一个接收链接的请求
BOOL CIocpSocket::PostAccept()
{
    SOCKET sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    SocketInfo* pSocketInfo = new SocketInfo(sockClient);
    char szBuff[MAXWORD] = { 0 };
    DWORD dwBytes = 0;
    MYOVERLAPPED* pOverlapped = new MYOVERLAPPED(pSocketInfo, IO_ACCEPT);
    BOOL bRet = AcceptEx(
        m_sockServer,//用来监听的服务端的socket
        sockClient,//客户端的socket
        szBuff,
        0,
        sizeof(sockaddr_in) + 16,
        sizeof(sockaddr_in) + 16,
        &dwBytes,

        (OVERLAPPED*)pOverlapped

        );
    //把接收链接的socket和注册到IOCP队列中
    HANDLE hSocketClient = CreateIoCompletionPort((HANDLE)sockClient, m_hIocp, NULL, 0);

    return bRet;
}
BOOL CIocpSocket::PostRecv(SocketInfo* pSocketInfo)
{

    MYOVERLAPPED* pOverlapped = new MYOVERLAPPED{ pSocketInfo ,IO_RECV };
    pOverlapped->m_wsaBuffRecv.buf = pOverlapped->m_SendBuffTmp;
    pOverlapped->m_wsaBuffRecv.len = MAXWORD;

    DWORD dwFlag = 0;
    int nRet = WSARecv(
        pSocketInfo->m_socket,
        &pOverlapped->m_wsaBuffRecv,
        1,  //缓冲区的WSABUF个数，一般给1
        0,
        &dwFlag,
        (OVERLAPPED*)pOverlapped,
        NULL
        );
    if (nRet == SOCKET_ERROR && WSAGetLastError() == WSA_IO_PENDING)
    {
        return true;
    }
    if (nRet == 0)
    {
        return TRUE;
    }
    return FALSE;
}
BOOL CIocpSocket::PostSend(SocketInfo* pSocketInfo)
{
    //如果缓冲区中没有数据，直接返回
    pSocketInfo->m_locSend.Lock();
    if (pSocketInfo->m_SendBuffer.GetSize() <= 0)
    {
        pSocketInfo->m_locSend.UnLock();
        return FALSE;
    }
    pSocketInfo->m_locSend.UnLock();

    MYOVERLAPPED* pOverlapped = new MYOVERLAPPED{ pSocketInfo ,IO_SEND };
    pOverlapped->m_wsaBuffSend.buf = new char[pSocketInfo->m_SendBuffer.GetSize()];
    pOverlapped->m_wsaBuffSend.len = pSocketInfo->m_SendBuffer.GetSize();
    pSocketInfo->m_SendBuffer.Read(pOverlapped->m_wsaBuffSend.buf, pOverlapped->m_wsaBuffSend.len);
    pSocketInfo->m_locSend.UnLock();
    int nRet = WSASend(
        pSocketInfo->m_socket,
        &pOverlapped->m_wsaBuffSend,
        1,
        NULL,
        0,
        (OVERLAPPED*)pOverlapped,
        NULL
        );
    if (nRet == SOCKET_ERROR && WSAGetLastError() == WSA_IO_PENDING)
    {
        return TRUE;
    }
    if (nRet == 0)
    {
        return TRUE;
    }
    return FALSE;
}

int CIocpSocket::WaitForEvent(SocketInfo*& pSocketInfo)
{
    DWORD dwBytesRecv = 0;//如果是读取请求，则返回读取的数据的个数
    ULONG_PTR  ulCompletionKey = NULL;
    MYOVERLAPPED* pOverLapped = NULL;
    BOOL bRet = GetQueuedCompletionStatus(
        m_hIocp,
        &dwBytesRecv,
        &ulCompletionKey,
        (LPOVERLAPPED*)&pOverLapped,
        INFINITE
        );

    pSocketInfo = pOverLapped->m_pSocketInfo;
    int nRetIoEvent = pOverLapped->m_ioEvent;

    switch (pOverLapped->m_ioEvent)
    {
    case IO_ACCEPT:
    {
        PostRecv(pOverLapped->m_pSocketInfo);
        PostAccept();
        break;
    }
    case IO_RECV:
    {
        //先将接受的数据放到接收缓冲区
        pOverLapped->m_pSocketInfo->m_RecvBuffer.Write(pOverLapped->m_SendBuffTmp,dwBytesRecv);
        PostRecv(pOverLapped->m_pSocketInfo);
        break;
    }
    case IO_SEND:
    {
        PostSend(pOverLapped->m_pSocketInfo);
        break;
    }
    default:
        break;
    }
    delete pOverLapped;
    return nRetIoEvent;
}
