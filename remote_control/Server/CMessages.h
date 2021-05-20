#pragma once
#include "CSocket.h"
#include "Packet.h"

class CFileDlg;
// ���������Ϣ
class CMessages
{
public:
	CMessages();
	~CMessages();

public:
	// ��ʼ���׽��ֿ�
	BOOL InitWSAStartup();
	BOOL StartSocket(char* pszIPAddr, USHORT usProt);// �������󶨣�����socket
	int RecvData(char* pBuff, int nLen);	// ��������
	BOOL SendData(const char* pBuff, int nLen); // ��������
	void CloaseResource();	// �ر��׽��� socket
	void SetHwnd(HWND hWnd);	// ���ô��ھ��

private:
	// ���߳�Ϊ���̣߳��ȴ��ͻ�������
	static DWORD WINAPI WorkThread(LPVOID lpParameter);
	// ���߳������������ݣ���������
	static DWORD WINAPI RecvSendThread(LPVOID lpParameter);
	// ����һ���̣߳���ʱ�ͻ��˷�����Ϣ���ͻ����Ƿ����� -- ������
	static DWORD WINAPI HeartBeatThread(LPVOID lpParameter);

private:
	void ParserCommand(PACKETHEAD ph);	// ��������

private:
	void ParserGetCmdData(PACKETHEAD ph);	// ��ȥ�ͻ��˷��͵�cmd����������Ϣ

private:
	void ParserGetClientDrive(PACKETHEAD ph);	// ��ȡ�ͻ����ļ�����
	void ParserGetNextFolder(PACKETHEAD ph);	// ��ȡ��ǰ�ļ�����һ��
	void ParserGetPrevFolder(PACKETHEAD ph);	// ��ȡ��ǰ�ļ�����һ��
	void ParserHaveNextFolder();	// ��ǰ�ļ�������һ��
	void ParserHavePrevFolder();	// ��ǰ�ļ�������һ��
	void ParserIsFloder();	// ��ǰ������ļ����ļ���
	void ParserIsFile();	// ��ǰ������ļ����ļ�
	void ParserGetFileSize();	// ��ȡ�ļ���С
	void ParserGetFileData(PACKETHEAD ph);	// ��ȡ�ļ�����
	void ParserFileDownSrcc();	// �����ļ��ɹ�
	void ParserGetFileNextData();	// ��ȡ�ļ�����һ������
private:
	void ParserGetScreenData();	// ��ȡ��Ļ����
	void SendScreenData();	// ������Ļ����
private:
	CTCPSocket m_Socket;	// ��ʼ���׽��֣��׷����ݵȲ���
	HWND m_hWnd;	// ���ھ��
	SOCKET m_ClientSocket;	// �ͻ���socket
	clock_t m_HeartBeatTime;	// ������������ʱ��
	CFileDlg* m_pFileDlg;
};