#include "CClientWorks.h"
#include <iostream>
using namespace std;

// 判断输入的IP是否符合格式
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
	system("title 客户端");

	char szBuff[32] = { 0 };
	USHORT port = 0;

	cout <<"[Client]: 请输入服务器的 IP 地址：";
	cin >> szBuff;
	fflush(stdin);

	if (!ISInputIP(szBuff))
	{
		cout << "[Client]: 输入的服务器的 IP 地址不符合格式";
		Sleep(2000);
		return 0;
	}

	cout << "[Client]: 请输入服务器的端口号：";
	cin >> port;
	fflush(stdin);

	CClientWorks cw;
	cw.StartClient(szBuff, port);

	return 0;
}