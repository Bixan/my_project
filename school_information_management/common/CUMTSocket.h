#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <list>
#include <sstream>
#include <mysql.h>
#include "CThreadPool.h"
#pragma comment(lib, "libmysql.lib")
using namespace std;

#include "ByteStreamBuff.h"
#include "CLock.h"

class CUMTSocket
{
public:
	CUMTSocket();
	~CUMTSocket();
	bool CreateSocket();
	bool Accept(const char* szIP, short nPort); // �����������ȴ�����
	bool Connect(const char* szIP, short nPort); // �ͻ����������ӷ�����
	bool Send(const char* pBuff, int nLen); // ��������
	bool Recv(char* pBuff, int nBufLen, OUT int& nBytesRecved);// ��������, nBytesRecved - ʵ�����ݴ�С
	bool Close(); // �ر�
	SOCKET m_SocketAry[64]; // ���֧��64���ͻ�������
	int m_nCurSocketCount; // ��ǰ���ӵĿͻ�������
private:
	SOCKET m_socket;
	sockaddr_in m_siDst;
	CThreadPool m_Pool;
private:
	enum PackageType
	{
		ESTABLISH, //��������
		DATA, //���ݰ�
		ACK, //ȷ�ϰ�
		ERR, //У�����
		FIN //�Ĵλ���
	};

	#define DATALEN 1024
	typedef struct tagPackage
	{
		tagPackage() :m_nType(0), m_nSeq(0), m_nCheck(0), m_nLen(0)
		{
			memset(m_data, 0, DATALEN);
		}
		tagPackage(int nType, int nSeq, const char* pBuff, int nLen) :
			m_nType(nType),
			m_nSeq(nSeq),
			m_nCheck(0) 
		{ 
			if (pBuff != nullptr)
			{
				m_nLen = nLen;
				memcpy(m_data, pBuff, nLen);
			}
			else
			{
				m_nLen = 0;
				memset(m_data, 0, DATALEN);
			}
		}
		int m_nType; //������
		int m_nSeq; //�������
		int m_nCheck; //У��ֵ
		int m_nLen; //��Ч����
		char m_data[DATALEN]; //������
	}PACKAGE, *PPACKAGE;

private:
	list<PACKAGE> m_lstRecv; //����������
	CLock m_lckForLstRecv; //���������������ͬ��
	CByteStreamBuff m_bufRead; //�������ݻ�����
	CLock m_lckForBufRead; //���ڽ������ݻ�������ͬ��

	typedef struct tagSendPkgInfo
	{
		bool m_bSended; //������Ƿ��Ѿ����͹�,���ڵȴ�ack
		time_t m_nLastSendTime; //���һ�η��͵�ʱ��
	}SENDPKGINFO, *PSENDPKGINFO;

	//����������
	list<pair<SENDPKGINFO, PACKAGE>> m_lstSend;
	CLock m_lckForLstSend; //���ڴ����������ͬ��

	int m_nNextRecvSeq; //��һ�����յİ������
	int m_nNextSendSeq; //��һ�����͵İ������
	int m_nCheckValue;

private:
	bool m_bRuning; //�������еı�־
	void DataFromList2Buf(); //���ݴӽ�������������
	void StartRecvFunction(); //�����������ݵ��߳�
	void StartSendFunction(); //�����������ݵ��߳�
	unsigned short CheckSum(char* pBuff, int len); // ��У��
	void GetCheckValue(const char* pBuff, int nBufLen);
	static DWORD CALLBACK RecvFunction(LPVOID lpParam); //�������ݵ��̻߳ص�
	static DWORD CALLBACK SendFunction(LPVOID lpParam); //�������ݵ��̻߳ص�

private:
	CLock m_lckForLog;
	void Log(stringstream& ss);
};