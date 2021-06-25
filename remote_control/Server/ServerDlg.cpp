
// ServerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Server.h"
#include "ServerDlg.h"
#include "resource.h"
#include "CCmdDlg.h"
#include "MyMessages.h"
#include "CFileDlg.h"
#include "CScreenDlg.h"

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
	virtual void DoDataExchange(CDataExchange* pDX);		// DDX/DDV 支持

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


// CServerDlg 对话框



CServerDlg::CServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent)
	, m_csServerLog(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ONLINE_CLIENT, m_ListOnlineClient);
	DDX_Control(pDX, IDC_BTN_OPEN_SERVER, m_btnOpenServer);
	DDX_Control(pDX, IDC_EDIT_SERVER_LOG, m_ServerLog);
	DDX_Text(pDX, IDC_EDIT_SERVER_LOG, m_csServerLog);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_BN_CLICKED(IDC_BTN_OPEN_SERVER, &CServerDlg::OnBnClickedBtnOpenServer)
	ON_BN_CLICKED(IDC_BTN_CLOSE_SERVER, &CServerDlg::OnBnClickedBtnCloseServer)
	ON_WM_CLOSE()

	ON_MESSAGE(WM_CREATE_SOCKETE_RROR, &CServerDlg::OnAcceptSucceed)
	ON_MESSAGE(WM_LISTEN_SOCKETE_RROR, &CServerDlg::OnAcceptSucceed)
	ON_MESSAGE(WM_CLIENT_ACCEPT_SUCCEED, &CServerDlg::OnAcceptSucceed)
	ON_MESSAGE(WM_BIND_SOCKET_ERROR, &CServerDlg::OnAcceptFailure)
	ON_MESSAGE(WM_CLIENT_ACCEPT_FAIL, &CServerDlg::OnAcceptFailure)
	ON_MESSAGE(WM_RECV_DATA_ERROR, &CServerDlg::OnRecvDataError)
	ON_MESSAGE(WM_SEND_DATA_ERROR, &CServerDlg::OnSendDataError)
	ON_MESSAGE(WM_CLIENT_QUIT, &CServerDlg::OnClientQuit)

	ON_BN_CLICKED(IDC_BTN_OPEN_CMD, &CServerDlg::OnBnClickedBtnOpenCmd)
	ON_BN_CLICKED(IDC_BTN_OPEN_DISK_FILE, &CServerDlg::OnBnClickedBtnOpenDiskFile)
	ON_BN_CLICKED(IDC_BTNI_OPEN_SCREEN, &CServerDlg::OnBnClickedBtniOpenScreen)
END_MESSAGE_MAP()


// CServerDlg 消息处理程序

BOOL CServerDlg::OnInitDialog()
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

	// 设置此对话框的图标。	当应用程序主窗口不是对话框时，框架将自动
	//	执行此操作
	SetIcon(m_hIcon, TRUE);	// 设置大图标
	SetIcon(m_hIcon, FALSE); // 设置小图标

	// TODO: 在此添加额外的初始化代码

	// 设置窗口句柄
	m_Messages.SetHwnd(GetSafeHwnd());

	// 初始化套接字库
	if (!m_Messages.InitWSAStartup())
	{
		AfxMessageBox("[Server]: 初始化套接字库失败");
		m_btnOpenServer.EnableWindow(FALSE);
	}

	CString csBuff;
	csBuff.Format("%s", "[Server]: 初始化套接字库成功\r\n");
	m_csServerLog += csBuff;

	//设置LIST控件表头
	SetListCtrlColumn();

	// 列表控件设定演示
	m_ListOnlineClient.SetExtendedStyle(m_ListOnlineClient.GetExtendedStyle() |
		LVS_EX_FULLROWSELECT |
		LVS_EX_HEADERDRAGDROP |
		LVS_EX_TWOCLICKACTIVATE |
		LVS_EX_GRIDLINES);

	UpdateData(FALSE);

	return TRUE;	// 除非将焦点设置到控件，否则返回 TRUE
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

//状态栏使用的数组
//static UINT indicators[] =
//{
//		IDS_STATUBAR
//};

// 如果向对话框添加最小化按钮，则需要下面的代码
//	来绘制该图标。	对于使用文档/视图模型的 MFC 应用程序，
//	这将由框架自动完成。

void CServerDlg::OnPaint()
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
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//设置LIST控件表头
void CServerDlg::SetListCtrlColumn()
{
	m_ListOnlineClient.InsertColumn(0, "IP地址", LVCFMT_CENTER, 200);
	m_ListOnlineClient.InsertColumn(1, "端口号", LVCFMT_CENTER, 120);
}

