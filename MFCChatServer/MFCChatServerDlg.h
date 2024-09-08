
// MFCChatServerDlg.h: 头文件
//

#pragma once
#include "CServerSocket.h"
#include "CConnectSocket.h"

// CMFCChatServerDlg 对话框
class CMFCChatServerDlg : public CDialogEx
{
// 构造
public:
	CMFCChatServerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCCHATSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStart();
	CListBox m_list;
	CServerSocket* m_server;
	CConnectSocket* m_client;
	CTime m_time;
	afx_msg void OnBnClickedBtnSend();
	CEdit m_input_edit;
	CString CatShowString(CString strInfo, CString strMsg);
	CString CatShowString(CString strMsg);
	afx_msg void OnBnClickedBtnClear();
	void updateControl();
	afx_msg void OnBnClickedBtnStop();
};
