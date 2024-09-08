#include "pch.h"
#include "CConnectSocket.h"
#include "MFCChatServer.h"
#include "MFCChatServerDlg.h"

CConnectSocket::CConnectSocket()
{
}

CConnectSocket::~CConnectSocket()
{
}

void CConnectSocket::OnReceive(int nErrorCode)
{
	TRACE("[ChatClient] OnReceive");
	CMFCChatServerDlg* dlg = (CMFCChatServerDlg*)AfxGetApp()->GetMainWnd();
	// 接收对端的消息
	char szRecvBuff[SEND_MAX_BUF] = { 0 };
	Receive(szRecvBuff, SEND_MAX_BUF, 0);
	TRACE("[ChatClient] 收到socket消息 %s", szRecvBuff);

	
	USES_CONVERSION;
	CString strRecvMsg = A2W(szRecvBuff);

	CString strShow;
	//dlg->m_time = CTime::GetCurrentTime();
	//strShow.Format(_T("%s对方:%s"), dlg->m_time.Format("%X "), strRecvMsg);
	strShow = dlg->CatShowString(strRecvMsg);
	dlg->m_list.AddString(strShow);

	CAsyncSocket::OnReceive(nErrorCode);
}
