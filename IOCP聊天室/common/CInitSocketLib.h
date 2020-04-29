#pragma once
#include <windows.h>
#pragma comment(lib,"Ws2_32.lib")
class CInitSocketLib
{
public:
    CInitSocketLib();
    ~CInitSocketLib();
private:
    static CInitSocketLib m_socket;
};

