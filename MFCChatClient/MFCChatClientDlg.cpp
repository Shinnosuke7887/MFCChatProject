
// MFCChatClientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCChatClient.h"
#include "MFCChatClientDlg.h"
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


// CMFCChatClientDlg 对话框



CMFCChatClientDlg::CMFCChatClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCCHATCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_client = nullptr;
}

void CMFCChatClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MSG_HIS, m_list);
	DDX_Control(pDX, IDC_EDIT_SEND_MSG, m_input_edit);
}

BEGIN_MESSAGE_MAP(CMFCChatClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CMFCChatClientDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_DISCONN, &CMFCChatClientDlg::OnBnClickedBtnDisconn)
	ON_BN_CLICKED(IDC_BTN_SEND, &CMFCChatClientDlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_NAME, &CMFCChatClientDlg::OnBnClickedButtonSaveName)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CMFCChatClientDlg::OnBnClickedBtnClear)
	ON_BN_CLICKED(IDC_CHECK_AUTO_SEND, &CMFCChatClientDlg::OnBnClickedCheckAutoSend)
END_MESSAGE_MAP()


// CMFCChatClientDlg 消息处理程序

BOOL CMFCChatClientDlg::OnInitDialog()
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

	// 设置默认的端口和ip
	GetDlgItem(IDC_EDIT_PORT)->SetWindowTextW(_T("8888"));
	GetDlgItem(IDC_IPADDRESS1)->SetWindowTextW(_T("127.0.0.1"));

	// 读取用户昵称配置文件
	WCHAR strName[1024] = { 0 };
	// 所有关于路径的大小都用  MAX_PATH(微软默认的)
	WCHAR strPath[MAX_PATH] = { 0 };
	// 获取当前路径
	GetCurrentDirectoryW(MAX_PATH, strPath);
	CString strFilePath;
	strFilePath.Format(L"%ls//Test.ini", strPath);// 前面的L 和 %ls 表示宽字节
	// 参数说明  1:项目名称  2:key名称 3:默认值  4:value值 5:value的长度  6:存储路径
	DWORD dwNum =  GetPrivateProfileStringW(_T("CHAT_CLIENT"), _T("NAME"), NULL, strName, 1024, strFilePath);
	if (dwNum > 0) {
		TRACE("[ChatClient] 读取到的name %ls", strName); // %s只会显示第一个字母，用%ls即可解决
		GetDlgItem(IDC_EDIT_NAME)->SetWindowTextW(strName);
		UpdateData(false);
	}
	else {
		// 配置文件缺失的情况
		WritePrivateProfileStringW(_T("CHAT_CLIENT"), _T("NAME"), L"客户端", strFilePath);
		GetDlgItem(IDC_EDIT_NAME)->SetWindowTextW(L"客户端");
		UpdateData(false);
	}

	// 更新界面的空间是否可用
	updateControl();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCChatClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCChatClientDlg::OnPaint()
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
HCURSOR CMFCChatClientDlg::OnQueryDragIcon()
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
CString CMFCChatClientDlg::CatShowString(CString strInfo, CString strMsg)
{
	CString strRes;
	m_time = CTime::GetCurrentTime();
	strRes.Format(_T("%s %s:%s"), m_time.Format("%X"), strInfo, strMsg);
	return strRes;
}
CString CMFCChatClientDlg::CatShowString(CString strMsg)
{
	CString strRes;
	m_time = CTime::GetCurrentTime();
	strRes.Format(_T("%s %s"), m_time.Format("%X"), strMsg);
	return strRes;
}

/**
 * 自动回复.
 * 
 */
void CMFCChatClientDlg::autoReply()
{
	if (((CButton*)GetDlgItem(IDC_CHECK_AUTO_SEND))->GetCheck()) {
		CString strAutoSendMsg;
		GetDlgItem(IDC_EDIT_AUTO_MSG)->GetWindowTextW(strAutoSendMsg);
		if (strAutoSendMsg.GetLength() >= 0) {

			CString strName;
			GetDlgItem(IDC_EDIT_NAME)->GetWindowTextW(strName);
			strAutoSendMsg = _T("[自动回复]") + strName + _T(":") + strAutoSendMsg;

			USES_CONVERSION;
			char* szSendBuff = T2A(strAutoSendMsg);

			TRACE("[ChatClient] 要发送的消息 %s", szSendBuff);

			// 往socket发送消息
			m_client->Send(szSendBuff, SEND_MAX_BUF, 0);

			CString strShow;

			strShow = CatShowString(strAutoSendMsg);
			m_list.AddString(strShow);
			UpdateData(FALSE);
		}
	}
}

/**
 * 更新控件的禁用可用状态.
 * 
 */
