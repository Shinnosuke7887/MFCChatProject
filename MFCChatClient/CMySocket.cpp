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
	// ��ȡ����
	CMFCChatClientDlg* dlg = (CMFCChatClientDlg*)AfxGetApp()->GetMainWnd();
	CString str;
	// ��ȡ��ǰ��ʱ��
	//dlg->m_time = CTime::GetCurrentTime();
	//str = dlg->m_time.Format("%X ");
	//str += _T("����������ӳɹ�");
	str = dlg->CatShowString(_T(""), _T("����������ӳɹ�"));
	dlg->m_list.AddString(str);
	// ���ø��෽��
	CAsyncSocket::OnConnect(nErrorCode);
}

void CMySocket::OnReceive(int nErrorCode)
{
	TRACE("[ChatClient] OnReceive");
	CMFCChatClientDlg* dlg = (CMFCChatClientDlg*)AfxGetApp()->GetMainWnd();
	// ���նԶ˵���Ϣ
	char szRecvBuff[SEND_MAX_BUF] = { 0 };
	Receive(szRecvBuff, SEND_MAX_BUF, 0);
	TRACE("[ChatClient] �յ�socket��Ϣ %s", szRecvBuff);


	USES_CONVERSION;
	CString strRecvMsg = A2W(szRecvBuff);

	CString strShow;
	// dlg->m_time = CTime::GetCurrentTime();
	// strShow.Format(_T("%s�Է�:%s"), dlg->m_time.Format("%X "), strRecvMsg);
	strShow = dlg->CatShowString(_T("�Է�"), strRecvMsg);
	dlg->m_list.AddString(strShow);

	// �Զ��ظ�
	dlg->autoReply();

	CAsyncSocket::OnReceive(nErrorCode);
}
