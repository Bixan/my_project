#include "CUMTSocket.h"
#include <time.h>
#include <iostream>

using namespace std;

CUMTSocket::CUMTSocket()
{
	m_socket = INVALID_SOCKET;
	ZeroMemory(m_SocketAry, sizeof(m_SocketAry));
	m_nCurSocketCount = 0;
}

CUMTSocket::~CUMTSocket()
{
	Close();
}

bool CUMTSocket::CreateSocket()
{
	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_socket == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

bool CUMTSocket::Accept(const char * szIP, short nPort)
{
	// 绑定端口
	sockaddr_in siSrc;
	siSrc.sin_family = AF_INET;
	inet_pton(AF_INET, (char*)szIP, &siSrc.sin_addr);
	siSrc.sin_port = htons(nPort);
	int nRet = bind(m_socket, (sockaddr*)&siSrc, sizeof(siSrc));
	if (nRet == SOCKET_ERROR)
	{
		return false;
	}

	// 三次握手
	PACKAGE pkg;
	int nLenOfSiDst = sizeof(m_siDst);
	nRet = recvfrom(m_socket, (char*)&pkg, sizeof(pkg), 0, (sockaddr*)&m_siDst, &nLenOfSiDst);
	if (nRet == SOCKET_ERROR || pkg.m_nType != ESTABLISH)
	{
		return false;
	}

	// 下一个数据包
	m_nNextRecvSeq = pkg.m_nSeq + 1; // 下一个包的序号
	m_nNextSendSeq = 0;
	PACKAGE pkgSend(ESTABLISH, m_nNextSendSeq, nullptr, 0);
	nRet = sendto(m_socket,
		(char*)&pkgSend,
		sizeof(pkgSend),
		0,
		(sockaddr*)&m_siDst,
		sizeof(m_siDst));
	if (nRet == SOCKET_ERROR)
	{
		return false;
	}

	sockaddr_in siDst;
	nLenOfSiDst = sizeof(siDst);
	nRet = recvfrom(m_socket,
		(char*)&pkg,
		sizeof(pkg),
		0,
		(sockaddr*)&siDst,
		&nLenOfSiDst);
	if (nRet == SOCKET_ERROR || 
		pkg.m_nType != ESTABLISH || 
		pkg.m_nSeq != m_nNextRecvSeq)
	{
		return false;
	}

	// 三次握手成功
	++m_nNextRecvSeq; // 下一个接收的包的序号
	++m_nNextSendSeq; // 下一个发送的包的序号

	// 保存连接成功的客户端数据信息
	m_SocketAry[m_nCurSocketCount++] = m_socket;
	m_Pool.Create(4);
	// 启动收发数据线程
	m_bRuning = true;
	StartRecvFunction(); // 接收数据线程
	StartSendFunction(); // 发送数据线程

	return true;
}

bool CUMTSocket::Connect(const char * szIP, short nPort)
{
	// 三次握手, 连接服务器
	m_siDst.sin_family = AF_INET;
	inet_pton(AF_INET, (char*)szIP, &m_siDst.sin_addr);
	m_siDst.sin_port = htons(nPort);
	int nLenOfSiDst = sizeof(m_siDst);

	m_nNextSendSeq = 0;
	PACKAGE pkgSend(ESTABLISH, m_nNextSendSeq, nullptr, 0);
	int nRet = sendto(m_socket,
		(char*)&pkgSend,
		sizeof(pkgSend),
		0,
		(sockaddr*)&m_siDst,
		sizeof(m_siDst));

	if (nRet == SOCKET_ERROR)
	{
		return false;
	}

	PACKAGE pkg;
	nRet = recvfrom(m_socket,
		(char*)&pkg,
		sizeof(pkg),
		0,
		(sockaddr*)&m_siDst,
		&nLenOfSiDst);

	if (nRet == SOCKET_ERROR || pkg.m_nType != ESTABLISH)
	{
		return false;
	}
	// 下一个接收的包的seq
	m_nNextRecvSeq = pkg.m_nSeq + 1;

	m_nNextSendSeq++; 
	PACKAGE pkgSend2(ESTABLISH, m_nNextSendSeq, nullptr, 0);
	nRet = sendto(m_socket,
		(char*)&pkgSend2,
		sizeof(pkgSend2),
		0,
		(sockaddr*)&m_siDst,
		sizeof(m_siDst));
	if (nRet == SOCKET_ERROR)
	{
		return false;
	}

	// 下一个发送的包的seq
	m_nNextSendSeq++;

	// 启动收发线程
	m_bRuning = true;
	StartRecvFunction();
	StartSendFunction();
	return true;
}

bool CUMTSocket::Send(const char* pBuff, int nLen)
{
	// 计算可以拆成几个包
	int nCount = (nLen % DATALEN == 0) ? (nLen / DATALEN) : (nLen / DATALEN + 1);

	// 将拆成的小包放入待发包链表
	for (int i = 0; i < nCount; i++)
	{
		// 最后一个包
		if (i == nCount - 1)
		{
			int nPkgLen = (nLen % DATALEN == 0) ?	DATALEN : (nLen % DATALEN);
			m_lckForLstSend.Lock();
			PACKAGE pkg(DATA, m_nNextSendSeq++, pBuff + i * DATALEN, nPkgLen);
			m_lstSend.push_back(pair<SENDPKGINFO, PACKAGE>({ false, 0 }, pkg));
			m_lckForLstSend.UnLock();

			m_lckForLog.Lock();
			cout << "[socket]: 拆数据到包 seq - "
				<< pkg.m_nSeq
				<< " len - "
				<< hex
				<< pkg.m_nLen
				<< endl;
			m_lckForLog.UnLock();
			break;
		}

		// 防止同步
		m_lckForLstSend.Lock();
		PACKAGE pkg(DATA, m_nNextSendSeq++, pBuff + i * DATALEN, DATALEN);
		// m_lstSend.push_back({ {false, 0}, pkg });
		m_lstSend.push_back(pair<SENDPKGINFO, PACKAGE>({ false, 0 }, pkg));
		m_lckForLstSend.UnLock();

		m_lckForLog.Lock();
		cout << "[socket]: 拆数据到包 seq - "
			<< pkg.m_nSeq
			<< " len - "
			<< hex
			<< pkg.m_nLen
			<< endl;
		m_lckForLog.UnLock();
	}

	return true;
}

bool CUMTSocket::Recv(char * pBuff, int nBufLen, OUT int & nBytesRecved)
{
	while (true)
	{
		m_lckForBufRead.Lock();
		if (m_bufRead.GetSize() <= 0)
		{
			// 没有数据,则切出线程,阻塞函数,等待数据的到来
			m_lckForBufRead.UnLock();
			Sleep(1);
			continue;
		}

		// 有数据,判断数据是否足够填满缓冲区, 
		// 自己缓冲区数据足够填满pBuff,则填满pBuff
		// 自己缓冲区数据不够,则有多少给多少
		nBytesRecved = (m_bufRead.GetSize() >= nBufLen) ? nBufLen : m_bufRead.GetSize();
		m_bufRead.Read(pBuff, nBytesRecved);
		m_lckForBufRead.UnLock();
		break;
	}

	return true;
}

bool CUMTSocket::Close()
{
	if (m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
		return true;
	}
	return false;
}

// 包排序
void CUMTSocket::DataFromList2Buf()
{
	// 将下一个序号的包的数据放入数据缓冲区
	m_lckForBufRead.Lock();
	m_lckForLstRecv.Lock();
	for (auto itr = m_lstRecv.begin();
		itr != m_lstRecv.end();
		itr++)
	{
		if (itr->m_nSeq == m_nNextRecvSeq)
		{
			m_lckForLog.Lock();
			cout << "[socket]: 包到缓冲区 seq - " << itr->m_nSeq
				<< " len - " << hex << itr->m_nSeq
				<< endl;
			m_lckForLog.UnLock();

			m_bufRead.Write(itr->m_data, itr->m_nLen);
			m_lstRecv.erase(itr);
			m_nNextRecvSeq++; // 下一个等待接收的包的序号

			break;
		}
	}
	m_lckForBufRead.UnLock();
	m_lckForLstRecv.UnLock();

}

void CUMTSocket::Log(stringstream & ss)
{
	m_lckForLog.Lock();
	cout << ss.str();
	m_lckForLog.UnLock();
}

void CUMTSocket::StartRecvFunction()
{
	HANDLE hThread = CreateThread(NULL, 0, RecvFunction, this, 0, NULL);
	CloseHandle(hThread);
}

void CUMTSocket::StartSendFunction()
{
	HANDLE hThread = CreateThread(NULL, 0, SendFunction, this, 0, NULL);
	CloseHandle(hThread);
}

unsigned short CUMTSocket::CheckSum(char* pBuff, int len)
{
	unsigned int sum = 0;
	while (len > 1)
	{
		sum += *pBuff++;
		len -= 1;
	}
	if (len)
	{
		sum += *(unsigned char*)pBuff;
	}
	return (unsigned short)(~sum);
}

void CUMTSocket::GetCheckValue(const char* pBuff, int nBufLen)
{
	short b = 0;
	char szBuff[MAXBYTE] = { 0 };
	memcpy(szBuff, pBuff, nBufLen);
	b = CheckSum(szBuff, strlen(szBuff));
	char szBuffTemp[24] = { 0 };
	wsprintf(szBuffTemp, "%04x", b & 0xffff);
	m_nCheckValue = atoi(szBuffTemp);
}

// 接收数据线程
DWORD CUMTSocket::RecvFunction(LPVOID lpParam)
{
	CUMTSocket* pThis = (CUMTSocket*)lpParam;
	while (pThis->m_bRuning)
	{
		// 定时
		fd_set fsRead;
		FD_ZERO(&fsRead);
		FD_SET(pThis->m_socket, &fsRead);
		timeval tv = { 10, 0 };
		int nRet = select(1, &fsRead, NULL, NULL, &tv);
		if (nRet == 0) // 超时
		{
			// 将下一个序号的包的数据放入数据缓冲区
			pThis->DataFromList2Buf();
			continue;
		}

		// 接收数据包
		PACKAGE pkg;
		sockaddr_in siDst;
		int nLenOfSiDst = sizeof(siDst);
		nRet = recvfrom(pThis->m_socket,
			(char*)&pkg,
			sizeof(pkg),
			0,
			(sockaddr*)&siDst,
			&nLenOfSiDst);
		if (nRet == SOCKET_ERROR)
		{
			continue;
		}

		// 判断包的类型
		switch (pkg.m_nType)
		{
		// 收到数据包
		case DATA:
		{
			// 将收到的包放到接收链表中等待处理
			pThis->m_lckForLstRecv.Lock();
			pThis->m_lstRecv.push_back(pkg);
			pThis->m_lckForLstRecv.UnLock();
			pThis->m_lckForLog.Lock();
			cout << "[socket]: 收到数据 seq - "
				<< pkg.m_nSeq
				<< " len - " << hex << pkg.m_nLen
				<< endl;
			pThis->m_lckForLog.UnLock();

			// 回复ack
			PACKAGE pkgSend(ACK, pkg.m_nSeq, nullptr, 0);
			int nRet = sendto(pThis->m_socket, 
				(char*)&pkgSend, sizeof(pkgSend), 
				0, 
				(sockaddr*)&pThis->m_siDst, sizeof(pThis->m_siDst));
			if (nRet == SOCKET_ERROR)
			{
				continue;
			}
			break;
		}

		// 收到ack包
		case ACK:
		{
			// 将收到ack的包说明接收成功, 从待发包链表中移除
			pThis->m_lckForLstSend.Lock();
			for (auto itr = pThis->m_lstSend.begin();
				itr != pThis->m_lstSend.end();
				itr++)
			{
				if (itr->second.m_nSeq == pkg.m_nSeq)
				{
					pThis->m_lstSend.erase(itr);

					pThis->m_lckForLog.Lock();
					cout << "[socket]: 收到ACK seq - "
						<< pkg.m_nSeq
						<< " len - " << hex << pkg.m_nLen
						<< endl;
					pThis->m_lckForLog.UnLock();
					break;
				}
			}
			pThis->m_lckForLstSend.UnLock();
			break;
		}
		}

		// 将下一个序号的包的数据放入数据缓冲区
		pThis->DataFromList2Buf();
	}
	return 0;
}

// 发送数据线程
DWORD CUMTSocket::SendFunction(LPVOID lpParam)
{
	CUMTSocket* pThis = (CUMTSocket*)lpParam;
	const time_t nTimeOut = 5; // 5秒超时,会重发
	while (pThis->m_bRuning)
	{
		pThis->m_lckForLstSend.Lock();
		for (auto& sendPkg:pThis->m_lstSend)
		{
			// 新加入链表的包发送
			if (!sendPkg.first.m_bSended)
			{
				sendto(pThis->m_socket, 
					(char*)&sendPkg.second, 
					sizeof(sendPkg.second), 
					0, 
					(sockaddr*)&pThis->m_siDst,
					sizeof(pThis->m_siDst));

				sendPkg.first.m_bSended = true; // 表明这个包已经发送过了
				sendPkg.first.m_nLastSendTime = clock();
				pThis->m_lckForLog.Lock();
				cout << "[socket]: 发送数据 seq - "
					<< sendPkg.second.m_nSeq
					<< " len - " << hex << sendPkg.second.m_nLen 
					<< endl;
				pThis->m_lckForLog.UnLock();
			}
			// 超时的包的重发
			else if ((clock() - sendPkg.first.m_nLastSendTime) / CLOCKS_PER_SEC >= nTimeOut)
			{
				sendto(pThis->m_socket,
					(char*)&sendPkg.second,
					sizeof(sendPkg.second),
					0,
					(sockaddr*)&pThis->m_siDst,
					sizeof(pThis->m_siDst));
				sendPkg.first.m_nLastSendTime = clock();

				pThis->m_lckForLog.Lock();
				cout << "[socket]: 重发数据 seq - "
					<< sendPkg.second.m_nSeq
					<< " len - " << hex << sendPkg.second.m_nLen
					<< endl;

				pThis->m_lckForLog.UnLock();
			}
		}
		pThis->m_lckForLstSend.UnLock();

		// 切除线程,防止空耗cpu
		Sleep(1);
	}
	return 0;
}