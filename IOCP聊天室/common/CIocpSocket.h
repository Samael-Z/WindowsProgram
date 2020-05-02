#pragma once

#include <WinSock2.h>

#include <windows.h>
#include <Mswsock.h>

#pragma comment(lib, "Mswsock.lib")
#include "CByteStreamBuff.h"
#include "Lock.h"
class CIocpSocket
{
public:
    enum IOCP_EVENT
    {
        IO_ACCEPT,
        IO_RECV,
        IO_SEND

    };
    struct SocketInfo
    {
        SOCKET     m_socket; //IOCP与客户端关联的socket
        CByteStreamBuff  m_RecvBuffer; //读取数据的缓冲区 ，iocp收数据的缓冲区
        CByteStreamBuff  m_SendBuffer;//写入数据的缓冲区 ，iocp发数据的缓冲区
        CLock m_locSend;
        CLock m_locRecv;
        SocketInfo(SOCKET sock) :m_socket(sock)
        {

        }
    };
    struct  MYOVERLAPPED
    {
        OVERLAPPED m_Overlapped; //把为每个通信用OVERLAPPED保存在这个结构体里
        
        WSABUF m_wsaBuffRecv; //收数据的缓冲区
        WSABUF m_wsaBuffSend;//发数据的缓冲区
        DWORD m_ioEvent; //枚举区每此操作的类型，接收链接，收数据，发数据
        char m_SendBuffTmp[MAXWORD];//收数据的临时缓冲区
        SocketInfo* m_pSocketInfo;
        MYOVERLAPPED(SocketInfo* pSocketInfo, DWORD ioEvent)//构造
            :m_pSocketInfo(pSocketInfo), m_ioEvent(ioEvent)
        {
            //OVERLAPPED要清空
            memset(&m_Overlapped, 0, sizeof(m_Overlapped));
            m_wsaBuffSend.buf = NULL;
            m_wsaBuffSend.len = 0;
        }
        ~MYOVERLAPPED()
        {
            if (m_wsaBuffSend.buf != NULL)
            {
                delete m_wsaBuffSend.buf;
            }
        }

    };
public:
    CIocpSocket();
    ~CIocpSocket();

    //创建IOCP
    BOOL CreateIocp(char* szIp, u_short nProt);
    //接收链接
    BOOL PostAccept();
    //接收数据
    BOOL PostRecv(SocketInfo* pSocketInfo);
    //发送数据
    BOOL PostSend(SocketInfo* pSocketInfo);
    //等待事件
    int WaitForEvent(SocketInfo*& pSocketInfo);
private:
    HANDLE m_hIocp;
    SOCKET m_sockServer;

};