void CMFCChatClientDlg::updateControl()
{
	if (m_client == nullptr) {
		GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(true);
		GetDlgItem(IDC_BTN_DISCONN)->EnableWindow(false);
		GetDlgItem(IDC_BTN_SEND)->EnableWindow(false);
	}
	else {
		GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(false);
		GetDlgItem(IDC_BTN_DISCONN)->EnableWindow(true);
		GetDlgItem(IDC_BTN_SEND)->EnableWindow(true);
	}
}


/**
 * 连接socket
 * 
 */
void CMFCChatClientDlg::OnBnClickedBtnConnect()
{
	CString strPort, strIp;
	GetDlgItem(IDC_EDIT_PORT)->GetWindowTextW(strPort);
	GetDlgItem(IDC_IPADDRESS1)->GetWindowTextW(strIp);
	
	// CString 转char *
	USES_CONVERSION; // 使用T2A需要定义这个
	LPCSTR szPort = (LPCSTR)T2A(strPort);
	LPCSTR szIp = (LPCSTR)T2A(strIp);
	TRACE("[ChatClient] Client start szPort = %s, szIP = %s", szPort, szIp);

	int iPort = _ttoi(strPort);
	// 创建socket
	m_client = new CMySocket;
	if (!m_client->Create()) {
		TRACE("[ChatClient] Client 创建socket失败. error_code=%d", GetLastError());
		return;
	}
	else {
		TRACE("[ChatClient] Client 创建socket成功");
	}
	// 连接 ip:CString类型 port:int类型
	if (m_client->Connect(strIp, iPort)== SOCKET_ERROR) {
		TRACE("[ChatClient] Client Connect失败. error_code=%d", GetLastError());
		return;
	}
	else {
		TRACE("[ChatClient] Client Connect成功.");
	}
	updateControl();

}

/**
 * 断开连接.
 * 
 */
void CMFCChatClientDlg::OnBnClickedBtnDisconn()
{
	if (m_client != nullptr) {
		m_client->Close();
		delete m_client;
		m_client = nullptr;

		CString strShow = CatShowString(_T("与服务器断开连接"));
		m_list.AddString(strShow);
		UpdateData(false);

		updateControl();
	}
}

/**
 * 发送消息.
 * 
 */
void CMFCChatClientDlg::OnBnClickedBtnSend()
{
	CString strTmpMsg,strShow;
	m_input_edit.GetWindowText(strTmpMsg);

	CString strName;
	GetDlgItem(IDC_EDIT_NAME)->GetWindowTextW(strName);
	strTmpMsg = strName + _T(":") + strTmpMsg;

	USES_CONVERSION;
	char* szSendBuff = T2A(strTmpMsg);

	TRACE("[ChatClient] 要发送的消息 %s", szSendBuff);

	// 往socket发送消息
	m_client->Send(szSendBuff, SEND_MAX_BUF, 0);
	//m_time = CTime::GetCurrentTime();
	//strShow.Format(_T("%s我:%s"), m_time.Format("%X "), strTmpMsg);
	strShow = CatShowString(strTmpMsg);
	m_list.AddString(strShow);
	m_input_edit.SetWindowTextW(_T(""));
	UpdateData(FALSE);
}

/**
 * 保存昵称.
 * 
 */
void CMFCChatClientDlg::OnBnClickedButtonSaveName()
{
	CString strName;
	GetDlgItem(IDC_EDIT_NAME)->GetWindowTextW(strName); // 从界面的读取用户填的值
	if (strName.GetLength() <= 0) {
		MessageBox(_T("昵称不能为空"));
		return;
	}

	if (IDOK == AfxMessageBox(_T("真的要修改昵称吗?"), MB_OKCANCEL)) {
		// 所有关于路径的大小都用  MAX_PATH(微软默认的)
		WCHAR strPath[MAX_PATH] = { 0 };
		// 获取当前路径
		GetCurrentDirectoryW(MAX_PATH, strPath);
		TRACE("[ChatClient] 当前路径 %ls", strPath); // %s只会显示第一个字母，用%ls即可解决
		CString strFilePath;
		strFilePath.Format(L"%ls//Test.ini", strPath);// 前面的L 和 %ls 表示宽字节
		// 参数说明  1:项目名称  2:key名称  3:value值  4:存储路径
		bool wRet = WritePrivateProfileStringW(_T("CHAT_CLIENT"), _T("NAME"), strName, strFilePath);

	}
}





/**
 * 清屏.
 * 
 */
void CMFCChatClientDlg::OnBnClickedBtnClear()
{
	m_list.ResetContent();
}

/**
 * 自动回复..
 * 
 */
void CMFCChatClientDlg::OnBnClickedCheckAutoSend()
{
	bool is_check = ((CButton*)(GetDlgItem(IDC_CHECK_AUTO_SEND)))->GetCheck();
	TRACE("[ChatClient] 自动回复 %d", is_check);
	((CButton*)(GetDlgItem(IDC_CHECK_AUTO_SEND)))->SetCheck(!is_check);
}
