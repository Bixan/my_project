#include "pch.h"
#include "CMessages.h"
#include "MyMessages.h"
#include "CFileDlg.h"

int g_nReadFileSize = 0;
CMessages::CMessages()
{
	m_hWnd = NULL;	// 窗口句柄
	m_ClientSocket = INVALID_SOCKET;	// 客户端socket
	m_HeartBeatTime = 0;	// 心跳包，保存时间
}

CMessages::~CMessages()
{
	CloaseResource();
}

// 初始化套接字库
BOOL CMessages::InitWSAStartup()
{
	return m_Socket.Startup();
}

// 创建，绑定，监听socket
BOOL CMessages::StartSocket(char* pszIPAddr, USHORT usProt)
{
	// 创建 socket
	if (!m_Socket.CreateSocket())
	{
		// 使用自定义消息，通知主窗口
		// 创建socket失败
		::PostMessage(m_hWnd, WM_CREATE_SOCKETE_RROR, NULL, NULL);
		return FALSE;
	}

	// 绑定 socket
	if (!m_Socket.BindSocket(pszIPAddr, usProt))
	{
		// 绑定socket失败
		::PostMessage(m_hWnd, WM_BIND_SOCKET_ERROR, NULL, NULL);
		return FALSE;
	}

	// 监听 socket
	if (!m_Socket.ListenSocket())
	{
		// 监听socket失败
		::PostMessage(m_hWnd, WM_LISTEN_SOCKETE_RROR, NULL, NULL);
		// 关闭套接字以及socket
		m_Socket.CloseSocket();
		m_Socket.Cleanup();
		return FALSE;
	}

	// 创建线程，等待客户端连接
	HANDLE hThread = CreateThread(NULL, 0, WorkThread, this, 0, NULL);

	CloseHandle(hThread);
	return TRUE;
}

// 接收数据
int CMessages::RecvData(char* pBuff, int nLen)
{
	return m_Socket.Recv(m_ClientSocket, pBuff, nLen);
}

// 发送数据
BOOL CMessages::SendData(const char* pBuff, int nLen)
{
	return m_Socket.Send(m_ClientSocket, pBuff, nLen);
}

// 关闭套接字 socket
void CMessages::CloaseResource()
{
	m_Socket.CloseSocket();
	m_Socket.Cleanup();
}

// 设置窗口句柄
void CMessages::SetHwnd(HWND hWnd)
{
	m_hWnd = hWnd;
}

DWORD __stdcall CMessages::WorkThread(LPVOID lpParameter)
{
	CMessages* pMess = (CMessages*)lpParameter;

	PSOCKADDR_IN PSI = new SOCKADDR_IN();
	// 初始化
	ZeroMemory(PSI, sizeof(SOCKADDR_IN));

	pMess->m_ClientSocket = pMess->m_Socket.AcceptSocket(*PSI);
	if (INVALID_SOCKET != pMess->m_ClientSocket)
	{
		// 客户端连接成功
		::PostMessage(pMess->m_hWnd,
			WM_CLIENT_ACCEPT_SUCCEED,
			(WPARAM)pMess->m_ClientSocket,
			(LPARAM)PSI);
	}
	else
	{
		// 客户端连接失败
		::PostMessage(pMess->m_hWnd,
			WM_CLIENT_ACCEPT_FAIL,
			NULL,
			NULL);
	}

	// 客户端连接成功，创建一个线程用来接收数据，发送数据
	HANDLE hRecvSendThread = CreateThread(NULL,
		0,
		RecvSendThread,
		pMess,
		0,
		NULL);

	// 线程结束，关闭句柄
	CloseHandle(hRecvSendThread);

	// 创建一个线程，定时客户端发送消息
	// 检查客户端是否在线 -- 心跳包
	HANDLE hHeartBeatThread = CreateThread(NULL,
		0,
		HeartBeatThread,
		pMess,
		0,
		NULL);
	pMess->m_HeartBeatTime = clock();
	CloseHandle(hHeartBeatThread);
	return 0;
}

