
// ClientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include "CLoginDlg.h"

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


// CClientDlg 对话框



CClientDlg::CClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
	, m_csStuName(_T(""))
	, m_csStuID(_T(""))
	, m_csClass(_T(""))
	, m_csClassID(_T(""))
	, m_csClassName(_T(""))
	, m_csCourseID(_T(""))
	, m_csCourseName(_T(""))
	, m_csSelectStuID(_T(""))
	, m_csSelectCourseID(_T(""))
	, m_csNewCourseID(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDE_STU_ID, m_csStuID);
	DDX_Text(pDX, IDE_STU_NAME, m_csStuName);
	DDX_Text(pDX, IDE_CLASS, m_csClass);
	DDX_Control(pDX, IDL_Info, m_ListShowQueryInfo);
	DDX_Control(pDX, IDC_COMBO_TABLES, m_Tables);
	DDX_Control(pDX, IDE_STU_ID, m_Edit_StuID);
	DDX_Control(pDX, IDE_STU_NAME, m_Edit_StuName);
	DDX_Control(pDX, IDE_CLASS, m_Edit_StuAtClassID);
	DDX_Text(pDX, IDE_CLASS_ID, m_csClassID);
	DDX_Control(pDX, IDE_CLASS_ID, m_Edit_ClassID);
	DDX_Text(pDX, IDE_CLASS_NAME, m_csClassName);
	DDX_Control(pDX, IDE_CLASS_NAME, m_Edit_ClassName);
	DDX_Text(pDX, IDE_COURSE_ID, m_csCourseID);
	DDX_Control(pDX, IDE_COURSE_ID, m_Edit_CourseID);
	DDX_Text(pDX, IDE_COURSE_NAME, m_csCourseName);
	DDX_Control(pDX, IDE_COURSE_NAME, m_Edit_CourseName);
	DDX_Text(pDX, IDE_SELECT_STU_ID, m_csSelectStuID);
	DDX_Control(pDX, IDE_SELECT_STU_ID, m_Edit_SelectStuID);
	DDX_Text(pDX, IDE_SELECT_COURSE_ID, m_csSelectCourseID);
	DDX_Control(pDX, IDE_SELECT_COURSE_ID, m_Edit_SelectCourseID);
	DDX_Text(pDX, IDE_SELECT_NEW_COURSE_ID2, m_csNewCourseID);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDB_QUERY, &CClientDlg::OnBnClickedQuery)
	ON_BN_CLICKED(IDB_INCREASE, &CClientDlg::OnBnClickedIncrease)
	ON_NOTIFY(NM_DBLCLK, IDL_Info, &CClientDlg::OnDblclkIdlInfo)
	ON_BN_CLICKED(IDB_DELETE, &CClientDlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDB_REVISE, &CClientDlg::OnBnClickedRevise)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CClientDlg 消息处理程序

BOOL CClientDlg::OnInitDialog()
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

	// 初始化数据库
	if (InitMySQL() < 0)
	{
		EndDialog(-1);
		return FALSE;
	}

	// 显示登录对话框
	//CLoginDlg logindlg;
	//if (logindlg.DoModal() != IDOK)
	//{
	//	EndDialog(-1);
	//	return FALSE;
	//}

	// 初始化combo tables 控件
	InitTables();

	// 初始化socket
	InitSocket();
	// 初始化列表控件
	m_ListShowQueryInfo.SetExtendedStyle(
		m_ListShowQueryInfo.GetExtendedStyle() |
		LVS_EX_FULLROWSELECT |
		LVS_EX_HEADERDRAGDROP |
		LVS_EX_TWOCLICKACTIVATE |
		LVS_EX_GRIDLINES);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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


