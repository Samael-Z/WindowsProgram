#pragma once
#include <WinSock2.h>
#include <windows.h>
#include <Mswsock.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")

#include "ByteStreamBuff.h"
#include "Lock.h"

class CIOCPSocket
{
public:
	enum IO_EVENT
	{
		IO_ACCEPT,
		IO_RECV,
		IO_SEND
	};

	//代表连接上的客户端
	struct SocketInfo
	{
		SOCKET m_sock;  //对应连接客户段的socket

		CLock m_lockRecv; //同步,用于m_buffRecv
		CByteStreamBuff m_buffRecv;  //读取数据的缓冲区, iocp收到的数据都放到这里

		CLock m_lockSend; //同步, 用于m_buffSend
		CByteStreamBuff m_buffSend; //写入数据的缓冲区, iocp发出的数据都从这里拿

		SocketInfo(SOCKET sock) :m_sock(sock){	}
	};


	struct CR36OVERLAPPED
	{
		OVERLAPPED m_overlapped;
		DWORD  m_ioEvent;		   //请求的类型
		SocketInfo* m_pSocketInfo; //保存客户端相关信息

		//临时缓冲区,接收到的数据iocp会放到这里, 然后这里的数据会放到真正的中间缓冲区
		char   m_szBuffRecvTmp[MAXWORD];
		WSABUF m_wsabuffForRecv; //接收数据的缓冲区

		WSABUF m_wsabuffForSend; //发送数据的缓冲区

		CR36OVERLAPPED(SocketInfo* pSocketInfo, DWORD ioEvent) :
			m_pSocketInfo(pSocketInfo), m_ioEvent(ioEvent)
		{
			memset(&m_overlapped, 0, sizeof(m_overlapped));
			m_wsabuffForSend.buf = NULL;
			m_wsabuffForSend.len = 0;
		}
		~CR36OVERLAPPED()
		{
			if (m_wsabuffForSend.buf != NULL)
			{
				delete m_wsabuffForSend.buf;
			}

		}
	};

public:
	CIOCPSocket();
	~CIOCPSocket();

	BOOL CreateIocp(char* szIp, unsigned short nPort);
	BOOL PostAccept();
	BOOL PostSend(SocketInfo* pSocketInfo);
	BOOL PostRecv(SocketInfo* pSocketInfo);
	int WaitForIoEvent(SocketInfo*& pSocketInfo);

private:
	SOCKET m_sockServer;
	HANDLE m_hIOCP;
};

