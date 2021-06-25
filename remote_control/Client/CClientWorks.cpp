#include "CClientWorks.h"
#include "zlib.h"
#pragma comment(lib, "zlib.lib")

SCREENINFO g_ScreenInfo;
DWORD g_dwFileSize = 0;	 // ���͵����ֽ���
CClientWorks::CClientWorks()
{
	m_QuitFlag = false;

	// ������CMD���
	m_bCmdRet = false;
	m_hReadOfChild = NULL;
	m_hWriteOfChild = NULL;
	m_hReadOfParent = NULL;
	m_hWriteOfParent = NULL;
}

CClientWorks::~CClientWorks()
{
}

// ����
BOOL CClientWorks::StartClient(char *szIpAddr, USHORT sProt)
{
	// �������
	if (!m_socket.Startup())
	{
		cout << "[Client]: �������ʧ��" << endl;
		return FALSE;
	}

	cout << "[Client]: �������ɹ�" << endl;

	// ����socket
	if (!m_socket.CreateSocket())
	{
		cout << "[Client]: ����socketʧ��" << endl;
		m_socket.Cleanup();
		return FALSE;
	}

	cout << "[Client]: ����socket�ɹ�" << endl;

	// ����
	if (!m_socket.ConnectSocket(szIpAddr, sProt))
	{
		cout << "[Client]: ����socketʧ��..." << endl;
		Sleep(3000);
		m_socket.CloseSocket();
		m_socket.Cleanup();
		return FALSE;
	}
	cout << "[Client]: socket ���ӳɹ�" << endl;
	cout << "[Client]: �ͻ����������������..." << endl;
	Sleep(1000);
	cout << "[Client]: �ͻ�������������ӳɹ�" << endl;
	m_QuitFlag = true;

	// �����̣߳�����������
	HANDLE hThread = CreateThread(NULL, 0, HeartThread, this, 0, NULL);
	CloseHandle(hThread);

	while (m_QuitFlag)
	{
		PACKETHEAD ph;

	// ����ͷ����
	int nRet = m_socket.Recv((char *)&ph, sizeof(ph));
	if (nRet <= 0)
	{
		cout << "[Client]: ���ƶ��Թر�, ���򼴽��˳�..." << endl;
		Sleep(2000);
		exit(0);
		break;
	}

	// ��������
	ParserCommand(ph);
	}

	return TRUE;
}


// �����������߳�
DWORD WINAPI CClientWorks::HeartThread(LPVOID lpParameter)
{
	CClientWorks *pThis = (CClientWorks *)lpParameter;

	// ����������
	while (true)
	{
		pThis->m_cs.EnterCS();
		PACKETHEAD ph;
		ph.m_nCmd = PK_HEART_BEAT;
		ph.m_nLen = 0;

		if (!pThis->m_socket.Send((char *)&ph, sizeof(ph)))
		{
			cout << "[Client]: ��������ͻ������ӳ�ʱ������������Ƿ��Ѿ�����..." << endl;
			return 0;
		}
		pThis->m_cs.LeaveCS();
	}

	return 0;
}


