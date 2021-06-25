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
	bool Accept(const char* szIP, short nPort); // 服务器用来等待连接
	bool Connect(const char* szIP, short nPort); // 客户端用来连接服务器
	bool Send(const char* pBuff, int nLen); // 发送数据
	bool Recv(char* pBuff, int nBufLen, OUT int& nBytesRecved);// 接收数据, nBytesRecved - 实收数据大小
	bool Close(); // 关闭
	SOCKET m_SocketAry[64]; // 最多支持64个客户端连接
	int m_nCurSocketCount; // 当前连接的客户端数量
private:
	SOCKET m_socket;
	sockaddr_in m_siDst;
	CThreadPool m_Pool;
private:
	enum PackageType
	{
		ESTABLISH, //三次握手
		DATA, //数据包
		ACK, //确认包
		ERR, //校验错误
		FIN //四次挥手
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
		int m_nType; //包类型
		int m_nSeq; //包的序号
		int m_nCheck; //校验值
		int m_nLen; //有效数据
		char m_data[DATALEN]; //包数据
	}PACKAGE, *PPACKAGE;

private:
	list<PACKAGE> m_lstRecv; //收数据链表
	CLock m_lckForLstRecv; //用于收数据链表的同步
	CByteStreamBuff m_bufRead; //接收数据缓冲区
	CLock m_lckForBufRead; //用于接收数据缓冲区的同步

	typedef struct tagSendPkgInfo
	{
		bool m_bSended; //这个包是否已经发送过,正在等待ack
		time_t m_nLastSendTime; //最后一次发送的时间
	}SENDPKGINFO, *PSENDPKGINFO;

	//待发包链表
	list<pair<SENDPKGINFO, PACKAGE>> m_lstSend;
	CLock m_lckForLstSend; //用于待发包链表的同步

	int m_nNextRecvSeq; //下一个接收的包的序号
	int m_nNextSendSeq; //下一个发送的包的序号
	int m_nCheckValue;

private:
	bool m_bRuning; //正在运行的标志
	void DataFromList2Buf(); //数据从接受链表到缓冲区
	void StartRecvFunction(); //启动接收数据的线程
	void StartSendFunction(); //启动发送数据的线程
	unsigned short CheckSum(char* pBuff, int len); // 包校验
	void GetCheckValue(const char* pBuff, int nBufLen);
	static DWORD CALLBACK RecvFunction(LPVOID lpParam); //接收数据的线程回调
	static DWORD CALLBACK SendFunction(LPVOID lpParam); //发送数据的线程回调

private:
	CLock m_lckForLog;
	void Log(stringstream& ss);
};