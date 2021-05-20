#pragma once
#include <afx.h>
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

class CTCPSocket
{
public:
  CTCPSocket();
  ~CTCPSocket();
  BOOL CreateSocket();	// ����
  BOOL BindSocket(char* pszIPAddr, USHORT usProt);	// ��
  BOOL ListenSocket();	// ����
  SOCKET AcceptSocket(SOCKADDR_IN &addr);	// �ȴ�����
  BOOL ConnectSocket(char* pszIPAddr, USHORT usProt);   // ����
  int Recv(SOCKET socket, char* pszBuff, int nLen);	// ��������
  BOOL Send(SOCKET socket, const char* pszBuff, int nLen);// ��������
  void CloseSocket(); // �ر�
  BOOL Startup();	// ��ʼ���׽��ֿ�
  BOOL Cleanup();	// �ر��׽��ֿ�
  void ShowErrorInfo(const char* prefix); // ��ʾ������Ϣ

private:
  SOCKET m_Socket;
};