// ��������
void CClientWorks::ParserCommand(PACKETHEAD ph)
{
	switch (ph.m_nCmd)
	{
	case PK_GET_DES_DRIVE:
	{
		// ������
		// ��ȡ�ļ�����
		ParserGetDrive();
		break;
	}
	case PK_GET_FOLDER_NEXT_DATA:
	{
		// ��ȡ��һ���ļ���
		ParserGetNextFolder(ph);
		break;
	}
	case PK_GET_FOLDER_PREV_DATA:
	{
		// ��ȡ��һ���ļ���
		ParserGetPrevFolder(ph);
		break;
	}
	case PK_DOWN_IS_FILE:
	{
		// �ļ�����
		ParserDownloadIsFile(ph);
		break;
	}
	case PK_START_DOWN_FILE:
	{
		// ��ʼ�����ļ�
		ParserBeginDownFile(ph);
		break;
	}
	case PK_UP_FILE_DATA:
	{
		// �ϴ��ļ�����
		ParserUploadFile(ph);
		break;
	}
	case PK_GET_FILE_DATA:
	{
		// ��ȡ�ļ�����
		ParserGetFileData(ph);
		break;
	}
	case PK_START_CMD:
	{
		// ����cmd
		ParserStartCmd();
		break;
	}
	case PK_INPUT_CMD_ORDER:
	{
		// cmd����
		ParserCmdOrder(ph);
		break;
	}
	case PK_QUIT_CMD:
	{
		// �˳�cmd
		m_bCmdRet = false;
		break;
	}
	case PK_CLIENT_SCREEN_DATA:
	{
		// ��ȡ�ͻ�����Ļ����
		ParserGetScreenData();
		break;
	}
	case PK_MOUSE_BUTTON_DBLCLK:
	{
		// ������˫��
		ParserSetClientMousePoint(ph);
		break;
	}
	case PK_QUIT_CLIENT_SCREEN:
	{
		// ��Ļ����
		cout << "[Client]: �������˳���Ļ���" << endl;
		break;
	}
	case PK_MOUSE_BUTTON_DOWN:
	{
		// ����������
		ParserMouseButtonDown(ph);
		break;
	}
	case PK_KEYBOARD_INPUT:
	{
		// ��������
		ParserKeyBoardInput(ph);
		break;
	}
	}
}

// ��ȡ��Ļ���ݷ���
void CClientWorks::ParserGetScreenData()
{
	// ��ȡ�����С
	int nWndWidth = GetSystemMetrics(SM_CXSCREEN);
	int nWndHeight = GetSystemMetrics(SM_CYSCREEN);

	// ��ȡ����dc
	HDC hDeskDc = GetWindowDC(NULL);

	// ���������ڴ�DC
	HDC MemoryDc = CreateCompatibleDC(hDeskDc);

	// ��������λͼ
	HBITMAP hMap = CreateCompatibleBitmap(hDeskDc,
		GetDeviceCaps(hDeskDc, HORZRES),
		GetDeviceCaps(hDeskDc, VERTRES));

	// ��λͼ
	SelectObject(MemoryDc, hMap);

	BitBlt(MemoryDc, 0, 0, nWndWidth, nWndHeight, hDeskDc, 0, 0, SRCCOPY);

	// ����洢λͼ�Ŀռ�
	int nSize = nWndWidth * nWndHeight * sizeof(COLORREF);
	char *pDestData = new char[nSize];
	ZeroMemory(pDestData, nSize);

	// ��λͼ��ɫ
	GetBitmapBits(hMap, nSize, pDestData);

	// ѹ���ļ�
	uLongf outSize;
	char *pCompression = new char[nSize];
	ZeroMemory(pCompression, nSize);

	// ѹ��
	compress((unsigned char*)pCompression, &outSize, (unsigned char*)pDestData, nSize);

	// ͷ��Ϣ
	PACKETHEAD	SendHead;
	SendHead.m_nCmd = PK_CLIENT_SCREEN_DATA;
	SendHead.m_nLen = 0;

	// ��Ļ�����Ϣ
	g_ScreenInfo.m_nWndHeight = nWndHeight;
	g_ScreenInfo.m_nWndWidth = nWndWidth;
	g_ScreenInfo.m_SrcSize = nSize;
	g_ScreenInfo.m_CompressedSize = outSize;

	m_cs.EnterCS();
	// ����ͷ����
	m_socket.Send((char*)&SendHead, sizeof(SendHead));

	// ������Ļ�����Ϣ
	m_socket.Send((char*)&g_ScreenInfo, sizeof(g_ScreenInfo));

	// ����ѹ����Ľ�����Ϣ
	m_socket.Send(pCompression, outSize);
	m_cs.LeaveCS();

	// ɾ������
	delete[] pCompression;
	delete[] pDestData;

	// ������Դ
	DeleteObject(hMap);
	DeleteDC(MemoryDc);
	ReleaseDC(NULL, hDeskDc);
}

