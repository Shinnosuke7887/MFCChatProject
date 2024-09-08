
// MFCChatClientDlg.h: 头文件
//

#pragma once

#include"CMySocket.h"

// CMFCChatClientDlg 对话框
class CMFCChatClientDlg : public CDialogEx
{
// 构造
public:
	CMFCChatClientDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCCHATCLIENT_DIALOG };
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
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnBnClickedBtnDisconn();
	CMySocket* m_client;
	// 历史消息展示框
	CListBox m_list;
	CTime m_time;
	// 收入消息展示框
	CEdit m_input_edit;
	afx_msg void OnBnClickedBtnSend();
	CString CatShowString(CString strInfo, CString strMsg);
	CString CatShowString(CString strMsg);
	void autoReply();
	void updateControl();
	afx_msg void OnBnClickedButtonSaveName();
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnBnClickedCheckAutoSend();
};
