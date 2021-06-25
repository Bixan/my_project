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
	// �󶨶˿�
	sockaddr_in siSrc;
	siSrc.sin_family = AF_INET;
	inet_pton(AF_INET, (char*)szIP, &siSrc.sin_addr);
	siSrc.sin_port = htons(nPort);
	int nRet = bind(m_socket, (sockaddr*)&siSrc, sizeof(siSrc));
	if (nRet == SOCKET_ERROR)
	{
		return false;
	}

	// ��������
	PACKAGE pkg;
	int nLenOfSiDst = sizeof(m_siDst);
	nRet = recvfrom(m_socket, (char*)&pkg, sizeof(pkg), 0, (sockaddr*)&m_siDst, &nLenOfSiDst);
	if (nRet == SOCKET_ERROR || pkg.m_nType != ESTABLISH)
	{
		return false;
	}

	// ��һ�����ݰ�
	m_nNextRecvSeq = pkg.m_nSeq + 1; // ��һ���������
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

	// �������ֳɹ�
	++m_nNextRecvSeq; // ��һ�����յİ������
	++m_nNextSendSeq; // ��һ�����͵İ������

	// �������ӳɹ��Ŀͻ���������Ϣ
	m_SocketAry[m_nCurSocketCount++] = m_socket;
	m_Pool.Create(4);
	// �����շ������߳�
	m_bRuning = true;
	StartRecvFunction(); // ���������߳�
	StartSendFunction(); // ���������߳�

	return true;
}

bool CUMTSocket::Connect(const char * szIP, short nPort)
{
	// ��������, ���ӷ�����
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
	// ��һ�����յİ���seq
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

	// ��һ�����͵İ���seq
	m_nNextSendSeq++;

	// �����շ��߳�
	m_bRuning = true;
	StartRecvFunction();
	StartSendFunction();
	return true;
}

