#include "pch.h"
#include "CMessages.h"
#include "MyMessages.h"
#include "CFileDlg.h"

int g_nReadFileSize = 0;
CMessages::CMessages()
{
	m_hWnd = NULL;	// ���ھ��
	m_ClientSocket = INVALID_SOCKET;	// �ͻ���socket
	m_HeartBeatTime = 0;	// ������������ʱ��
}

CMessages::~CMessages()
{
	CloaseResource();
}

// ��ʼ���׽��ֿ�
BOOL CMessages::InitWSAStartup()
{
	return m_Socket.Startup();
}

// �������󶨣�����socket
BOOL CMessages::StartSocket(char* pszIPAddr, USHORT usProt)
{
	// ���� socket
	if (!m_Socket.CreateSocket())
	{
		// ʹ���Զ�����Ϣ��֪ͨ������
		// ����socketʧ��
		::PostMessage(m_hWnd, WM_CREATE_SOCKETE_RROR, NULL, NULL);
		return FALSE;
	}

	// �� socket
	if (!m_Socket.BindSocket(pszIPAddr, usProt))
	{
		// ��socketʧ��
		::PostMessage(m_hWnd, WM_BIND_SOCKET_ERROR, NULL, NULL);
		return FALSE;
	}

	// ���� socket
	if (!m_Socket.ListenSocket())
	{
		// ����socketʧ��
		::PostMessage(m_hWnd, WM_LISTEN_SOCKETE_RROR, NULL, NULL);
		// �ر��׽����Լ�socket
		m_Socket.CloseSocket();
		m_Socket.Cleanup();
		return FALSE;
	}

	// �����̣߳��ȴ��ͻ�������
	HANDLE hThread = CreateThread(NULL, 0, WorkThread, this, 0, NULL);

	CloseHandle(hThread);
	return TRUE;
}

// ��������
int CMessages::RecvData(char* pBuff, int nLen)
{
	return m_Socket.Recv(m_ClientSocket, pBuff, nLen);
}

// ��������
BOOL CMessages::SendData(const char* pBuff, int nLen)
{
	return m_Socket.Send(m_ClientSocket, pBuff, nLen);
}

// �ر��׽��� socket
void CMessages::CloaseResource()
{
	m_Socket.CloseSocket();
	m_Socket.Cleanup();
}

// ���ô��ھ��
void CMessages::SetHwnd(HWND hWnd)
{
	m_hWnd = hWnd;
}

DWORD __stdcall CMessages::WorkThread(LPVOID lpParameter)
{
	CMessages* pMess = (CMessages*)lpParameter;

	PSOCKADDR_IN PSI = new SOCKADDR_IN();
	// ��ʼ��
	ZeroMemory(PSI, sizeof(SOCKADDR_IN));

	pMess->m_ClientSocket = pMess->m_Socket.AcceptSocket(*PSI);
	if (INVALID_SOCKET != pMess->m_ClientSocket)
	{
		// �ͻ������ӳɹ�
		::PostMessage(pMess->m_hWnd,
			WM_CLIENT_ACCEPT_SUCCEED,
			(WPARAM)pMess->m_ClientSocket,
			(LPARAM)PSI);
	}
	else
	{
		// �ͻ�������ʧ��
		::PostMessage(pMess->m_hWnd,
			WM_CLIENT_ACCEPT_FAIL,
			NULL,
			NULL);
	}

	// �ͻ������ӳɹ�������һ���߳������������ݣ���������
	HANDLE hRecvSendThread = CreateThread(NULL,
		0,
		RecvSendThread,
		pMess,
		0,
		NULL);

	// �߳̽������رվ��
	CloseHandle(hRecvSendThread);

	// ����һ���̣߳���ʱ�ͻ��˷�����Ϣ
	// ���ͻ����Ƿ����� -- ������
	HANDLE hHeartBeatThread = CreateThread(NULL,
		0,
		HeartBeatThread,
		pMess,
		0,
		NULL);
	pMess->m_HeartBeatTime = clock();
	CloseHandle(hHeartBeatThread);
	return 0;
}

