#pragma once
#include<afxsock.h>
class CMySocket :
    public CAsyncSocket
{
public:
    CMySocket();
    // ���� virtual ������Ҳ��ִ��
    virtual ~CMySocket();
    // ���ͻ�������ʱ����
    virtual void OnConnect(int nErrorCode);
    // ���յ�����Ϣ����
    virtual void OnReceive(int nErrorCode);
};

