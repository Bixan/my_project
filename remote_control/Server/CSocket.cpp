#include "pch.h"
#include "CSocket.h"



CTCPSocket::CTCPSocket()
{
	m_Socket = INVALID_SOCKET;
}

CTCPSocket::~CTCPSocket()
{
	CloseSocket(); // �ر�socket
	Cleanup(); // �ر��׽��ֿ�
}

// ��ʼ���׽���
BOOL CTCPSocket::CreateSocket()
{
	// ��ʼ���׽���(˵��ʹ�õ�Э��)
	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_Socket)
	{
		ShowErrorInfo("[Server]: socket ");
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
	if (::bind(m_Socket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		ShowErrorInfo("[Server]: bind Error ");
		return FALSE;
	}
	return TRUE;
}

// ���׽������������������ӵ�״̬
BOOL CTCPSocket::ListenSocket()
{
	if (::listen(m_Socket, SOMAXCONN) == SOCKET_ERROR)
	{
		ShowErrorInfo("[Server]: listen Error: ");
		return FALSE;
	}
	return TRUE;
}

// �ȴ��ͻ�������
SOCKET CTCPSocket::AcceptSocket(SOCKADDR_IN& addr)
{
	int nSize = sizeof(addr);
	// ���ܿͻ�������

	return ::accept(m_Socket, (sockaddr*)&addr, &nSize); // ����
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
	if (::connect(m_Socket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		ShowErrorInfo("[Server]: connect Error ");
		return FALSE;
	}
	return TRUE;
}

// ��������
int CTCPSocket::Recv(SOCKET socket, char* pszBuff, int nLen)
{
	return ::recv(socket, pszBuff, nLen, 0);
}

// ��������
BOOL CTCPSocket::Send(SOCKET socket, const char* pszBuff, int nLen)
{
	if (::send(socket, pszBuff, nLen, 0) == SOCKET_ERROR)
	{
		return FALSE;
	}
	return TRUE;
}

// �ر�socket
void CTCPSocket::CloseSocket()
{
	if (m_Socket != INVALID_SOCKET)
	{
		::closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
	}
}

// ��ʼ���׽��ֿ�
BOOL CTCPSocket::Startup()
{
	// ��ʼ���׽��ֿ⣬˵��Ҫʹ�ò������׽��ֵİ汾
	WSADATA wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ShowErrorInfo("[Server]: WSAStartup");
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
	CString cs;
	cs = szBuff;
	AfxMessageBox(cs);

	LocalFree(lpMsgBuf);
}