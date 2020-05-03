#include "IOCPSocket.h"



CIOCPSocket::CIOCPSocket()
{
}


CIOCPSocket::~CIOCPSocket()
{
}

BOOL CIOCPSocket::CreateIocp(char* szIp, unsigned short nPort)
{
	// 	1) 创建socket
	m_sockServer = socket(
		AF_INET,
		SOCK_STREAM, //流式 
		IPPROTO_TCP);//tcp协议

	// 	2) 绑定端口
	sockaddr_in siServer;
	siServer.sin_family = AF_INET;
	siServer.sin_port = htons(nPort);
	siServer.sin_addr.S_un.S_addr = inet_addr(szIp);
	int nRet = bind(m_sockServer, (sockaddr*)&siServer, sizeof(siServer));
	if (nRet == SOCKET_ERROR)
	{
		return FALSE;
	}

	// 	3) 监听
	nRet = listen(m_sockServer, SOMAXCONN);
	if (nRet == SOCKET_ERROR)
	{
		return FALSE;
	}

	//创建IOCP对象
	m_hIOCP = CreateIoCompletionPort(
		INVALID_HANDLE_VALUE,
		NULL,
		NULL, //自定义参数
		0);//自动计算线程的个数, 如果是双核,则是4线程, 如果是单核,则是双线程
	if (m_hIOCP == NULL)
	{
		return FALSE;
	}

	//把socket注册给iocp, 关联socket和iocp
	HANDLE hRet = CreateIoCompletionPort(
		(HANDLE)m_sockServer,
		m_hIOCP,
		NULL,
		0);

	return TRUE;
}


//向iocp队列中投递一个接收连接的请求
BOOL CIOCPSocket::PostAccept()
{
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SocketInfo* pClientInfo = new SocketInfo(sockClient);

	char szBuff[MAXWORD] = { 0 };
	DWORD dwBytesRet = 0;
	CR36OVERLAPPED* pOverlapped = new CR36OVERLAPPED(pClientInfo, IO_ACCEPT);
	BOOL bRet = AcceptEx(
		m_sockServer, //用来监听的socket
		sockClient, //未来和客户端建立连接用的, 
		szBuff,
		0, //让iocp在连接建立完之后立即通知我们, 不用接收客户端发的第一个包
		sizeof(sockaddr_in) + 16,//MSDN要求加上16
		sizeof(sockaddr_in) + 16,//MSDN要求加上16
		&dwBytesRet,
		(OVERLAPPED*)pOverlapped); //每个请求配一个这个结构体

	HANDLE hRet = CreateIoCompletionPort(
		(HANDLE)sockClient,
		m_hIOCP,
		NULL,
		0);


	return bRet;
}

//向队列中投递一个接收数据的请求
BOOL CIOCPSocket::PostRecv(SocketInfo* pSocketInfo)
{
	//用于接收数据的缓冲区
	CR36OVERLAPPED* pOverlapped = new CR36OVERLAPPED(pSocketInfo, IO_RECV);
	pOverlapped->m_wsabuffForRecv.buf = pOverlapped->m_szBuffRecvTmp;
	pOverlapped->m_wsabuffForRecv.len = MAXWORD;

	DWORD dwFlag = 0;
	int nRet = WSARecv(
		pSocketInfo->m_sock,
		&pOverlapped->m_wsabuffForRecv,
		1, //缓冲区中WSABUF的个数, 一般只有一个
		0,
		&dwFlag,
		(OVERLAPPED*)pOverlapped,
		NULL);
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

int CIOCPSocket::WaitForIoEvent(SocketInfo*& pSocketInfo)
{
	DWORD dwBytesRecv = 0; //如果是读请求,则返回读取数据的个数
	CR36OVERLAPPED* pOverlapped = NULL;
	ULONG_PTR pCompleteKey = NULL;
	BOOL bRet = GetQueuedCompletionStatus(
		m_hIOCP,
		&dwBytesRecv,
		&pCompleteKey,
		(LPOVERLAPPED*)&pOverlapped,
		INFINITE
	);
	pSocketInfo = pOverlapped->m_pSocketInfo;
	int nRetIoEvent = pOverlapped->m_ioEvent;

	switch (pOverlapped->m_ioEvent)
	{
	case IO_ACCEPT:
		PostRecv(pOverlapped->m_pSocketInfo);

		//继续向队列中投递接收连接的请求,用来连接后面的客户端
		PostAccept();
		break;
	case IO_RECV:
	{
		//收到的数据放入接收缓冲区中
		pOverlapped->m_pSocketInfo->m_buffRecv.Write(
			pOverlapped->m_szBuffRecvTmp,
			dwBytesRecv);

		//再丢一个收数据的请求
		PostRecv(pOverlapped->m_pSocketInfo);
		break;
	}
	case IO_SEND:
	{
		//发送完数据之后, 向队列中投递一个接收数据的请求,继续接收后面的数据
		PostSend(pOverlapped->m_pSocketInfo);
		break;
	}
	default:
		break;
	}

	delete pOverlapped;

	return nRetIoEvent;
}

BOOL CIOCPSocket::PostSend(SocketInfo* pSocketInfo)
{
	//如果缓冲区中没有数据, 则不再向队列中投递发送数据的请求
	pSocketInfo->m_lockSend.Lock();
	if (pSocketInfo->m_buffSend.GetSize() <= 0)
	{
		pSocketInfo->m_lockSend.UnLock();
		return FALSE;
	}
	pSocketInfo->m_lockSend.UnLock();


	//用于发送数据的缓冲区
	CR36OVERLAPPED* pOverlapped = new CR36OVERLAPPED(pSocketInfo, IO_SEND);

	//从发送缓冲区中取出数据,将数据发送出去
	pSocketInfo->m_lockSend.Lock();
	pOverlapped->m_wsabuffForSend.buf = new char[pSocketInfo->m_buffSend.GetSize()];
	pOverlapped->m_wsabuffForSend.len = pSocketInfo->m_buffSend.GetSize();
	pSocketInfo->m_buffSend.Read(pOverlapped->m_wsabuffForSend.buf, pOverlapped->m_wsabuffForSend.len);
	pSocketInfo->m_lockSend.UnLock();

	int nRet = WSASend(
		pSocketInfo->m_sock,
		&pOverlapped->m_wsabuffForSend,
		1, //缓冲区中WSABUF的个数, 一般只有一个
		NULL,
		0,
		(OVERLAPPED*)pOverlapped,
		NULL);
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