/*
如果向对话框添加最小化按钮，则需要下面的代码来绘制该图标。
对于使用文档/视图模型的 MFC 应用程序，这将由框架自动完成。
*/
void CClientDlg::OnPaint()
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
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 初始化数据库
#if 1
int CClientDlg::InitMySQL()
{
	// 初始化数据库
	if (mysql_init(&m_mysql) == NULL)
	{
		AfxMessageBox(mysql_error(&m_mysql));
		return -1;
	}

	// 设置字符集
	if (!mysql_set_character_set(&m_mysql, "gbk"))
	{
		TRACE("New client character set: %s\n",
			mysql_character_set_name(&m_mysql));
	}

	// 连接数据库
	if (mysql_real_connect(&m_mysql,
		"localhost",
		"root",
		"Li991230.",
		NULL,
		3306,
		NULL, 0) == NULL)
	{
		AfxMessageBox(mysql_error(&m_mysql));
		return -1;
	}
	// 使用创建的学生表
	char szUSETable[0x100] = "USE STU_TABLE";
	if (mysql_real_query(&m_mysql, szUSETable, strlen(szUSETable) + 1) != 0)
	{
		AfxMessageBox(mysql_error(&m_mysql));
		return -1;
	}

	return 0;
}
#endif // 0

void CClientDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	mysql_close(&m_mysql);
	// TODO: 在此处添加消息处理程序代码
}

// 查询数据库表
void CClientDlg::OnBnClickedQuery()
{
	// 向服务器信号，需要增加数据
#if 1
	m_socket.Send("query", strlen("query"));
	Sleep(100);
#endif // 0

	UpdateData(TRUE);

	// 清空列表内所有数据
	m_ListShowQueryInfo.DeleteAllItems();
	int nCount = m_ListShowQueryInfo.GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < nCount; i++) {
		m_ListShowQueryInfo.DeleteColumn(0);
	}

	// 获取控件ID
	int nSel = m_Tables.GetCurSel();
	CString csSQL;
	CString csFmt;
	switch (nSel)
	{
	case 0:
	{
		// 学生表
		csSQL = "SELECT * FROM T_STUDENT WHERE 1=1";
		if (!m_csStuID.IsEmpty())
		{
			csFmt.Format(" and STU_ID='%s'", m_csStuID);
			csSQL += csFmt;
		}
		if (!m_csStuName.IsEmpty())
		{
			csFmt.Format(" and STU_NAME='%s'", m_csStuName);
			csSQL += csFmt;
		}
		if (!m_csClass.IsEmpty())
		{
			csFmt.Format(" and CLASS_ID='%s'", m_csClass);
			csSQL += csFmt;
		}
		// 向服务器发送数据
		//m_socket.Send(csSQL, csSQL.GetLength());
		//Sleep(100);
		//UpdateListInfo();
		break;
	}
	case 1:
	{
		// 班级表
		csSQL = "SELECT * FROM T_CLASS WHERE 1=1";
		if (!m_csClassID.IsEmpty())
		{
			csFmt.Format(" and CLASS_ID='%s'", m_csClassID);
			csSQL += csFmt;
		}
		if (!m_csClassName.IsEmpty())
		{
			csFmt.Format(" and CLASS_NAME='%s'", m_csClassName);
			csSQL += csFmt;
		}
		//m_socket.Send(csSQL, csSQL.GetLength());
		//Sleep(100);
		//UpdateListInfo();
		break;
	}
	case 2:
	{
		// 课程表
		csSQL = "SELECT * FROM T_COURSE WHERE 1=1";
		if (!m_csCourseID.IsEmpty())
		{
			csFmt.Format(" and COURSE_ID='%s'", m_csCourseID);
			csSQL += csFmt;
		}
		if (!m_csCourseName.IsEmpty())
		{
			csFmt.Format(" and COURSE_NAME='%s'", m_csCourseName);
			csSQL += csFmt;
		}
		//m_socket.Send(csSQL, csSQL.GetLength());
		//Sleep(100);
		//UpdateListInfo();
		break;
	}
	case 3:
	{
		// 选课表
		csSQL = "SELECT * FROM T_SELECT WHERE 1=1";
		if (!m_csSelectStuID.IsEmpty())
		{
			csFmt.Format(" and STU_ID='%s'", m_csSelectStuID);
			csSQL += csFmt;
		}
		if (!m_csSelectCourseID.IsEmpty())
		{
			csFmt.Format(" and COURSE_ID='%s'", m_csSelectCourseID);
			csSQL += csFmt;
		}
		//m_socket.Send(csSQL, csSQL.GetLength());
		//Sleep(100);
		//UpdateListInfo();
		break;
	}
	}