// ��ȡ����������
void CClientWorks::ParserGetDrive()
{
	DWORD dwSize = GetLogicalDriveStrings(0, NULL);

	char *pBuff = new char[dwSize];
	ZeroMemory(pBuff, dwSize);

	GetLogicalDriveStrings(dwSize, pBuff);

	// ����ͷ
	PACKETHEAD	SendHead;
	SendHead.m_nCmd = PK_GET_DES_DRIVE;
	SendHead.m_nLen = dwSize;

	m_cs.EnterCS();
	if (!m_socket.Send((char *)&SendHead, sizeof(SendHead)))
	{
		cout << "[Client]: ���������������ʧ��" << endl;
		delete[] pBuff;
		return;
	}

	// ��������
	if (!m_socket.Send(pBuff, dwSize))
	{
		cout << "[Client]: ���������������ʧ��" << endl;
		delete[] pBuff;
		return;
	}
	m_cs.LeaveCS();

	delete[] pBuff;
}

// ��ȡ��һ���ļ���
void CClientWorks::ParserGetNextFolder(PACKETHEAD ph)
{
	char *pBuff = new char[ph.m_nLen + 5];
	ZeroMemory(pBuff, ph.m_nLen + 5);

	// ����ͷ����
	int nRet = m_socket.Recv(pBuff, ph.m_nLen);

	if (nRet == 0 || nRet == SOCKET_ERROR)
	{
		cout << "[Client]: ���շ���������ʧ��" << endl;
		delete[] pBuff;
		return;
	}

	strcat(pBuff, "\\*.*");
	WIN32_FIND_DATA FindFileData = { 0 };
	HANDLE hFindFile = FindFirstFile(pBuff, &FindFileData);

	if (INVALID_HANDLE_VALUE == hFindFile)
	{
		// û����һ���ļ���
		PACKETHEAD SendHead;
		SendHead.m_nCmd = PK_NO_NEXT_FOLDER;
		SendHead.m_nLen = 0;

		m_cs.EnterCS();
		if (!m_socket.Send((char *)&SendHead, sizeof(SendHead)))
		{
			cout << "[Client]: ���������������ʧ��" << endl;
			delete[] pBuff;
			pBuff = NULL;
			return;
		}
		m_cs.LeaveCS();
		return;
	}

	// ����һ���ļ���
	PACKETHEAD NoticeHead;
	NoticeHead.m_nCmd = PK_HAVE_NEXT_FOLDER;
	NoticeHead.m_nLen = 0;

	m_cs.EnterCS();
	if (!m_socket.Send((char *)&NoticeHead, sizeof(NoticeHead)))
	{
		cout << "[Client]: ���������������ʧ��" << endl;
		delete[] pBuff;
		pBuff = NULL;
		return;
	}
	m_cs.LeaveCS();

	while (true)
	{
		BOOL bRet = FindNextFile(hFindFile, &FindFileData);

		if (!bRet)
		{
			break;
		}

		// �����ȡ�����ļ���
		string strFileName = FindFileData.cFileName;

		if (strFileName == "." || strFileName == "..")
		{
			continue;
		}

		// ����ͷ
		PACKETHEAD Sendph;
		Sendph.m_nCmd = PK_GET_FOLDER_NEXT_DATA;
		Sendph.m_nLen = strFileName.length() + 1;

		m_cs.EnterCS();
		if (!m_socket.Send((char *)&Sendph, sizeof(Sendph)))
		{
			cout << "[Client]: ���������������ʧ��" << endl;
			delete[] pBuff;
			pBuff = NULL;
			return;
		}

		// ��������
		if (!m_socket.Send(strFileName.c_str(), Sendph.m_nLen))
		{
			cout << "[Client]: ���������������ʧ��" << endl;
			delete[] pBuff;
			pBuff = NULL;
			return;
		}
		m_cs.LeaveCS();
	}

	FindClose(hFindFile);
	delete[] pBuff;
	pBuff = NULL;
}