// 该线程用来接收数据，发送数据
DWORD __stdcall CMessages::RecvSendThread(LPVOID lpParameter)
{
	CMessages* pMess = (CMessages*)lpParameter;
	while (true)
	{
		PACKETHEAD ph;

		// 接收数据
		pMess->m_Socket.Recv(pMess->m_ClientSocket, (char*)&ph, sizeof(ph));

		// 解析接收到的数据
		pMess->ParserCommand(ph);
	}

	return 0;
}

// 检测客户端在线情况
DWORD __stdcall CMessages::HeartBeatThread(LPVOID lpParameter)
{
	CMessages* pMess = (CMessages*)lpParameter;

	while (true)
	{
		Sleep(1000); //每隔一秒检查一次

		clock_t tCurrent = clock();	//获取当前时间

		if ((tCurrent - pMess->m_HeartBeatTime) / CLOCKS_PER_SEC > 5)
		{
			AfxMessageBox("[Server]: 客户端已下线");
			::PostMessage(pMess->m_hWnd, WM_CLIENT_QUIT, NULL, NULL);
			return 0;
		}
	}
	return 0;
}

// 解析命令
void CMessages::ParserCommand(PACKETHEAD ph)
{
	switch (ph.m_nCmd)
	{
	case PK_HEART_BEAT:
	{
		// 发送心跳包
		m_HeartBeatTime = clock();
		break;
	}
	case PK_OUTPUT_CMD_DATA:
	{
		// cmd 命令获取的数据
		ParserGetCmdData(ph);
		break;
	}
	case PK_GET_DES_DRIVE:
	{
		// 获取客户端磁盘
		ParserGetClientDrive(ph);
		break;
	}
	case PK_GET_FOLDER_NEXT_DATA:
	{
		// 获取当前文件夹下一层数据
		ParserGetNextFolder(ph);
		break;
	}
	case PK_GET_FOLDER_PREV_DATA:
	{
		// 获取当前文件夹上一层数据
		ParserGetPrevFolder(ph);
		break;
	}
	case PK_HAVE_NEXT_FOLDER:
	{
		// 当前文件有下一层
		ParserHaveNextFolder();
		break;
	}
	case PK_NO_NEXT_FOLDER:
	{
		// 当前文件没有下一层
		break;
	}
	case PK_HAVE_PREV_FOLDER:
	{
		// 当前文件有上一层
		ParserHavePrevFolder();
		break;
	}
	case PK_NO_PREV_FOLDER:
	{
		// 当前文件没有上一层
		break;
	}
	case PK_IS_FOLDER:
	{
		// 选中的是文件夹
		ParserIsFloder();
		break;
	}
	case PK_IS_FILE:
	{
		// 选中的是文件
		ParserIsFile();
		break;
	}
	case PK_GET_FILE_SIZE:
	{
		// 文件大小
		ParserGetFileSize();
		break;
	}
	case PK_GET_FILE_DATA:
	{
		// 获取文件数据
		ParserGetFileData(ph);
		break;
	}
	case PK_GET_FILE_NEXT_DATA:
	{
		// 获取文件下一份数据
		ParserGetFileNextData();
		break;
	}
	case PK_DOWN_FILE_SUCC:
	{
		// 文件下载成功
		ParserFileDownSrcc();
		break;
	}
	case PK_CLIENT_SCREEN_DATA:
	{
		// 获取屏幕数据
		ParserGetScreenData();
		break;
	}
	}
}

// 通过cmd命令输入的对应数据信息
void CMessages::ParserGetCmdData(PACKETHEAD ph)
{
	// 根据数据长度new对应空间
	char* pBuff = new char[ph.m_nLen];
	// 初始化
	ZeroMemory(pBuff, ph.m_nLen);

	if (!m_Socket.Recv(m_ClientSocket, pBuff, ph.m_nLen))
	{
		// 接收数据失败
		::PostMessage(m_hWnd, WM_RECV_DATA_ERROR, NULL, NULL);
		delete[] pBuff;
		return;
	}
	// 接收数据成功
	::PostMessage(m_hWnd, WM_GET_CMD_DATA, (WPARAM)pBuff, NULL);
}