// 客户端连接成功，将客户端信息显示到列表控件上
void CServerDlg::SetListCtrlText(SOCKET socket, PSOCKADDR_IN psi)
{
	CString csBuff;
	m_ListOnlineClient.InsertItem(0, inet_ntoa(psi->sin_addr));
	csBuff.Format("%d", psi->sin_port);
	m_ListOnlineClient.SetItemText(0, 1, csBuff);
}

//=======================
LRESULT CServerDlg::OnCreateScoketErr(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox("[Server]: socket 创建失败");
	return 0;
}

LRESULT CServerDlg::OnBindScoketErr(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox("[Server]: socket 绑定失败");
	return 0;
}

LRESULT CServerDlg::OnListenScoketErr(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox("[Server]: socket 监听失败");
	return 0;
}

// 客户端连接成功
LRESULT CServerDlg::OnAcceptSucceed(WPARAM wParam, LPARAM lParam)
{
	SOCKET ClientSocket = (SOCKET)wParam;
	PSOCKADDR_IN psi = (PSOCKADDR_IN)lParam;

	// 显示连接成功的客户端信息
	SetListCtrlText(ClientSocket, psi);

	delete psi;

	// 状态条提示信息
	m_csServerLog += "[Server]: 客户端连接成功\r\n";
	m_csServerLog += "[Server]: 当前服务器已有设备连接\r\n";
	UpdateData(FALSE);

	return 0;
}

// 客户端连接失败
LRESULT CServerDlg::OnAcceptFailure(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox("[Server]: 客户端连接失败");
	// 状态条提示信息
	m_StatusBar.SetPaneText(0, "等待连接失败，请尝试重启后，在进行等待连接");

	// 恢复打开服务器的按钮
	m_btnOpenServer.EnableWindow(TRUE);
	return 0;
}

// 接收数据失败
LRESULT CServerDlg::OnRecvDataError(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox("[Server]: 接收客户端数据失败");
	return 0;
}

// 发送数据失败
LRESULT CServerDlg::OnSendDataError(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox("[Server]: 向客户端发送数据失败");
	return 0;
}

// 客户端下线
LRESULT CServerDlg::OnClientQuit(WPARAM wParam, LPARAM lParam)
{
	// 清空列表中的数据
	m_ListOnlineClient.DeleteAllItems();
	return 0;
}

// 开启服务器
void CServerDlg::OnBnClickedBtnOpenServer()
{
	// TODO: 在此添加控件通知处理程序代码
	CString csServerIP;
	USHORT usPort = 0;

	// 获取 服务器的IP地址以及端口
	GetDlgItemText(IDC_EDIT_SERVER_IP, csServerIP);
	usPort = GetDlgItemInt(IDC_EDIT_SERVER_PORT);

	if (!m_Messages.StartSocket(csServerIP.GetBuffer(), usPort))
	{
		return;
	}

	// socket 初始化成功，将按钮设置为灰色
	m_btnOpenServer.EnableWindow(FALSE);

	// 设置状态栏提示信息
	m_csServerLog += "[Server]: 等待客户端连接...\r\n";
	UpdateData(FALSE);
}

// 关闭服务器
void CServerDlg::OnBnClickedBtnCloseServer()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Messages.CloaseResource(); // 释放资源
	EndDialog(0); // 结束对话框
}

// 远程cmd
void CServerDlg::OnBnClickedBtnOpenCmd()
{
	CCmdDlg cmdDlg;
	cmdDlg.SetMessages(&m_Messages);
	cmdDlg.DoModal();
	m_Messages.SetHwnd(GetSafeHwnd());
	m_csServerLog += "[Server]: 远程客户端cmd完成\r\n";
	UpdateData(FALSE);
}

// 打开目标磁盘文件
void CServerDlg::OnBnClickedBtnOpenDiskFile()
{
	CFileDlg FileDlg;
	FileDlg.SetMessages(&m_Messages);
	FileDlg.DoModal();
	m_Messages.SetHwnd(GetSafeHwnd());
	m_csServerLog += "[Server]: 查看客户端磁盘文件完成\r\n";
	UpdateData(FALSE);
}

// 查看目标屏幕
void CServerDlg::OnBnClickedBtniOpenScreen()
{
	CScreenDlg scrDlg;
	scrDlg.SetMessages(&m_Messages);
	scrDlg.DoModal();
	m_Messages.SetHwnd(GetSafeHwnd());
	m_csServerLog += "[Server]: 客户端屏幕查看完成\r\n";
	UpdateData(FALSE);
}

// 关闭主对话框
void CServerDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnClose();
	m_Messages.CloaseResource(); // 释放资源
}