#pragma once
#include <afx.h>
#include <winsock2.h>
#include <Windows.h>
#pragma comment(lib, "Ws2_32.lib")

class CTCPSocket
{
public:
	CTCPSocket();
	~CTCPSocket();
	// ����
	BOOL CreateSocket();
	// ��
	BOOL BindSocket(char* pszIPAddr, USHORT usProt);
	// ����
	BOOL ListenSocket();
	// �ȴ�����
	SOCKET AcceptSocket(SOCKADDR_IN &addr);
	// ����
	BOOL ConnectSocket(char* pszIPAddr, USHORT usProt);
	// ��������
	int Recv(char* pszBuff, int nLen);
	// ��������
	BOOL Send(const char* pszBuff, int nLen);
	// �ر�����
	void CloseSocket();
	// ��ʼ���׽��ֿ�
	BOOL Startup();	
	// �ر��׽��ֿ�
	BOOL Cleanup();
	void ShowErrorInfo(const char* prefix);	// ��ʾ������Ϣ
private:
	SOCKET m_socket;
};