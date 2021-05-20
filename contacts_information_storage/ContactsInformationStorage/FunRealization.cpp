#include "FunRealization.h"

// �����ļ���С������ļ������Դ洢����λ��ϵ�˵���Ϣ
int g_nFileSize = (1024 * 1024 * 5) / (sizeof(TAGCONTACTS));
// �ļ����ַ���������
int g_nCharCount = 0;
// ��¼�ļ�����Ч�ĺ�������
int g_nChineseCharCount = 0;

/*
�������ܣ����г���
��������
����ֵ��return 0 ���������˳�
*/
int StartProgram()
{
	// FormattedData();
	int nNumber = 0;  // ��¼�û����������
	int nStatus = 0;	// ��¼scanf()�ķ���ֵ
	while (1)
	{
		system("cls");
		Menu(); // ��ʾ���ܲ˵�
		nStatus = scanf("%d", &nNumber);	// ���������ַ���1����֮����0
		rewind(stdin);
		// ����scanf����ֵ���ж�������������ֵ����һ����������ʾ��Ӧ�Ĵ�����Ϣ
		if (nStatus == 0)
		{
			printf("�����������󣬳��򼴽��˳�...\r\n");
			// Sleep(2000);
			break;
		}
		else
		{
			// �û�������ֵΪ 0�������˳�
			if (nNumber == 0)
			{
				printf("���������˳�...\r\n");
				// Sleep(2000);
				break;
			}
			// �û��������ֵ��Ҫ��0~8�����ڣ��������ֵ���������䣬ϵͳ�˳�
			if (nNumber < 0 || nNumber > 9)
			{
				printf("�����������󣬳��򼴽��˳�...\r\n");
				break;
			}
			else
			{
				switch (nNumber)
				{
				case 1:
					system("cls");
					// ����IncreaseDataģ�飬�����ϵ����Ϣ���ļ���ȥ
					IncreaseData();
					break;
				case 2:
					system("cls");
					// ����DeleteDataģ�飬����ɾ��ָ����ϵ����Ϣ����
					DeleteData();
					break;
				case 3:
					system("cls");
					// ����ReviseDataģ�飬�����޸�ָ����ϵ����Ϣ����
					ReviseData();
					break;
				case 4:
					system("cls");
					// ����InquireDataģ�飬��ѯ�ļ�����ϵ����Ϣ
					InquireData();
					break;
				case 5:
					system("cls");
					// ����ShowDataģ�飬���ļ��ڴ洢��������ϵ����Ϣ��ʾ������̨
					ShowData();
					break;
				case 6:
					system("cls");
					// ����ShowFileDataRatioģ�飬���ļ��ڴ洢��������Ϣ����ͳ��
					ShowFileDataRatio();
					break;
				case 7:
					system("cls");
					// ����ShowFileDataRatioģ�飬��ʾ�ļ��ڴ��ʹ�÷ֲ�
					ShowMemoryFootprint();
					break;
				case 8:
					system("cls");
					// ����Defragmentationģ�飬���ļ��ڵĿ���λ�ý�����Ƭ����
					Defragmentation();		 
					// �ٴε���ShowFileDataRatioģ�飬��ʾ�ļ��ڴ��ʹ�÷ֲ�
					ShowMemoryFootprint();
					break;
				case 9:
					// ��ʽ����Ƭ�������ļ�����
					InitializationData();
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
void Menu()
{
	printf("\r\n");
	printf("\t***** ��ϵ����Ϣ�洢ϵͳ *****\r\n\n");
	printf("\t0.�˳�����\r\n");
	printf("\t1.�����ϵ����Ϣ\r\n");
	printf("\t2.ɾ����ϵ����Ϣ\r\n");
	printf("\t3.�޸���ϵ����Ϣ\r\n");
	printf("\t4.��ѯ��ϵ����Ϣ\n");
	printf("\t5.��ʾ�ڴ��д洢��������ϵ������\r\n");
	printf("\t6.ͳ��ϵͳ�ڸ��ַ����ִ���������\r\n");
	printf("\t7.��ʾ�ڴ�ռ�����\r\n");
	printf("\t8.��Ƭ����\r\n");
	printf("\t9.��ʼ��������ϵ����Ϣ���ļ�\r\n");
	printf("\r\n");
	printf("������ 0~9 ������Ӧ���ܵĲ�����");
}

/*
�������ܣ���������ϵ����Ϣ���ļ���ʽ��
��������
����ֵ��return 0 �����˳�
*/
int InitializationData()
{
	FILE* fp = NULL;
	fp = fopen("data.bin", "wb+"); // ����ļ������ڣ��ͳ�ʼ��һ���ļ�
	if (fp == NULL)
	{
		// ����ļ���ʧ�ܣ��˳�����
		exit(-1);
	}

	// ���ļ�ָ��fp���ڸ��ļ�ָ����ļ���ʼ��
	if (fseek(fp, 0, SEEK_SET))
	{
		exit(ferror(fp));
	}
	// Ϊ�ṹ������ĳ�Ա����ֵ
	TAGCONTACTS perInfo = {
		unusedData, 0, '\0', '\0'
	};

	// ����������ʼ���������д�뵽�ļ���
	for (int i = 0; i < g_nFileSize; i++)
	{
		if (fwrite(&perInfo, sizeof(perInfo), 1, fp) != 1)
		{
			printf("����д��ʧ��\r\n");
			goto EXIT_SHDOFILE;
		}
	}
	printf("data.bin �ļ���ʼ��(��ʽ��)�ɹ�!\r\n");

EXIT_SHDOFILE:
	if (fp)
	{
		fclose(fp); // �ر��ļ�
		fp = NULL;
	}
	return 0;
}

/*
�������ܣ���ʽ���洢��ϵ���������ļ�
����ֵ����
��������
*/
void initializationChinese()
{
	FILE* fpChineseChar = NULL;

	// ��¼������ϵ�˵�������
	fpChineseChar = fopen("ChineseChar.bin", "wb+");

	// �ж��ļ� ChineseChar.bin �Ƿ�򿪳ɹ������Ƿ�Ϊ��
	if (fpChineseChar == NULL)
	{
		printf("�ļ���ʧ�ܣ������ļ��Ƿ����\r\n");
		exit(-1);
	}
	// ���ļ�ָ�� fpChinese ����ָ����ļ���ʼ��
	if (fseek(fpChineseChar, 0, SEEK_SET))
	{
		exit(ferror(fpChineseChar));
	}

	TAGTEMPNAME tempName = {
		'\0'
	};
	// ��ʼ������Ϊ5M
	for (int i = 0; i < g_nFileSize; i++)
	{
		// �����ĺ������д�뵽�ļ�
		if (fwrite(&tempName, sizeof(tempName), 1, fpChineseChar) != 1)
		{
			printf("����д��ʧ��\r\n");
			goto EXIT_SHDOFILE;
		}
	}
	// printf("ChineseChar.bin �ļ���ʽ���ɹ�!\r\n");
EXIT_SHDOFILE:
	if (fpChineseChar)
	{
		fclose(fpChineseChar);	// �ر��ļ�
		fpChineseChar = NULL;
	}
}

/*
�������ܣ���ָ�����ļ��ڴ洢��Ӧ����ϵ����Ϣ
��������
����ֵ��return 0 ���������˳�
*/
int IncreaseData()
{
	int nTempAge = 0;	// �����û��������ϵ������
	int nStatus = 0;	// ��¼scanf()�ķ���ֵ
	FILE* fp = NULL;

	fp = fopen("data.bin", "rb+");	// �ж��ļ��Ƿ���ڣ����Ƿ�򿪳ɹ�
	if (fp == NULL)
	{
		printf("�ļ���ʧ�ܣ������ļ��Ƿ���ڣ�\r\n");
		exit(-1);
	}

	// ���ļ�ָ��fp���ڸ��ļ�ָ����ļ���ʼ��
	if (fseek(fp, 0, SEEK_SET))
	{
		exit(ferror(fp));
	}
	// ����һ��������ϵ����Ϣ�Ľṹ�����
	TAGCONTACTS perInfo;

	for (int i = 0; i < g_nFileSize; i++)
	{
		if (fread(&perInfo, sizeof(perInfo), 1, fp) != 1)
		{
			printf("�ļ���ȡ����\r\n");
			goto EXIT_SHDOFILE;
		}
		// �жϵ�ǰ�����Ƿ�Ϊʹ��
		if (perInfo.enDataType == unusedData)
		{
			printf("��������Ҫ�洢��ϵ�˵�������");	// �û�����
			fgets(perInfo.szName, 32, stdin);
			rewind(stdin);
			// �����ַ�������� '\n'
			if (perInfo.szName[strlen(perInfo.szName) - 1] == '\n')
			{
				perInfo.szName[strlen(perInfo.szName) - 1] = '\0';
			}
			while (1)
			{
				printf("��������Ҫ�洢��ϵ�˵�����(0~150)��");
				nStatus = scanf("%d", &nTempAge);
				rewind(stdin);
				// ����scanf����ֵ���ж�������������ֵ����һ����������ʾ��Ӧ�Ĵ�����Ϣ
				if (nStatus == 0)
				{
					printf("����������������������\r\n");
				}
				else
				{
					// ���û���������Ϣ���Ƶ�0~150֮��
					if (nTempAge < 0 || nTempAge >150)
					{
						printf("����������䲻���Ϸ�Χ������������\r\n");
					}
					else
					{
						perInfo.nAge = nTempAge;
						break;
					}
				}
			}
			printf("��������Ҫ�洢��ϵ�˵ĵ绰���룺");
			fgets(perInfo.szTel, 12, stdin);
			rewind(stdin);
			// �����ַ�������� '\n'
			if (perInfo.szTel[strlen(perInfo.szTel) - 1] == '\n')
			{
				perInfo.szTel[strlen(perInfo.szTel) - 1] = '\0';
			}

			// ���������ڴ���Ϊ�Ѿ�ʹ��
			perInfo.enDataType = usedData;
			// ���ļ�ָ��ָ��ǰλ��
			if (fseek(fp, -(int)(sizeof(perInfo)), SEEK_CUR))
			{
				exit(ferror(fp));
			}
			// ������д�뵽�ļ���
			if (fwrite(&perInfo, sizeof(perInfo), 1, fp) != 1)
			{
				printf("����д��ʧ��\r\n");
				goto EXIT_SHDOFILE;
			}
			printf(perInfo.szName);
			printf(" ��ϵ����Ϣ��ӳɹ�!\r\n");

			break;
		}
	}

EXIT_SHDOFILE:
	if (fp)
	{
		fclose(fp);	// �ر��ļ�
		fp = NULL;
	}
	return 0;
}

/*
�������ܣ���ָ�����ļ���ɾ����Ӧ����ϵ����Ϣ
��������
����ֵ��return 0 ���������˳�
*/
int DeleteData()
{
	char szTempName[32] = { 0 };	// �����û�Ҫɾ����ϵ�˵�����
	char szTempTel[12] = { 0 };	// �����û�Ҫɾ����ϵ�˵��ֻ�����
	int nTempAge = 0;		// �����û�Ҫɾ����ϵ�˵�����
	char szTag[2] = { 0 };
	FILE* fp = NULL;
	fp = fopen("data.bin", "rb+");
	// �ж��ļ��Ƿ���ڣ����Ƿ�򿪳ɹ�
	if (fp == NULL)
	{
		printf("�ļ���ʧ�ܣ������ļ��Ƿ����\r\n");
		goto EXIT_SHDOFILE;
	}
	// ����һ��������ϵ����Ϣ�Ľṹ�����
	TAGCONTACTS perInfo;
	// ���ļ�ָ��fp���ڸ��ļ�ָ����ļ���ʼ��
	if (fseek(fp, 0, SEEK_SET))
	{
		exit(ferror(fp));
	}

	// ��ʾ�ļ����Ѿ����ڵ�����
	printf("��ǰ�ļ��ڱ����������ϵ���������£�\r\n\n");
	for (int i = 0; i < g_nFileSize; i++)
	{
		if (fread(&perInfo, sizeof(perInfo), 1, fp) != 1)
		{
			printf("�ļ���ȡ����\r\n");
			exit(ferror(fp));
		}

		if (perInfo.enDataType == usedData)
		{
			printf("������");
			printf(perInfo.szName);
			printf("\r\n");
			printf("���䣺%d\r\n", perInfo.nAge);
			printf("�ֻ��ţ�");
			printf(perInfo.szTel);
			printf("\r\n");
			printf("------------------------------------------------\r\n");
		}
	}
	// �ٴα��������ļ�ָ�������ļ���ʼ��
	if (fseek(fp, 0, SEEK_SET))
	{
		exit(ferror(fp));
	}

	printf("��������Ҫɾ������ϵ��������"); // �û�����Ҫɾ������ϵ������
	fgets(szTempName, 32, stdin);
	rewind(stdin);
	// �����ַ�������� '\n'
	if (szTempName[strlen(szTempName) - 1] == '\n')
	{
		szTempName[strlen(szTempName) - 1] = '\0';
	}
	printf("��������Ҫɾ������ϵ�˵����䣺");
	scanf("%d", &nTempAge);
	rewind(stdin);
	printf("��������Ҫɾ������ϵ�˵ĵ绰���룺");
	fgets(szTempTel, 12, stdin);
	rewind(stdin);
	// �����ַ�������� '\n'
	if (szTempTel[strlen(szTempTel) - 1] == '\n')
	{
		szTempTel[strlen(szTempTel) - 1] = '\0';
	}

	for (int i = 0; i < g_nFileSize; i++)
	{
		// �ж������Ƿ��ȡ�ɹ�
		if (fread(&perInfo, sizeof(perInfo), 1, fp) != 1)
		{
			printf("ɾ����ϵ������ʱ�����ݶ�ȡʧ��\r\n");
			goto EXIT_SHDOFILE;
		}
		// �жϵ�ǰ�����Ƿ�Ϊʹ��
		if (perInfo.enDataType == usedData)
		{
			// �жϵ�ǰ�ļ�������ϵ�˵���������Ҫɾ������ϵ����Ϣ��ͬ
			if ((perInfo.nAge == nTempAge) && 
				(strcmp(perInfo.szName, szTempName) == 0) && 
				(strcmp(perInfo.szTel, szTempTel) == 0))
			{
				printf("������ѯ����Ҫɾ������ϵ�����ļ��ڣ��Ƿ����ɾ��(Y/yȷ��ɾ����������ȡ��ɾ��)��");
				scanf("%1s", szTag);
				if ((strcmp(szTag, "Y") == 0) || (strcmp(szTag, "y") == 0))
				{
					perInfo.enDataType = unusedData;
					// �����ļ�ָ��ָ���λ��
					if (fseek(fp, -(int)sizeof(perInfo), SEEK_CUR))
					{
						exit(ferror(fp));
					}
					// �����º����ϵ����Ϣд�뵽�ļ�
					if (fwrite(&perInfo, sizeof(perInfo), 1, fp) != 1)
					{
						printf("����д��ʧ��\r\n");
						goto EXIT_SHDOFILE;
					}
					printf("��ϵ�� %s ����Ϣɾ���ɹ�!\r\n", perInfo.szName);
					goto EXIT_SHDOFILE;
				}
				else
				{
					printf("��ȡ��ɾ��\r\n");
					goto EXIT_SHDOFILE;
				}
			}
		}
	}
	printf("������ѯ���ļ���û�в�ѯ������ %s ����Ϣ!\r\n", szTempName);
EXIT_SHDOFILE:
	if (fp)
	{
		fclose(fp); // �ر��ļ�
		fp = NULL;
	}
	return 0;
}

/*
�������ܣ���ָ�����ļ����޸���Ӧ����ϵ����Ϣ
��������
����ֵ��return 0 ���������˳�
*/
int ReviseData()
{
	char szTempName[32] = { 0 };  // �����û�Ҫ�޸���ϵ�˵�����
	char szTempTel[12] = { 0 }; // �����û�Ҫ�޸���ϵ�˵��ֻ�����
	int nTempAge = 0; // �����û�Ҫ�޸���ϵ�˵�����
	char szTag[2] = { 0 };
	FILE* fp = NULL;
	fp = fopen("data.bin", "rb+");
	// �ж��ļ��Ƿ���ڣ����Ƿ�򿪳ɹ�
	if (fp == NULL)
	{
		printf("�ļ���ʧ��\r\n");
		exit(-1);
	}
	// ����һ��������ϵ����Ϣ�Ľṹ�����
	TAGCONTACTS perInfo;
	// ���ļ�ָ��fp���ڸ��ļ�ָ����ļ���ʼ��
	if (fseek(fp, 0, SEEK_SET))
	{
		exit(ferror(fp));
	}

	// ��ʾ�ļ����Ѿ����ڵ�����
	printf("��ǰ�ļ��ڱ����������ϵ���������£�\r\n\n");
	for (int i = 0; i < g_nFileSize; i++)
	{
		if (fread(&perInfo, sizeof(perInfo), 1, fp) != 1)
		{
			printf("�ļ���ȡ����\r\n");
			exit(ferror(fp));
		}
		// ��ʾ�Ѵ洢����ϵ����Ϣ
		if (perInfo.enDataType == usedData)
		{
			printf("������");
			printf(perInfo.szName);
			printf("\r\n");
			printf("���䣺%d\r\n", perInfo.nAge);
			printf("�ֻ��ţ�");
			printf(perInfo.szTel);
			printf("\r\n");
			printf("------------------------------------------------\r\n");
		}
	}

	// �ٴα���,���ļ�ָ��fp���ڸ��ļ�ָ����ļ���ʼ��
	if (fseek(fp, 0, SEEK_SET))
	{
		exit(ferror(fp));
	}

	printf("��������Ҫ�޸ĵ���ϵ��������");	// �û�����
	fgets(szTempName, 32, stdin);
	rewind(stdin);
	// �����ַ�������� '\n'
	if (szTempName[strlen(szTempName) - 1] == '\n')
	{
		szTempName[strlen(szTempName) - 1] = '\0';
	}

	printf("��������Ҫ�޸ĵ���ϵ�˵����䣺");
	scanf("%d", &nTempAge);
	rewind(stdin);

	printf("��������Ҫ�޸ĵ���ϵ�˵ĵ绰���룺");
	fgets(szTempTel, 12, stdin);
	rewind(stdin);
	// �����ַ�������� '\n'
	if (szTempTel[strlen(szTempTel) - 1] == '\n')
	{
		szTempTel[strlen(szTempTel) - 1] = '\0';
	}

	for (int i = 0; i < g_nFileSize; i++)
	{
		// �����ȡָ�����ļ�ʧ�ܣ���ָ��goto���ر��ļ�
		if (fread(&perInfo, sizeof(perInfo), 1, fp) == 0)
		{
			printf("�޸���ϵ������ʱ���ļ���ȡʧ��\r\n");
			goto EXIT_SHDOFILE;
		}
		if (perInfo.enDataType == usedData)
		{
			// �������ϵ���������ļ��ڴ��ڵ���ϵ�����������䡢�ֻ�������жԱ�
			if ((perInfo.nAge == nTempAge) && (strcmp(perInfo.szName, szTempName) == 0)
				&& (strcmp(perInfo.szTel, szTempTel) == 0))
			{
				printf("������ѯ����Ҫ�޸ĵ���ϵ�����ļ��ڣ��Ƿ�����޸�(Y/yȷ���޸ģ�������ȡ���޸�)��");
				scanf("%1s", szTag);
				rewind(stdin);
				if ((strcmp(szTag, "Y") == 0) || (strcmp(szTag, "y") == 0))
				{
					printf("���������޸ĺ���ϵ�˵�������");
					fgets(perInfo.szName, 32, stdin);
					rewind(stdin);
					// �����ַ�������� '\n'
					if (perInfo.szName[strlen(perInfo.szName) - 1] == '\n')
					{
						perInfo.szName[strlen(perInfo.szName) - 1] = '\0';
					}

					printf("�������޸ĺ���ϵ�˵����䣺");
					scanf("%d", &perInfo.nAge);
					rewind(stdin);

					printf("���������޸ĺ���ϵ�˵ĵ绰���룺");
					fgets(perInfo.szTel, 12, stdin);
					rewind(stdin);
					// �����ַ�������� '\n'
					if (perInfo.szTel[strlen(perInfo.szTel) - 1] == '\n')
					{
						perInfo.szTel[strlen(perInfo.szTel) - 1] = '\0';
					}

					// �����ļ�ָ��ָ���λ��
					if (fseek(fp, -(int)sizeof(perInfo), SEEK_CUR))
					{
						exit(ferror(fp));
					}
					// ���޸ĺ������д�뵽�ļ���
					if (fwrite(&perInfo, sizeof(perInfo), 1, fp) == 0)
					{
						printf("����д��ʧ��\r\n");
						goto EXIT_SHDOFILE;
					}
					printf("ԭ��ϵ�� %s ����Ϣ�޸ĳɹ�\r\n", szTempName);
					goto EXIT_SHDOFILE;
				}
				else
				{
					printf("����ȡ���޸�\r\n");
					goto EXIT_SHDOFILE;
				}
			}
		}
	}
	printf("������ѯ���ļ���û�в�ѯ������ %s ����Ϣ!\r\n", szTempName);

EXIT_SHDOFILE:
	if (fp)
	{
		fclose(fp); // �ر��ļ����˳�����
		fp = NULL;
	}
	return 0;
}

/*
�������ܣ���ʾ�ļ��ڵ�������Ч����ϵ������
��������
����ֵ��return 0 ���������˳�
*/
int ShowData()
{
	// ��ʽ���洢�ļ�����ϵ���������ļ�����ÿ�α����ļ��ڵ�����ʱ���ڽ���Ч����ϵ���������д洢
	initializationChinese();
	FILE* fp = NULL;
	FILE* fpChinese = NULL; // ָ�� ChineseChar.bin �ļ������ļ������� data.bin �ļ������Ч��ϵ�����ݵ�����

	fp = fopen("data.bin", "rb+");
	// �ж��ļ� data.bin �Ƿ�򿪳ɹ������Ƿ�Ϊ��
	if (fp == NULL)
	{
		printf("�ļ���ʧ�ܣ������ļ��Ƿ����\r\n");
		exit(-1);
	}

	// ��¼������ϵ�˵�������
	fpChinese = fopen("ChineseChar.bin", "rb+");
	if (fpChinese == NULL)	  // �ж��ļ� ChineseChar.bin �Ƿ�򿪳ɹ������Ƿ�Ϊ��
	{
		printf("�ļ���ʧ�ܣ������ļ��Ƿ����\r\n");
		exit(-1);
	}

	// Ϊ������ϵ����Ϣ�ṹ�嶨��һ��������
	TAGCONTACTS perInfo;

	// Ϊ�ļ�����Ч����ϵ�����ֽṹ�嶨��һ����������������ʱ�洢�ļ�����Ч����ϵ������
	TAGTEMPNAME tempName;

	// ���ļ�ָ��fp���ڸ��ļ�ָ����ļ���ʼ��
	if (fseek(fp, 0, SEEK_SET))
	{
		exit(ferror(fp));
	}

	// ���ļ�ָ�� fpChinese ����ָ����ļ���ʼ��
	if (fseek(fpChinese, 0, SEEK_SET))
	{
		exit(ferror(fpChinese));
	}

	// �����ļ�����Ч����ϵ����Ϣ
	printf("��ǰ�ļ��ڱ����������Ч��ϵ���������£�\r\n\n");
	for (int i = 0; i < g_nFileSize; i++)
	{
		if (fread(&perInfo, sizeof(perInfo), 1, fp) != 1)
		{
			printf("�ļ���ȡ���������ļ�����\r\n");
			exit(ferror(fp));
		}
		if (perInfo.enDataType == usedData)
		{
			printf("������");
			printf(perInfo.szName);
			printf("\r\n");
			printf("���䣺%d\r\n", perInfo.nAge);
			printf("�ֻ��ţ�");
			printf(perInfo.szTel);
			printf("\r\n");
			printf("------------------------------------------------\r\n");

			// ��fpChineseָ����ļ��ڶ�ȡһ��tempName�ֽڴ�С�����ݵ�tempName��
			if (fread(&tempName, sizeof(tempName), 1, fpChinese) != 1)
			{
				printf("�ļ���ȡ���������ļ�����\r\n");
				goto EXIT_SHDOFILE;
			}
			strcpy(tempName.szName, perInfo.szName);
			// ���ļ�ָ��fpChineseָ��ǰ�ļ�λ��
			if (fseek(fpChinese, -(int)(sizeof(tempName)), SEEK_CUR))
			{
				exit(ferror(fpChinese));
			}
			// ��copy������ϵ������д�뵽ָ�����ļ���ȥ
			if (fwrite(&tempName, sizeof(tempName), 1, fpChinese) != 1)
			{
				printf("��ϵ����Ϣд��ʧ��\r\n");
				goto EXIT_SHDOFILE;
			}
		}
	}
EXIT_SHDOFILE:
	if (fp)
	{
		fclose(fp);	// �ر��ļ�
		fp = NULL;
	}
	if (fpChinese)
	{
		fclose(fpChinese);	// �ر��ļ�
		fpChinese = NULL;
	}
	return 0;
}

/*
�������ܣ���ѯ��ϵ�����ݲ˵�
��������
����ֵ����
*/
void InquireMenu()
{
	// ��ʾ�ڴ��д洢���ַ�����Ϣ
	ShowData();

	printf("\r\n\n");
	printf("\t�����Ӧ�ı�Ž�����Ӧ�Ĳ��� \n");
	printf("\t0.�˳���ѯ\r\n");
	printf("\t1.��׼��ѯ\r\n");
	printf("\t2.ģ����ѯ\r\n\n");
}

/*
�������ܣ���ȷ��ѯ�ļ���ָ������Ч��ϵ��������Ϣ
��������
����ֵ��return 0 ���������˳�
*/
int PerciseQueryContactData()
{
	char szTempName[32] = { 0 };	// �����û��������ϵ�˵�����
	char szTempTel[12] = { 0 };	// �����û��������ϵ�˵��ֻ�����
	int nTempAge = 0;		// �����û��������ϵ�˵�����
	FILE* fp = NULL;
	fp = fopen("data.bin", "rb+");
	// �ж��ļ��Ƿ�򿪳ɹ�
	if (fp == NULL)
	{
		goto EXIT_SHDOFILE;
	}
	// Ϊ������ϵ����Ϣ�ṹ�嶨��һ��������
	TAGCONTACTS perInfo;

	// ���ļ�ָ���ƶ����ļ��Ŀ�ʼλ��
	if (fseek(fp, 0, SEEK_SET))
	{
		exit(ferror(fp));
	}
	// ��ȷ��ѯ���û���Ҫ�����Ӧ�����������䣬�绰����
	printf("��������Ҫ��ѯ����ϵ��������");
	fgets(szTempName, 32, stdin);
	rewind(stdin);
	// �����ַ�������� '\n'
	if (szTempName[strlen(szTempName) - 1] == '\n')
	{
		szTempName[strlen(szTempName) - 1] = '\0';
	}

	printf("��������Ҫ��ѯ����ϵ�˵����䣺");
	scanf("%d", &nTempAge);
	rewind(stdin);

	printf("��������Ҫ��ѯ����ϵ�˵ĵ绰���룺");
	fgets(szTempTel, 12, stdin);
	rewind(stdin);
	// �����ַ�������� '\n'
	if (szTempTel[strlen(szTempTel) - 1] == '\n')
	{
		szTempTel[strlen(szTempTel) - 1] = '\0';
	}

	printf("������ѯ�����ѯ����ϵ����Ϣ���ڴ��У���Ϣ���£�\r\n");
	for (int i = 0; i < g_nFileSize; i++)
	{
		// ���ļ��ж�ȡָ������
		if (fread(&perInfo, sizeof(perInfo), 1, fp) != 1)
		{
			printf("�ļ���ȡʧ��\r\n");
			goto EXIT_SHDOFILE;
		}

		// �жϵ�ǰ�����Ƿ�Ϊʹ��
		if (perInfo.enDataType == usedData)
		{
			// ���û���Ҫ��ѯ����ϵ�����ݺ��ļ��ڱ�������ݽ��жԱ�
			if ((perInfo.nAge == nTempAge) && (strcmp(perInfo.szName, szTempName) == 0)
				&& (strcmp(perInfo.szTel, szTempTel) == 0))
			{
				printf("������");	// �ڿ���̨��ʾ���ҵ���������Ϣ��û����ʾ����ʾ�ļ���û�ж�Ӧ������
				printf(perInfo.szName);
				printf("\t\t");
				printf("���䣺%d\t", perInfo.nAge);
				printf("�ֻ��ţ�");
				printf(perInfo.szTel);
				printf("\r\n");
			}
		}
	}

EXIT_SHDOFILE:
	if (fp)
	{
		fclose(fp); // �ر��ļ�
		fp = NULL;
	}
	return 0;
}

/*
�������ܣ�ģ����ѯ�ļ���ָ������Ч��ϵ��������Ϣ�������ļ��д洢����ϵ�������е��ַ�����
��������
����ֵ��return 0 ���������˳�
*/
int FuzzyInquireContactData()
{
	char szTempName[32] = { 0 };	// �����û��������ϵ������
	char szTempTel[12] = { 0 };
	char* strPosition = NULL;		// ���ڼ�¼��Ӧ���ַ����ַ����г��ֵ�λ��
	FILE* fp = NULL;
	fp = fopen("data.bin", "rb+");
	// �ж��ļ��Ƿ�򿪳ɹ�
	if (fp == NULL)
	{
		printf("ģ����ѯʱ���ļ���ʧ��\r\n");
		exit(-1);
	}
	// Ϊ������ϵ����Ϣ�ṹ�嶨��һ��������
	TAGCONTACTS perInfo;

	// ���ļ�ָ���ƶ����ļ��Ŀ�ʼλ��
	if (fseek(fp, 0, SEEK_SET))
	{
		exit(ferror(fp));
	}

	printf("��������Ҫģ����ѯ�İ�����ϵ�����������ݣ�");
	fgets(szTempName, 32, stdin);
	rewind(stdin);
	// �����ַ�������� '\n'
	if (szTempName[strlen(szTempName) - 1] == '\n')
	{
		szTempName[strlen(szTempName) - 1] = '\0';
	}
	/*
	printf("��������Ҫ��ѯ����ϵ�˵ĵ绰����(��������)��");
	fgets(szTempTel, 11, stdin);
	rewind(stdin);
	// �����ַ�������� '\n'
	if (szTempTel[strlen(szTempTel) - 1] == '\n')
	{
		szTempTel[strlen(szTempTel) - 1] = '\0';
	}
	*/
	printf("ͨ��ģ����ѯ���ļ��ڱ��� %s �������У�\r\n", szTempName);
	for (int i = 0; i < g_nFileSize; i++)
	{
		// ���ļ��ж�ȡָ������
		if (fread(&perInfo, sizeof(perInfo), 1, fp) != 1)
		{
			printf("ģ������ʱ���ļ���ȡʧ��\r\n");
			goto EXIT_SHDOFILE;
		}
		// �жϵ�ǰ�����Ƿ�Ϊʹ��
		if (perInfo.enDataType == usedData)
		{
			// ���û���Ҫ��ѯ����ϵ�����ݺ��ļ��ڱ�������ݽ��жԱ�
			strPosition = strstr(perInfo.szName, szTempName);
			if (strPosition != NULL)
			{
				printf("������");	// �ڿ���̨��ʾ���ҵ���������Ϣ��û����ʾ����ʾ�ļ���û�ж�Ӧ������
				printf(perInfo.szName);
				printf("\t\t");
				printf("���䣺%d\t", perInfo.nAge);
				printf("�ֻ��ţ�");
				printf(perInfo.szTel);
				printf("\r\n");
			}
		}
	}

EXIT_SHDOFILE:
	if (fp)
	{
		fclose(fp);	// �ر��ļ�
		fp = NULL;
	}
	return 0;
}
/*
�������ܣ���ѯ�ļ���������Ч����ϵ�����ݣ�֧�־�ȷ��ѯ��ģ����ѯ
��������
����ֵ��return 0 ���������˳�
*/
int InquireData()
{
	int nInNumber = 0;	// ��¼�û��������ֵ
	int nRetValue = 0;	// ��¼scanf�����ķ���ֵ
	while (1)
	{
		system("cls");
		InquireMenu();	// ��ʾ���ܲ˵�
		printf("������ 0~2 ������Ӧ���ܵĲ�����");
		nRetValue = scanf("%d", &nInNumber);
		rewind(stdin);
		if (nRetValue == 0)	// ����scanf����ֵ����Ӧ���ж�
		{
			printf("��������������󣬲�ѯ���򼴽��˳�...\r\n");
			break;
		}
		else
		{
			// ������ֵΪ0�˳�����
			if (nInNumber == 0)
			{
				printf("��ѯ���򼴽��˳�...\r\n");
				break;
			}
			if (nInNumber < 0 || nInNumber > 2)	// ������ֵ��Ҫ��0~2����
			{
				printf("���������Ϣ������������ȷ�ı��\r\n");
				break;
			}
			else
			{
				switch (nInNumber)
				{
				case 1:
					PerciseQueryContactData();	// ��ȷ����
					break;
				case 2:
					FuzzyInquireContactData();	// ģ����ѯ
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
ģ�鹦�ܣ����ݴ�����ַ�ֵ�ͽṹ�������һ���жϸ��ַ��Ƿ�Ϊ����
������
	����1��szTempWord��ָ���ȡ�ļ��е��ַ�ֵ
	����2��fileHead��ָ�򱣴溺����Ϣ�Ľṹ��
����ֵ������ָ������Ϣ�Ľṹ��
*/
CHINESECHAR* queryWords(char* szTempWord, CHINESECHAR* fileHead)
{
	int nTag = 1;	// ���Ĭ��Ϊ����
	CHINESECHAR* pChineseChar = NULL;
	pChineseChar = fileHead;	// ָ��ṹ�����fileHead
	// ��pTempΪ��nTagΪ1��ѭ������
	while (pChineseChar != NULL && nTag == 1)
	{
		/*
		���ṹ�����fileHead�еĳ�ԱszTempWord��
		�ṹ�����fileHeadpTemp�еĳ�ԱszWord��ȵ�ʱ��nTag��Ϊ0
		�������ȣ�ָ���ļ��е���һ����Ա�����Ա�
		*/
		if (strcmp(pChineseChar->szWord, szTempWord) == 0)
		{
			nTag = 0;
		}
		else
		{
			pChineseChar = pChineseChar->next;
		}
	}
	// nTagΪ0���ṹ�����pTemp�еĳ�ԱnCount++
	if (nTag == 0)
	{
		pChineseChar->nCount++;
	}
	else
	{
		// ���nTagΪ��Ϊ1���ṹ�����pTempָ��ṹ�����fileHead
		pChineseChar = fileHead;
		fileHead = (CHINESECHAR*)malloc(sizeof(CHINESECHAR));  // ����ռ�洢��Ӧ�ĺ�����Ϣ
		// ���ú���copy���ṹ�������Ӧ�ĳ�Ա��
		strcpy(fileHead->szWord, szTempWord);
		fileHead->nCount = 1;	// ������Ϊ1
		// ָ����һ����ϵ������
		fileHead->next = pChineseChar;
	}
	// ����fileHead��ָ�򱣴溺����Ϣ�Ľṹ��
	return fileHead;
}

/*
�������ܣ��ļ�ָ��fpChinese��ָ����ļ���ʼ��ÿ�ζ�ȡ�����ֽڱ��浽��ʱ�ַ������У�
	�ж������ڴ��е���ֵ�Ƿ����127С��0�������������������һ���жϣ������жϽ����
	���������еĺ������ݳ��ֵĴ������Լ���ռ�����ı���
������g_nCharCount ���ļ����ַ������ֳ��ֵ��ܴ������ݹ���
����ֵ��return 0 ���������˳�
*/
int ShowDataChinese(int g_nCharCount)
{
	int i = 0;
	char szTempWord[3];	 // �����ȡ������ʱ����
	CHINESECHAR* fileHead = NULL;

	FILE* fpChinese;
	fpChinese = fopen("ChineseChar.bin", "rb");
	// �ж��ļ��Ƿ�򿪳ɹ�
	if (fpChinese == NULL)
	{
		printf("�ļ���ȡʧ��\r\n");
		exit(-1);
	}
	// ���ļ�ָ��fp���ڸ��ļ�ָ����ļ���ʼ��
	if (fseek(fpChinese, 0, SEEK_SET))
	{
		exit(ferror(fpChinese));
	}
	// ���ļ�ָ��fpChinese������ָ���ļ���ĩβ�ͽ���ѭ��
	while (!feof(fpChinese))
	{
		// �ж��ļ�ָ��fpChinese��ָ����ļ��гɹ�ÿ�ζ�ȡ�����ֽڵ����ݵ���ʱ�ַ�������
		if (fscanf(fpChinese, "%2s", szTempWord) != EOF)
		{
			// �ж��ַ�����szTempWord�б�������ݵ���ֵС��0�������127�ͽ�����һ������
			if (szTempWord[i] < 0 || szTempWord[i + 1]>127)
			{
				// ���������ı������Ե���ֵ�ͽṹ�����������������
				fileHead = queryWords(szTempWord, fileHead);
			}
		}
	}

	// ����queryWords���ؽ���������Ӧ�ĺ�����Ϣ
	CHINESECHAR* pChinese = fileHead;
	g_nChineseCharCount = 0;	// ÿ�δ��ļ��ڶ�ȡ��Ч�ĺ��ָ���ʱ��������Ϊ0
	while (pChinese != NULL)
	{
		// ��ȡ�ļ�����Ч�ĺ����ܸ���
		g_nChineseCharCount = g_nChineseCharCount + pChinese->nCount;
		pChinese = pChinese->next;
	}
	// ���ṹ��ָ���ٴ������ļ���ͷ��
	pChinese = fileHead;
	g_nCharCount += g_nChineseCharCount;	// ����Ч�ĺ��ָ�������Ч��Ӣ���ַ����
	while (pChinese != NULL)
	{
		// ����ļ�����Ч�ĺ��֣��Լ���ռ���ַ����ı���
		printf("%s:\t\t%d��   \t%.2lf%%\r\n", pChinese->szWord, pChinese->nCount,
			(((double)pChinese->nCount) / g_nCharCount) * 100);
		pChinese = pChinese->next;
	}
	// �ر��ļ�
	if (fpChinese)
	{
		fclose(fpChinese);
		fpChinese = NULL;
	}
	return 0;
}

/*
�ú����Ĺ���Ϊ��ͳ���ļ��ڵ���Ч����ϵ����Ϣ�еĺ��֡���ĸ���ִ�Сд�������ֳ��ֵĴ�����
	��ռ�ܴ����ı���
������	 ��
����ֵ��return 0 ���������˳�
*/
int ShowFileDataRatio()
{
	int i = 0;
	int j = 0;
	int nTotalCount = 0;	// ��Сд��ĸ�����ֳ��ֵ��ܴ���
	int nLettersLarge[26] = { 0 };	// ���ڴ洢 26 ����д��ĸ
	int nLettersSmall[26] = { 0 };	// ���ڴ洢 26 ��Сд��ĸ
	int nNumbers[10] = { 0 };		// ���ڴ洢 10 ������
	int nLetsLarge = 0;		// �����ڴ��д洢��26����д��ĸ
	int nLetsSmall = 0;		// �����ڴ��д洢��26��Сд��ĸ
	int nNums = 0;		// �����ڴ��д洢��10������

	FILE* fp = NULL;
	fp = fopen("data.bin", "rb+");
	if (fp == NULL)
	{
		// �ж��ļ��Ƿ�򿪳ɹ�
		printf("�ļ���ʧ�ܣ������ļ��Ƿ����\r\n");
		exit(-1);
	}
	// Ϊ������ϵ����Ϣ�ṹ�嶨��һ��������
	TAGCONTACTS perInfo;
	// ���ļ�ָ�������ļ���ʼ��
	if (fseek(fp, 0, SEEK_SET))
	{
		exit(ferror(fp));
	}
	// �����ļ������е�����
	for (int i = 0; i < g_nFileSize; i++)
	{
		if (fread(&perInfo, sizeof(perInfo), 1, fp) != 1)
		{
			printf("��ʾ�ַ�����ʱ���ļ���ȡ����\r\n");
			goto EXIT_SHDOFILE;
		}
		// �жϵ�ǰ�����Ƿ�Ϊʹ��
		if (perInfo.enDataType == usedData)
		{
			for (int j = 0; j < (int)(strlen(perInfo.szName)); j++)
			{
				// ��ȡ�ļ��ڵ�ǰ��ϵ�˵�����
				if (isupper((unsigned char)(perInfo.szName[j])))
				{
					// �ж϶�ȡ�������Ƿ�ʱ��дӢ����ĸ
					nLettersLarge[(perInfo.szName[j]) - 'A']++;
					nTotalCount++;
				}
				else if (isalpha((unsigned char)(perInfo.szName[j])))	
				{
					// �ж϶�ȡ�������Ƿ�ʱ��СдӢ����ĸ
					nLettersSmall[(perInfo.szName[j]) - 'a']++;
					nTotalCount++;
				}
				else if (isdigit((unsigned char)(perInfo.szName[j])))
				{
					// �ж϶�ȡ���������Ƿ�������
					nNumbers[(perInfo.szName[j]) - '0']++;
					nTotalCount++;
				}
			}
			for (int j = 0; j < (int)(strlen(perInfo.szTel)); j++)
			{
				// ��ȡ�ļ��ڵ�ǰ��ϵ�˵ĵ绰����
				if (isupper((unsigned char)(perInfo.szTel[j])))
				{
					// �ж϶�ȡ�������Ƿ�ʱ��дӢ����ĸ
					nLettersLarge[(perInfo.szTel[j]) - 'A']++;
					nTotalCount++;
				}
				else if (isalpha((unsigned char)(perInfo.szTel[j])))
				{
					// �ж϶�ȡ�������Ƿ�ʱ��дӢ����ĸ
					nLettersSmall[(perInfo.szTel[j]) - 'a']++;
					nTotalCount++;
				}
				else if (isdigit((unsigned char)(perInfo.szTel[j])))	
				{
					// �ж϶�ȡ���������Ƿ�������
					nNumbers[(perInfo.szTel[j]) - '0']++;
					nTotalCount++;
				}
			}
		}
	}
	// �����ڴ��и���д��ĸ���ֵĴ�������ռ���ʣ����ڴ��д�д��ĸ��������0�����
	printf("�ļ��д洢�ĺ��֡��ַ������ֳ��ִ���������������£�\r\n");
	printf("�ַ�����	 ���ִ���  ռ�����ı���\r\n");

	g_nCharCount = nTotalCount;	// ����ʱ���ļ����ַ�����������ֵΪȫ�ֱ���g_nCharCount
	ShowDataChinese(g_nCharCount);  // ����ShowDataChinese��ʾ�ļ������ĵĴ������Լ���ռ����
	g_nCharCount += g_nChineseCharCount;
	nTotalCount = g_nCharCount;

	for (nLetsLarge = 0; nLetsLarge <= 26; nLetsLarge++)
	{
		if (nLettersLarge[nLetsLarge] > 0)
		{
			printf("%c:\t\t%d��   \t%.2lf%%\r\n", 'A' + nLetsLarge, 
				nLettersLarge[nLetsLarge],
				((double)nLettersLarge[nLetsLarge] / nTotalCount) * 100);
		}
	}
	// �����ڴ��и�Сд��ĸ���ֵĴ�������ռ���ʣ����ڴ���Сд��ĸ��������0�����
	for (nLetsSmall = 0; nLetsSmall <= 26; nLetsSmall++)
	{
		if (nLettersSmall[nLetsSmall] > 0)
		{
			printf("%c:\t\t%d��   \t%.2lf%%\r\n", 'a' + nLetsSmall, 
				nLettersSmall[nLetsSmall],
				((double)nLettersSmall[nLetsSmall] / nTotalCount) * 100);
		}
	}
	// �����ڴ��и����ֳ��ֵĴ�������ռ���ʣ����ڴ���������������0�����
	for (nNums = 0; nNums <= 9; nNums++)
	{
		if (nNumbers[nNums] > 0)
		{
			printf("%c:\t\t%d��   \t%.2lf%%\r\n", 
				'0' + nNums, nNumbers[nNums],
				((double)nNumbers[nNums] / nTotalCount) * 100);
		}
	}

EXIT_SHDOFILE:
	if (fp)
	{
		fclose(fp);	// �ر��ļ�
		fp = NULL;
	}
	return 0;
}

/*
�ú����Ĺ���Ϊ����ʾ�ļ������ݿռ��ռ�������U(ʹ��)��F(δʹ��)
��������
����ֵ��return 0 ���������˳�
*/
int ShowMemoryFootprint()
{
	int nCount = 0;
	FILE* fp = NULL;
	fp = fopen("data.bin", "rb+");
	if (fp == NULL)
	{
		// �ж��ļ��Ƿ�򿪳ɹ�
		printf("�ļ���ʧ�ܣ������ļ��Ƿ����\r\n");
		exit(-1);
	}
	// Ϊ������ϵ����Ϣ�ṹ�嶨��һ��������
	TAGCONTACTS perInfo;
	// ���ļ�ָ�������ļ���ʼ��
	if (fseek(fp, 0, SEEK_SET))
	{
		exit(ferror(fp));
	}
	printf("��ǰ�ļ����ڴ��ʹ���������U(ʹ��)��F(δʹ��)\r\n");
	for (int i = 0; i < g_nFileSize; i ++)
	{
		// ���ݶ�ȡʧ�ܣ��˳����򣬹ر��ļ�
		if (fread(&perInfo, sizeof(perInfo), 1, fp) != 1)
		{
			goto EXIT_SHDOFILE;
		}
		// ����ʹ�õ��ļ����ݱ�ʾΪU
		if (perInfo.enDataType == usedData)
		{
			// printf("U");
			int nUsed = 0;	// ������ʾ�ļ�����Ч����ϵ����Ϣ
			while (nUsed < 52)
			{
				printf("U");
				nUsed++;
			}
			printf("\r\n");
		}
		else
		{
			// ��δʹ�õ��ļ����ݱ�ʾΪF
			int nUnused = 0;		// ������ʾ�ļ��ڿյ���ϵ����Ϣ
			while (nUnused < 52)
			{
				printf("F");
				nUnused++;
			}
			printf("\r\n");
			// printf("F");
			nCount++;
			if (nCount > 100)	  // δʹ����������5000��ʱ���� ...... ��ʾ
			{
				printf("......\r\n");
				break;
			}
		}
	}
EXIT_SHDOFILE:
	if (fp)
	{
		fclose(fp);	// �˳�����ر��ļ�
		fp = NULL;
	}
	return 0;
}

/*
�������ܣ��ڲ��洢����ʱ���Խ��ļ��д��ڵĿ����ݽ�����Ƭ����
��������
����ֵ��return 0 ���������˳�
*/
int Defragmentation()
{
	FILE* fp = NULL;
	FILE* fpTemp = NULL;
	fp = fopen("data.bin", "rb+");	// �򿪱�����ϵ����Ϣ���ļ�
	fpTemp = fopen("newdata.bin", "wb+");	// ���ڴ洢��ϵ����Ϣ����ʱ�ļ�
	if (fp == NULL)
	{
		// �ж��ļ��Ƿ�򿪳ɹ�
		printf("�ļ�data.bin��ʧ��\r\n");
		exit(-1);
	}
	if (fpTemp == NULL)
	{
		// �ж��ļ��Ƿ�򿪳ɹ�
		printf("�ļ�newdata.bin��ʧ��\r\n");
		exit(-1);
	}
	// Ϊ������ϵ����Ϣ�ṹ�嶨��һ����������������ֵ
	TAGCONTACTS perInfo = {
			unusedData, 
			0, 
			'\0', '\0'
	};
	// Ϊ��ʱ������ϵ����Ϣ�ṹ�嶨��һ��������
	TAGCONTACTS perInfoTemp;

	for (int i = 0; i < g_nFileSize; i++)
	{
		// ΪfpTempָ����ļ�ÿ�ζ���sizeof(perInfo)������ݵ��ļ���ȥ(��ʼ���ڴ�)
		if (fwrite(&perInfo, sizeof(perInfo), 1, fpTemp) != 1)
		{
			printf("���ݶ�ȡʧ�ܣ������ļ��Ƿ���Ч\r\n");
			goto EXIT_SHDOFILE;
		}
	}
	// ���ļ�ָ�������ļ���ʼ��
	if (fseek(fpTemp, 0, SEEK_SET))
	{
		exit(ferror(fp));
	}

	for (int i = 0; i < g_nFileSize; i += sizeof(fp))
	{
		// ���ļ�ָ��fp��ָ����ļ��ڶ�ȡsizeof(perInfoTemp)�ֽ����ݵ�perInfoTemp��ȥ
		if (fread(&perInfoTemp, sizeof(perInfoTemp), 1, fp) != 1)
		{
			goto EXIT_SHDOFILE;
		}
		// �жϵ�ǰ�ļ��е������Ƿ�ʹ��
		if (perInfoTemp.enDataType == usedData)
		{
			// ������д�뵽fpTempָ����ļ���
			if (fwrite(&perInfoTemp, sizeof(perInfoTemp), 1, fpTemp) != 1)
			{
				printf("���ݶ�ȡʧ��\r\n");
				goto EXIT_SHDOFILE;
			}
		}
	}

EXIT_SHDOFILE:
	if (fp)
	{
		fclose(fp);	// �ر��ļ�ָ��fpָ����ļ�
		fp = NULL;
	}
	if (fpTemp != NULL)
	{
		fclose(fpTemp);	// �ر��ļ�ָ��fpTempָ����ļ�
		fpTemp = NULL;
	}
	// �ڹر��ļ���Դ�ļ�����ɾ��
	if (unlink("data.bin") == 0)	// ɾ��ԭ�ļ�
	{
		// printf("Դ�ļ�ɾ���ɹ�\r\n");
	}
	// �����ڴ洢��Ƭ��������ʱ�ļ���������
	if (rename("newdata.bin", "data.bin") == 0)
	{
		printf("������Ƭ�ɹ�!\r\n");
	}
	return 0;
}