// 屏幕
void CMessages::ParserGetScreenData()
{
	PSCREENINFO pScreenInfo = new SCREENINFO();

	// 接收屏幕相关信息
	int nRet = m_Socket.Recv(m_ClientSocket, (char*)pScreenInfo, sizeof(SCREENINFO));

	if (nRet <= 0)
	{
		// 接收数据失败
		::PostMessage(m_hWnd, WM_RECV_DATA_ERROR, NULL, NULL);
		return;
	}

	char* pScreenData = new char[pScreenInfo->m_CompressedSize];
	ZeroMemory(pScreenData, pScreenInfo->m_CompressedSize);

	// 保存屏幕数据
	int nTotal = pScreenInfo->m_CompressedSize;
	int nCurSize = 0;

	while (nCurSize != nTotal)
	{
		int nRetByte = m_Socket.Recv(m_ClientSocket, pScreenData + nCurSize, nTotal - nCurSize);

		if (nRetByte <= 0)
		{
			::PostMessage(m_hWnd, WM_RECV_DATA_ERROR, NULL, NULL);
			return;
		}
		nCurSize += nRetByte;
	}

	// 获取屏幕数据成功
	::PostMessage(m_hWnd,
		WM_CLIENT_SCREEN_DATA,
		(WPARAM)pScreenInfo,
		(LPARAM)pScreenData);
}

// 发送屏幕数据
void CMessages::SendScreenData()
{
	PACKETHEAD sendHead;
	sendHead.m_nCmd = PK_CLIENT_SCREEN_DATA;
	sendHead.m_nLen = 0;

	SendData((char*)&sendHead, sizeof(sendHead));
}

// 获取客户端磁盘
void CMessages::ParserGetClientDrive(PACKETHEAD ph)
{
	// 根据数据长度new对应空间
	char* pBuff = new char[ph.m_nLen];
	// 初始化
	ZeroMemory(pBuff, ph.m_nLen);

	int* pnLen = new int(ph.m_nLen);

	// 接收头数据
	int nRet = m_Socket.Recv(m_ClientSocket, pBuff, ph.m_nLen);

	if (nRet <= 0)
	{
		// 接收数据失败
		::PostMessage(m_hWnd, WM_RECV_DATA_ERROR, NULL, NULL);
		// 释放空间
		delete[] pBuff;
		delete pnLen;
		return;
	}
	// 接收数据成功
	::PostMessage(m_hWnd, WM_DRIVE_INFO, (WPARAM)pBuff, (LPARAM)pnLen);
}

// 下一层文件
void CMessages::ParserGetNextFolder(PACKETHEAD ph)
{
	// 根据数据长度new对应空间
	char* pBuff = new char[ph.m_nLen];
	// 初始化
	ZeroMemory(pBuff, ph.m_nLen);

	// 接收头数据
	int nRet = m_Socket.Recv(m_ClientSocket, pBuff, ph.m_nLen);

	if (nRet <= 0)
	{
		// 接收数据失败
		::PostMessage(m_hWnd, WM_RECV_DATA_ERROR, NULL, NULL);
		// 释放空间
		delete[] pBuff;
		return;
	}
	// 接收数据成功
	::PostMessage(m_hWnd, WM_GET_FOLDER_NEXT_DATA, (WPARAM)pBuff, NULL);
}

