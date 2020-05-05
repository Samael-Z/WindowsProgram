#include "CUdpSocket.h"

CUdpSocket::CUdpSocket()
{
}

CUdpSocket::~CUdpSocket()
{
}

BOOL CUdpSocket::CreateSocket(char* szIp, u_short uPort)
{
    m_SockServer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (m_SockServer == INVALID_SOCKET)
    {
        return FALSE;
    }
    sockaddr_in siServer;
    siServer.sin_family = AF_INET;
    siServer.sin_port = htons(uPort);
    siServer.sin_addr.S_un.S_addr = inet_addr(szIp);
    if (bind(m_SockServer,(sockaddr*)&siServer,sizeof(siServer)) == SOCKET_ERROR)
    {
        return FALSE;
    }
    m_nPort = ntohs(siServer.sin_port);
    return TRUE;
}

int CUdpSocket::RecvFrom(char* pBuff, int nSize, sockaddr_in* psi)
{
    int psiLen = sizeof(sockaddr_in);
   
    return recvfrom(m_SockServer, pBuff, nSize, 0, (sockaddr*)psi, &psiLen);
}

int CUdpSocket::SendTO(char* pBuff, int nSize, sockaddr_in* psi)
{
    return sendto(m_SockServer, pBuff, nSize, 0, (sockaddr*)psi, sizeof(sockaddr_in));
}

BOOL CUdpSocket::RecvPackage(DATAPACKAGE* pPackage, sockaddr_in* psi)
{
    
    int nRet = RecvFrom((char*)&pPackage->m_hdr, sizeof(pPackage->m_hdr), psi);
    if (nRet == 0|| nRet == SOCKET_ERROR)
    {
        return FALSE;
    }
    //如果发送的数据包不是登陆包 ，再接受数据、
    //否则登陆包的缓冲区里面没有内容，再new一个有长度的缓冲区空间会有问题

    if (pPackage->m_hdr.m_nDataType != DT_LOGINOUT)
    {
        if (pPackage->m_hdr.m_nDataLen != 0)
        {
            pPackage->m_pBuff = new char[pPackage->m_hdr.m_nDataLen];
            nRet = RecvFrom(pPackage->m_pBuff, pPackage->m_hdr.m_nDataLen, psi);
            if (nRet <= 0)
            {
                return FALSE;
            }
        }
    }
    
    return TRUE;
}

BOOL CUdpSocket::SendPackage(DATAPACKAGE* pPackage, sockaddr_in* psi)
{
    int nRet = SendTO((char*)&pPackage->m_hdr, sizeof(pPackage->m_hdr), psi);
    if ( nRet == SOCKET_ERROR)
    {
        return FALSE;
    }
    //如果发送的数据包不是登陆包 ，再发送数据、
    //否则登陆包的缓冲区里面没有内容，再new一个有长度的缓冲区空间会有问题
    if (pPackage->m_hdr.m_nDataType != DT_LOGINOUT)
    {
        if (pPackage->m_hdr.m_nDataLen != 0)
        {
            nRet = SendTO(pPackage->m_pBuff, pPackage->m_hdr.m_nDataLen, psi);
            if (nRet == SOCKET_ERROR)
            {
                return FALSE;
            }
        }
    }
    
    return TRUE;
}

void CUdpSocket::CloseSocket()
{
    closesocket(m_SockServer);
    m_SockServer = INVALID_SOCKET;
}
