#pragma once
#include "InitSocket.h"
class UdpSocket
{
public:
    UdpSocket();
    ~UdpSocket();
public:
    SOCKET   CreateSocketUDP();
   BOOL   CloseSocketUDP();
   BOOL   BindUDP(const char* pIp ,u_short uPort);

public:
    SOCKET  _SokcetUDP = INVALID_SOCKET;



public:
    //解析命令的处理函数

};



//包标志
enum ChatCommand
{
    C2S_LOGIN,      //上线            客户端的发包类型
    C2S_LOGOUT,     //下线
    C2S_GROUP,      //群聊包
    C2S_PRIVATE,    //私聊包

    S2C_LOGIN,      //上线            服务端的发包类型  
    S2C_LOGOUT,     //下线
    S2C_GROUP,      //群聊包
    S2C_PRIVATE,    //私聊包
};

//用户的信息
#define NICKLEN 128
typedef struct tagUserInfo
{
    WORD    _wPort;             //用户的端口号
    DWORD   _dwIp;              //用户的ip地址
    char    _szNick[NICKLEN];   //用户的名字
}USERINFO, * PUSERINFO;

#define MSGLEN 256
typedef struct tagPackage
{
    ChatCommand     _nCommand;              //包的类型
    USERINFO        _ClientSend;                //发送信息的客户端
    USERINFO        _uiTo;                  //私聊中接受信息的客户端
    char            _szMsg[MSGLEN];         //发送的信息
}PACKAGE, * PPACKAGE;


bool SentTo(SOCKET sock, USERINFO& ui, PACKAGE& pkg);