// 上一层文件
void CMessages::ParserGetPrevFolder(PACKETHEAD ph)
{
	// 根据数据长度new对应空间
	char* pBuff = new char[ph.m_nLen];
	// 初始化
	ZeroMemory(pBuff, ph.m_nLen);

	// 接收头数据
	int nRet = m_Socket.Recv(m_ClientSocket, pBuff, ph.m_nLen);

	if (nRet <= 0)
	{
		// 接收数据失败
		::PostMessage(m_hWnd, WM_RECV_DATA_ERROR, NULL, NULL);
		// 释放空间
		delete[] pBuff;
		return;
	}
	// 接收数据成功
	::PostMessage(m_hWnd, WM_GET_FOLDER_PREV_DATA, (WPARAM)pBuff, NULL);
}

// 存在下一层文件
void CMessages::ParserHaveNextFolder()
{
	::PostMessage(m_hWnd, WM_HAVE_NEXT_FILE, NULL, NULL);
}

// 存在上一层文件
void CMessages::ParserHavePrevFolder()
{
	::PostMessage(m_hWnd, WM_HAVE_PREV_FILE, NULL, NULL);
}

// 当前点击的是文件夹
void CMessages::ParserIsFloder()
{
	::PostMessage(m_hWnd, WM_IS_FOLDER, NULL, NULL);
}

// 当前点击的是文件
void CMessages::ParserIsFile()
{
	::PostMessage(m_hWnd, WM_IS_FILE, NULL, NULL);
}

// 获取文件大小
void CMessages::ParserGetFileSize()
{
	PLARGE_INTEGER pSize = new LARGE_INTEGER();
	ZeroMemory(pSize, sizeof(LARGE_INTEGER));

	// 接收数据
	int nRet = m_Socket.Recv(m_ClientSocket, (char*)pSize, sizeof(PLARGE_INTEGER));

	if (nRet <= 0)
	{
		::PostMessage(m_hWnd, WM_RECV_DATA_ERROR, NULL, NULL);
		delete pSize;
		return;
	}
	// 获取文件大小成功
	::PostMessage(m_hWnd, WM_GET_FILE_SIZE, (WPARAM)pSize, NULL);
}

// 获取文件数据
void CMessages::ParserGetFileData(PACKETHEAD ph)
{
	int* pLen = new int(0);
	*pLen = ph.m_nLen;

	char* pBuff = new char[ph.m_nLen];
	ZeroMemory(pBuff, ph.m_nLen);

	// 获取文件字节数
	int nTotal = ph.m_nLen;
	int nCurSize = 0;

	while (nCurSize != nTotal)
	{
		int nRetByte = m_Socket.Recv(m_ClientSocket, pBuff + nCurSize, nTotal - nCurSize);

		if (nRetByte <= 0)
		{
			::PostMessage(m_hWnd, WM_RECV_DATA_ERROR, NULL, NULL);
			delete[] pBuff;
			return;
		}
		g_nReadFileSize += nTotal;
		nCurSize += nRetByte;
	}
	// 获取文件数据成功
	::PostMessage(m_hWnd, WM_GET_FILE_DATA, (WPARAM)pBuff, (LPARAM)pLen);
}

// 文件下载成功
void CMessages::ParserFileDownSrcc()
{
	::PostMessage(m_hWnd, WM_DOWN_FILE_DATA_SUCC, NULL, NULL);
}

// 文件下一条数据
void CMessages::ParserGetFileNextData()
{
	PLARGE_INTEGER plSrcFileSize = new LARGE_INTEGER();
	ZeroMemory(plSrcFileSize, sizeof(LARGE_INTEGER));

	//接收数据
	int nRet = m_Socket.Recv(m_ClientSocket, (char*)plSrcFileSize, sizeof(LARGE_INTEGER));

	if (nRet <= 0)
	{
		// 接收数据失败
		::PostMessage(m_hWnd, WM_RECV_DATA_ERROR, NULL, NULL);
		delete plSrcFileSize;
		return;
	}
	// 获取文件下一条数据成功
	::PostMessage(m_hWnd, WM_GET_FILE_NEXT_DATA, (WPARAM)plSrcFileSize, NULL);
}
