
// MyPCHunterDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MyPCHunter.h"
#include "MyPCHunterDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "common.h"
#include "CGetGDT.h"
#include "CGetProcess.h"
#include "CGetIDT.h"
#include "CGetSSDT.h"
#include "CGetDrvModule.h"
#include "CRWProcessMem.h"

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


// CMyPCHunterDlg 对话框



CMyPCHunterDlg::CMyPCHunterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYPCHUNTER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyPCHunterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
}

BEGIN_MESSAGE_MAP(CMyPCHunterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMyPCHunterDlg::OnTcnSelchangeTab1)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMyPCHunterDlg 消息处理程序

BOOL CMyPCHunterDlg::OnInitDialog()
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
	SetIcon(m_hIcon, TRUE); // 设置大图标
	SetIcon(m_hIcon, FALSE); // 设置小图标

	// TODO: 在此添加额外的初始化代码

	// 加载驱动
	LoadDriver();
	// 启动驱动
	StartDriver();

	// 打开设备对象
	g_hDev = CreateFile(L"\\\\?\\MyPCHunterDev", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	if (g_hDev == INVALID_HANDLE_VALUE)
	{
		CString Buffer;
		Buffer.Format(L"[MyPCHunter]：程序初始化失败，请检查驱动是否安装以及启动\n");
		MessageBox(Buffer);
		return FALSE;
	}

	// 发送PID 
	int PID = _getpid();
	DWORD dwSize = 0;
	DeviceIoControl(g_hDev, GETPID, &PID, sizeof(int), NULL, NULL, &dwSize, NULL);

	// 初始化Tab控件
	m_TabCtrl.InsertItem(0, L"GDT");
	m_TabCtrl.InsertItem(1, L"进程");
	m_TabCtrl.InsertItem(2, L"IDT");
	m_TabCtrl.InsertItem(3, L"SSDT");
	m_TabCtrl.InsertItem(4, L"驱动模块");
	m_TabCtrl.InsertItem(5, L"读写进程内存");

	// 创建窗口对象并保存
	m_TabCtrl.m_DialogObj[0] = new CGetGDT();
	m_TabCtrl.m_DialogObj[1] = new CGetProcess();
	m_TabCtrl.m_DialogObj[2] = new CGetIDT();
	m_TabCtrl.m_DialogObj[3] = new CGetSSDT();
	m_TabCtrl.m_DialogObj[4] = new CGetDrvModule();
	m_TabCtrl.m_DialogObj[5] = new CRWProcessMem();

	m_TabCtrl.m_DialogObj[0]->Create(IDD_CMyGDT, &m_TabCtrl);
	m_TabCtrl.m_DialogObj[1]->Create(IDD_CMyProcess, &m_TabCtrl);
	m_TabCtrl.m_DialogObj[2]->Create(IDD_CGetIDT, &m_TabCtrl);
	m_TabCtrl.m_DialogObj[3]->Create(IDD_CGetSSDT, &m_TabCtrl);
	m_TabCtrl.m_DialogObj[4]->Create(IDD_CGetDrvModule, &m_TabCtrl);
	m_TabCtrl.m_DialogObj[5]->Create(IDD_CRWProcessMem, &m_TabCtrl);

	CRect pos;
	m_TabCtrl.GetClientRect(pos); // 转换为客户端坐标
	pos.DeflateRect(2, 30, 3, 2); // 移动坐标，免得盖住菜单了

	// 初始化窗口
	m_TabCtrl.m_DialogObj[0]->MoveWindow(pos);
	m_TabCtrl.m_DialogObj[0]->ShowWindow(SW_SHOW);
	m_TabCtrl.m_DialogObj[1]->ShowWindow(SW_HIDE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMyPCHunterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMyPCHunterDlg::OnPaint()
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
HCURSOR CMyPCHunterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMyPCHunterDlg::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	CRect cRect;
	GetClientRect(cRect);
	cRect.DeflateRect(2, 30, 3, 2);

	// 获取所选择的Tab项
	DWORD dwSelIdx = m_TabCtrl.GetCurSel();
	for (DWORD i = 0; i < m_TabCtrl.m_dwTabNum; ++i)
	{
		if (dwSelIdx == i)
		{
			m_TabCtrl.m_DialogObj[i]->MoveWindow(cRect);
			m_TabCtrl.m_DialogObj[i]->ShowWindow(SW_SHOW);
		}
		else
		{
			m_TabCtrl.m_DialogObj[i]->ShowWindow(SW_HIDE);
		}
	}
	*pResult = 0;
}

// 加载驱动
void CMyPCHunterDlg::LoadDriver()
{
	TCHAR DriverFilePath[MAX_PATH] = L"C:\\Documents and Settings\\51asm\\桌面\\MyPCHunterDriver.sys";
	// 打开服务管理器
	m_hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (m_hSCManager == NULL) 
	{
		// 保存错误信息
		m_dwLastError = GetLastError();
	}
	// 创建服务
	m_hService = CreateService(m_hSCManager, 
		L"MyDriver1", 
		L"MyDriver1", 
		SERVICE_ALL_ACCESS, 
		SERVICE_KERNEL_DRIVER, 
		SERVICE_DEMAND_START, 
		SERVICE_ERROR_IGNORE, 
		DriverFilePath,
		NULL, NULL, NULL, NULL, NULL);
	if (m_hService == NULL) 
	{
		// 保存错误信息
		m_dwLastError = GetLastError();
	}
}

// 启动驱动
void CMyPCHunterDlg::StartDriver()
{
	// 判断服务是否存在，存在就打开
	if (m_dwLastError == ERROR_SERVICE_EXISTS) 
	{
		m_hService = OpenService(m_hService, L"MyDriver", SERVICE_ALL_ACCESS);
	}

	// 判断服务是否创建成功
	if (!m_hService)
	{
		// 服务创建失败，关闭服务管理器句柄
		CloseServiceHandle(m_hSCManager);
		return;
	}

	SERVICE_STATUS status;

	CString cs;
	// 查询服务状态
	if (QueryServiceStatus(m_hService, &status) == 0)
	{
		cs.Format(L"%d", GetLastError());
		//MessageBox(cs);
	}
	// 服务处于暂停状态
	if (status.dwCurrentState == SERVICE_STOPPED)
	{
		// 启动服务
		StartService(m_hService, NULL, NULL);

		Sleep(1000);

		// 查询服务状态
		if (QueryServiceStatus(m_hService, &status))
		{
			cs.Format(L"%d", GetLastError());
			//MessageBox(cs);
		}

		// 服务是否处于运行状态
		if (status.dwCurrentState != SERVICE_RUNNING)
		{
			MessageBox(L"驱动启动失败，请检查驱动是否安装");
			CloseServiceHandle(m_hSCManager);
			CloseServiceHandle(m_hService);
		}
	}
}

// 停止驱动
void CMyPCHunterDlg::StopDriver()
{
	CloseHandle(g_hDev);

	SERVICE_STATUS status;

	CString cs;
	// 查询服务状态
	if (QueryServiceStatus(m_hService, &status) == 0)
	{
		cs.Format(L"%d", GetLastError());
		//MessageBox(cs);
	}

	// 如果不处于暂停状态
	if (status.dwCurrentState != SERVICE_STOPPED)
	{
		// 停止服务
		ControlService(m_hService, SERVICE_CONTROL_STOP, &status);

		// 直到服务停止
		while (QueryServiceStatus(m_hService, &status) == TRUE)
		{
			Sleep(status.dwWaitHint);
			break;
		}
	}
}

// 卸载驱动
void CMyPCHunterDlg::UnLoadDriver()
{
	// 删除驱动（服务）
	if (!DeleteService(m_hService))
	{
		// 驱动卸载失败
		CString cs;
		cs.Format(_T("%d"), GetLastError());
		//MessageBox(L"DeleteService Failed", cs);
	}

	// 关闭服务句柄
	CloseServiceHandle(m_hService);
	// 关闭服务管理器句柄
	CloseServiceHandle(m_hSCManager);
}


void CMyPCHunterDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	// 停止服务
	StopDriver();
	// 卸载驱动
	UnLoadDriver();
}
