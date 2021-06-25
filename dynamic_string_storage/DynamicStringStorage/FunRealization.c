#define _CRT_SECURE_NO_DEPRECATE
#include "FunRealization.h"

// ָ������Ĵ�С�ռ�Ϊ��1024 * 1024 �ֽ�
char g_szBuf[1024 * 1024] = { 0 };

// ��¼�ڴ��д洢���ַ�������
int g_nCount = 0;

/*
�������ܣ���������
��������
����ֵ��return 0 �����˳��ɹ�
*/
int StartProgram()
{
	int nNumber = 0; // ��¼�û����������
	int nStatus = 0; // ��¼scanf()�ķ���ֵ
	while (1)
	{
		system("cls");
		ProBeginMenu();	// ��ʾ���ܲ˵�
		rewind(stdin);
		nStatus = scanf("%d", &nNumber); // ���������ַ���1����֮����0
		// ����scanf����ֵ���ж�������������ֵ����һ����������ʾ��Ӧ�Ĵ�����Ϣ
		if (nStatus == 0)
		{
			printf("�����������󣬳��򼴽��˳�...\r\n");
			Sleep(2000);
			break;
		}
		else
		{
			// �û�������ֵΪ 0�������˳�
			if (nNumber == 0)
			{
				printf("���������˳�...\r\n");
				Sleep(2000);
				break;
			}
			// �û��������ֵ��Ҫ��0~8�����ڣ��������ֵ���������䣬ϵͳ�˳�
			if (nNumber < 0 || nNumber > 8)
			{
				printf("�����������󣬳��򼴽��˳�...\r\n");
				Sleep(2000);
				break;
			}
			else
			{
				system("cls");
				switch (nNumber)
				{
				case 1:
					// ����ַ���
					IncreaseString();
					break;
				case 2:
					// ɾ��ָ�����ַ���
					DeleteString();
					break;
				case 3:
					// �޸�ָ�����ַ���
					ReviseString();
					break;
				case 4:
					// ��ѯ�ַ���
					InquireString();
					break;
				case 5:
					// ��ʾ�ڴ������е��ַ���
					ShowString();
					break;
				case 6:
					// ��ʾ�ڴ����ַ������ֵĴ���������
					ShowStringRatio();
					break;
				case 7:
					// ��ʾ�ڴ�ʹ�÷ֲ�
					ShowMemoryFootprint();
					break;
				case 8:
					// ��Ƭ����
					Defragmentation();
					// ��ʾ�ڴ�ʹ�÷ֲ�
					ShowMemoryFootprint();
					break;
				default:
					printf("�������벻�ԣ�����������\r\n");
					break;
				}
			}
			system("pause");
		}
	}
	return 0;
}
/*
�������ܣ�����ʼ�˵�
����ֵ����
��������
*/
void ProBeginMenu()
{
	printf("\r\n\n");
	printf("\t******************  �ַ����洢����  ******************\n");
	printf("\t*\t0.�˳�����\r\n");
	printf("\t*\t1.����ַ���\r\n");
	printf("\t*\t2.ɾ���ַ���\r\n");
	printf("\t*\t3.�޸��ַ���\r\n");
	printf("\t*\t4.��ѯ�ַ���\r\n");
	printf("\t*\t5.��ʾ�ڴ��д洢�������ַ���\r\n");
	printf("\t*\t6.ͳ��ϵͳ�ڸ��ַ����ִ���������\r\n");
	printf("\t*\t7.��ʾ�ڴ�ռ�����\r\n");
	printf("\t*\t8.��Ƭ����\r\n");
	printf("\t******************************************************\n\n");
	printf("\r\n");
	printf("������ 0~8 ������Ӧ���ܵĲ�����");
}

