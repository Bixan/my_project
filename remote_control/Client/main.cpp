#include "CClientWorks.h"
#include <iostream>
using namespace std;

// �ж������IP�Ƿ���ϸ�ʽ
BOOL ISInputIP(char* pBuff)
{
	int nCount = 0;
	for (size_t i = 0; i < strlen(pBuff); i++)
	{
		if (pBuff[i] == '.')
		{
			nCount++;
		}
		if (nCount == 3)
		{
			return TRUE;
		}
	}
	return FALSE;
}

int main(int argc, char* argv[])
{
	system("title �ͻ���");

	char szBuff[32] = { 0 };
	USHORT port = 0;

	cout <<"[Client]: ������������� IP ��ַ��";
	cin >> szBuff;
	fflush(stdin);

	if (!ISInputIP(szBuff))
	{
		cout << "[Client]: ����ķ������� IP ��ַ�����ϸ�ʽ";
		Sleep(2000);
		return 0;
	}

	cout << "[Client]: ������������Ķ˿ںţ�";
	cin >> port;
	fflush(stdin);

	CClientWorks cw;
	cw.StartClient(szBuff, port);

	return 0;
}