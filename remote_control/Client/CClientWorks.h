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
	BOOL StartClient(char *szIpAddr, USHORT sProt);	// �����ͻ���
	static DWORD WINAPI HeartThread(LPVOID lpParameter);	// �����������߳�
	void ParserCommand(PACKETHEAD head);	// ��������

private:
	void ParserStartCmd();	// ����CMD
	static DWORD WINAPI CmdWorkThread(LPVOID lpParameter);
	void ParserCmdOrder(PACKETHEAD head);// ƥ�䵽CMD����

private:
	void ParserGetDrive(); // ��ȡ�ͻ��˴�������
	void ParserGetNextFolder(PACKETHEAD head);	// ��ȡ��ǰ�ļ�����һ��
	void ParserGetPrevFolder(PACKETHEAD head);	// ��ȡ��ǰ�ļ�����һ��
	void ParserDownloadIsFile(PACKETHEAD head);	// ���ص��Ƿ����ļ�
	void ParserBeginDownFile(PACKETHEAD head);	// ��ʼ�����ļ�
	void ParserUploadFile(PACKETHEAD head);	// �ϴ��ļ�
	void ParserGetFileData(PACKETHEAD head);	// ��ȡ�ļ�����
	void ParserSetClientMousePoint(PACKETHEAD head);	// ��ָ��������Ӧ����¼�
	void ParserMouseButtonDown(PACKETHEAD head);	// �ͻ�������������
	void ParserKeyBoardInput(PACKETHEAD head);	 // ��������
private:
	void ParserGetScreenData();	// ��ȡ��Ļ���ݲ������ݷ��͸�������

private:
	CTCPSocket m_socket;
	string m_strUploadFilePath; // �����ϴ��ļ���·��
	bool m_QuitFlag;	// �˳���־
	bool m_bCmdRet;
	HANDLE m_hReadOfChild;
	HANDLE m_hWriteOfChild;
	HANDLE m_hReadOfParent;
	HANDLE m_hWriteOfParent;

	CCriticalSection m_cs;	// �ٽ���������ͬ������
};