/*
�ú����Ĺ���Ϊ���ַ�����ӹ���
����ֵ��
	return 0 �ַ�����ӳɹ�
	return -1�ַ������ʧ��
������	��
*/
int IncreaseString()
{
	int i = 0;
	int nCount = 1;	// Ϊ�ַ�����ӱ��
	char szTempString[100] = { 0 };	// �����û��������ʱ����
	printf("��������Ҫ�洢���ַ�����");
	rewind(stdin);
	fgets(szTempString, 99, stdin);
	// �����ַ�������� '\n'
	if (szTempString[strlen(szTempString) - 1] == '\n')
	{
		szTempString[strlen(szTempString) - 1] = '\0';
	}
	for (i = 0; i < sizeof(g_szBuf); i += (BLOCK_SIZE + 1))
	{
		// �жϵ�ǰ�ռ��Ƿ�ʹ��
		if (g_szBuf[i] == UNUSED_BLOCKSPACE)
		{
			// ���ַ����洢���ڴ��к�����ǣ����˿�ռ���Ϊ��ʹ��
			g_szBuf[i] = USED_BLOCKSPACE;

			if (g_szBuf[i + 1] == g_nCount)
			{
				// �жϵ�ǰ��ı��λ���Ƿ���ڴ����Ѿ������ַ������ܱ���Ƿ����
				g_nCount = g_nCount + 1;
				g_szBuf[i + 1] = g_nCount;
			}
			if (g_szBuf[i + 1] < g_nCount)
			{
				// �����ǰ��ı��λ��С���ܱ�ţ���˵����λ���ϵ��ַ����ѱ�ɾ��
				g_nCount = nCount;
				g_szBuf[i + 1] = g_nCount;
			}
			strcpy(&g_szBuf[i + 2], szTempString); // ��������ַ���������ָ�����ڴ����ȥ
			printf("�ַ��� %s ��ӳɹ�\r\n", &g_szBuf[i + 2]);
			return 0;
		}
		nCount = nCount + 1; // ѭ��һ�Σ�������һ
	}
	printf("�ַ��� %s ���ʧ��\r\n", szTempString);
	return -1;
}

/*
�ú����Ĺ���Ϊ���ַ���ɾ������
����ֵ��
	return 0 �ַ���ɾ���ɹ�
	return -1 �ַ���ɾ��ʧ��
��������
*/
int DeleteString()
{
	int i = 0;
	char szTempString[100] = { 0 };	// �����û��������ʱ����
	ShowString(); // ��ʾ�ڴ����Ѵ��ڵ��ַ���
	printf("��������Ҫɾ�����ַ�����");
	rewind(stdin);
	fgets(szTempString, 99, stdin);
	// �����ַ�������� '\n'
	if (szTempString[strlen(szTempString) - 1] == '\n')
	{
		szTempString[strlen(szTempString) - 1] = '\0';
	}
	for (i = 0; i < sizeof(g_szBuf); i += (BLOCK_SIZE + 1))
	{
		// �жϵ�ǰ�ռ��Ƿ�ʹ��
		if (g_szBuf[i] == USED_BLOCKSPACE)
		{
			// �ж�Ҫɾ�����ַ������ڴ��д洢�������ַ������Ƚϣ��ж��Ƿ����
			if (strcmp(&g_szBuf[i + 2], szTempString) == 0)
			{
				// �ж����������󣬽��˿�ռ���Ϊδʹ��
				g_szBuf[i] = UNUSED_BLOCKSPACE;
				// �����ڴ���м�¼�ַ�����ŵ�ֵ��0
				g_szBuf[i + 1] = 0;
				printf("������ѯ����Ҫɾ�����ַ��� %s �ڸ��ڴ���\r\n", &g_szBuf[i + 2]);
				printf("�ַ��� %s ����ɾ����...\r\n", &g_szBuf[i + 2]);
				Sleep(2000);
				printf("�ַ��� %s ɾ���ɹ�\r\n", &g_szBuf[i + 2]);
				return 0;
			}
		}
	}
	printf("ɾ��ʧ�ܣ�������ѯ���ַ��� %s ���ڸ��ڴ���\r\n", szTempString);
	return -1;
}

