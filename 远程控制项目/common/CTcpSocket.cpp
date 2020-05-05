#include "CTcpSocket.h"

BOOL CTcpSocket::CreateSocket()
{
    //创建socket
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_socket == INVALID_SOCKET)
    {
        return FALSE;
    }
    return TRUE;
}

BOOL CTcpSocket::BindListen(char* szIp, u_short nPort)
{
    //绑定端口
   
    m_siClientSocket.sin_family = AF_INET;
    m_siClientSocket.sin_addr.S_un.S_addr = inet_addr(szIp);
    m_siClientSocket.sin_port = htons(nPort);
    int nRet = bind(m_socket, (sockaddr*)&m_siClientSocket, sizeof(m_siClientSocket));
    if (nRet == SOCKET_ERROR)
    {
        
        return FALSE;
    }
    //监听
    nRet = listen(m_socket, SOMAXCONN);
    if (nRet == SOCKET_ERROR)
    {
        return FALSE;
    }
    return TRUE;
}

BOOL CTcpSocket::Accept(CTcpSocket* pTcpSocket)
{
   
        sockaddr_in siOfClinets;
        int siOflen = sizeof(siOfClinets);
        SOCKET socketClients = accept(m_socket, (sockaddr*)&siOfClinets, &siOflen);
        if (socketClients == INVALID_SOCKET)
        {
            return FALSE;
        }
        pTcpSocket->m_socket = socketClients;
        pTcpSocket->m_siClientSocket = siOfClinets;
        return TRUE;
        
   
}

BOOL CTcpSocket::Recv(char* pBuff, int* pnLen)
{
   
    int nRet = recv(m_socket, pBuff, *pnLen, 0);
    if (nRet <= 0)
    {
        return FALSE;
    }
    *pnLen = nRet;
    return TRUE;
   
}

BOOL CTcpSocket::Send(char* pBuff, int* pnLen)
{
  
    int  nRet = send(m_socket, pBuff, *pnLen, 0);
    if (nRet <= 0)
    {
        return FALSE;
    }
    *pnLen = nRet;
    return TRUE;
}

BOOL CTcpSocket::Connect(char* szIp, u_short nPort)
{
    m_siClientSocket.sin_family = AF_INET;
    m_siClientSocket.sin_addr.S_un.S_addr = inet_addr(szIp);
    m_siClientSocket.sin_port = htons(nPort);
    int nRet = connect(m_socket, (sockaddr*)&m_siClientSocket, sizeof(m_siClientSocket));
    if (nRet  == SOCKET_ERROR)
    {
        return FALSE;
    }
    return TRUE;
}

BOOL CTcpSocket::RecvPackage(DATAPACKAGE* pPackage)
{
    int nLen = sizeof(pPackage->m_hdr);
    BOOL bRet = Recv((char*)&(pPackage->m_hdr), &nLen);
    if (!bRet)
    {
        return FALSE;
    }
    if (pPackage->m_hdr.m_nDataLen != 0)
    {
        pPackage->m_pBuff = new char[pPackage->m_hdr.m_nDataLen];
        bRet = Recv(pPackage->m_pBuff, &pPackage->m_hdr.m_nDataLen);
        if (!bRet)
        {
            return FALSE;
        }
    }
    return TRUE;
}

BOOL CTcpSocket::SendPackage(DATAPACKAGE* pPackage)
{
    int sendLen = sizeof(pPackage->m_hdr);
    BOOL bRet = Send((char*)&pPackage->m_hdr, &sendLen);
    if (!bRet)
    {
        return FALSE;
    }
    if (pPackage->m_hdr.m_nDataLen!=0)
    {
        bRet = Send(pPackage->m_pBuff, &pPackage->m_hdr.m_nDataLen);
        if (!bRet)
        {
            return FALSE;
        }
    }
    return TRUE;
}

void CTcpSocket::CloseSocket()
{
    closesocket(m_socket);
}



const sockaddr_in& CTcpSocket::GetSocketAddrIn() const
{
    return m_siClientSocket;
}