// ���߳������������ݣ���������
DWORD __stdcall CMessages::RecvSendThread(LPVOID lpParameter)
{
	CMessages* pMess = (CMessages*)lpParameter;
	while (true)
	{
		PACKETHEAD ph;

		// ��������
		pMess->m_Socket.Recv(pMess->m_ClientSocket, (char*)&ph, sizeof(ph));

		// �������յ�������
		pMess->ParserCommand(ph);
	}

	return 0;
}

// ���ͻ����������
DWORD __stdcall CMessages::HeartBeatThread(LPVOID lpParameter)
{
	CMessages* pMess = (CMessages*)lpParameter;

	while (true)
	{
		Sleep(1000); //ÿ��һ����һ��

		clock_t tCurrent = clock();	//��ȡ��ǰʱ��

		if ((tCurrent - pMess->m_HeartBeatTime) / CLOCKS_PER_SEC > 5)
		{
			AfxMessageBox("[Server]: �ͻ���������");
			::PostMessage(pMess->m_hWnd, WM_CLIENT_QUIT, NULL, NULL);
			return 0;
		}
	}
	return 0;
}

// ��������
void CMessages::ParserCommand(PACKETHEAD ph)
{
	switch (ph.m_nCmd)
	{
	case PK_HEART_BEAT:
	{
		// ����������
		m_HeartBeatTime = clock();
		break;
	}
	case PK_OUTPUT_CMD_DATA:
	{
		// cmd �����ȡ������
		ParserGetCmdData(ph);
		break;
	}
	case PK_GET_DES_DRIVE:
	{
		// ��ȡ�ͻ��˴���
		ParserGetClientDrive(ph);
		break;
	}
	case PK_GET_FOLDER_NEXT_DATA:
	{
		// ��ȡ��ǰ�ļ�����һ������
		ParserGetNextFolder(ph);
		break;
	}
	case PK_GET_FOLDER_PREV_DATA:
	{
		// ��ȡ��ǰ�ļ�����һ������
		ParserGetPrevFolder(ph);
		break;
	}
	case PK_HAVE_NEXT_FOLDER:
	{
		// ��ǰ�ļ�����һ��
		ParserHaveNextFolder();
		break;
	}
	case PK_NO_NEXT_FOLDER:
	{
		// ��ǰ�ļ�û����һ��
		break;
	}
	case PK_HAVE_PREV_FOLDER:
	{
		// ��ǰ�ļ�����һ��
		ParserHavePrevFolder();
		break;
	}
	case PK_NO_PREV_FOLDER:
	{
		// ��ǰ�ļ�û����һ��
		break;
	}
	case PK_IS_FOLDER:
	{
		// ѡ�е����ļ���
		ParserIsFloder();
		break;
	}
	case PK_IS_FILE:
	{
		// ѡ�е����ļ�
		ParserIsFile();
		break;
	}
	case PK_GET_FILE_SIZE:
	{
		// �ļ���С
		ParserGetFileSize();
		break;
	}
	case PK_GET_FILE_DATA:
	{
		// ��ȡ�ļ�����
		ParserGetFileData(ph);
		break;
	}
	case PK_GET_FILE_NEXT_DATA:
	{
		// ��ȡ�ļ���һ������
		ParserGetFileNextData();
		break;
	}
	case PK_DOWN_FILE_SUCC:
	{
		// �ļ����سɹ�
		ParserFileDownSrcc();
		break;
	}
	case PK_CLIENT_SCREEN_DATA:
	{
		// ��ȡ��Ļ����
		ParserGetScreenData();
		break;
	}
	}
}

// ͨ��cmd��������Ķ�Ӧ������Ϣ
void CMessages::ParserGetCmdData(PACKETHEAD ph)
{
	// �������ݳ���new��Ӧ�ռ�
	char* pBuff = new char[ph.m_nLen];
	// ��ʼ��
	ZeroMemory(pBuff, ph.m_nLen);

	if (!m_Socket.Recv(m_ClientSocket, pBuff, ph.m_nLen))
	{
		// ��������ʧ��
		::PostMessage(m_hWnd, WM_RECV_DATA_ERROR, NULL, NULL);
		delete[] pBuff;
		return;
	}
	// �������ݳɹ�
	::PostMessage(m_hWnd, WM_GET_CMD_DATA, (WPARAM)pBuff, NULL);
}