// ��ȡ��һ���ļ���
void CClientWorks::ParserGetPrevFolder(PACKETHEAD ph)
{
	char *pBuff = new char[ph.m_nLen + 9];
	ZeroMemory(pBuff, ph.m_nLen + 9);

	// ���շ��������͵�����Э��ͷ
	int nRet = m_socket.Recv(pBuff, ph.m_nLen);

	if (nRet == 0 || nRet == SOCKET_ERROR)
	{
		cout << "[Client]: ���շ��������͵�����ʧ��" << endl;
		delete[] pBuff;
		return;
	}

	strcat(pBuff, "\\..\\*.*");
	WIN32_FIND_DATA FindFileData = { 0 };
	HANDLE hFindFile = FindFirstFile(pBuff, &FindFileData);

	if (INVALID_HANDLE_VALUE == hFindFile)
	{
		// û����һ���ļ���
		PACKETHEAD SendHead;
		SendHead.m_nCmd = PK_NO_PREV_FOLDER;
		SendHead.m_nLen = 0;

		m_cs.EnterCS();
		// ��������
		if (!m_socket.Send((char *)&ph, sizeof(ph)))
		{
			cout << "[Client]: ���������������ʧ��" << endl;
			delete[] pBuff;
			return;
		}
		m_cs.LeaveCS();
		return;
	}

	// ����һ���ļ���
	PACKETHEAD NoticeHead;
	NoticeHead.m_nCmd = PK_HAVE_PREV_FOLDER;
	NoticeHead.m_nLen = 0;

	m_cs.EnterCS();
	if (!m_socket.Send((char *)&NoticeHead, sizeof(NoticeHead)))
	{
		cout << "[Client]: ���������������ʧ��" << endl;
		delete[] pBuff;
		return;
	}
	m_cs.LeaveCS();

	while (true)
	{
		BOOL bRet = FindNextFile(hFindFile, &FindFileData);

		if (!bRet)
		{
			break;
		}

		string strFileName = FindFileData.cFileName;
		if (strFileName == "." || strFileName == "..")
		{
			continue;
		}

		// ����ͷ
		PACKETHEAD Sendph;
		Sendph.m_nCmd = PK_GET_FOLDER_PREV_DATA;
		Sendph.m_nLen = strFileName.length() + 1;

		m_cs.EnterCS();
		if (!m_socket.Send((char *)&Sendph, sizeof(Sendph)))
		{
			cout << "[Client]: ���������������ʧ��" << endl;
			delete[] pBuff;
			return;
		}

		// ��������
		if (!m_socket.Send(strFileName.c_str(), Sendph.m_nLen))
		{
			cout << "[Client]: ���������������ʧ��" << endl;
			delete[] pBuff;
			return;
		}
		m_cs.LeaveCS();
	}

	FindClose(hFindFile);
	delete[] pBuff;
}

// ���ص��Ƿ����ļ�
void CClientWorks::ParserDownloadIsFile(PACKETHEAD ph)
{
	// ���뻺����
	char *pBuff = new char[ph.m_nLen];
	ZeroMemory(pBuff, ph.m_nLen);

	// ��������
	if (!m_socket.Recv(pBuff, ph.m_nLen))
	{
		cout << "[Client]: ���շ���������ʧ��" << endl;
		delete[] pBuff;
		return;
	}

	WIN32_FIND_DATA FindFileData = { 0 };
	HANDLE hFindFile = FindFirstFile(pBuff, &FindFileData);

	if (INVALID_HANDLE_VALUE == hFindFile)
	{
		cout << "[Client]: ���ļ�ʧ��" << endl;
		return;
	}

	// ������ļ��У�������Ϣ���߿��ƶ��޷�����
	if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		// ������ѡ�������ļ����ļ���
		PACKETHEAD SendHead;
		SendHead.m_nCmd = PK_IS_FOLDER;
		SendHead.m_nLen = 0;

		m_cs.EnterCS();
		if (!m_socket.Send((char *)&SendHead, sizeof(SendHead)))
		{
			cout << "[Client]: ���������������ʧ��" << endl;
			return;
		}
		m_cs.LeaveCS();
	}
	// ������ļ������߿��ƶ����ļ�������Խ�������

	else
	{
		// ������ѡ�������ļ����ļ���
		PACKETHEAD SendHead;
		SendHead.m_nCmd = PK_IS_FILE;
		SendHead.m_nLen = 0;

		m_cs.EnterCS();
		if (!m_socket.Send((char *)&SendHead, sizeof(SendHead)))
		{
			cout << "[Client]: ������Ϣʧ��" << endl;
			return;
		}
		m_cs.LeaveCS();
	}

	FindClose(hFindFile);
	delete[] pBuff;
}

