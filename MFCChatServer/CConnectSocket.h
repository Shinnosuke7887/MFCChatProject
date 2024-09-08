#pragma once
#include <afxsock.h>
class CConnectSocket :
    public CAsyncSocket
{
public:
    CConnectSocket();
    virtual ~CConnectSocket();

    /**
     * 接受到socket消息.
     * 
     * \param nErrorCode
     */
    void OnReceive(int nErrorCode);
};

