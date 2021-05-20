#include "pch.h"
#include "CSocket.h"



CTCPSocket::CTCPSocket()
{
	m_Socket = INVALID_SOCKET;
}

CTCPSocket::~CTCPSocket()
{
	CloseSocket();	// 关闭socket
	Cleanup();	// 关闭套接字库
}

// 初始化套接字
BOOL CTCPSocket::CreateSocket()
{
	// 初始化套接字(说明使用的协议)
	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_Socket)
	{
		ShowErrorInfo("[Server]: socket ");
		return FALSE;
	}
	return TRUE;
}

// 绑定端口 IP地址
BOOL CTCPSocket::BindSocket(char* pszIPAddr, USHORT usProt)
{
	// socket 初始化成功
	sockaddr_in addr;	// sockaddr_in 结构体将端口，IP地址，保留数据分开
	addr.sin_family = AF_INET;	// 使用IPV4协议
	addr.sin_port = ::htons(usProt);	// 向操作系统申请一个端口
	addr.sin_addr.S_un.S_addr = ::inet_addr(pszIPAddr);

	// 将本地地址和套接字进行关联
	if (::bind(m_Socket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		ShowErrorInfo("[Server]: bind Error ");
		return FALSE;
	}
	return TRUE;
}

// 将套接字置于侦听传入连接的状态
BOOL CTCPSocket::ListenSocket()
{
	if (::listen(m_Socket, SOMAXCONN) == SOCKET_ERROR)
	{
		ShowErrorInfo("[Server]: listen Error: ");
		return FALSE;
	}
	return TRUE;
}

// 等待客户端连接
SOCKET CTCPSocket::AcceptSocket(SOCKADDR_IN& addr)
{
	int nSize = sizeof(addr);
	// 接受客户端连接

	return ::accept(m_Socket, (sockaddr*)&addr, &nSize); //阻塞
}

// 建立到指定套接字的连接
BOOL CTCPSocket::ConnectSocket(char* pszIPAddr, USHORT usProt)
{
	// socket 初始化成功
	sockaddr_in addr;	// sockaddr_in 结构体将端口，IP地址，保留数据分开
	addr.sin_family = AF_INET;	// 使用IPV4协议
	addr.sin_port = ::htons(usProt);	// 向操作系统申请一个端口
	addr.sin_addr.S_un.S_addr = inet_addr(pszIPAddr);

	// 将本地地址和套接字进行关联
	if (::connect(m_Socket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		ShowErrorInfo("[Server]: connect Error ");
		return FALSE;
	}
	return TRUE;
}

// 接收数据
int CTCPSocket::Recv(SOCKET socket, char* pszBuff, int nLen)
{
	return ::recv(socket, pszBuff, nLen, 0);
}

// 发送数据
BOOL CTCPSocket::Send(SOCKET socket, const char* pszBuff, int nLen)
{
	if (::send(socket, pszBuff, nLen, 0) == SOCKET_ERROR)
	{
		return FALSE;
	}
	return TRUE;
}

// 关闭socket
void CTCPSocket::CloseSocket()
{
	if (m_Socket != INVALID_SOCKET)
	{
		::closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
	}
}

// 初始化套接字库
BOOL CTCPSocket::Startup()
{
	// 初始化套接字库，说明要使用伯克利套接字的版本
	WSADATA wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ShowErrorInfo("[Server]: WSAStartup");
		return FALSE;
	}
	// 检查版本
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		Cleanup();	// 关闭套接字库
		return FALSE;
	}
	return TRUE;
}

// 关闭套接字库
BOOL CTCPSocket::Cleanup()
{
	if (::WSACleanup() != 0)
	{
		return FALSE;
	}
	return TRUE;
}

// 显示错误信息
void CTCPSocket::ShowErrorInfo(const char* prefix)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
	);
	char szBuff[MAXBYTE] = { 0 };
	sprintf(szBuff, "codes: %p, %s: , %s: ", (void*)WSAGetLastError(),prefix, (char*)lpMsgBuf);
	CString cs;
	cs = szBuff;
	AfxMessageBox(cs);

	LocalFree(lpMsgBuf);
}