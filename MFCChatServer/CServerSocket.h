#pragma once

#include<afxsock.h>

class CServerSocket :
    public CAsyncSocket
{
public:
    CServerSocket();
    virtual ~CServerSocket();

    /**
     * ���ܿͻ��˵�����
     * 
     * \param nErrorCode
     */
    void OnAccept(int nErrorCode);
};