/*
�ú����Ĺ���Ϊ���ַ����޸Ĺ���
����ֵ��
	return 0  �ַ����޸ĳɹ�
	return -1 �ַ����޸�ʧ��
��������
*/
int ReviseString()
{
	int i = 0;
	char szTempString[100] = { 0 };	// �����û��������ʱ����
	ShowString(); // ��ʾ�ַ����˵�
	printf("��������Ҫ�޸ĵ��ַ�����");
	rewind(stdin);
	fgets(szTempString, 99, stdin);
	// �����ַ��������'\n'
	if (szTempString[strlen(szTempString) - 1] == '\n')
	{
		szTempString[strlen(szTempString) - 1] = '\0';
	}
	for (i = 0; i < sizeof(g_szBuf); i += (BLOCK_SIZE + 1))
	{
		// �����ڴ�飬�ж��ڴ���ʹ�����
		if (g_szBuf[i] == USED_BLOCKSPACE)
		{
			/*
			�����⵽��ǰ��ʹ�ã��ͽ�������ַ������ڴ���д洢���ַ������жԱ�
			�����ȣ�ִ�п��ڵ����
			*/
			if (strcmp(&g_szBuf[i + 2], szTempString) == 0)
			{
				printf("������ѯ����Ҫ�޸ĵ��ַ����ڸ��ڴ��У���������Ҫ�޸ĺ�����ݣ�");
				rewind(stdin);
				fgets(szTempString, 99, stdin);	// ����Ҫ�޸ĺ���ַ���
				// ͬ�������ַ�������� '\n'
				if (szTempString[strlen(szTempString) - 1] == '\n')
				{
					szTempString[strlen(szTempString) - 1] = '\0';
				}
				// �����ĺ���ַ���ȫ�����ݸ��ǵ���ǰ����ڴ���ȥ
				strcpy(&g_szBuf[i + 2], szTempString);
				printf("�ַ����޸���...\r\n");
				Sleep(2000);
				printf("�ַ����޸ĳɹ����޸ĺ���ַ���Ϊ��%s\r\n", &g_szBuf[i + 2]);
				return 0;
			}
		}
	}
	printf("����ʧ�ܣ�������ѯ����Ҫ�޸ĵ��ַ������ڸ��ڴ���\r\n");
	return -1;
}

/*
�ú����Ĺ���Ϊ���ַ�����ѯ�˵�
����ֵ����
��������
*/
void InquireMenu()
{
	ShowString();	// ��ʾ�ڴ��д洢���ַ�����Ϣ
	printf("\r\n\n");
	printf("*********************  �����Ӧ�ı�Ž�����Ӧ�Ĳ���  *********************\n");
	printf("*\t0.�˳���ѯ\r\n");
	printf("*\t1.��׼��ѯ\r\n");
	printf("*\t2.ģ����ѯ\r\n");
	printf("**************************************************************************\n");
}

/*
�ú����Ĺ���Ϊ����ѯϵͳ�ڵ��ַ�������������������֮��ʾ�ڴ��в�����
����ֵ��
	return 0 ���������˳�
������	��
*/
int InquireString()
{
	int nInNumber = 0; // ��¼�û��������ֵ
	int nRetValue = 0; // ��¼scanf�����ķ���ֵ
	while (1)
	{
		system("cls");
		InquireMenu(); // ��ʾ���ܲ˵�
		printf("������ 0~2 ������Ӧ���ܵĲ�����");
		rewind(stdin);
		nRetValue = scanf("%d", &nInNumber);
		// ����scanf����ֵ����Ӧ���ж�
		if (nRetValue == 0)
		{
			printf("��������������󣬲�ѯ���򼴽��˳�...\r\n");
			Sleep(2000);
			break;
		}
		else
		{
			// ����0�˳�����
			if (nInNumber == 0)
			{
				printf("��ѯ���򼴽��˳�...\r\n");
				Sleep(2000);
				break;
			}
			// ������ֵ��Ҫ��0~2����
			if (nInNumber < 0 || nInNumber > 2)
			{
				printf("���������Ϣ������������ȷ�ı��\r\n");
				break;
			}
			else
			{
				switch (nInNumber)
				{
				case 1:
					// ��ȷ����
					StrPerciseInquire();
					break;
				case 2:
					// ģ����ѯ
					StrFuzzyInquire();
					break;
				default:
					printf("���������Ϣ���ԣ�����������\r\n");
					break;
				}
			}
			system("pause");
		}
	}
	return 0;
}

