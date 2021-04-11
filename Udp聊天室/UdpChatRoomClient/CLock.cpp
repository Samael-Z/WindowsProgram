#include "pch.h"
#include "CLock.h"

CLock::CLock()
{
    InitializeCriticalSection(&_cs);
}

CLock::~CLock()
{
    DeleteCriticalSection(&_cs);
}

void CLock::Lock()
{
    EnterCriticalSection(&_cs);
}

void CLock::Unlock()
{
    LeaveCriticalSection(&_cs);
}