#if 0
	m_socket.Send(csSQL, csSQL.GetLength());
	Sleep(100);
	UpdateListInfo();
#endif // 0

	// 显示查询结果
	ShowQueryData(csSQL);
}

// 更新列表信息
void CClientDlg::UpdateListInfo()
{
	char szBytesAllRecv[0x200] = { 0 };
	int nBytesRecv = 0; // 当前接收字节数
	char szBuffTemp[100] = { 0 };
	memcpy(szBuffTemp, szBytesAllRecv, nBytesRecv);
	m_socket.Recv(szBytesAllRecv, sizeof(szBytesAllRecv), nBytesRecv);

	// 遍历字段
	CString cs;
	cs = szBytesAllRecv;
	int nIdx[10] = { 0 };
	int nCount = 0;
	int n = 0;
	for (size_t i = 0; i < cs.GetLength(); i++)
	{
		i = cs.Find('-', i);
		if (i != -1 || i < 100)
		{
			nCount++;
			nIdx[n] = i;
		}
		else
		{
			break;
		}
		n++;
	}
	if (nCount == 1)
	{
		char szField11[20] = { 0 };
		char szField22[20] = { 0 };
		memcpy(szField11, cs, nIdx[0]);
		memcpy(szField22, szBytesAllRecv + nIdx[0] + 1, strlen(szBytesAllRecv));

		m_ListShowQueryInfo.InsertColumn(0, szField11, LVCFMT_LEFT, 100);
		m_ListShowQueryInfo.InsertColumn(1, szField22, LVCFMT_LEFT, 100);

		// 接收查询的表数据
		nBytesRecv = 0; // 当前接收字节数
		memset(szBytesAllRecv, 0, sizeof(szBytesAllRecv));
		memset(szBuffTemp, 0, sizeof(szBuffTemp));
		memcpy(szBuffTemp, szBytesAllRecv, nBytesRecv);
		m_socket.Recv(szBytesAllRecv, sizeof(szBytesAllRecv), nBytesRecv);
		ShowTableData(szBytesAllRecv);
		return;
	}
	if (nCount == 2)
	{
		char szField1[20] = { 0 };
		char szField2[20] = { 0 };
		char szField3[20] = { 0 };
		memcpy(szField1, cs, nIdx[0]);
		int nTemp1 = nIdx[0];
		int nTemp2 = nIdx[1];
		int nTemp3 = nTemp2 - nTemp1;
		memcpy(szField2, szBytesAllRecv + nIdx[0] + 1, nTemp3 - 1);
		memcpy(szField3, szBytesAllRecv + nIdx[1] + 1, strlen(szBytesAllRecv));
		m_ListShowQueryInfo.InsertColumn(0, szField1, LVCFMT_LEFT, 100);
		m_ListShowQueryInfo.InsertColumn(1, szField2, LVCFMT_LEFT, 100);
		m_ListShowQueryInfo.InsertColumn(2, szField3, LVCFMT_LEFT, 100);

		// 接收查询的表数据
		nBytesRecv = 0; // 当前接收字节数
		memset(szBytesAllRecv, 0, sizeof(szBytesAllRecv));
		memset(szBuffTemp, 0, sizeof(szBuffTemp));
		memcpy(szBuffTemp, szBytesAllRecv, nBytesRecv);
		m_socket.Recv(szBytesAllRecv, sizeof(szBytesAllRecv), nBytesRecv);
		ShowTableData(szBytesAllRecv);
		return;
	}
}