// ��ʼ�����ļ�
void CClientWorks::ParserBeginDownFile(PACKETHEAD ph)
{
	// ���
	g_dwFileSize = 0;
	char* pDownFilePath = new char[ph.m_nLen];
	ZeroMemory(pDownFilePath, ph.m_nLen);

	// ��ȡ���������͵��������ļ���·��
	if (!m_socket.Recv(pDownFilePath, ph.m_nLen))
	{
		cout << "[Client]: ��ȡ·��ʧ��" << endl;
		delete[] pDownFilePath;
		return;
	}

	// ��Դ�ļ�
	HANDLE hSRcFile = CreateFile(pDownFilePath,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	// ��ջ�����
	delete[] pDownFilePath;
	pDownFilePath = NULL;

	// ���ļ�ʧ��
	if (hSRcFile == INVALID_HANDLE_VALUE)
	{
		cout << "[Client]: �����ļ�ʱ�����ļ�·��ʧ��" << endl;
		// �ر��ļ�
		CloseHandle(hSRcFile);
		return;
	}

	// ��ȡҪ�����ļ������ֽ���
	LARGE_INTEGER lSrcFileSize = { 0 };
	BOOL bRet = GetFileSizeEx(hSRcFile, &lSrcFileSize);

	if (!bRet)
	{
		cout << "[Client]: ��ȡ�����ļ����ֽڴ�Сʧ��" << endl;
		// �ر��ļ�
		CloseHandle(hSRcFile);
		return;
	}

	// �����ļ���С
	PACKETHEAD SendFileSizeph;
	SendFileSizeph.m_nCmd = PK_GET_FILE_SIZE; // ������������������ļ������ֽڴ�С
	SendFileSizeph.m_nLen = 0;

	m_cs.EnterCS();
	if (!m_socket.Send((char *)&SendFileSizeph, sizeof(SendFileSizeph)))
	{
		cout << "[Client]: ������������ļ��ֽڴ�Сʧ��" << endl;
		return;
	}
	// �����������ļ������ֽڴ�С
	if (!m_socket.Send((char *)&lSrcFileSize, sizeof(lSrcFileSize)))
	{
		cout << "[Client]: ������������ļ��ֽڴ�Сʧ��" << endl;
		return;
	}
	m_cs.LeaveCS();

	// ѭ�������ļ�����
	const int nCopySize = 0x10000;
	while (true)
	{
		char szBuff[nCopySize] = { 0 };
		// �洢��ȡ�����ļ��ֽ���
		DWORD dwBytesToRead = 0;
		Sleep(200);
		// ��ȡ�ļ�����
		bRet = ReadFile(hSRcFile, szBuff, nCopySize, &dwBytesToRead, NULL);

		if (!bRet)
		{
			cout << "[Client]: ��ȡ����ʧ��" << endl;
			CloseHandle(hSRcFile);
			return;
		}

		// ���Ͷ�ȡ�����ļ�����
		PACKETHEAD DataHead;
		DataHead.m_nCmd = PK_GET_FILE_DATA;
		DataHead.m_nLen = dwBytesToRead;

		m_cs.EnterCS();
		// ����ͷ
		if (!m_socket.Send((char *)&DataHead, sizeof(DataHead)))
		{
			cout << "[Client]: ������������ļ��ֽڴ�Сʧ��" << endl;
			return;
		}

		// ���Ͷ�ȡ�����ļ�����
		if (!m_socket.Send(szBuff, dwBytesToRead))
		{
			cout << "[Client]: ������������ļ��ֽڴ�Сʧ��" << endl;
			return;
		}
		m_cs.LeaveCS();

		//cout << "[Client]: ������������ļ��ֽ�����" << dwBytesToRead << endl;
		// ��¼ÿ�η��͵������ļ����ݳ���
		g_dwFileSize += dwBytesToRead;

		// ��ȡ���ļ�β -- ���ڶ�ȡ��һ������
		if (dwBytesToRead < nCopySize)
		{
			break;
		}

		// �ȴ��������������ݺ󣬷�����һ������
		PACKETHEAD HeadNext;
		if (!m_socket.Recv((char *)&HeadNext, sizeof(HeadNext)))
		{
			cout << "[Client]: ��������ʧ��" << endl;
			return;
		}

		if (HeadNext.m_nCmd != PK_GET_FILE_NEXT_DATA)
		{
			return;
		}
	}
	// �ļ����ݷ�����ϣ��ر��ļ����
	CloseHandle(hSRcFile);

	cout << "[Client]: ������������ļ����ֽ�����" << g_dwFileSize << endl;

	//	�ļ����سɹ�����������������Ϣ
	PACKETHEAD Sendph;
	Sendph.m_nCmd = PK_DOWN_FILE_SUCC;
	Sendph.m_nLen = 0;

	m_cs.EnterCS();
	if (!m_socket.Send((char *)&Sendph, sizeof(Sendph)))
	{
		cout << "[Client]: ������������ļ��ֽڴ�Сʧ��" << endl;
		return;
	}
	m_cs.LeaveCS();
}

// �ϴ��ļ�
void CClientWorks::ParserUploadFile(PACKETHEAD ph)
{
	char* pUpFilePath = new char[ph.m_nLen];
	ZeroMemory(pUpFilePath, ph.m_nLen);

	// ���շ��������͵��ļ��ϴ�·��
	if (!m_socket.Recv(pUpFilePath, ph.m_nLen))
	{
		cout << "[Client]: ��ȡ�ϴ��ļ�·��ʧ��" << endl;
		delete[] pUpFilePath;
		return;
	}

	// ����Ŀ���ļ�
	HANDLE hDstFile = CreateFile(pUpFilePath,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hDstFile)
	{
		cout << "[Client]: �ϴ��ļ�ʱ��������·��ʧ��" << endl;
		delete[] pUpFilePath;
		return;
	}

	LARGE_INTEGER lSrcFileSize = { 0 };
	BOOL bRet = GetFileSizeEx(hDstFile, &lSrcFileSize);

	m_strUploadFilePath = pUpFilePath;
	CloseHandle(hDstFile);

	delete[] pUpFilePath;

	// ��Ҫ��һ������
	PACKETHEAD Getph;
	Getph.m_nCmd = PK_GET_FILE_NEXT_DATA;
	Getph.m_nLen = 0;

	m_cs.EnterCS();
	if (!m_socket.Send((char *)&Getph, sizeof(Getph)))
	{
		cout << "[Client]: ������������ļ��ֽڴ�Сʧ��" << endl;
		return;
	}

	// �����ļ���С
	if (!m_socket.Send((char *)&lSrcFileSize, sizeof(LARGE_INTEGER)))
	{
		cout << "[Client]: ������������ļ��ֽڴ�Сʧ��" << endl;
		return;
	}
	m_cs.LeaveCS();
}

// ��ȡ�ϴ��ļ�������
void CClientWorks::ParserGetFileData(PACKETHEAD ph)
{
	// ����洢�ռ�
	char* pBuff = new char[ph.m_nLen];
	ZeroMemory(pBuff, ph.m_nLen);

	int nTotal = ph.m_nLen;
	int nCurSize = 0;

	// �����ļ�����
	while (nCurSize != nTotal)
	{
		int nRetByte = m_socket.Recv(pBuff + nCurSize, nTotal - nCurSize);
		if (nRetByte <= 0)
		{
			return;
		}
		nCurSize += nRetByte;
	}

	HANDLE hDstFile = CreateFile(m_strUploadFilePath.c_str(),
		GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	// �����ļ�ָ�뵽β��
	SetFilePointer(hDstFile, 0, 0, FILE_END);

	// д���ļ�����
	DWORD dwWriteSize = 0;
	WriteFile(hDstFile, pBuff, ph.m_nLen, &dwWriteSize, NULL);

	// ��ȡ�ļ���С
	LARGE_INTEGER lSrcFileSize = { 0 };
	BOOL bRet = GetFileSizeEx(hDstFile, &lSrcFileSize);

	CloseHandle(hDstFile);
	delete[] pBuff;

	// ��ȡ�ļ�����һ��������
	PACKETHEAD Getph;
	Getph.m_nCmd = PK_GET_FILE_NEXT_DATA;
	Getph.m_nLen = 0;

	m_cs.EnterCS();
	// ���������������
	if (!m_socket.Send((char *)&Getph, sizeof(Getph)))
	{
		cout << "[Client]: ���������������ʧ��" << endl;
		return;
	}

	// �����ļ��ֽڴ�С
	if (!m_socket.Send((char *)&lSrcFileSize, sizeof(LARGE_INTEGER)))
	{
		cout << "[Client]: ���������������ʧ��" << endl;
		return;
	}
	m_cs.LeaveCS();
}

// �ƶ��ͻ�����굽ָ��λ��
void CClientWorks::ParserSetClientMousePoint(PACKETHEAD head)
{
	char* pBuff = new char[head.m_nLen];
	ZeroMemory(pBuff, head.m_nLen);

	// ���շ��������͵�����
	if (!m_socket.Recv(pBuff, head.m_nLen))
	{
		cout << "[Client]: ��ȡ�������ʧ��" << endl;
		delete[] pBuff;
		return;
	}
	int nIdx;
	string str;
	str = pBuff;
	nIdx = str.find("-");

	// ��������
	char szBuffX[20] = { 0 };
	char szBuffY[20] = { 0 };
	memcpy(szBuffX, pBuff, nIdx);
	memcpy(szBuffY, pBuff + nIdx+ 1, strlen(pBuff));

	int nMouseX = atoi(szBuffX);
	int nMouseY = atoi(szBuffY);
	// ���ͻ�������ƶ���ָ��λ��
	BOOL bRet = ::SetCursorPos(nMouseX, nMouseY);
	if (bRet)
	{
		// ���������µ���
		::mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, nMouseX, nMouseY, 0, 0);
		Sleep(20);
		::mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, nMouseX, nMouseY, 0, 0);
	}
	return;
}

