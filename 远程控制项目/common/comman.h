#pragma once
enum DataType
{
    DT_LOGIN,                  //登录包
    DT_LOGIN_OK,               //登陆成功
    DT_LOGINOUT,               //退出
    DT_MSG,                    //聊天数据包
    DT_HEART,                  //心跳包
    DT_GETDRIVERS,               //获取盘符
    DT_DRIVERS,                  //返回盘符
    DT_GETDIR,                   //获取目录
    DT_DIR,                      //返回目录
    DT_GETCMD,                   //获取cmd命令数据
    DT_CMD,                      //返回cmd数据 
    DT_GETPROCESS,               //获取进程列表
    DT_PROCESS,                  //返回进程列表
    DT_GETTHREAD,                //获取线程list
    DT_THREAD,                   //返回线程list
    DT_TERPROCESS,               //结束进程
    DT_GETREG_ROOT,              //获取注册表HKEY_CLASSES_ROOT类型   
    DT_GETREG_CUR_USER,          //获取注册表HKEY_CURRENT_USER类型 
    DT_GETREG_LOC_MACH,          //获取注册表HKEY_LOCAL_MACHINE类型 
    DT_GETREG_USERS,             //获取注册表HKEY_USERS类型 
    DT_GETREG_CUR_CONFIG,        //获取注册表HKEY_CURRENT_CONFIG类型 
    DT_REG,                      //返回注册表列表
    DT_GETITEM,                  //获取注册表的数据项
    DT_ITEM,                     //返回注册表的数据项

};
typedef struct tagPackageHeader
{
    int m_nDataLen;  //数据的长度
    int m_nDataType; //数据的类型
}PACKAGEHEADER,*PPACKAGEHEADER ;

typedef struct Status
{
    BOOL  m_bIsExit = FALSE;
    int   m_nIndex;

}STATUS, * PSTATUS;