void CClientDlg::ShowTableData(const char* pszBuff)
{
	char buf[1024] = { 0 };
	ULONG i = 0;
	if (pszBuff[i] != '\0')
	{ 
		// 字符串不为空
		while (true)
		{
			while (pszBuff[i] == '\r' || pszBuff[i] == '\n') i++; // 跳过换行符
			if (pszBuff[i] == '\0') break; // 读取结束
			if (1 == sscanf_s(pszBuff + i, "%1023[^\r\n]", buf, 1024))
			{
				// 遍历字段
				CString cs;
				cs = buf;
				int nIdx[10] = { 0 };
				int nCount = 0;
				int n = 0;
				for (size_t i = 0; i < cs.GetLength(); i++)
				{
					i = cs.Find('-', i);
					if (i != -1 || i < 100)
					{
						nCount++;
						nIdx[n] = i;
					}
					else
					{
						break;
					}
					n++;
				}
				int nItem = 0;
				if (nCount == 1)
				{
					char szField11[20] = { 0 };
					char szField22[20] = { 0 };
					memcpy(szField11, cs, nIdx[0]);
					memcpy(szField22, buf + nIdx[0] + 1, strlen(buf));
					m_ListShowQueryInfo.InsertItem(nItem, szField11);
					m_ListShowQueryInfo.SetItemText(nItem, 1, szField22);
				}
				if (nCount == 2)
				{
					char szField1[20] = { 0 };
					char szField2[20] = { 0 };
					char szField3[20] = { 0 };
					memcpy(szField1, cs, nIdx[0]);
					int nTemp1 = nIdx[0];
					int nTemp2 = nIdx[1];
					int nTemp3 = nTemp2 - nTemp1;
					memcpy(szField2, buf + nIdx[0] + 1, nTemp3 - 1);
					memcpy(szField3, buf + nIdx[1] + 1, strlen(buf));
					m_ListShowQueryInfo.InsertItem(nItem, szField1);
					m_ListShowQueryInfo.SetItemText(nItem, 1, szField2);
					m_ListShowQueryInfo.SetItemText(nItem, 2, szField3);
				}
				i += strlen(buf);
			}
			else
			{
				break; // 出错退出
			}
		}
	}
}

// 初始化socket
void CClientDlg::InitSocket()
{
	// 创建socket
	m_socket.CreateSocket();
	// 连接服务器
	m_socket.Connect("127.0.0.1", 0x6868);
	//AfxMessageBox("连接服务器成功");
}

void CClientDlg::ShowQueryData(CString cs)
{
	// 查询结果
	if (mysql_real_query(&m_mysql, cs.GetBuffer(0), cs.GetLength() + 1) != 0)
	{
		AfxMessageBox(mysql_error(&m_mysql));
		return;
	}

	// 保存结果
	MYSQL_RES* result = mysql_store_result(&m_mysql);

	// 遍历字段
	unsigned int num_fields;
	unsigned int i;
	MYSQL_FIELD* fields;

	int nItem = 0;
	num_fields = mysql_num_fields(result);
	fields = mysql_fetch_fields(result);
	for (i = 0; i < num_fields; i++)
	{
		m_ListShowQueryInfo.InsertColumn(i, fields[i].name, LVCFMT_LEFT, 100);
	}

	// 遍历数据
	MYSQL_ROW row;
	while ((row = mysql_fetch_row(result)))
	{
		unsigned long* lengths;
		lengths = mysql_fetch_lengths(result);
		for (i = 0; i < num_fields; i++)
		{
			if (i == 0)
			{
				m_ListShowQueryInfo.InsertItem(nItem, row[i]);
			}
			m_ListShowQueryInfo.SetItemText(nItem, i, row[i]);
		}
		nItem++;
	}

	// 释放
	mysql_free_result(result);
}

// 初始化combo tables 控件
void CClientDlg::InitTables()
{
	m_Tables.AddString("学生表");
	m_Tables.AddString("班级表");
	m_Tables.AddString("课程表");
	m_Tables.AddString("选课表");
	m_Tables.SetCurSel(0);
}