/*
�ú����Ĺ���Ϊ����ѯ�ڴ����Ƿ������Ҫ��ѯ���ַ����������Ǿ�ȷ��ѯ��
	������Ҫ�û�����Ҫ��ѯ���ַ���ȫ���ַ�
����ֵ��
	return 0 ��ѯ�ɹ�
��������
*/
int StrPerciseInquire()
{
	int i = 0;
	char szTempString[100] = { 0 };	// �����û�������ַ������������ֵ�ǿ�����֮��õ�
	printf("��������Ҫ��ѯ���ַ�����");
	rewind(stdin);
	fgets(szTempString, 99, stdin);
	// ���������ַ�����β�Ļ��з�
	if (szTempString[strlen(szTempString) - 1] == '\n')
	{
		szTempString[strlen(szTempString) - 1] = '\0';
	}
	printf("ͨ����ȷ��ѯ�����ڴ����ҵ�����Ҫ��ѯ���ַ����У�\r\n");
	for (i = 0; i < sizeof(g_szBuf); i += (BLOCK_SIZE + 1))
	{
		if (g_szBuf[i] == USED_BLOCKSPACE)
		{
			// �жϵ�ǰ�ڴ���е����ݺ��û�������ַ����Ƿ���ȣ���Ⱦ������Ӧ������
			if (strcmp(&g_szBuf[i + 2], szTempString) == 0)
			{
				printf("%d: %s\r\n", g_szBuf[i + 1], &g_szBuf[i + 2]);
			}
		}
	}
	return 0;
}

/*
�ú����Ĺ���Ϊ��ģ����ѯ��������ڴ��к����û�����ָ���ַ����ַ���
����ֵ��
	return 0 ��ѯ�ɹ�
��������
*/
int StrFuzzyInquire()
{
	int i = 0;
	char* strPosition = NULL; // ���ڼ�¼��Ӧ���ַ����ַ����г��ֵ�λ��
	char szTempString[100] = { 0 }; // �����û�������ַ������������ֵʱ������֮��õ�
	printf("��������Ҫ��ѯ���ַ�����");
	rewind(stdin);
	fgets(szTempString, 99, stdin);
	// ���������ַ�����β�Ļ��з�
	if (szTempString[strlen(szTempString) - 1] == '\n')
	{
		szTempString[strlen(szTempString) - 1] = '\0';
	}
	printf("ͨ��ģ����ѯ���ڴ��д��� �ַ�/�ַ��� %s ���ַ��������У�\r\n", szTempString);
	for (i = 0; i < sizeof(g_szBuf); i += (BLOCK_SIZE + 1))
	{
		// �жϵ�ǰ�ڴ���Ƿ�ʹ��
		if (g_szBuf[i] == USED_BLOCKSPACE)
		{
			// ��¼����ַ����ڴ���г��ֵ�λ�ã������������ַ��������ݽ������
			strPosition = strstr(&g_szBuf[i + 2], szTempString);
			if (strPosition != NULL)
			{
				printf("%d: %s\r\n", g_szBuf[i + 1], &g_szBuf[i + 2]);
			}
		}
	}
	return 0;
}