// ��Ļ
void CMessages::ParserGetScreenData()
{
	PSCREENINFO pScreenInfo = new SCREENINFO();

	// ������Ļ�����Ϣ
	int nRet = m_Socket.Recv(m_ClientSocket, (char*)pScreenInfo, sizeof(SCREENINFO));

	if (nRet <= 0)
	{
		// ��������ʧ��
		::PostMessage(m_hWnd, WM_RECV_DATA_ERROR, NULL, NULL);
		return;
	}

	char* pScreenData = new char[pScreenInfo->m_CompressedSize];
	ZeroMemory(pScreenData, pScreenInfo->m_CompressedSize);

	// ������Ļ����
	int nTotal = pScreenInfo->m_CompressedSize;
	int nCurSize = 0;

	while (nCurSize != nTotal)
	{
		int nRetByte = m_Socket.Recv(m_ClientSocket, pScreenData + nCurSize, nTotal - nCurSize);

		if (nRetByte <= 0)
		{
			::PostMessage(m_hWnd, WM_RECV_DATA_ERROR, NULL, NULL);
			return;
		}
		nCurSize += nRetByte;
	}

	// ��ȡ��Ļ���ݳɹ�
	::PostMessage(m_hWnd,
		WM_CLIENT_SCREEN_DATA,
		(WPARAM)pScreenInfo,
		(LPARAM)pScreenData);
}

// ������Ļ����
void CMessages::SendScreenData()
{
	PACKETHEAD sendHead;
	sendHead.m_nCmd = PK_CLIENT_SCREEN_DATA;
	sendHead.m_nLen = 0;

	SendData((char*)&sendHead, sizeof(sendHead));
}

// ��ȡ�ͻ��˴���
void CMessages::ParserGetClientDrive(PACKETHEAD ph)
{
	// �������ݳ���new��Ӧ�ռ�
	char* pBuff = new char[ph.m_nLen];
	// ��ʼ��
	ZeroMemory(pBuff, ph.m_nLen);

	int* pnLen = new int(ph.m_nLen);

	// ����ͷ����
	int nRet = m_Socket.Recv(m_ClientSocket, pBuff, ph.m_nLen);

	if (nRet <= 0)
	{
		// ��������ʧ��
		::PostMessage(m_hWnd, WM_RECV_DATA_ERROR, NULL, NULL);
		// �ͷſռ�
		delete[] pBuff;
		delete pnLen;
		return;
	}
	// �������ݳɹ�
	::PostMessage(m_hWnd, WM_DRIVE_INFO, (WPARAM)pBuff, (LPARAM)pnLen);
}

// ��һ���ļ�
void CMessages::ParserGetNextFolder(PACKETHEAD ph)
{
	// �������ݳ���new��Ӧ�ռ�
	char* pBuff = new char[ph.m_nLen];
	// ��ʼ��
	ZeroMemory(pBuff, ph.m_nLen);

	// ����ͷ����
	int nRet = m_Socket.Recv(m_ClientSocket, pBuff, ph.m_nLen);

	if (nRet <= 0)
	{
		// ��������ʧ��
		::PostMessage(m_hWnd, WM_RECV_DATA_ERROR, NULL, NULL);
		// �ͷſռ�
		delete[] pBuff;
		return;
	}
	// �������ݳɹ�
	::PostMessage(m_hWnd, WM_GET_FOLDER_NEXT_DATA, (WPARAM)pBuff, NULL);
}

