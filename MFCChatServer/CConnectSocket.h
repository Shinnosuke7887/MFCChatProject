#pragma once
#include <afxsock.h>
class CConnectSocket :
    public CAsyncSocket
{
public:
    CConnectSocket();
    virtual ~CConnectSocket();

    /**
     * ���ܵ�socket��Ϣ.
     * 
     * \param nErrorCode
     */
    void OnReceive(int nErrorCode);
};