// 增加
void CClientDlg::OnBnClickedIncrease()
{
	// 向服务器信号，需要增加数据
	m_socket.Send("increase", strlen("increase"));
	Sleep(100);
	UpdateData(TRUE);

	// 获取点击表的索引
	int nSel = m_Tables.GetCurSel();
	CString csSQL;
	CString csFmt;
	switch (nSel)
	{
	case 0:
	{
		// 学生表增加数据
		csSQL = "INSERT INTO T_STUDENT VALUES(";
		if (!m_csStuID.IsEmpty())
		{
			csFmt.Format("'%s',", m_csStuID);
			csSQL += csFmt;
		}
		if (!m_csStuName.IsEmpty())
		{
			csFmt.Format("'%s',", m_csStuName);
			csSQL += csFmt;
		}
		if (!m_csClass.IsEmpty())
		{
			csFmt.Format("'%s'", m_csClass);
			csSQL += csFmt;
		}
		csSQL += ");";

		// 向服务器发送数据
		m_socket.Send(csSQL, csSQL.GetLength());
		Sleep(100);
		break;
	}
	case 1:
	{
		// 班级表增加数据
		csSQL = "INSERT INTO T_CLASS VALUES(";
		if (!m_csClassID.IsEmpty())
		{
			csFmt.Format("'%s',", m_csClassID);
			csSQL += csFmt;
		}
		if (!m_csClassName.IsEmpty())
		{
			csFmt.Format("'%s'", m_csClassName);
			csSQL += csFmt;
		}
		csSQL += ");";

		// 向服务器发送数据
		m_socket.Send(csSQL, csSQL.GetLength());
		Sleep(100);
		break;
	}
	case 2:
	{
		// 课程表增加数据
		csSQL = "INSERT INTO T_COURSE VALUES(";
		if (!m_csCourseID.IsEmpty())
		{
			csFmt.Format("'%s',", m_csCourseID);
			csSQL += csFmt;
		}
		if (!m_csCourseName.IsEmpty())
		{
			csFmt.Format("'%s'", m_csCourseName);
			csSQL += csFmt;
		}
		csSQL += ");";

		// 向服务器发送数据
		m_socket.Send(csSQL, csSQL.GetLength());
		Sleep(100);
		break;
	}
	case 3:
	{
		// 选课表增加数据
		csSQL = "INSERT INTO T_SELECT VALUES(";
		if (!m_csSelectStuID.IsEmpty())
		{
			csFmt.Format("'%s',", m_csSelectStuID);
			csSQL += csFmt;
		}
		if (!m_csSelectCourseID.IsEmpty())
		{
			csFmt.Format("'%s'", m_csSelectCourseID);
			csSQL += csFmt;
		}
		csSQL += ");";

		// 向服务器发送数据
		m_socket.Send(csSQL, csSQL.GetLength());
		Sleep(100);
		break;
	}
	}
}