bool CUMTSocket::Send(const char* pBuff, int nLen)
{
	// ������Բ�ɼ�����
	int nCount = (nLen % DATALEN == 0) ? (nLen / DATALEN) : (nLen / DATALEN + 1);

	// ����ɵ�С���������������
	for (int i = 0; i < nCount; i++)
	{
		// ���һ����
		if (i == nCount - 1)
		{
			int nPkgLen = (nLen % DATALEN == 0) ?	DATALEN : (nLen % DATALEN);
			m_lckForLstSend.Lock();
			PACKAGE pkg(DATA, m_nNextSendSeq++, pBuff + i * DATALEN, nPkgLen);
			m_lstSend.push_back(pair<SENDPKGINFO, PACKAGE>({ false, 0 }, pkg));
			m_lckForLstSend.UnLock();

			m_lckForLog.Lock();
			cout << "[socket]: �����ݵ��� seq - "
				<< pkg.m_nSeq
				<< " len - "
				<< hex
				<< pkg.m_nLen
				<< endl;
			m_lckForLog.UnLock();
			break;
		}

		// ��ֹͬ��
		m_lckForLstSend.Lock();
		PACKAGE pkg(DATA, m_nNextSendSeq++, pBuff + i * DATALEN, DATALEN);
		// m_lstSend.push_back({ {false, 0}, pkg });
		m_lstSend.push_back(pair<SENDPKGINFO, PACKAGE>({ false, 0 }, pkg));
		m_lckForLstSend.UnLock();

		m_lckForLog.Lock();
		cout << "[socket]: �����ݵ��� seq - "
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
			// û������,���г��߳�,��������,�ȴ����ݵĵ���
			m_lckForBufRead.UnLock();
			Sleep(1);
			continue;
		}

		// ������,�ж������Ƿ��㹻����������, 
		// �Լ������������㹻����pBuff,������pBuff
		// �Լ����������ݲ���,���ж��ٸ�����
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

// ������
void CUMTSocket::DataFromList2Buf()
{
	// ����һ����ŵİ������ݷ������ݻ�����
	m_lckForBufRead.Lock();
	m_lckForLstRecv.Lock();
	for (auto itr = m_lstRecv.begin();
		itr != m_lstRecv.end();
		itr++)
	{
		if (itr->m_nSeq == m_nNextRecvSeq)
		{
			m_lckForLog.Lock();
			cout << "[socket]: ���������� seq - " << itr->m_nSeq
				<< " len - " << hex << itr->m_nSeq
				<< endl;
			m_lckForLog.UnLock();

			m_bufRead.Write(itr->m_data, itr->m_nLen);
			m_lstRecv.erase(itr);
			m_nNextRecvSeq++; // ��һ���ȴ����յİ������

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

// ���������߳�
DWORD CUMTSocket::RecvFunction(LPVOID lpParam)
{
	CUMTSocket* pThis = (CUMTSocket*)lpParam;
	while (pThis->m_bRuning)
	{
		// ��ʱ
		fd_set fsRead;
		FD_ZERO(&fsRead);
		FD_SET(pThis->m_socket, &fsRead);
		timeval tv = { 10, 0 };
		int nRet = select(1, &fsRead, NULL, NULL, &tv);
		if (nRet == 0) // ��ʱ
		{
			// ����һ����ŵİ������ݷ������ݻ�����
			pThis->DataFromList2Buf();
			continue;
		}

		// �������ݰ�
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

		// �жϰ�������
		switch (pkg.m_nType)
		{
		// �յ����ݰ�
		case DATA:
		{
			// ���յ��İ��ŵ����������еȴ�����
			pThis->m_lckForLstRecv.Lock();
			pThis->m_lstRecv.push_back(pkg);
			pThis->m_lckForLstRecv.UnLock();
			pThis->m_lckForLog.Lock();
			cout << "[socket]: �յ����� seq - "
				<< pkg.m_nSeq
				<< " len - " << hex << pkg.m_nLen
				<< endl;
			pThis->m_lckForLog.UnLock();

			// �ظ�ack
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

		// �յ�ack��
		case ACK:
		{
			// ���յ�ack�İ�˵�����ճɹ�, �Ӵ������������Ƴ�
			pThis->m_lckForLstSend.Lock();
			for (auto itr = pThis->m_lstSend.begin();
				itr != pThis->m_lstSend.end();
				itr++)
			{
				if (itr->second.m_nSeq == pkg.m_nSeq)
				{
					pThis->m_lstSend.erase(itr);

					pThis->m_lckForLog.Lock();
					cout << "[socket]: �յ�ACK seq - "
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

		// ����һ����ŵİ������ݷ������ݻ�����
		pThis->DataFromList2Buf();
	}
	return 0;
}

// ���������߳�
DWORD CUMTSocket::SendFunction(LPVOID lpParam)
{
	CUMTSocket* pThis = (CUMTSocket*)lpParam;
	const time_t nTimeOut = 5; // 5�볬ʱ,���ط�
	while (pThis->m_bRuning)
	{
		pThis->m_lckForLstSend.Lock();
		for (auto& sendPkg:pThis->m_lstSend)
		{
			// �¼�������İ�����
			if (!sendPkg.first.m_bSended)
			{
				sendto(pThis->m_socket, 
					(char*)&sendPkg.second, 
					sizeof(sendPkg.second), 
					0, 
					(sockaddr*)&pThis->m_siDst,
					sizeof(pThis->m_siDst));

				sendPkg.first.m_bSended = true; // ����������Ѿ����͹���
				sendPkg.first.m_nLastSendTime = clock();
				pThis->m_lckForLog.Lock();
				cout << "[socket]: �������� seq - "
					<< sendPkg.second.m_nSeq
					<< " len - " << hex << sendPkg.second.m_nLen 
					<< endl;
				pThis->m_lckForLog.UnLock();
			}
			// ��ʱ�İ����ط�
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
				cout << "[socket]: �ط����� seq - "
					<< sendPkg.second.m_nSeq
					<< " len - " << hex << sendPkg.second.m_nLen
					<< endl;

				pThis->m_lckForLog.UnLock();
			}
		}
		pThis->m_lckForLstSend.UnLock();

		// �г��߳�,��ֹ�պ�cpu
		Sleep(1);
	}
	return 0;
}