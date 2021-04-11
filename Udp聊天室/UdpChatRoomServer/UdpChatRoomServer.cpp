// UdpChatRoomServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "UdpSocket.h"
#include <list>
#include <String>
using namespace std;

void OnC2SLogin(SOCKET& sock, PACKAGE& pkg);
void OnC2SLogout(SOCKET& sock, PACKAGE& pkg);
void OnC2SGroup(SOCKET& sock, PACKAGE& pkg);
void OnC2SPrivate(SOCKET& sock, PACKAGE& pkg);
list<USERINFO> lstClientsInfo;
int main()
{
   
 


    UdpSocket  SeverScoket;
    
    SeverScoket.CreateSocketUDP();
    printf("Socket: %d\r\n", SeverScoket._SokcetUDP);
   BOOL bRet =   SeverScoket.BindUDP("127.0.0.1", 0x9527);
   if (bRet)
   {
       printf("绑定成功---端口号：%d\r\n", 0x9527);
   }
   else
   {
       printf("绑定端口失败");
   }

   while (true)
   {
       PACKAGE  pkg;
       sockaddr_in  si = { 0 };
       int nSiLen = sizeof(si);
       int nRet = recvfrom(SeverScoket._SokcetUDP, (char*)&pkg, sizeof(pkg), 0, (sockaddr*)&si, &nSiLen);

       switch (pkg._nCommand)
       {
       case  C2S_LOGIN:
       {
            //处理用户上线，，写一个单独函数
           pkg._ClientSend._wPort = si.sin_port;
           OnC2SLogin(SeverScoket._SokcetUDP, pkg);
       }
       break;
       case  C2S_LOGOUT:
       {
           //处理用户下线，，写一个单独函数
           OnC2SLogout(SeverScoket._SokcetUDP, pkg);
       }
       break;
       case  C2S_GROUP:
       {
           //处理用户公聊，，写一个单独函数
           OnC2SGroup(SeverScoket._SokcetUDP, pkg);
       }
       break;
       case  C2S_PRIVATE:
       {
           //处理用户私聊，，写一个单独函数
           OnC2SPrivate(SeverScoket._SokcetUDP, pkg);
       }
       break;
       }
   }

   //main函数的返回点
    return 0;
}



void OnC2SLogin(SOCKET& sock, PACKAGE& pkg)
{
    //显示出来用户上线了
    printf("Ip:%s---Port:%d :%s 上线了\r\n", 
        inet_ntoa(*(in_addr*)&pkg._ClientSend._dwIp),
        ntohs(pkg._ClientSend._wPort),
        pkg._ClientSend._szNick);

    //告诉用户目前在线的用户信息
    for (auto& User : lstClientsInfo)
    {
        //依次构造一个包，把已经上线用户的信息赋值当到包里面
        //依次发送给新上线的用的，让新上线的用户知道
        //把新上线的用户添加到链表里面
        PACKAGE pkgOnlineUser ;
        memset(&pkgOnlineUser, 0, sizeof(pkgOnlineUser));
        pkgOnlineUser._nCommand =S2C_LOGIN;
        pkgOnlineUser._ClientSend = User;
        SentTo(sock, pkg._ClientSend, pkgOnlineUser);
       
    }

    //把新上线的用户添加到链表里面
    lstClientsInfo.push_back(pkg._ClientSend);

    //告诉别的用户，有新用户上线了（包括新用户自己）
    pkg._nCommand = S2C_LOGIN;
    for (auto& AllUser : lstClientsInfo)
    {
        SentTo(sock, AllUser, pkg);
    }

}
void OnC2SLogout(SOCKET& sock, PACKAGE& pkg)
{
    printf("用户下线了 ip:%s port:%d nick:%s\r\n",
        inet_ntoa(*(in_addr*)&pkg._ClientSend._dwIp),
        ntohs(pkg._ClientSend._wPort),
        pkg._ClientSend._szNick);

    pkg._nCommand = S2C_LOGOUT;
    //修改包的类型，发送给每一个在线用户，xx下线了
    for (auto& AllUser : lstClientsInfo)
    { 
        SentTo(sock, AllUser, pkg);
    }
    //在链表中找到这个用户，然后干掉他
    //这种可以遍历删除掉这个用户，上面的不行，难道只有这种可以删除迭代器
    for ( auto itr = lstClientsInfo.cbegin(); itr!= lstClientsInfo.cend(); ++itr)
    {
        if (itr->_dwIp == pkg._ClientSend._dwIp 
            && itr->_wPort == pkg._ClientSend._wPort)
        {
            lstClientsInfo.erase(itr);
            break;
        }
    }

}
void OnC2SGroup(SOCKET& sock, PACKAGE& pkg)
{
    //公聊应该把数据包转发给所有的在线用户

    for (auto& user : lstClientsInfo)
    {
        pkg._nCommand = S2C_GROUP;

        SentTo(sock, user, pkg);
    }
}
void OnC2SPrivate(SOCKET& sock, PACKAGE& pkg)
{

    //私聊,找到私聊的对象然后发送数据包

    pkg._nCommand = S2C_PRIVATE;
    for (auto itr = lstClientsInfo.begin();itr!= lstClientsInfo.end(); ++itr)
    {
        if (itr->_dwIp == pkg._uiTo._dwIp && itr->_wPort == pkg._uiTo._wPort)
        {
            SentTo(sock, *itr, pkg);
        }
    }
}