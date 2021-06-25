#include "CSocket.h"

CTCPSocket::CTCPSocket()
{
	m_socket = INVALID_SOCKET;
}

CTCPSocket::~CTCPSocket()
{
	// �ر�socket
	CloseSocket();
	// �ر��׽��ֿ�
	Cleanup();
}

// ��ʼ���׽���
BOOL CTCPSocket::CreateSocket()
{
	// ��ʼ���׽���(˵��ʹ�õ�Э��)
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_socket)
	{
		ShowErrorInfo("[Error]: socket ");
		return FALSE;
	}
	return TRUE;
}

// �󶨶˿� IP��ַ
BOOL CTCPSocket::BindSocket(char* pszIPAddr, USHORT usProt)
{
	// socket ��ʼ���ɹ�
	sockaddr_in addr; // sockaddr_in �ṹ�彫�˿ڣ�IP��ַ���������ݷֿ�
	addr.sin_family = AF_INET; // ʹ��IPV4Э��
	addr.sin_port = ::htons(usProt); // �����ϵͳ����һ���˿�
	addr.sin_addr.S_un.S_addr = ::inet_addr(pszIPAddr);

	// �����ص�ַ���׽��ֽ��й���
	if (::bind(m_socket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		ShowErrorInfo("[Error]: bind Error ");
		return FALSE;
	}
	return TRUE;
}

// ���׽������������������ӵ�״̬
BOOL CTCPSocket::ListenSocket()
{
	if (::listen(m_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		ShowErrorInfo("[Error]: listen Error: ");
		return FALSE;
	}
	return TRUE;
}

// �ȴ��ͻ�������
SOCKET CTCPSocket::AcceptSocket(SOCKADDR_IN& addr)
{
	int nSize = sizeof(addr);
	// ���ܿͻ�������

	return ::accept(m_socket, (sockaddr*)&addr, &nSize); // ����
}

// ������ָ���׽��ֵ�����
BOOL CTCPSocket::ConnectSocket(char* pszIPAddr, USHORT usProt)
{
	// socket ��ʼ���ɹ�
	sockaddr_in addr; // sockaddr_in �ṹ�彫�˿ڣ�IP��ַ���������ݷֿ�
	addr.sin_family = AF_INET; // ʹ��IPV4Э��
	addr.sin_port = ::htons(usProt); // �����ϵͳ����һ���˿�
	addr.sin_addr.S_un.S_addr = inet_addr(pszIPAddr);

	// �����ص�ַ���׽��ֽ��й���
	if (::connect(m_socket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		ShowErrorInfo("[Error]: connect Error ");
		return FALSE;
	}
	return TRUE;
}

// ��������
int CTCPSocket::Recv(char* pszBuff, int nLen)
{
	return ::recv(m_socket, pszBuff, nLen, 0);
}

// ��������
BOOL CTCPSocket::Send(const char* pszBuff, int nLen)
{
	if (::send(m_socket, pszBuff, nLen, 0) == SOCKET_ERROR)
	{
		return FALSE;
	}
	return TRUE;
}

// �ر�socket
void CTCPSocket::CloseSocket()
{
	if (m_socket != INVALID_SOCKET)
	{
		::closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
}

// ��ʼ���׽��ֿ�
BOOL CTCPSocket::Startup()
{
	// ��ʼ���׽��ֿ⣬˵��Ҫʹ�ò������׽��ֵİ汾
	WSADATA wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ShowErrorInfo("[Error]: WSAStartup");
		return FALSE;
	}
	// ���汾
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		Cleanup(); // �ر��׽��ֿ�
		return FALSE;
	}
	return TRUE;
}

// �ر��׽��ֿ�
BOOL CTCPSocket::Cleanup()
{
	if (::WSACleanup() != 0)
	{
		return FALSE;
	}
	return TRUE;
}


// ��ʾ������Ϣ
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
	sprintf(szBuff, "codes: %p, %s: , %s: ", 
		(void*)WSAGetLastError(), 
		prefix, 
		(char*)lpMsgBuf);

	puts(szBuff);
	LocalFree(lpMsgBuf);
}