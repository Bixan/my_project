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
	// 创建
	BOOL CreateSocket();
	// 绑定
	BOOL BindSocket(char* pszIPAddr, USHORT usProt);
	// 监听
	BOOL ListenSocket();
	// 等待连接
	SOCKET AcceptSocket(SOCKADDR_IN &addr);
	// 连接
	BOOL ConnectSocket(char* pszIPAddr, USHORT usProt);
	// 接收数据
	int Recv(char* pszBuff, int nLen);
	// 发送数据
	BOOL Send(const char* pszBuff, int nLen);
	// 关闭连接
	void CloseSocket();
	// 初始化套接字库
	BOOL Startup();	
	// 关闭套接字库
	BOOL Cleanup();
	void ShowErrorInfo(const char* prefix);	// 显示错误信息
private:
	SOCKET m_socket;
};