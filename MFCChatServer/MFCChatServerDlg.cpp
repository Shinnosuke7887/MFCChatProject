
// MFCChatServerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCChatServer.h"
#include "MFCChatServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCChatServerDlg 对话框



CMFCChatServerDlg::CMFCChatServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCCHATSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_server = nullptr;
	m_client = nullptr;
}

void CMFCChatServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MSG_HIS, m_list);
	DDX_Control(pDX, IDC_EDIT_SEND_MSG, m_input_edit);
}

BEGIN_MESSAGE_MAP(CMFCChatServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, &CMFCChatServerDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_SEND, &CMFCChatServerDlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CMFCChatServerDlg::OnBnClickedBtnClear)
	ON_BN_CLICKED(IDC_BTN_STOP, &CMFCChatServerDlg::OnBnClickedBtnStop)
END_MESSAGE_MAP()


// CMFCChatServerDlg 消息处理程序

BOOL CMFCChatServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	GetDlgItem(IDC_EDIT_PORT)->SetWindowTextW(_T("8888"));

	updateControl();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCChatServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCChatServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCChatServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/**
 * 格式化输出消息.
 *
 * \param strInfo
 * \param strMsg
 * \return "10:00:01 我:一条消息"
 */
CString CMFCChatServerDlg::CatShowString(CString strInfo, CString strMsg)
{
	CString strRes;
	m_time = CTime::GetCurrentTime();
	strRes.Format(_T("%s %s:%s"), m_time.Format("%X"), strInfo, strMsg);
	return strRes;
}
CString CMFCChatServerDlg::CatShowString(CString strMsg)
{
	CString strRes;
	m_time = CTime::GetCurrentTime();
	strRes.Format(_T("%s %s"), m_time.Format("%X"), strMsg);
	return strRes;
}

/**
 * 连接socket
 * 
 */
void CMFCChatServerDlg::OnBnClickedBtnStart()
{
	CString strPort;
	GetDlgItem(IDC_EDIT_PORT)->GetWindowTextW(strPort);
	USES_CONVERSION; // 使用T2A需要定义这个
	LPCSTR szPort = (LPCSTR)T2A(strPort);
	int iPort = _ttoi(strPort);
	TRACE("[ChatClient] Server start szPort = %d", iPort);
	// 创建socket
	m_server = new CServerSocket();
	// 传入端口表示创建服务器对象
	if (!m_server->Create(iPort)) {
		TRACE("[ChatClient] Server 创建socket失败. error_code=%d", GetLastError());
		return;
	}
	else {
		TRACE("[ChatClient] Server 创建socket成功");
	}
	// 监听请求
	if (!m_server->Listen()) {
		TRACE("[ChatClient] Server Listen失败. error_code=%d", GetLastError());
		return;
	}
	else {
		TRACE("[ChatClient] Server Listen成功");
	}
	CString str;
	//m_time = CTime::GetCurrentTime();
	//str = m_time.Format("%X ");
	//str += _T("建立服务");
	str = CatShowString(_T(""), _T("建立服务"));
	m_list.AddString(str);
	updateControl();
}

/**
 * 发送消息.
 * 
 */
void CMFCChatServerDlg::OnBnClickedBtnSend()
{
	CString strTmpMsg, strShow;
	m_input_edit.GetWindowText(strTmpMsg);

	USES_CONVERSION;
	char* szSendBuff = T2A(strTmpMsg);

	TRACE("[ChatClient] 要发送的消息 %s", szSendBuff);

	// 往socket发送消息
	m_client->Send(szSendBuff, SEND_MAX_BUF, 0);
	//m_time = CTime::GetCurrentTime();
	//strShow.Format(_T("%s我:%s"), m_time.Format("%X "), strTmpMsg);
	strShow = CatShowString(_T("我"), strTmpMsg);
	m_list.AddString(strShow);
	m_input_edit.SetWindowTextW(_T(""));
	UpdateData(FALSE);
}


/**
 * 清屏.
 * 
 */
void CMFCChatServerDlg::OnBnClickedBtnClear()
{
	m_list.ResetContent();
}

/**
 * 修改界面控件的状态.
 * 
 */
void CMFCChatServerDlg::updateControl()
{
	if (m_server == nullptr) {
		GetDlgItem(IDC_BTN_START)->EnableWindow(true);
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(false);
		GetDlgItem(IDC_BTN_SEND)->EnableWindow(false);
	}
	else {
		GetDlgItem(IDC_BTN_START)->EnableWindow(false);
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(true);
		GetDlgItem(IDC_BTN_SEND)->EnableWindow(true);
	}
}

/**
 * 断开连接.
 * 
 */
void CMFCChatServerDlg::OnBnClickedBtnStop()
{
	if (m_client != nullptr) {
		m_client->Close();
		delete m_client;
		m_client = nullptr;
	}
	
	if (m_server != nullptr) {
		m_server->Close();
		delete m_server;
		m_server = nullptr;
	}
	CString strShow = CatShowString(_T("服务器已经停止"));
	m_list.AddString(strShow);
	updateControl();


}