/*
�ú����Ĺ���Ϊ����ʾ�ڴ����Ѿ��洢���ַ���
����ֵ����
��������
*/
void ShowString()
{
	int i = 0;
	printf("******************** ��ǰ�ڴ����ѱ���������ַ������� *******************\r\n");
	for (i = 0; i < sizeof(g_szBuf); i += (BLOCK_SIZE + 1))
	{
		// �����ڴ��д洢���������ݣ����������
		if (g_szBuf[i] == USED_BLOCKSPACE)
		{
			printf("%d: %s\r\n", g_szBuf[i + 1], &g_szBuf[i + 2]);
		}
	}
	printf("*************************************************************************\r\n");
}

/*
�ú����Ĺ���Ϊ��ͳ��������ַ����ڵ���ĸ���ִ�Сд�������ֳ��ֵĴ�������ռ�ܴ����ı���
����ֵ����
��������
*/
void ShowStringRatio()
{
	int i = 0;
	int j = 0;
	int nTotalCount = 0; // ��Сд��ĸ�����ֳ��ֵ��ܴ���
	int nLettersLarge[26] = { 0 }; // ���ڴ洢 26 ����д��ĸ
	int nLettersSmall[26] = { 0 }; // ���ڴ洢 26 ��Сд��ĸ
	int nNumbers[10] = { 0 };	// ���ڴ洢 10 ������
	int nLetsLarge = 0;	// �����ڴ��д洢��26����д��ĸ
	int nLetsSmall = 0;	// �����ڴ��д洢��26��Сд��ĸ
	int nNums = 0; // �����ڴ��д洢��10������

	for (i = 0; i < sizeof(g_szBuf); i += (BLOCK_SIZE + 1))
	{
		if (g_szBuf[i] == USED_BLOCKSPACE)
		{
			for (j = 0; j < (int)strlen(&g_szBuf[i + 1]); j++)
			{
				if (isupper((unsigned char)g_szBuf[i + j + 1]))
				{
					// ɨ���ڴ����������ݣ�����д��ĸ���浽nLettersLarge��
					nLettersLarge[g_szBuf[i + j + 1] - 'A']++;
					// �ܼ�����1
					nTotalCount++;
				}
				else if (isalpha((unsigned char)g_szBuf[i + j + 1]))
				{
					// ɨ���ڴ����������ݣ���Сд��ĸ���浽nLettersSmall��
					nLettersSmall[g_szBuf[i + j + 1] - 'a']++;	
					// �ܼ�����1
					nTotalCount++;
				}
				else if (isdigit((unsigned char)g_szBuf[i + j + 1]))
				{
					// ɨ���ڴ����������ݣ������ֱ��浽nNumbers��
					nNumbers[g_szBuf[i + j + 1] - '0']++;
					// �ܼ�����1
					nTotalCount++;	
				}
			}
		}
	}
	printf("******************* �ڴ��д洢���ַ������ֳ��ִ��������� *******************\r\n");
	printf("�ַ�������  ���ִ���  ռ�����ı���\r\n");

	// �����ڴ��и���д��ĸ���ֵĴ�������ռ���ʣ����ڴ��д�д��ĸ��������0�����
	for (nLetsLarge = 0; nLetsLarge <= 26; nLetsLarge++)
	{
		if (nLettersLarge[nLetsLarge] > 0)
		{
			printf("   %c��\t\t%d  \t%.2lf%%\r\n", 
				'A' + nLetsLarge, 
				nLettersLarge[nLetsLarge],
				((double)nLettersLarge[nLetsLarge] / nTotalCount) * 100);
		}
	}

	// �����ڴ��и�Сд��ĸ���ֵĴ�������ռ���ʣ����ڴ���Сд��ĸ��������0�����
	for (nLetsSmall = 0; nLetsSmall <= 26; nLetsSmall++)
	{
		if (nLettersSmall[nLetsSmall] > 0)
		{
			printf("   %c��\t\t%d  \t%.2lf%%\r\n", 
				'a' + nLetsSmall, 
				nLettersSmall[nLetsSmall],
				((double)nLettersSmall[nLetsSmall] / nTotalCount) * 100);
		}
	}

	// �����ڴ��и����ֳ��ֵĴ�������ռ���ʣ����ڴ���������������0�����
	for (nNums = 0; nNums <= 9; nNums++)
	{
		if (nNumbers[nNums] > 0)
		{
			printf("   %c��\t\t%d  \t%.2lf%%\r\n", 
				'0' + nNums, 
				nNumbers[nNums],
				((double)nNumbers[nNums] / nTotalCount) * 100);
		}
	}
	printf("****************************************************************************\r\n");
}

