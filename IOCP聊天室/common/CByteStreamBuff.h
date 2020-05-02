#pragma once
#include <Windows.h>
#include <vector>
using namespace std;
class CByteStreamBuff
{
public:
    CByteStreamBuff();
    ~CByteStreamBuff();
    //写入缓冲区，附加在缓冲区末尾
    void Write(const char* pBuff, int nSize);
    //从缓冲区读取指定字节数，读取的数据自动从缓冲区删除
    void Read(char* pBuff, int nSize);
    //从缓冲区读取指定字节数，读取数据不删除
    void Peek(char* pBuff, int nSize);
    //获取缓冲区数据大小
    int GetSize() const;
private:
    vector<char> m_vctBuff;

};

