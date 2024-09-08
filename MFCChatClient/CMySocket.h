#pragma once
#include<afxsock.h>
class CMySocket :
    public CAsyncSocket
{
public:
    CMySocket();
    // 加上 virtual 让子类也会执行
    virtual ~CMySocket();
    // 当客户端连接时触发
    virtual void OnConnect(int nErrorCode);
    // 当收到新消息触发
    virtual void OnReceive(int nErrorCode);
};