// 双击列表项，将列表上的项值更新到Edit控件上
void CClientDlg::OnDblclkIdlInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	
	// 获取点击表的索引
	int nSelIdx = m_Tables.GetCurSel();
	int nSel = 0;
	CString csBuff;
	switch (nSelIdx)
	{
	case 0:
	{
		// 学生表
		nSel = m_ListShowQueryInfo.GetNextItem(-1, LVIS_SELECTED);
		if (nSel < 0)
		{
			return;
		}
		csBuff = m_ListShowQueryInfo.GetItemText(nSel, 0);
		m_Edit_StuID.SetWindowText(csBuff);
		csBuff = m_ListShowQueryInfo.GetItemText(nSel, 1);
		m_Edit_StuName.SetWindowText(csBuff);
		csBuff = m_ListShowQueryInfo.GetItemText(nSel, 2);
		m_Edit_StuAtClassID.SetWindowText(csBuff);
		break;
	}
	case 1:
	{
		// 班级表
		nSel = m_ListShowQueryInfo.GetNextItem(-1, LVIS_SELECTED);
		if (nSel < 0)
		{
			return;
		}
		csBuff = m_ListShowQueryInfo.GetItemText(nSel, 0);
		m_Edit_ClassID.SetWindowText(csBuff);
		csBuff = m_ListShowQueryInfo.GetItemText(nSel, 1);
		m_Edit_ClassName.SetWindowText(csBuff);
		break;
	}
	case 2:
	{
		// 课程表
		nSel = m_ListShowQueryInfo.GetNextItem(-1, LVIS_SELECTED);
		if (nSel < 0)
		{
			return;
		}
		csBuff = m_ListShowQueryInfo.GetItemText(nSel, 0);
		m_Edit_CourseID.SetWindowText(csBuff);
		csBuff = m_ListShowQueryInfo.GetItemText(nSel, 1);
		m_Edit_CourseName.SetWindowText(csBuff);
		break;
	}
	case 3:
	{
		// 选课表
		nSel = m_ListShowQueryInfo.GetNextItem(-1, LVIS_SELECTED);
		if (nSel < 0)
		{
			return;
		}
		csBuff = m_ListShowQueryInfo.GetItemText(nSel, 0);
		m_Edit_SelectStuID.SetWindowText(csBuff);
		csBuff = m_ListShowQueryInfo.GetItemText(nSel, 1);
		m_Edit_SelectCourseID.SetWindowText(csBuff);
		break;
	}
	}
	// 将列表控件上的数据更新到Edit控件上
	*pResult = 0;
}

//  删除表项
void CClientDlg::OnBnClickedDelete()
{
	// 向服务器发送信号，需要删除数据
	m_socket.Send("delete", strlen("delete"));
	Sleep(100);
	UpdateData(TRUE);

	// 获取点击表的索引
	int nSel = m_Tables.GetCurSel();
	CString csSQL;
	CString csFmt;
	switch (nSel)
	{
	case 0:
	{
		// 学生表
		csSQL = "DELETE FROM T_STUDENT WHERE ";
		if (!m_csStuID.IsEmpty())
		{
			csFmt.Format("STU_ID = '%s' AND ", m_csStuID);
			csSQL += csFmt;
		}
		if (!m_csStuName.IsEmpty())
		{
			csFmt.Format("STU_NAME = '%s' AND ", m_csStuName);
			csSQL += csFmt;
		}
		if (!m_csClass.IsEmpty())
		{
			csFmt.Format("CLASS_ID = '%s';", m_csClass);
			csSQL += csFmt;
		}

		// 向服务器发送要删除的表数据
		m_socket.Send(csSQL, csSQL.GetLength());
		Sleep(100);
		break;
	}
	case 1:
	{
		// 班级表
		csSQL = "DELETE FROM T_CLASS WHERE ";
		if (!m_csClassID.IsEmpty())
		{
			csFmt.Format("CLASS_ID = '%s' AND ", m_csClassID);
			csSQL += csFmt;
		}
		if (!m_csClassName.IsEmpty())
		{
			csFmt.Format("CLASS_NAME = '%s';", m_csClassName);
			csSQL += csFmt;
		}

		// 向服务器发送要删除的表数据
		m_socket.Send(csSQL, csSQL.GetLength());
		Sleep(100);
		break;
	}
	case 2:
	{
		// 课程表
		csSQL = "DELETE FROM T_COURSE WHERE ";
		if (!m_csCourseID.IsEmpty())
		{
			csFmt.Format("COURSE_ID = '%s' AND ", m_csCourseID);
			csSQL += csFmt;
		}
		if (!m_csCourseName.IsEmpty())
		{
			csFmt.Format("COURSE_NAME = '%s';", m_csCourseName);
			csSQL += csFmt;
		}

		// 向服务器发送数据
		m_socket.Send(csSQL, csSQL.GetLength());
		Sleep(100);
		break;
	}
	case 3:
	{
		// 选课表
		csSQL = "DELETE FROM T_SELECT WHERE ";
		if (!m_csSelectStuID.IsEmpty())
		{
			csFmt.Format("STU_ID = '%s' AND ", m_csSelectStuID);
			csSQL += csFmt;
		}
		if (!m_csSelectCourseID.IsEmpty())
		{
			csFmt.Format("COURSE_ID = '%s';", m_csSelectCourseID);
			csSQL += csFmt;
		}

		// 向服务器发送数据
		m_socket.Send(csSQL, csSQL.GetLength());
		Sleep(100);
		break;
	}
	}
}

