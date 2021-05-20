#pragma once
#include <afx.h>
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

class CTCPSocket
{
public:
  CTCPSocket();
  ~CTCPSocket();
  BOOL CreateSocket();	// 创建
  BOOL BindSocket(char* pszIPAddr, USHORT usProt);	// 绑定
  BOOL ListenSocket();	// 监听
  SOCKET AcceptSocket(SOCKADDR_IN &addr);	// 等待连接
  BOOL ConnectSocket(char* pszIPAddr, USHORT usProt);   // 连接
  int Recv(SOCKET socket, char* pszBuff, int nLen);	// 接收数据
  BOOL Send(SOCKET socket, const char* pszBuff, int nLen);// 发送数据
  void CloseSocket(); // 关闭
  BOOL Startup();	// 初始化套接字库
  BOOL Cleanup();	// 关闭套接字库
  void ShowErrorInfo(const char* prefix); // 显示错误信息

private:
  SOCKET m_Socket;
};