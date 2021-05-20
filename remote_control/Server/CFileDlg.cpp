// CFileDlg.cpp: 实现文件
//

#include "pch.h"
#include "Server.h"
#include "CFileDlg.h"
#include "afxdialogex.h"


// CFileDlg 对话框

IMPLEMENT_DYNAMIC(CFileDlg, CDialogEx)

CFileDlg::CFileDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CFileDlg, pParent)
{
	// 结构体初始化
	ZeroMemory(&m_LIDownFileSize, sizeof(m_LIDownFileSize));
	ZeroMemory(&m_LICurlDownFileSize, sizeof(m_LICurlDownFileSize));
}

CFileDlg::~CFileDlg()
{
}

void CFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_FILE_PATH, m_EditFilePath);
	DDX_Control(pDX, IDC_LIST_FILE, m_ListFile);
	DDX_Control(pDX, IDC_PROGRESS_DOWNUP, m_ProgressDownUp);
}


BEGIN_MESSAGE_MAP(CFileDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FILE, &CFileDlg::OnDblclkListFile)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_FILE, &CFileDlg::OnRclickListFile)
	ON_COMMAND(ID_MN_UP_FILE, &CFileDlg::OnUploadFile)
	ON_COMMAND(ID_MN_DOWN_FILE, &CFileDlg::OnDownIsFile)
	ON_MESSAGE(WM_DRIVE_INFO, &CFileDlg::OnGetFileDrive)
	ON_MESSAGE(WM_GET_FOLDER_NEXT_DATA, &CFileDlg::OnGetNextFolder)
	ON_MESSAGE(WM_GET_FOLDER_PREV_DATA, &CFileDlg::OnGetPrevFolder)
	ON_MESSAGE(WM_HAVE_NEXT_FILE, &CFileDlg::OnHaveNextFile)
	ON_MESSAGE(WM_HAVE_PREV_FILE, &CFileDlg::OnHavePrevFile)
	ON_MESSAGE(WM_IS_FOLDER, &CFileDlg::OnIsFolder)
	ON_MESSAGE(WM_IS_FILE, &CFileDlg::OnIsFile)
	ON_MESSAGE(WM_GET_FILE_SIZE, &CFileDlg::OnGetFileSize)
	ON_MESSAGE(WM_GET_FILE_DATA, &CFileDlg::OnGetFileData)
	ON_MESSAGE(WM_DOWN_FILE_DATA_SUCC, &CFileDlg::OnDownFileDataSucc)
	ON_MESSAGE(WM_FILE_UPLOAD_SUCC, &CFileDlg::OnUploadFileDataSucc)
	ON_MESSAGE(WM_GET_FILE_NEXT_DATA, &CFileDlg::OnGetFileNextData)
	ON_MESSAGE(WM_RECV_DATA_ERROR, &CFileDlg::OnRecvFileDataError)
	ON_MESSAGE(WM_SEND_DATA_ERROR, &CFileDlg::OnSendFileDataError)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CFileDlg 消息处理程序


BOOL CFileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:	在此添加额外的初始化

	m_pMessages->SetHwnd(GetSafeHwnd());

	//发送获取驱动器的头信息
	PACKETHEAD SendHead;
	SendHead.m_nCmd = PK_GET_DES_DRIVE;
	SendHead.m_nLen = 0;

	if (!m_pMessages->SendData((char*)&SendHead, sizeof(SendHead)))
	{
		AfxMessageBox("发送数据失败");
		return 0;
	}

	//设置List风格为选中一行
	DWORD dwStyle = m_ListFile.GetExtendedStyle();
	m_ListFile.SetExtendedStyle(dwStyle | LVS_EX_FULLROWSELECT);

	//添加List控件表头
	m_ListFile.InsertColumn(0, "名称", LVCFMT_CENTER, 500);
	//m_ListFile.InsertColumn(1, "类型", LVCFMT_CENTER, 100);
	//m_ListFile.InsertColumn(2, "大小", LVCFMT_CENTER, 200);
	// 列表控件设定演示
	m_ListFile.SetExtendedStyle(m_ListFile.GetExtendedStyle() |
		LVS_EX_FULLROWSELECT |
		LVS_EX_HEADERDRAGDROP |
		LVS_EX_TWOCLICKACTIVATE |
		LVS_EX_GRIDLINES);

	return TRUE;	// return TRUE unless you set the focus to a control
					// 异常: OCX 属性页应返回 FALSE
}


void CFileDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}

// 获取客户端磁盘驱动
LRESULT CFileDlg::OnGetFileDrive(WPARAM wParam, LPARAM lParam)
{
	// 删除控件上的所有数据
	m_ListFile.DeleteAllItems();
	// 申请缓冲区
	char* pBuff = (char*)wParam;
	int* pnLen = (int*)lParam;

	// 添加文件磁盘到列表控件
	char* pTemBuff = pBuff;
	int nBuffSize = *pnLen;
	while (nBuffSize > 0)
	{
		m_ListFile.InsertItem(m_ListFile.GetItemCount(), pTemBuff);
		pTemBuff += strlen(pTemBuff) + 1;
		nBuffSize = strlen(pTemBuff);
	}

	// 清空缓冲区
	delete[] pBuff;
	delete pnLen;

	return 0;
}

// 获取当前文件夹的上一层
LRESULT CFileDlg::OnGetPrevFolder(WPARAM wParam, LPARAM lParam)
{
	char* pBuff = (char*)wParam;
	m_ListFile.InsertItem(m_ListFile.GetItemCount(), pBuff);
	delete[] pBuff;
	return 0;
}

// 获取当前文件夹的下一层
LRESULT CFileDlg::OnGetNextFolder(WPARAM wParam, LPARAM lParam)
{
	char* pBuff = (char*)wParam;
	m_ListFile.InsertItem(m_ListFile.GetItemCount(), pBuff);
	delete[] pBuff;
	return 0;
}

// 有上一层文件
LRESULT CFileDlg::OnHavePrevFile(WPARAM wParam, LPARAM lParam)
{
	// 将当前路径从栈中弹出
	m_stack.pop();

	// 将栈顶的路径更新到路径控件上
	m_EditFilePath.SetWindowText(m_stack.top());

	// 更新列表控件中的数据项
	m_ListFile.DeleteAllItems();
	m_ListFile.InsertItem(m_ListFile.GetItemCount(), "...");

	return 0;
}

// 有下一层文件
LRESULT CFileDlg::OnHaveNextFile(WPARAM wParam, LPARAM lParam)
{
	// 将当前路径加入到栈中
	m_stack.push(m_csPrevFilePath);
	// 更新路径控件上的内容
	m_EditFilePath.SetWindowText(m_csPrevFilePath);

	// 更新列表控件中的数据项
	m_ListFile.DeleteAllItems();
	m_ListFile.InsertItem(m_ListFile.GetItemCount(), "...");

	return 0;
}

// 选中的是文件夹
LRESULT CFileDlg::OnIsFolder(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox("[Server]: 你选中的是文件夹，无法下载，请更换到文件再进行下载");
	return 0;
}

// 选中的是文件
LRESULT CFileDlg::OnIsFile(WPARAM wParam, LPARAM lParam)
{
	CFileDialog FileDlg(FALSE, 
		"File(*.*)|*.*",
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"File(*.*)|*.*||",
		this);

	// 弹出保存文件的对话框
	FileDlg.DoModal();

	// 保存下载文件所到的路径
	m_csSaveDownFilePath = FileDlg.GetPathName();

	// 开始下载文件
	PACKETHEAD ph;
	ph.m_nCmd = PK_START_DOWN_FILE;
	ph.m_nLen = m_csDownFilePath.GetLength() + 1;

	// 发送数据长度
	if (!m_pMessages->SendData((char*)&ph, sizeof(ph)))
	{
		AfxMessageBox("[Server]: 发送下载文件字节大小失败");
		return 0;
	}

	// 向客户端发送所下载文件的绝对路径
	if (!m_pMessages->SendData(m_csDownFilePath.GetBuffer(), ph.m_nLen))
	{
		AfxMessageBox("[Server]: 发送下载文件数据失败");
		return 0;
	}
	// 开启定时器
	SetTimer(1, 500, NULL);

	return 0;
}