// ����������
void CClientWorks::ParserMouseButtonDown(PACKETHEAD head)
{
	char* pBuff = new char[head.m_nLen];
	ZeroMemory(pBuff, head.m_nLen);

	// ���շ��������͵�����
	if (!m_socket.Recv(pBuff, head.m_nLen))
	{
		cout << "[Client]: ��ȡ�������ʧ��" << endl;
		delete[] pBuff;
		return;
	}
	int nIdx;
	string str;
	str = pBuff;
	nIdx = str.find("-");

	// ��������
	char szBuffX[20] = { 0 };
	char szBuffY[20] = { 0 };
	memcpy(szBuffX, pBuff, nIdx);
	memcpy(szBuffY, pBuff + nIdx + 1, strlen(pBuff));

	int nMouseX = atoi(szBuffX);
	int nMouseY = atoi(szBuffY);
	// ���ͻ�������ƶ���ָ��λ��
	BOOL bRet = ::SetCursorPos(nMouseX, nMouseY);
	if (bRet)
	{
		// ���������� -- ���µ���
		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	}
	return;
}

// ��������
void CClientWorks::ParserKeyBoardInput(PACKETHEAD head)
{
	char* pBuff = new char[head.m_nLen];
	ZeroMemory(pBuff, head.m_nLen);

	// ���շ��������͵�����
	if (!m_socket.Recv(pBuff, head.m_nLen))
	{
		cout << "[Client]: ��ȡ�ϴ��ļ�·��ʧ��" << endl;
		delete[] pBuff;
		return;
	}
	// �������ּ�
	char szBuff[2] = { 0 };
	memcpy(szBuff, pBuff, strlen(pBuff));
	if (szBuff[0] >= '0' && szBuff[0] <= '9')
	{
		keybd_event(szBuff[0], 0, 0, 0);
		keybd_event(szBuff[0], 0, KEYEVENTF_KEYUP, 0);
	}
	// ��ĸ��д��
	if (szBuff[0] >= 'A' && szBuff[0] <= 'Z')
	{
		keybd_event(szBuff[0], 0, 0, 0);
		keybd_event(szBuff[0], 0, KEYEVENTF_KEYUP, 0);
	}
	// ��ĸСд��
	if (szBuff[0] >= 'a' && szBuff[0] <= 'z')
	{
		keybd_event(szBuff[0], 0, 0, 0);
		keybd_event(szBuff[0], 0, KEYEVENTF_KEYUP, 0);
	}
}

