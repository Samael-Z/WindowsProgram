#pragma once
enum DataType
{
    DT_LOGIN,           //登录包
    DT_LOGIN_OK,        //登陆成功
    DT_LOGINOUT,        //退出
    DT_MSG,             //聊天数据包
    DT_HEART            //心跳包
};
typedef struct tagPackageHeader
{
    int m_nDataLen;  //数据的长度
    int m_nDataType; //数据的类型
}PACKAGEHEADER,*PPACKAGEHEADER ;