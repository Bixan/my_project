#pragma once
#include "CSocket.h"
#include "Packet.h"

class CFileDlg;
// 处理各种消息
class CMessages
{
public:
	CMessages();
	~CMessages();

public:
	// 初始化套接字库
	BOOL InitWSAStartup();
	BOOL StartSocket(char* pszIPAddr, USHORT usProt);// 创建，绑定，监听socket
	int RecvData(char* pBuff, int nLen);	// 接收数据
	BOOL SendData(const char* pBuff, int nLen); // 发送数据
	void CloaseResource();	// 关闭套接字 socket
	void SetHwnd(HWND hWnd);	// 设置窗口句柄

private:
	// 该线程为主线程，等待客户端连接
	static DWORD WINAPI WorkThread(LPVOID lpParameter);
	// 该线程用来接收数据，接收数据
	static DWORD WINAPI RecvSendThread(LPVOID lpParameter);
	// 创建一个线程，定时客户端发送消息检查客户端是否在线 -- 心跳包
	static DWORD WINAPI HeartBeatThread(LPVOID lpParameter);

private:
	void ParserCommand(PACKETHEAD ph);	// 解析命令

private:
	void ParserGetCmdData(PACKETHEAD ph);	// 过去客户端发送的cmd命令数据信息

private:
	void ParserGetClientDrive(PACKETHEAD ph);	// 获取客户端文件磁盘
	void ParserGetNextFolder(PACKETHEAD ph);	// 获取当前文件的下一层
	void ParserGetPrevFolder(PACKETHEAD ph);	// 获取当前文件的上一层
	void ParserHaveNextFolder();	// 当前文件夹有下一层
	void ParserHavePrevFolder();	// 当前文件夹有上一层
	void ParserIsFloder();	// 当前点击的文件是文件夹
	void ParserIsFile();	// 当前点击的文件是文件
	void ParserGetFileSize();	// 获取文件大小
	void ParserGetFileData(PACKETHEAD ph);	// 获取文件数据
	void ParserFileDownSrcc();	// 下载文件成功
	void ParserGetFileNextData();	// 获取文件的下一条数据
private:
	void ParserGetScreenData();	// 获取屏幕数据
	void SendScreenData();	// 发送屏幕数据
private:
	CTCPSocket m_Socket;	// 初始化套接字，首发数据等操作
	HWND m_hWnd;	// 窗口句柄
	SOCKET m_ClientSocket;	// 客户端socket
	clock_t m_HeartBeatTime;	// 心跳包，保存时间
	CFileDlg* m_pFileDlg;
};