/*
�ú����Ĺ���Ϊ����ʾ�ڴ�ռ��ռ�������U(ʹ��)��F(δʹ��)
����ֵ����
��������
*/
void ShowMemoryFootprint()
{
	int i = 0;
	printf("\n\n************************* ��ǰ�ڴ����ڴ���ʹ��������� **************************\r\n");
	printf("*************************        U(ʹ��)��F(δʹ��)      **************************\r\n\n");

	for (i = 0; i < sizeof(g_szBuf); i += (BLOCK_SIZE + 1))
	{
		// �����ڴ棬�鿴�ڴ���Ƿ�ʹ��
		if (g_szBuf[i] == USED_BLOCKSPACE)
		{
			printf("U"); // ����ʹ�õ��ڴ�����ַ� U ��ʾ
		}
		else
		{
			printf("F"); // ��δʹ�õ��ڴ�����ַ� F ��ʾ
		}
	}
	printf("\n\n*****************************************************************************\r\n\n");
}

/*
�ú����Ĺ���Ϊ�����ڴ��еĿ�λ�ý�����Ƭ���������ڴ��д洢�����ݶ������±��
����ֵ����
��������
*/
void Defragmentation()
{
	int i = 0;
	int nNoBlock = 0; // ��¼δʹ�õĿ�
	int nCount = 1;	// ��¼ѭ������
	for (i = 0; i < sizeof(g_szBuf); i += (BLOCK_SIZE + 1))
	{
		// �����ڴ��е�ǰ���Ƿ�ʹ��
		if (g_szBuf[i] == USED_BLOCKSPACE)
		{
			// ����Ŀ�ʼ��Ǻ���ʱ��Ŀ�ʼ������Ƚ�
			if (i != nNoBlock)
			{
				/*
				�����ڴ��е����ݣ����ûʹ�ÿ�������ʹ�ù��Ŀ飬
				�ͽ�ʹ�õĿ���ڴ�����ȫ��������ǰ��ûʹ�õ��ڴ����
				*/
				memcpy(&g_szBuf[nNoBlock], &g_szBuf[i], (BLOCK_SIZE + 1));
				g_szBuf[i] = UNUSED_BLOCKSPACE;	// ���žͽ�����ڴ���Ϊδʹ��
			}
			nNoBlock = nNoBlock + (BLOCK_SIZE + 1);	// ����nTemp����ֵ
		}
	}
	// ���ڴ���ʣ�µ����ݽ������±��
	for (i = 0; i < sizeof(g_szBuf); i += (BLOCK_SIZE + 1))
	{
		// �жϵ�ǰ�ռ��Ƿ�ʹ��
		if (g_szBuf[i] == USED_BLOCKSPACE)
		{
			// ��ǰ��ı�ź��ڴ����Ѿ������ַ������ܱ����ȣ�˵����һ��û�����ݴ洢
			if (g_szBuf[i + 1] == g_nCount)
			{
				g_nCount = g_nCount + 1;
				g_szBuf[i + 1] = g_nCount;
			}
			// �����ǰ��ı��λ�ô��ڻ�����ܱ�ţ��ͽ�nCount��ֵ��ֵ����Ӧ�ı��λ����ȥ
			else
			{
				g_nCount = nCount;
				g_szBuf[i + 1] = g_nCount;
			}
		}
		// ��¼ѭ��������ѭ��һ�Σ�������һ
		nCount = nCount + 1;
	}
	printf("�ڴ���Ƭ����������...\r\n");
	Sleep(2000);
	printf("�ڴ���Ƭ����ɹ�\r\n");
}