// 修改表数据项
void CClientDlg::OnBnClickedRevise()
{
	// 向服务器发送信号，需要修改数据
	m_socket.Send("revise", strlen("revise"));
	Sleep(100);
	UpdateData(TRUE);

	// 获取点击表的索引
	int nSel = m_Tables.GetCurSel();
	CString csSQL;
	CString csFmt;
	switch (nSel)
	{
	case 0:
	{
		// 学生表
		csSQL = "UPDATE T_STUDENT SET ";
		// 学生姓名
		if (!m_csStuName.IsEmpty())
		{
			csFmt.Format("STU_NAME = '%s' WHERE ", m_csStuName);
			csSQL += csFmt;
		}
		// 学生ID
		if (!m_csStuID.IsEmpty())
		{
			csFmt.Format("STU_ID = '%s';", m_csStuID);
			csSQL += csFmt;
		}

		// 向服务器发送要删除的表数据
		m_socket.Send(csSQL, csSQL.GetLength());
		Sleep(100);
		break;
	}
	case 1:
	{
		// 班级表
		csSQL = "UPDATE T_CLASS SET ";
		// 班级名
		if (!m_csClassName.IsEmpty())
		{
			csFmt.Format("CLASS_NAME = '%s' WHERE ", m_csClassName);
			csSQL += csFmt;
		}
		// 班级ID
		if (!m_csClassID.IsEmpty())
		{
			csFmt.Format("CLASS_ID = '%s';", m_csClassID);
			csSQL += csFmt;
		}

		// 向服务器发送要删除的表数据
		m_socket.Send(csSQL, csSQL.GetLength());
		Sleep(100);
		break;
	}
	case 2:
	{
		// 课程表
		csSQL = "UPDATE T_COURSE SET ";
		// 课程名
		if (!m_csCourseName.IsEmpty())
		{
			csFmt.Format("COURSE_NAME = '%s' WHERE ", m_csCourseName);
			csSQL += csFmt;
		}
		// 课程ID
		if (!m_csCourseID.IsEmpty())
		{
			csFmt.Format("COURSE_ID = '%s';", m_csCourseID);
			csSQL += csFmt;
		}

		// 向服务器发送要删除的表数据
		m_socket.Send(csSQL, csSQL.GetLength());
		Sleep(100);
		break;
	}
	case 3:
	{
		// 选课表
		csSQL = "UPDATE T_SELECT SET ";
		// 新课程ID
		if (!m_csNewCourseID.IsEmpty())
		{
			csFmt.Format("COURSE_ID = '%s' WHERE ", m_csNewCourseID);
			csSQL += csFmt;
		}
		if (!m_csSelectStuID.IsEmpty())
		{
			csFmt.Format("STU_ID = '%s' AND ", m_csSelectStuID);
			csSQL += csFmt;
		}
		if (!m_csSelectCourseID.IsEmpty())
		{
			csFmt.Format("COURSE_ID = '%s';", m_csSelectCourseID);
			csSQL += csFmt;
		}
		// 向服务器发送数据
		m_socket.Send(csSQL, csSQL.GetLength());
		Sleep(100);
		break;
	}
	}
}

void CClientDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString csFmt;
	// 向服务器发送数据
	csFmt.Format("%s", "exit");
	m_socket.Send(csFmt, csFmt.GetLength());
	Sleep(100);
	CDialogEx::OnClose();
}