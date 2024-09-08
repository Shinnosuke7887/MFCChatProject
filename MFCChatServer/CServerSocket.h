#pragma once

#include<afxsock.h>

class CServerSocket :
    public CAsyncSocket
{
public:
    CServerSocket();
    virtual ~CServerSocket();

    /**
     * 接受客户端的连接
     * 
     * \param nErrorCode
     */
    void OnAccept(int nErrorCode);
};

