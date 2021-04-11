#pragma once
#include <windows.h>
class CLock
{
public:
    CLock();
    ~CLock();
    void Lock();
    void Unlock();
private:
    CRITICAL_SECTION  _cs;
};