// 获取文件大小
LRESULT CFileDlg::OnGetFileSize(WPARAM wParam, LPARAM lParam)
{
	m_LIDownFileSize = *(PLARGE_INTEGER)wParam;
	delete (PLARGE_INTEGER)wParam;

	//创建目标文件
	HANDLE hDstFile = CreateFile(m_csSaveDownFilePath,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	//关闭文件句柄
	CloseHandle(hDstFile);

	return 0;
}

// 获取客户端发送的文件数据 -- 写入文件
LRESULT CFileDlg::OnGetFileData(WPARAM wParam, LPARAM lParam)
{
	char* pBuff = (char*)wParam;
	int* pnLen = (int*)lParam;

	HANDLE hDstFile = CreateFile(m_csSaveDownFilePath,
		GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	// 移动文件指针到文件末尾
	SetFilePointer(hDstFile, 0, 0, FILE_END);

	// 将获取到的数据写入到文件
	DWORD dwWriteSize = 0;
	WriteFile(hDstFile, pBuff, *pnLen, &dwWriteSize, NULL);


	// 下载的文件字节数增加
	m_LICurlDownFileSize.QuadPart += *pnLen;
	// 当前下载的文件字节数等于总下载文件字节数是停止获取文件数据
	if (m_LIDownFileSize.QuadPart == m_LICurlDownFileSize.QuadPart)
	{
		CloseHandle(hDstFile);
		// 清理缓冲区
		delete[] pBuff;
		delete pnLen;
		pBuff = NULL;
		pnLen = NULL;
		CString csFmt;
		csFmt.Format("写入总字节数为：%d", m_LIDownFileSize.QuadPart);
		AfxMessageBox(csFmt);
		return 0;
	}
	CloseHandle(hDstFile);

	// 清理缓冲区
	delete[] pBuff;
	delete pnLen;
	pBuff = NULL;
	pnLen = NULL;

	// 向客户端发送获取下一条数据
	PACKETHEAD ph;
	ph.m_nCmd = PK_GET_FILE_NEXT_DATA;
	ph.m_nLen = 0;

	if (!m_pMessages->SendData((char*)&ph, sizeof(ph)))
	{
		AfxMessageBox("[Server]: 发送数据失败");
		return 0;
	}

	return 0;
}

// 下载文件数据成功
LRESULT CFileDlg::OnDownFileDataSucc(WPARAM wParam, LPARAM lParam)
{
	// 销毁计时器
	KillTimer(1);
	m_ProgressDownUp.SetPos(100);
	AfxMessageBox("[Server]: 文件下载成功，请到对应的位置查看");

	CString csFmt;
	csFmt.Format("写入总字节数为：%d", m_LIDownFileSize.QuadPart);
	AfxMessageBox(csFmt);

	// 文件下载成功，更新控件数值
	m_ProgressDownUp.SetPos(0);
	// 清空存储文件数据的变量
	ZeroMemory(&m_LIDownFileSize, sizeof(m_LIDownFileSize));
	ZeroMemory(&m_LICurlDownFileSize, sizeof(m_LICurlDownFileSize));
	return 0;
}

// 文件数据上传成功
LRESULT CFileDlg::OnUploadFileDataSucc(WPARAM wParam, LPARAM lParam)
{
	// 销毁计时器
	KillTimer(1);
	m_ProgressDownUp.SetPos(100);
	AfxMessageBox("[Server]: 文件上传成功，请到对应的位置查看");
	// 文件下载成功，更新控件数值
	m_ProgressDownUp.SetPos(0);
	// 清空存储文件数据的变量
	ZeroMemory(&m_LIDownFileSize, sizeof(m_LIDownFileSize));
	ZeroMemory(&m_LICurlDownFileSize, sizeof(m_LICurlDownFileSize));
	return 0;
}

// 获取文件下一条数据
LRESULT CFileDlg::OnGetFileNextData(WPARAM wParam, LPARAM lParam)
{
	PLARGE_INTEGER plSrcFileSize = (PLARGE_INTEGER)wParam;

	// 打开源文件
	HANDLE hSRcFile = CreateFile(m_csUploadFilePath,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	// 源文件打开失败
	if (INVALID_HANDLE_VALUE == hSRcFile)
	{
		AfxMessageBox("[Server]: 打开上传文件路径失败");
		CloseHandle(hSRcFile);
		return 0;
	}

	//读取文件大小
	LARGE_INTEGER lSrcFileSize = { 0 };
	BOOL bRet = GetFileSizeEx(hSRcFile, &lSrcFileSize);

	if (!bRet)
	{
		AfxMessageBox("[Server]: 获取上传文件大小失败");
		//关闭文件
		CloseHandle(hSRcFile);
		return 0;
	}

	//保存文件总大小
	m_LIDownFileSize = lSrcFileSize;

	//设置文件指针
	SetFilePointer(hSRcFile, plSrcFileSize->LowPart, &plSrcFileSize->HighPart, FILE_CURRENT);

	const int nCopySize = 0x10000;
	char szBuff[nCopySize] = { 0 };

	//读取数据
	DWORD dwBytesToRead = 0;
	bRet = ReadFile(hSRcFile, szBuff, nCopySize, &dwBytesToRead, NULL);

	if (!bRet)
	{
		AfxMessageBox("[Server]: 读取文件数据失败");
		CloseHandle(hSRcFile);
		return 0;
	}

	if (dwBytesToRead == 0)
	{
		// 文件上传成功
		PostMessage(WM_FILE_UPLOAD_SUCC, NULL, NULL);
		return 0;
	}

	// 文件上传没有成功，继续上传
	PACKETHEAD ph;
	ph.m_nCmd = PK_GET_FILE_DATA;
	ph.m_nLen = dwBytesToRead;

	// 发送数据长度
	if (!m_pMessages->SendData((char*)&ph, sizeof(ph)))
	{
		AfxMessageBox("[Server]: 文件数据长度发送失败");
		return 0;
	}

	// 发送数据
	if (!m_pMessages->SendData(szBuff, dwBytesToRead))
	{
		AfxMessageBox("[Server]: 文件数据发送失败");
		return 0;
	}

	// 当前所下载文件字节大小增加
	m_LICurlDownFileSize.QuadPart += dwBytesToRead;

	CloseHandle(hSRcFile);

	return 0;
}

// 接收数据失败
LRESULT CFileDlg::OnRecvFileDataError(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox("[Server]: 接收数据失败");
	return 0;
}

// 发送数据失败
LRESULT CFileDlg::OnSendFileDataError(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox("[Server]: 发送数据失败");
	return 0;
}


void CFileDlg::SetMessages(CMessages* pMessages)
{
	m_pMessages = pMessages;
}

// 左键双击列表控件
void CFileDlg::OnDblclkListFile(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	//获取左键双击的项
	POSITION pos = m_ListFile.GetFirstSelectedItemPosition();
	m_nIdx = m_ListFile.GetNextSelectedItem(pos);

	if (m_nIdx == -1)
	{
		return;
	}

	// 解析双击的项
	DoubleListCtrl();

}

// 右键列表控件弹出功能菜单
void CFileDlg::OnRclickListFile(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	//获取右键单击的项
	POSITION pos = m_ListFile.GetFirstSelectedItemPosition();
	m_nIdx = m_ListFile.GetNextSelectedItem(pos);

	if (m_nIdx == -1)
	{
		return;
	}

	// 栈空退出
	if (m_stack.empty())
	{
		return;
	}

	// 设置鼠标的坐标是相对于ListCtrl的
	m_ListFile.ClientToScreen(&pNMItemActivate->ptAction);

	CMenu menu;

	// 从资源加载菜单
	menu.LoadMenu(IDR_MN_DOWN_UP_FILE);

	// 获取子菜单
	CMenu* pSubMenu = menu.GetSubMenu(0);

	// 弹出子菜单
	pSubMenu->TrackPopupMenu(
		TPM_LEFTALIGN,
		pNMItemActivate->ptAction.x,
		pNMItemActivate->ptAction.y,
		this);
}

void CFileDlg::DoubleListCtrl()
{
	// 获取双击的文件名
	CString strPath = m_ListFile.GetItemText(m_nIdx, 0);

	// 如果栈中只有一个数据，并且双击的使 ..
	if (m_stack.size() == 1 && strPath == "...")
	{
		// 发送获取到的目标设备磁盘驱动的头信息
		PACKETHEAD ph;
		ph.m_nCmd = PK_GET_DES_DRIVE;
		ph.m_nLen = 0;

		if (!m_pMessages->SendData((char*)&ph, sizeof(ph)))
		{
			AfxMessageBox("发送数据失败");
			return;
		}

		// 栈顶弹出数据
		m_stack.pop();
		m_EditFilePath.SetWindowText("");

		return;
	}

	// 如果栈是空的
	if (m_stack.empty())
	{
		// 发送获取驱动器的头信息
		PACKETHEAD ph;
		ph.m_nCmd = PK_GET_FOLDER_NEXT_DATA;	// 获取当前文件夹的下一层数据
		ph.m_nLen = strPath.GetLength() + 1;

		// 发送数据长度
		if (!m_pMessages->SendData((char*)&ph, sizeof(ph)))
		{
			AfxMessageBox("发送数据长度失败");
			return;
		}

		// 发送数据
		if (!m_pMessages->SendData(strPath.GetBuffer(), ph.m_nLen))
		{
			AfxMessageBox("发送数据失败");
			return;
		}

		int nFind = strPath.Find("\\");

		m_csPrevFilePath = strPath.Left(nFind);
	}
	else
	{
		if (strPath == "...")
		{
			CString strPrvePath = m_stack.top();

			// 发送获上一层文件夹的头信息
			PACKETHEAD ph;
			ph.m_nCmd = PK_GET_FOLDER_PREV_DATA;	// 获取当前文件夹的上一层数据
			ph.m_nLen = strPrvePath.GetLength() + 1;

			if (!m_pMessages->SendData((char*)&ph, sizeof(ph)))
			{
				AfxMessageBox("发送数据长度失败");
				return;
			}

			// 发送数据
			if (!m_pMessages->SendData(strPrvePath.GetBuffer(), ph.m_nLen))
			{
				AfxMessageBox("发送数据失败");
				return;
			}
		}
		else
		{
			m_csPrevFilePath = m_stack.top();
			m_csPrevFilePath = m_csPrevFilePath + "\\" + strPath;

			// 发送获取下一层文件夹的头信息
			PACKETHEAD ph;
			ph.m_nCmd = PK_GET_FOLDER_NEXT_DATA;
			ph.m_nLen = m_csPrevFilePath.GetLength() + 1;

			if (!m_pMessages->SendData((char*)&ph, sizeof(ph)))
			{
				AfxMessageBox("发送数据长度失败");
				return;
			}

			// 发送数据
			if (!m_pMessages->SendData(m_csPrevFilePath.GetBuffer(), ph.m_nLen))
			{
				AfxMessageBox("发送数据失败");
				return;
			}
		}
	}
}

// 下载文件
void CFileDlg::OnDownIsFile()
{
	CString strPath = m_ListFile.GetItemText(m_nIdx, 0);

	if (strPath == "...")
	{
		AfxMessageBox("所选项不是文件, 无法下载");
		return;
	}

	m_csDownFilePath = m_stack.top();
	m_csDownFilePath = m_csDownFilePath + "\\" + strPath;

	// 向客户端发送下载申请，客户端返回知否可以下载
	PACKETHEAD ph;
	ph.m_nCmd = PK_DOWN_IS_FILE;	 // 下载的是否是文件
	ph.m_nLen = m_csDownFilePath.GetLength() + 1;

	// 发送数据长度
	if (!m_pMessages->SendData((char*)&ph, sizeof(ph)))
	{
		AfxMessageBox("发送数据长度失败");
		return;
	}

	// 发送数据信息
	if (!m_pMessages->SendData(m_csDownFilePath.GetBuffer(), ph.m_nLen))
	{
		AfxMessageBox("发送数据失败");
		return;
	}
}

// 上传文件
void CFileDlg::OnUploadFile()
{
	// 弹出选择文件对话框
	CFileDialog fileDlg(TRUE);
	fileDlg.DoModal();

	// 将选择的文件显示在文本编辑框中
	m_csUploadFilePath = fileDlg.GetPathName();

	// 获取文件名
	char ch = '\\';
	char* pBuff = strrchr(m_csUploadFilePath.GetBuffer(), ch) + 1;

	CString csUploadFilePath;
	GetDlgItemText(IDC_EDIT_FILE_PATH, csUploadFilePath);
	csUploadFilePath = csUploadFilePath + "\\" + pBuff;

	PACKETHEAD ph;
	ph.m_nCmd = PK_UP_FILE_DATA;
	ph.m_nLen = csUploadFilePath.GetLength() + 1;

	// 发送头
	if (!m_pMessages->SendData((char*)&ph, sizeof(ph)))
	{
		AfxMessageBox("发送数据长度失败");
		return;
	}

	// 发送要上传的文件名
	if (!m_pMessages->SendData(csUploadFilePath.GetBuffer(), ph.m_nLen))
	{
		AfxMessageBox("发送数据失败");
		return;
	}

	// 开启定时器
	SetTimer(1, 500, NULL);
}

// 定时器
void CFileDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int nRate = (int)((double)m_LICurlDownFileSize.QuadPart / (double)m_LIDownFileSize.QuadPart * 100);
	m_ProgressDownUp.SetPos(nRate);
	CDialogEx::OnTimer(nIDEvent);
}
