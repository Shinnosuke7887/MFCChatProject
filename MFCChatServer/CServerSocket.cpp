#include "pch.h"
#include "CServerSocket.h"
#include "MFCChatServer.h"
#include "MFCChatServerDlg.h"

CServerSocket::CServerSocket()
{
}

CServerSocket::~CServerSocket()
{
}

void CServerSocket::OnAccept(int nErrorCode)
{
	TRACE("[ChatClient] OnAccept");
	CMFCChatServerDlg* dlg = (CMFCChatServerDlg*)AfxGetApp()->GetMainWnd();
	dlg->m_client = new CConnectSocket;
	// ���ø��෽�������Ǹ�д��connectSocket�ഫ��ȥ
	Accept(*(dlg->m_client));

	CString str, strIp;
	UINT iPort;
	dlg->m_client->GetPeerName(strIp, iPort);
	dlg->m_time = CTime::GetCurrentTime();
	//str = dlg->m_time.Format("%X ");
	//str += _T("�ͻ������ӳɹ�");
	str.Format(_T("%s�ͻ������ӳɹ� ip:%s port:%d"),
		dlg->m_time.Format("%X "), strIp, iPort);
	dlg->m_list.AddString(str);
	dlg->m_list.UpdateData(false);

	CAsyncSocket::OnAccept(nErrorCode);
}