// ƥ�䵽CMD����
void CClientWorks::ParserStartCmd()
{
	BOOL bRet = FALSE;

	// ��ȫ����
	SECURITY_ATTRIBUTES sa = { 0 };
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	// �������ܵ�
	bRet = CreatePipe(&m_hReadOfParent, &m_hWriteOfChild, &sa, 0);
	if (!bRet)
	{
		cout << "[Client]: �����ܵ�ʧ��" << endl;
		return;
	}

	// �����ӹܵ�
	bRet = CreatePipe(&m_hReadOfChild, &m_hWriteOfParent, &sa, 0);
	if (!bRet)
	{
		cout << "[Client]: �����ܵ�ʧ��" << endl;
		return;
	}

	// �򿪿ͻ���CMD����
	STARTUPINFO si = { 0 };
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = m_hReadOfChild;
	si.hStdOutput = m_hWriteOfChild;
	si.hStdError = m_hWriteOfChild;

	_PROCESS_INFORMATION pi = { 0 };

	// ����CMD����
	bRet = CreateProcess(NULL,
		"cmd.exe",
		NULL,
		NULL,
		TRUE,
		CREATE_NO_WINDOW,
		NULL,
		NULL,
		&si,
		&pi);

	if (!bRet)
	{
		OutputDebugString("��������ʧ�ܣ�");
		return;
	}

	// �����߳�
	HANDLE hThread = CreateThread(NULL, 0, CmdWorkThread, this, 0, NULL);

	if (hThread == NULL)
	{
		cout << "[Client]: ����cmd�߳�ʧ��" << endl;
		return;
	}
	cout << "[Client]: cmd �����ɹ�" << endl;

	// �رվ��
	CloseHandle(hThread);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

// �����ܵ���������
DWORD WINAPI CClientWorks::CmdWorkThread(LPVOID lpParameter)
{
	CClientWorks* pThis = (CClientWorks *)lpParameter;

	pThis->m_bCmdRet = true;

	while (pThis->m_bCmdRet)
	{
		DWORD dwAail = 0; // �ܵ���ʣ����ֽ���
		BOOL bRet = PeekNamedPipe(pThis->m_hReadOfParent, NULL, 0, NULL, &dwAail, NULL);

		if (dwAail > 0)
		{
			// ����ռ�
			char* pBuff = new char[dwAail + 1];
			memset(pBuff, 0, dwAail + 1);

			// �ӹܵ��ж�ȡ����
			ReadFile(pThis->m_hReadOfParent, pBuff, dwAail, &dwAail, NULL);

			// ����ȡ�����ݷ��͵��ͻ���

			// �ȷ��Ͱ�ͷ
			PACKETHEAD ph;
			ph.m_nCmd = PK_OUTPUT_CMD_DATA;
			ph.m_nLen = dwAail + 1;

			pThis->m_cs.EnterCS();
			if (!pThis->m_socket.Send((char *)&ph, sizeof(ph)))
			{
				cout << "[Client]: ���������������ʧ��" << endl;
				delete[] pBuff;
				return 0;
			}


			if (!pThis->m_socket.Send(pBuff, dwAail + 1))
			{
				cout << "[Client]: ���������������ʧ��" << endl;
				delete[] pBuff;
				return 0;
			}
			pThis->m_cs.LeaveCS();

			// �ͷŻ�����
			delete[] pBuff;
		}
	}

	pThis->m_hReadOfChild = NULL;
	pThis->m_hWriteOfChild = NULL;
	pThis->m_hReadOfParent = NULL;
	pThis->m_hWriteOfParent = NULL;

	return 0;
}

// ƥ�䵽CMD����
void CClientWorks::ParserCmdOrder(PACKETHEAD ph)
{
	// ���뻺����
	char* pBuff = new char[ph.m_nLen];
	ZeroMemory(pBuff, ph.m_nLen);
	// ��������
	int nRet = m_socket.Recv(pBuff, ph.m_nLen);
	if (nRet <= 0)
	{
		cout << "[Client]: ��ȡ����������cmd����ʧ��" << endl;
		delete[] pBuff;
		return;
	}

	// ������д��ܵ���
	DWORD dwWriteBytes = 0;
	WriteFile(m_hWriteOfParent, pBuff, ph.m_nLen, &dwWriteBytes, NULL);
	// �ͷŻ�����
	delete[] pBuff;
}