// ��һ���ļ�
void CMessages::ParserGetPrevFolder(PACKETHEAD ph)
{
	// �������ݳ���new��Ӧ�ռ�
	char* pBuff = new char[ph.m_nLen];
	// ��ʼ��
	ZeroMemory(pBuff, ph.m_nLen);

	// ����ͷ����
	int nRet = m_Socket.Recv(m_ClientSocket, pBuff, ph.m_nLen);

	if (nRet <= 0)
	{
		// ��������ʧ��
		::PostMessage(m_hWnd, WM_RECV_DATA_ERROR, NULL, NULL);
		// �ͷſռ�
		delete[] pBuff;
		return;
	}
	// �������ݳɹ�
	::PostMessage(m_hWnd, WM_GET_FOLDER_PREV_DATA, (WPARAM)pBuff, NULL);
}

// ������һ���ļ�
void CMessages::ParserHaveNextFolder()
{
	::PostMessage(m_hWnd, WM_HAVE_NEXT_FILE, NULL, NULL);
}

// ������һ���ļ�
void CMessages::ParserHavePrevFolder()
{
	::PostMessage(m_hWnd, WM_HAVE_PREV_FILE, NULL, NULL);
}

// ��ǰ��������ļ���
void CMessages::ParserIsFloder()
{
	::PostMessage(m_hWnd, WM_IS_FOLDER, NULL, NULL);
}

// ��ǰ��������ļ�
void CMessages::ParserIsFile()
{
	::PostMessage(m_hWnd, WM_IS_FILE, NULL, NULL);
}

// ��ȡ�ļ���С
void CMessages::ParserGetFileSize()
{
	PLARGE_INTEGER pSize = new LARGE_INTEGER();
	ZeroMemory(pSize, sizeof(LARGE_INTEGER));

	// ��������
	int nRet = m_Socket.Recv(m_ClientSocket, (char*)pSize, sizeof(PLARGE_INTEGER));

	if (nRet <= 0)
	{
		::PostMessage(m_hWnd, WM_RECV_DATA_ERROR, NULL, NULL);
		delete pSize;
		return;
	}
	// ��ȡ�ļ���С�ɹ�
	::PostMessage(m_hWnd, WM_GET_FILE_SIZE, (WPARAM)pSize, NULL);
}

// ��ȡ�ļ�����
void CMessages::ParserGetFileData(PACKETHEAD ph)
{
	int* pLen = new int(0);
	*pLen = ph.m_nLen;

	char* pBuff = new char[ph.m_nLen];
	ZeroMemory(pBuff, ph.m_nLen);

	// ��ȡ�ļ��ֽ���
	int nTotal = ph.m_nLen;
	int nCurSize = 0;

	while (nCurSize != nTotal)
	{
		int nRetByte = m_Socket.Recv(m_ClientSocket, pBuff + nCurSize, nTotal - nCurSize);

		if (nRetByte <= 0)
		{
			::PostMessage(m_hWnd, WM_RECV_DATA_ERROR, NULL, NULL);
			delete[] pBuff;
			return;
		}
		g_nReadFileSize += nTotal;
		nCurSize += nRetByte;
	}
	// ��ȡ�ļ����ݳɹ�
	::PostMessage(m_hWnd, WM_GET_FILE_DATA, (WPARAM)pBuff, (LPARAM)pLen);
}

// �ļ����سɹ�
void CMessages::ParserFileDownSrcc()
{
	::PostMessage(m_hWnd, WM_DOWN_FILE_DATA_SUCC, NULL, NULL);
}

// �ļ���һ������
void CMessages::ParserGetFileNextData()
{
	PLARGE_INTEGER plSrcFileSize = new LARGE_INTEGER();
	ZeroMemory(plSrcFileSize, sizeof(LARGE_INTEGER));

	//��������
	int nRet = m_Socket.Recv(m_ClientSocket, (char*)plSrcFileSize, sizeof(LARGE_INTEGER));

	if (nRet <= 0)
	{
		// ��������ʧ��
		::PostMessage(m_hWnd, WM_RECV_DATA_ERROR, NULL, NULL);
		delete plSrcFileSize;
		return;
	}
	// ��ȡ�ļ���һ�����ݳɹ�
	::PostMessage(m_hWnd, WM_GET_FILE_NEXT_DATA, (WPARAM)plSrcFileSize, NULL);
}
