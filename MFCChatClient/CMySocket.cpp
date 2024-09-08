#include "pch.h"
#include "CMySocket.h"
#include "MFCChatClient.h"
#include "MFCChatClientDlg.h"

CMySocket::CMySocket()
{
}

CMySocket::~CMySocket()
{
}

void CMySocket::OnConnect(int nErrorCode)
{
	TRACE("[ChatClient] OnConnect");
	// 获取窗口
	CMFCChatClientDlg* dlg = (CMFCChatClientDlg*)AfxGetApp()->GetMainWnd();
	CString str;
	// 获取当前的时间
	//dlg->m_time = CTime::GetCurrentTime();
	//str = dlg->m_time.Format("%X ");
	//str += _T("与服务器连接成功");
	str = dlg->CatShowString(_T(""), _T("与服务器连接成功"));
	dlg->m_list.AddString(str);
	// 调用父类方法
	CAsyncSocket::OnConnect(nErrorCode);
}

void CMySocket::OnReceive(int nErrorCode)
{
	TRACE("[ChatClient] OnReceive");
	CMFCChatClientDlg* dlg = (CMFCChatClientDlg*)AfxGetApp()->GetMainWnd();
	// 接收对端的消息
	char szRecvBuff[SEND_MAX_BUF] = { 0 };
	Receive(szRecvBuff, SEND_MAX_BUF, 0);
	TRACE("[ChatClient] 收到socket消息 %s", szRecvBuff);


	USES_CONVERSION;
	CString strRecvMsg = A2W(szRecvBuff);

	CString strShow;
	// dlg->m_time = CTime::GetCurrentTime();
	// strShow.Format(_T("%s对方:%s"), dlg->m_time.Format("%X "), strRecvMsg);
	strShow = dlg->CatShowString(_T("对方"), strRecvMsg);
	dlg->m_list.AddString(strShow);

	// 自动回复
	dlg->autoReply();

	CAsyncSocket::OnReceive(nErrorCode);
}
