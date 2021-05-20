#include "FunRealization.h"

// 根据文件大小计算该文件最多可以存储多少位联系人的信息
int g_nFileSize = (1024 * 1024 * 5) / (sizeof(TAGCONTACTS));
// 文件内字符串的总数
int g_nCharCount = 0;
// 记录文件内有效的汉字数量
int g_nChineseCharCount = 0;

/*
函数功能：运行程序
参数：无
返回值：return 0 程序正常退出
*/
int StartProgram()
{
	// FormattedData();
	int nNumber = 0;  // 记录用户输入的数字
	int nStatus = 0;	// 记录scanf()的返回值
	while (1)
	{
		system("cls");
		Menu(); // 显示功能菜单
		nStatus = scanf("%d", &nNumber);	// 输入是数字返回1，反之返回0
		rewind(stdin);
		// 根据scanf返回值作判定，如果输入的数值不是一个整数，提示相应的错误信息
		if (nStatus == 0)
		{
			printf("您的输入有误，程序即将退出...\r\n");
			// Sleep(2000);
			break;
		}
		else
		{
			// 用户输入数值为 0，程序退出
			if (nNumber == 0)
			{
				printf("程序正在退出...\r\n");
				// Sleep(2000);
				break;
			}
			// 用户输入的数值需要在0~8区间内，输入的数值超过该区间，系统退出
			if (nNumber < 0 || nNumber > 9)
			{
				printf("您的输入有误，程序即将退出...\r\n");
				break;
			}
			else
			{
				switch (nNumber)
				{
				case 1:
					system("cls");
					// 调用IncreaseData模块，添加联系人信息到文件中去
					IncreaseData();
					break;
				case 2:
					system("cls");
					// 调用DeleteData模块，进行删除指定联系人信息操作
					DeleteData();
					break;
				case 3:
					system("cls");
					// 调用ReviseData模块，进行修改指定联系人信息操作
					ReviseData();
					break;
				case 4:
					system("cls");
					// 调用InquireData模块，查询文件内联系人信息
					InquireData();
					break;
				case 5:
					system("cls");
					// 调用ShowData模块，将文件内存储的所有联系人信息显示到控制台
					ShowData();
					break;
				case 6:
					system("cls");
					// 调用ShowFileDataRatio模块，将文件内存储的数据信息进行统计
					ShowFileDataRatio();
					break;
				case 7:
					system("cls");
					// 调用ShowFileDataRatio模块，显示文件内存的使用分布
					ShowMemoryFootprint();
					break;
				case 8:
					system("cls");
					// 调用Defragmentation模块，对文件内的空先位置进行碎片整理
					Defragmentation();		 
					// 再次调用ShowFileDataRatio模块，显示文件内存的使用分布
					ShowMemoryFootprint();
					break;
				case 9:
					// 格式化碎片整理后的文件数据
					InitializationData();
					break; 
				default:
					printf("您的输入不对，请重新输入\r\n");
					break;
				}
			}
			system("pause");
		}
	}
	return 0;
}

/*
函数功能：程序开始菜单
返回值：无
参数：无
*/
void Menu()
{
	printf("\r\n");
	printf("\t***** 联系人信息存储系统 *****\r\n\n");
	printf("\t0.退出程序\r\n");
	printf("\t1.添加联系人信息\r\n");
	printf("\t2.删除联系人信息\r\n");
	printf("\t3.修改联系人信息\r\n");
	printf("\t4.查询联系人信息\n");
	printf("\t5.显示内存中存储的所有联系人数据\r\n");
	printf("\t6.统计系统内各字符出现次数及比率\r\n");
	printf("\t7.显示内存占用情况\r\n");
	printf("\t8.碎片整理\r\n");
	printf("\t9.初始化保存联系人信息的文件\r\n");
	printf("\r\n");
	printf("请输入 0~9 进行相应功能的操作：");
}

/*
函数功能：将保存联系人信息的文件格式化
参数：无
返回值：return 0 正常退出
*/
int InitializationData()
{
	FILE* fp = NULL;
	fp = fopen("data.bin", "wb+"); // 如果文件不存在，就初始化一个文件
	if (fp == NULL)
	{
		// 如果文件打开失败，退出程序
		exit(-1);
	}

	// 将文件指针fp置于该文件指向的文件开始处
	if (fseek(fp, 0, SEEK_SET))
	{
		exit(ferror(fp));
	}
	// 为结构体变量的成员赋初值
	TAGCONTACTS perInfo = {
		unusedData, 0, '\0', '\0'
	};

	// 遍历，将初始化后的数据写入到文件中
	for (int i = 0; i < g_nFileSize; i++)
	{
		if (fwrite(&perInfo, sizeof(perInfo), 1, fp) != 1)
		{
			printf("数据写入失败\r\n");
			goto EXIT_SHDOFILE;
		}
	}
	printf("data.bin 文件初始化(格式化)成功!\r\n");

EXIT_SHDOFILE:
	if (fp)
	{
		fclose(fp); // 关闭文件
		fp = NULL;
	}
	return 0;
}

/*
函数功能：格式化存储联系人姓名的文件
返回值：无
参数：无
*/
void initializationChinese()
{
	FILE* fpChineseChar = NULL;

	// 记录输入联系人的中文名
	fpChineseChar = fopen("ChineseChar.bin", "wb+");

	// 判断文件 ChineseChar.bin 是否打开成功，且是否为空
	if (fpChineseChar == NULL)
	{
		printf("文件打开失败，请检查文件是否存在\r\n");
		exit(-1);
	}
	// 将文件指针 fpChinese 置于指向的文件开始处
	if (fseek(fpChineseChar, 0, SEEK_SET))
	{
		exit(ferror(fpChineseChar));
	}

	TAGTEMPNAME tempName = {
		'\0'
	};
	// 初始化的数为5M
	for (int i = 0; i < g_nFileSize; i++)
	{
		// 将更改后的数据写入到文件
		if (fwrite(&tempName, sizeof(tempName), 1, fpChineseChar) != 1)
		{
			printf("数据写入失败\r\n");
			goto EXIT_SHDOFILE;
		}
	}
	// printf("ChineseChar.bin 文件格式化成功!\r\n");
EXIT_SHDOFILE:
	if (fpChineseChar)
	{
		fclose(fpChineseChar);	// 关闭文件
		fpChineseChar = NULL;
	}
}

/*
函数功能：在指定的文件内存储相应的联系人信息
参数：无
返回值：return 0 程序正常退出
*/
int IncreaseData()
{
	int nTempAge = 0;	// 保存用户输入的联系人年龄
	int nStatus = 0;	// 记录scanf()的返回值
	FILE* fp = NULL;

	fp = fopen("data.bin", "rb+");	// 判断文件是否存在，且是否打开成功
	if (fp == NULL)
	{
		printf("文件打开失败，请检查文件是否存在！\r\n");
		exit(-1);
	}

	// 将文件指针fp置于该文件指向的文件开始处
	if (fseek(fp, 0, SEEK_SET))
	{
		exit(ferror(fp));
	}
	// 定义一个保存联系人信息的结构体变量
	TAGCONTACTS perInfo;

	for (int i = 0; i < g_nFileSize; i++)
	{
		if (fread(&perInfo, sizeof(perInfo), 1, fp) != 1)
		{
			printf("文件读取错误\r\n");
			goto EXIT_SHDOFILE;
		}
		// 判断当前数据是否为使用
		if (perInfo.enDataType == unusedData)
		{
			printf("请输入你要存储联系人的姓名：");	// 用户输入
			fgets(perInfo.szName, 32, stdin);
			rewind(stdin);
			// 处理字符串后面的 '\n'
			if (perInfo.szName[strlen(perInfo.szName) - 1] == '\n')
			{
				perInfo.szName[strlen(perInfo.szName) - 1] = '\0';
			}
			while (1)
			{
				printf("请输入你要存储联系人的年龄(0~150)：");
				nStatus = scanf("%d", &nTempAge);
				rewind(stdin);
				// 根据scanf返回值作判定，如果输入的数值不是一个整数，提示相应的错误信息
				if (nStatus == 0)
				{
					printf("您的输入有误，请重新输入\r\n");
				}
				else
				{
					// 将用户的年龄信息限制到0~150之间
					if (nTempAge < 0 || nTempAge >150)
					{
						printf("你输入的年龄不符合范围，请重新输入\r\n");
					}
					else
					{
						perInfo.nAge = nTempAge;
						break;
					}
				}
			}
			printf("请输入你要存储联系人的电话号码：");
			fgets(perInfo.szTel, 12, stdin);
			rewind(stdin);
			// 处理字符串后面的 '\n'
			if (perInfo.szTel[strlen(perInfo.szTel) - 1] == '\n')
			{
				perInfo.szTel[strlen(perInfo.szTel) - 1] = '\0';
			}

			// 将该数据内存标记为已经使用
			perInfo.enDataType = usedData;
			// 将文件指针指向当前位置
			if (fseek(fp, -(int)(sizeof(perInfo)), SEEK_CUR))
			{
				exit(ferror(fp));
			}
			// 将数据写入到文件内
			if (fwrite(&perInfo, sizeof(perInfo), 1, fp) != 1)
			{
				printf("数据写入失败\r\n");
				goto EXIT_SHDOFILE;
			}
			printf(perInfo.szName);
			printf(" 联系人信息添加成功!\r\n");

			break;
		}
	}

EXIT_SHDOFILE:
	if (fp)
	{
		fclose(fp);	// 关闭文件
		fp = NULL;
	}
	return 0;
}

/*
函数功能：在指定的文件内删除相应的联系人信息
参数：无
返回值：return 0 程序正常退出
*/
int DeleteData()
{
	char szTempName[32] = { 0 };	// 保存用户要删除联系人的姓名
	char szTempTel[12] = { 0 };	// 保存用户要删除联系人的手机号码
	int nTempAge = 0;		// 保存用户要删除联系人的年龄
	char szTag[2] = { 0 };
	FILE* fp = NULL;
	fp = fopen("data.bin", "rb+");
	// 判断文件是否存在，且是否打开成功
	if (fp == NULL)
	{
		printf("文件打开失败，请检查文件是否存在\r\n");
		goto EXIT_SHDOFILE;
	}
	// 定义一个保存联系人信息的结构体变量
	TAGCONTACTS perInfo;
	// 将文件指针fp置于该文件指向的文件开始处
	if (fseek(fp, 0, SEEK_SET))
	{
		exit(ferror(fp));
	}

	// 显示文件中已经存在的数据
	printf("当前文件内保存的所有联系人数据如下：\r\n\n");
	for (int i = 0; i < g_nFileSize; i++)
	{
		if (fread(&perInfo, sizeof(perInfo), 1, fp) != 1)
		{
			printf("文件读取错误\r\n");
			exit(ferror(fp));
		}

		if (perInfo.enDataType == usedData)
		{
			printf("姓名：");
			printf(perInfo.szName);
			printf("\r\n");
			printf("年龄：%d\r\n", perInfo.nAge);
			printf("手机号：");
			printf(perInfo.szTel);
			printf("\r\n");
			printf("------------------------------------------------\r\n");
		}
	}
	// 再次遍历，将文件指针至于文件开始处
	if (fseek(fp, 0, SEEK_SET))
	{
		exit(ferror(fp));
	}

	printf("请输入你要删除的联系人姓名："); // 用户输入要删除的联系人数据
	fgets(szTempName, 32, stdin);
	rewind(stdin);
	// 处理字符串后面的 '\n'
	if (szTempName[strlen(szTempName) - 1] == '\n')
	{
		szTempName[strlen(szTempName) - 1] = '\0';
	}
	printf("请输入你要删除的联系人的年龄：");
	scanf("%d", &nTempAge);
	rewind(stdin);
	printf("请输入你要删除的联系人的电话号码：");
	fgets(szTempTel, 12, stdin);
	rewind(stdin);
	// 处理字符串后面的 '\n'
	if (szTempTel[strlen(szTempTel) - 1] == '\n')
	{
		szTempTel[strlen(szTempTel) - 1] = '\0';
	}

	for (int i = 0; i < g_nFileSize; i++)
	{
		// 判断数据是否读取成功
		if (fread(&perInfo, sizeof(perInfo), 1, fp) != 1)
		{
			printf("删除联系人数据时，数据读取失败\r\n");
			goto EXIT_SHDOFILE;
		}
		// 判断当前数据是否为使用
		if (perInfo.enDataType == usedData)
		{
			// 判断当前文件数据联系人的姓名和需要删除的联系人信息相同
			if ((perInfo.nAge == nTempAge) && 
				(strcmp(perInfo.szName, szTempName) == 0) && 
				(strcmp(perInfo.szTel, szTempTel) == 0))
			{
				printf("经过查询，你要删除的联系人在文件内，是否进行删除(Y/y确定删除，其它键取消删除)：");
				scanf("%1s", szTag);
				if ((strcmp(szTag, "Y") == 0) || (strcmp(szTag, "y") == 0))
				{
					perInfo.enDataType = unusedData;
					// 更新文件指针指向的位置
					if (fseek(fp, -(int)sizeof(perInfo), SEEK_CUR))
					{
						exit(ferror(fp));
					}
					// 将更新后的联系人信息写入到文件
					if (fwrite(&perInfo, sizeof(perInfo), 1, fp) != 1)
					{
						printf("数据写入失败\r\n");
						goto EXIT_SHDOFILE;
					}
					printf("联系人 %s 的信息删除成功!\r\n", perInfo.szName);
					goto EXIT_SHDOFILE;
				}
				else
				{
					printf("已取消删除\r\n");
					goto EXIT_SHDOFILE;
				}
			}
		}
	}
	printf("经过查询，文件中没有查询到关于 %s 的信息!\r\n", szTempName);
EXIT_SHDOFILE:
	if (fp)
	{
		fclose(fp); // 关闭文件
		fp = NULL;
	}
	return 0;
}

/*
函数功能：在指定的文件内修改相应的联系人信息
参数：无
返回值：return 0 程序正常退出
*/
int ReviseData()
{
	char szTempName[32] = { 0 };  // 保存用户要修改联系人的姓名
	char szTempTel[12] = { 0 }; // 保存用户要修改联系人的手机号码
	int nTempAge = 0; // 保存用户要修改联系人的年龄
	char szTag[2] = { 0 };
	FILE* fp = NULL;
	fp = fopen("data.bin", "rb+");
	// 判断文件是否存在，且是否打开成功
	if (fp == NULL)
	{
		printf("文件打开失败\r\n");
		exit(-1);
	}
	// 定义一个保存联系人信息的结构体变量
	TAGCONTACTS perInfo;
	// 将文件指针fp置于该文件指向的文件开始处
	if (fseek(fp, 0, SEEK_SET))
	{
		exit(ferror(fp));
	}

	// 显示文件中已经存在的数据
	printf("当前文件内保存的所有联系人数据如下：\r\n\n");
	for (int i = 0; i < g_nFileSize; i++)
	{
		if (fread(&perInfo, sizeof(perInfo), 1, fp) != 1)
		{
			printf("文件读取错误\r\n");
			exit(ferror(fp));
		}
		// 显示已存储的联系人信息
		if (perInfo.enDataType == usedData)
		{
			printf("姓名：");
			printf(perInfo.szName);
			printf("\r\n");
			printf("年龄：%d\r\n", perInfo.nAge);
			printf("手机号：");
			printf(perInfo.szTel);
			printf("\r\n");
			printf("------------------------------------------------\r\n");
		}
	}

	// 再次遍历,将文件指针fp置于该文件指向的文件开始处
	if (fseek(fp, 0, SEEK_SET))
	{
		exit(ferror(fp));
	}

	printf("请输入你要修改的联系人姓名：");	// 用户输入
	fgets(szTempName, 32, stdin);
	rewind(stdin);
	// 处理字符串后面的 '\n'
	if (szTempName[strlen(szTempName) - 1] == '\n')
	{
		szTempName[strlen(szTempName) - 1] = '\0';
	}

	printf("请输入你要修改的联系人的年龄：");
	scanf("%d", &nTempAge);
	rewind(stdin);

	printf("请输入你要修改的联系人的电话号码：");
	fgets(szTempTel, 12, stdin);
	rewind(stdin);
	// 处理字符串后面的 '\n'
	if (szTempTel[strlen(szTempTel) - 1] == '\n')
	{
		szTempTel[strlen(szTempTel) - 1] = '\0';
	}

	for (int i = 0; i < g_nFileSize; i++)
	{
		// 如果读取指定的文件失败，则指向goto语句关闭文件
		if (fread(&perInfo, sizeof(perInfo), 1, fp) == 0)
		{
			printf("修改联系人数据时，文件读取失败\r\n");
			goto EXIT_SHDOFILE;
		}
		if (perInfo.enDataType == usedData)
		{
			// 输入的联系人姓名和文件内存在的联系人姓名、年龄、手机号码进行对比
			if ((perInfo.nAge == nTempAge) && (strcmp(perInfo.szName, szTempName) == 0)
				&& (strcmp(perInfo.szTel, szTempTel) == 0))
			{
				printf("经过查询，你要修改的联系人在文件内，是否进行修改(Y/y确定修改，其它键取消修改)：");
				scanf("%1s", szTag);
				rewind(stdin);
				if ((strcmp(szTag, "Y") == 0) || (strcmp(szTag, "y") == 0))
				{
					printf("请输入你修改后联系人的姓名：");
					fgets(perInfo.szName, 32, stdin);
					rewind(stdin);
					// 处理字符串后面的 '\n'
					if (perInfo.szName[strlen(perInfo.szName) - 1] == '\n')
					{
						perInfo.szName[strlen(perInfo.szName) - 1] = '\0';
					}

					printf("输入你修改后联系人的年龄：");
					scanf("%d", &perInfo.nAge);
					rewind(stdin);

					printf("请输入你修改后联系人的电话号码：");
					fgets(perInfo.szTel, 12, stdin);
					rewind(stdin);
					// 处理字符串后面的 '\n'
					if (perInfo.szTel[strlen(perInfo.szTel) - 1] == '\n')
					{
						perInfo.szTel[strlen(perInfo.szTel) - 1] = '\0';
					}

					// 更新文件指针指向的位置
					if (fseek(fp, -(int)sizeof(perInfo), SEEK_CUR))
					{
						exit(ferror(fp));
					}
					// 将修改后的数据写入到文件内
					if (fwrite(&perInfo, sizeof(perInfo), 1, fp) == 0)
					{
						printf("数据写入失败\r\n");
						goto EXIT_SHDOFILE;
					}
					printf("原联系人 %s 的信息修改成功\r\n", szTempName);
					goto EXIT_SHDOFILE;
				}
				else
				{
					printf("你已取消修改\r\n");
					goto EXIT_SHDOFILE;
				}
			}
		}
	}
	printf("经过查询，文件中没有查询到关于 %s 的信息!\r\n", szTempName);

EXIT_SHDOFILE:
	if (fp)
	{
		fclose(fp); // 关闭文件，退出程序
		fp = NULL;
	}
	return 0;
}

/*
函数功能：显示文件内的所有有效的联系人数据
参数：无
返回值：return 0 程序正常退出
*/
int ShowData()
{
	// 格式化存储文件内联系人姓名的文件，当每次遍历文件内的数据时，在将有效的联系人姓名进行存储
	initializationChinese();
	FILE* fp = NULL;
	FILE* fpChinese = NULL; // 指向 ChineseChar.bin 文件，该文件将保存 data.bin 文件里的有效联系人数据的姓名

	fp = fopen("data.bin", "rb+");
	// 判断文件 data.bin 是否打开成功，且是否为空
	if (fp == NULL)
	{
		printf("文件打开失败，请检查文件是否存在\r\n");
		exit(-1);
	}

	// 记录输入联系人的中文名
	fpChinese = fopen("ChineseChar.bin", "rb+");
	if (fpChinese == NULL)	  // 判断文件 ChineseChar.bin 是否打开成功，且是否为空
	{
		printf("文件打开失败，请检查文件是否存在\r\n");
		exit(-1);
	}

	// 为保存联系人信息结构体定义一个变量名
	TAGCONTACTS perInfo;

	// 为文件中有效的联系人名字结构体定义一个变量名，用于临时存储文件中有效的联系人名字
	TAGTEMPNAME tempName;

	// 将文件指针fp置于该文件指向的文件开始处
	if (fseek(fp, 0, SEEK_SET))
	{
		exit(ferror(fp));
	}

	// 将文件指针 fpChinese 置于指向的文件开始处
	if (fseek(fpChinese, 0, SEEK_SET))
	{
		exit(ferror(fpChinese));
	}

	// 遍历文件中有效的联系人信息
	printf("当前文件内保存的所有有效联系人数据如下：\r\n\n");
	for (int i = 0; i < g_nFileSize; i++)
	{
		if (fread(&perInfo, sizeof(perInfo), 1, fp) != 1)
		{
			printf("文件读取错误，请检查文件数据\r\n");
			exit(ferror(fp));
		}
		if (perInfo.enDataType == usedData)
		{
			printf("姓名：");
			printf(perInfo.szName);
			printf("\r\n");
			printf("年龄：%d\r\n", perInfo.nAge);
			printf("手机号：");
			printf(perInfo.szTel);
			printf("\r\n");
			printf("------------------------------------------------\r\n");

			// 从fpChinese指向的文件内读取一个tempName字节大小的数据到tempName中
			if (fread(&tempName, sizeof(tempName), 1, fpChinese) != 1)
			{
				printf("文件读取错误，请检查文件数据\r\n");
				goto EXIT_SHDOFILE;
			}
			strcpy(tempName.szName, perInfo.szName);
			// 将文件指针fpChinese指向当前文件位置
			if (fseek(fpChinese, -(int)(sizeof(tempName)), SEEK_CUR))
			{
				exit(ferror(fpChinese));
			}
			// 将copy到的联系人姓名写入到指定的文件中去
			if (fwrite(&tempName, sizeof(tempName), 1, fpChinese) != 1)
			{
				printf("联系人信息写入失败\r\n");
				goto EXIT_SHDOFILE;
			}
		}
	}
EXIT_SHDOFILE:
	if (fp)
	{
		fclose(fp);	// 关闭文件
		fp = NULL;
	}
	if (fpChinese)
	{
		fclose(fpChinese);	// 关闭文件
		fpChinese = NULL;
	}
	return 0;
}

/*
函数功能：查询联系人数据菜单
参数：无
返回值：无
*/
void InquireMenu()
{
	// 显示内存中存储的字符串信息
	ShowData();

	printf("\r\n\n");
	printf("\t输入对应的编号进行相应的操作 \n");
	printf("\t0.退出查询\r\n");
	printf("\t1.精准查询\r\n");
	printf("\t2.模糊查询\r\n\n");
}

/*
函数功能：精确查询文件内指定的有效联系人数据信息
参数：无
返回值：return 0 程序正常退出
*/
int PerciseQueryContactData()
{
	char szTempName[32] = { 0 };	// 保存用户输入的联系人的姓名
	char szTempTel[12] = { 0 };	// 保存用户输入的联系人的手机号码
	int nTempAge = 0;		// 保存用户输入的联系人的年龄
	FILE* fp = NULL;
	fp = fopen("data.bin", "rb+");
	// 判断文件是否打开成功
	if (fp == NULL)
	{
		goto EXIT_SHDOFILE;
	}
	// 为保存联系人信息结构体定义一个变量名
	TAGCONTACTS perInfo;

	// 将文件指针移动到文件的开始位置
	if (fseek(fp, 0, SEEK_SET))
	{
		exit(ferror(fp));
	}
	// 精确查询，用户需要输入对应的姓名，年龄，电话号码
	printf("请输入你要查询的联系人姓名：");
	fgets(szTempName, 32, stdin);
	rewind(stdin);
	// 处理字符串后面的 '\n'
	if (szTempName[strlen(szTempName) - 1] == '\n')
	{
		szTempName[strlen(szTempName) - 1] = '\0';
	}

	printf("请输入你要查询的联系人的年龄：");
	scanf("%d", &nTempAge);
	rewind(stdin);

	printf("请输入你要查询的联系人的电话号码：");
	fgets(szTempTel, 12, stdin);
	rewind(stdin);
	// 处理字符串后面的 '\n'
	if (szTempTel[strlen(szTempTel) - 1] == '\n')
	{
		szTempTel[strlen(szTempTel) - 1] = '\0';
	}

	printf("经过查询，你查询的联系人信息在内存中，信息如下：\r\n");
	for (int i = 0; i < g_nFileSize; i++)
	{
		// 从文件中读取指定数据
		if (fread(&perInfo, sizeof(perInfo), 1, fp) != 1)
		{
			printf("文件读取失败\r\n");
			goto EXIT_SHDOFILE;
		}

		// 判断当前数据是否为使用
		if (perInfo.enDataType == usedData)
		{
			// 对用户将要查询的联系人数据和文件内保存的数据进行对比
			if ((perInfo.nAge == nTempAge) && (strcmp(perInfo.szName, szTempName) == 0)
				&& (strcmp(perInfo.szTel, szTempTel) == 0))
			{
				printf("姓名：");	// 在控制台显示查找到的数据信息，没有显示，表示文件内没有对应的数据
				printf(perInfo.szName);
				printf("\t\t");
				printf("年龄：%d\t", perInfo.nAge);
				printf("手机号：");
				printf(perInfo.szTel);
				printf("\r\n");
			}
		}
	}

EXIT_SHDOFILE:
	if (fp)
	{
		fclose(fp); // 关闭文件
		fp = NULL;
	}
	return 0;
}

/*
函数功能：模糊查询文件内指定的有效联系人数据信息，输入文件中存储的联系人姓名中的字符即可
参数：无
返回值：return 0 程序正常退出
*/
int FuzzyInquireContactData()
{
	char szTempName[32] = { 0 };	// 保存用户输入的联系人姓名
	char szTempTel[12] = { 0 };
	char* strPosition = NULL;		// 用于记录对应的字符在字符串中出现的位置
	FILE* fp = NULL;
	fp = fopen("data.bin", "rb+");
	// 判断文件是否打开成功
	if (fp == NULL)
	{
		printf("模糊查询时，文件打开失败\r\n");
		exit(-1);
	}
	// 为保存联系人信息结构体定义一个变量名
	TAGCONTACTS perInfo;

	// 将文件指针移动到文件的开始位置
	if (fseek(fp, 0, SEEK_SET))
	{
		exit(ferror(fp));
	}

	printf("请输入你要模糊查询的包含联系人姓名的数据：");
	fgets(szTempName, 32, stdin);
	rewind(stdin);
	// 处理字符串后面的 '\n'
	if (szTempName[strlen(szTempName) - 1] == '\n')
	{
		szTempName[strlen(szTempName) - 1] = '\0';
	}
	/*
	printf("请输入你要查询的联系人的电话号码(部分内容)：");
	fgets(szTempTel, 11, stdin);
	rewind(stdin);
	// 处理字符串后面的 '\n'
	if (szTempTel[strlen(szTempTel) - 1] == '\n')
	{
		szTempTel[strlen(szTempTel) - 1] = '\0';
	}
	*/
	printf("通过模糊查询，文件内保存 %s 的数据有：\r\n", szTempName);
	for (int i = 0; i < g_nFileSize; i++)
	{
		// 从文件中读取指定数据
		if (fread(&perInfo, sizeof(perInfo), 1, fp) != 1)
		{
			printf("模糊查找时，文件读取失败\r\n");
			goto EXIT_SHDOFILE;
		}
		// 判断当前数据是否为使用
		if (perInfo.enDataType == usedData)
		{
			// 对用户将要查询的联系人数据和文件内保存的数据进行对比
			strPosition = strstr(perInfo.szName, szTempName);
			if (strPosition != NULL)
			{
				printf("姓名：");	// 在控制台显示查找到的数据信息，没有显示，表示文件内没有对应的数据
				printf(perInfo.szName);
				printf("\t\t");
				printf("年龄：%d\t", perInfo.nAge);
				printf("手机号：");
				printf(perInfo.szTel);
				printf("\r\n");
			}
		}
	}

EXIT_SHDOFILE:
	if (fp)
	{
		fclose(fp);	// 关闭文件
		fp = NULL;
	}
	return 0;
}
/*
函数功能：查询文件内所有有效的联系人数据，支持精确查询、模糊查询
参数：无
返回值：return 0 程序正常退出
*/
int InquireData()
{
	int nInNumber = 0;	// 记录用户输入的数值
	int nRetValue = 0;	// 记录scanf函数的返回值
	while (1)
	{
		system("cls");
		InquireMenu();	// 显示功能菜单
		printf("请输入 0~2 进行相应功能的操作：");
		nRetValue = scanf("%d", &nInNumber);
		rewind(stdin);
		if (nRetValue == 0)	// 根据scanf返回值最相应的判断
		{
			printf("您输入的输入有误，查询程序即将退出...\r\n");
			break;
		}
		else
		{
			// 输入数值为0退出程序
			if (nInNumber == 0)
			{
				printf("查询程序即将退出...\r\n");
				break;
			}
			if (nInNumber < 0 || nInNumber > 2)	// 输入数值需要在0~2区间
			{
				printf("您输入的信息有误，请输入正确的编号\r\n");
				break;
			}
			else
			{
				switch (nInNumber)
				{
				case 1:
					PerciseQueryContactData();	// 精确查找
					break;
				case 2:
					FuzzyInquireContactData();	// 模糊查询
					break;
				default:
					printf("您输入的信息不对，请重新输入\r\n");
					break;
				}
			}
			system("pause");
		}
	}
	return 0;
}

/*
模块功能：根据传入的字符值和结构体变量进一步判断该字符是否为汉字
参数：
	参数1：szTempWord，指向读取文件中的字符值
	参数2：fileHead，指向保存汉字信息的结构体
返回值：返回指向汉字信息的结构体
*/
CHINESECHAR* queryWords(char* szTempWord, CHINESECHAR* fileHead)
{
	int nTag = 1;	// 标记默认为中文
	CHINESECHAR* pChineseChar = NULL;
	pChineseChar = fileHead;	// 指向结构体变量fileHead
	// 当pTemp为空nTag为1，循环结束
	while (pChineseChar != NULL && nTag == 1)
	{
		/*
		当结构体变量fileHead中的成员szTempWord和
		结构体变量fileHeadpTemp中的成员szWord相等的时候，nTag置为0
		如果不相等，指向文件中的下一个成员继续对比
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
	// nTag为0，结构体变量pTemp中的成员nCount++
	if (nTag == 0)
	{
		pChineseChar->nCount++;
	}
	else
	{
		// 如果nTag为不为1，结构体变量pTemp指向结构体变量fileHead
		pChineseChar = fileHead;
		fileHead = (CHINESECHAR*)malloc(sizeof(CHINESECHAR));  // 分配空间存储对应的汉字信息
		// 将该汉字copy到结构体变量对应的成员中
		strcpy(fileHead->szWord, szTempWord);
		fileHead->nCount = 1;	// 次数置为1
		// 指向下一个联系人姓名
		fileHead->next = pChineseChar;
	}
	// 返回fileHead，指向保存汉字信息的结构体
	return fileHead;
}

/*
函数功能：文件指针fpChinese从指向的文件开始处每次读取两个字节保存到临时字符数组中，
	判断其在内存中的数值是否大于127小于0，满足这个条件进行下一步判断，根据判断结果，
	输出最后所有的汉字数据出现的次数，以及所占总数的比率
参数：g_nCharCount 将文件中字符、数字出现的总次数传递过来
返回值：return 0 程序正常退出
*/
int ShowDataChinese(int g_nCharCount)
{
	int i = 0;
	char szTempWord[3];	 // 保存读取到的临时数据
	CHINESECHAR* fileHead = NULL;

	FILE* fpChinese;
	fpChinese = fopen("ChineseChar.bin", "rb");
	// 判断文件是否打开成功
	if (fpChinese == NULL)
	{
		printf("文件读取失败\r\n");
		exit(-1);
	}
	// 将文件指针fp置于该文件指向的文件开始处
	if (fseek(fpChinese, 0, SEEK_SET))
	{
		exit(ferror(fpChinese));
	}
	// 当文件指针fpChinese到达所指向文件的末尾就结束循环
	while (!feof(fpChinese))
	{
		// 判断文件指针fpChinese从指向的文件中成功每次读取两个字节的数据到临时字符数组中
		if (fscanf(fpChinese, "%2s", szTempWord) != EOF)
		{
			// 判断字符数组szTempWord中保存的数据的数值小于0，或大于127就进行下一步操作
			if (szTempWord[i] < 0 || szTempWord[i + 1]>127)
			{
				// 将满足中文编码特性的数值和结构体变量当作参数传递
				fileHead = queryWords(szTempWord, fileHead);
			}
		}
	}

	// 根据queryWords返回结果，输出对应的汉字信息
	CHINESECHAR* pChinese = fileHead;
	g_nChineseCharCount = 0;	// 每次从文件内读取有效的汉字个数时，将其置为0
	while (pChinese != NULL)
	{
		// 读取文件内有效的汉字总个数
		g_nChineseCharCount = g_nChineseCharCount + pChinese->nCount;
		pChinese = pChinese->next;
	}
	// 将结构体指针再次置于文件开头出
	pChinese = fileHead;
	g_nCharCount += g_nChineseCharCount;	// 将有效的汉字个数和有效的英文字符相加
	while (pChinese != NULL)
	{
		// 输出文件内有效的汉字，以及所占总字符数的比率
		printf("%s:\t\t%d次   \t%.2lf%%\r\n", pChinese->szWord, pChinese->nCount,
			(((double)pChinese->nCount) / g_nCharCount) * 100);
		pChinese = pChinese->next;
	}
	// 关闭文件
	if (fpChinese)
	{
		fclose(fpChinese);
		fpChinese = NULL;
	}
	return 0;
}

/*
该函数的功能为：统计文件内的有效的联系人信息中的汉字、字母（分大小写）、数字出现的次数，
	及占总次数的比率
参数：	 无
返回值：return 0 程序正常退出
*/
int ShowFileDataRatio()
{
	int i = 0;
	int j = 0;
	int nTotalCount = 0;	// 大小写字母及数字出现的总次数
	int nLettersLarge[26] = { 0 };	// 用于存储 26 个大写字母
	int nLettersSmall[26] = { 0 };	// 用于存储 26 个小写字母
	int nNumbers[10] = { 0 };		// 用于存储 10 个数字
	int nLetsLarge = 0;		// 遍历内存中存储的26个大写字母
	int nLetsSmall = 0;		// 遍历内存中存储的26个小写字母
	int nNums = 0;		// 遍历内存中存储的10个数字

	FILE* fp = NULL;
	fp = fopen("data.bin", "rb+");
	if (fp == NULL)
	{
		// 判断文件是否打开成功
		printf("文件打开失败，请检查文件是否存在\r\n");
		exit(-1);
	}
	// 为保存联系人信息结构体定义一个变量名
	TAGCONTACTS perInfo;
	// 将文件指针至于文件开始处
	if (fseek(fp, 0, SEEK_SET))
	{
		exit(ferror(fp));
	}
	// 遍历文件中所有的数据
	for (int i = 0; i < g_nFileSize; i++)
	{
		if (fread(&perInfo, sizeof(perInfo), 1, fp) != 1)
		{
			printf("显示字符比率时，文件读取错误\r\n");
			goto EXIT_SHDOFILE;
		}
		// 判断当前数据是否为使用
		if (perInfo.enDataType == usedData)
		{
			for (int j = 0; j < (int)(strlen(perInfo.szName)); j++)
			{
				// 读取文件内当前联系人的姓名
				if (isupper((unsigned char)(perInfo.szName[j])))
				{
					// 判断读取的内容是否时大写英文字母
					nLettersLarge[(perInfo.szName[j]) - 'A']++;
					nTotalCount++;
				}
				else if (isalpha((unsigned char)(perInfo.szName[j])))	
				{
					// 判断读取的内容是否时大小写英文字母
					nLettersSmall[(perInfo.szName[j]) - 'a']++;
					nTotalCount++;
				}
				else if (isdigit((unsigned char)(perInfo.szName[j])))
				{
					// 判断读取到的内容是否是数字
					nNumbers[(perInfo.szName[j]) - '0']++;
					nTotalCount++;
				}
			}
			for (int j = 0; j < (int)(strlen(perInfo.szTel)); j++)
			{
				// 读取文件内当前联系人的电话号码
				if (isupper((unsigned char)(perInfo.szTel[j])))
				{
					// 判断读取的内容是否时大写英文字母
					nLettersLarge[(perInfo.szTel[j]) - 'A']++;
					nTotalCount++;
				}
				else if (isalpha((unsigned char)(perInfo.szTel[j])))
				{
					// 判断读取的内容是否时大写英文字母
					nLettersSmall[(perInfo.szTel[j]) - 'a']++;
					nTotalCount++;
				}
				else if (isdigit((unsigned char)(perInfo.szTel[j])))	
				{
					// 判断读取到的内容是否是数字
					nNumbers[(perInfo.szTel[j]) - '0']++;
					nTotalCount++;
				}
			}
		}
	}
	// 遍历内存中各大写字母出现的次数及所占比率，将内存中大写字母数量大于0的输出
	printf("文件中存储的汉字、字符、数字出现次数及比率情况如下：\r\n");
	printf("字符数据	 出现次数  占总数的比率\r\n");

	g_nCharCount = nTotalCount;	// 将此时的文件内字符串的总数赋值为全局变量g_nCharCount
	ShowDataChinese(g_nCharCount);  // 调用ShowDataChinese显示文件内中文的次数，以及所占比率
	g_nCharCount += g_nChineseCharCount;
	nTotalCount = g_nCharCount;

	for (nLetsLarge = 0; nLetsLarge <= 26; nLetsLarge++)
	{
		if (nLettersLarge[nLetsLarge] > 0)
		{
			printf("%c:\t\t%d次   \t%.2lf%%\r\n", 'A' + nLetsLarge, 
				nLettersLarge[nLetsLarge],
				((double)nLettersLarge[nLetsLarge] / nTotalCount) * 100);
		}
	}
	// 遍历内存中各小写字母出现的次数及所占比率，将内存中小写字母数量大于0的输出
	for (nLetsSmall = 0; nLetsSmall <= 26; nLetsSmall++)
	{
		if (nLettersSmall[nLetsSmall] > 0)
		{
			printf("%c:\t\t%d次   \t%.2lf%%\r\n", 'a' + nLetsSmall, 
				nLettersSmall[nLetsSmall],
				((double)nLettersSmall[nLetsSmall] / nTotalCount) * 100);
		}
	}
	// 遍历内存中各数字出现的次数及所占比率，将内存中数字数量大于0的输出
	for (nNums = 0; nNums <= 9; nNums++)
	{
		if (nNumbers[nNums] > 0)
		{
			printf("%c:\t\t%d次   \t%.2lf%%\r\n", 
				'0' + nNums, nNumbers[nNums],
				((double)nNumbers[nNums] / nTotalCount) * 100);
		}
	}

EXIT_SHDOFILE:
	if (fp)
	{
		fclose(fp);	// 关闭文件
		fp = NULL;
	}
	return 0;
}

/*
该函数的功能为：显示文件内数据空间的占用情况，U(使用)，F(未使用)
参数：无
返回值：return 0 程序正常退出
*/
int ShowMemoryFootprint()
{
	int nCount = 0;
	FILE* fp = NULL;
	fp = fopen("data.bin", "rb+");
	if (fp == NULL)
	{
		// 判断文件是否打开成功
		printf("文件打开失败，请检查文件是否存在\r\n");
		exit(-1);
	}
	// 为保存联系人信息结构体定义一个变量名
	TAGCONTACTS perInfo;
	// 将文件指针至于文件开始处
	if (fseek(fp, 0, SEEK_SET))
	{
		exit(ferror(fp));
	}
	printf("当前文件中内存的使用情况如下U(使用)，F(未使用)\r\n");
	for (int i = 0; i < g_nFileSize; i ++)
	{
		// 数据读取失败，退出程序，关闭文件
		if (fread(&perInfo, sizeof(perInfo), 1, fp) != 1)
		{
			goto EXIT_SHDOFILE;
		}
		// 将已使用的文件数据表示为U
		if (perInfo.enDataType == usedData)
		{
			// printf("U");
			int nUsed = 0;	// 用于显示文件内有效的联系人信息
			while (nUsed < 52)
			{
				printf("U");
				nUsed++;
			}
			printf("\r\n");
		}
		else
		{
			// 将未使用的文件数据表示为F
			int nUnused = 0;		// 用于显示文件内空的联系人信息
			while (nUnused < 52)
			{
				printf("F");
				nUnused++;
			}
			printf("\r\n");
			// printf("F");
			nCount++;
			if (nCount > 100)	  // 未使用数量超过5000的时候用 ...... 表示
			{
				printf("......\r\n");
				break;
			}
		}
	}
EXIT_SHDOFILE:
	if (fp)
	{
		fclose(fp);	// 退出程序关闭文件
		fp = NULL;
	}
	return 0;
}

/*
函数功能：在不存储数据时，对将文件中存在的空数据进行碎片整理
参数：无
返回值：return 0 程序正常退出
*/
int Defragmentation()
{
	FILE* fp = NULL;
	FILE* fpTemp = NULL;
	fp = fopen("data.bin", "rb+");	// 打开保存联系人信息的文件
	fpTemp = fopen("newdata.bin", "wb+");	// 用于存储联系人信息的临时文件
	if (fp == NULL)
	{
		// 判断文件是否打开成功
		printf("文件data.bin打开失败\r\n");
		exit(-1);
	}
	if (fpTemp == NULL)
	{
		// 判断文件是否打开成功
		printf("文件newdata.bin打开失败\r\n");
		exit(-1);
	}
	// 为保存联系人信息结构体定义一个变量名，并赋初值
	TAGCONTACTS perInfo = {
			unusedData, 
			0, 
			'\0', '\0'
	};
	// 为临时保存联系人信息结构体定义一个变量名
	TAGCONTACTS perInfoTemp;

	for (int i = 0; i < g_nFileSize; i++)
	{
		// 为fpTemp指向的文件每次读入sizeof(perInfo)大的数据到文件中去(初始化内存)
		if (fwrite(&perInfo, sizeof(perInfo), 1, fpTemp) != 1)
		{
			printf("数据读取失败，请检查文件是否有效\r\n");
			goto EXIT_SHDOFILE;
		}
	}
	// 将文件指针至于文件开始处
	if (fseek(fpTemp, 0, SEEK_SET))
	{
		exit(ferror(fp));
	}

	for (int i = 0; i < g_nFileSize; i += sizeof(fp))
	{
		// 从文件指针fp所指向的文件内读取sizeof(perInfoTemp)字节数据到perInfoTemp中去
		if (fread(&perInfoTemp, sizeof(perInfoTemp), 1, fp) != 1)
		{
			goto EXIT_SHDOFILE;
		}
		// 判断当前文件中的数据是否使用
		if (perInfoTemp.enDataType == usedData)
		{
			// 将数据写入到fpTemp指向的文件内
			if (fwrite(&perInfoTemp, sizeof(perInfoTemp), 1, fpTemp) != 1)
			{
				printf("数据读取失败\r\n");
				goto EXIT_SHDOFILE;
			}
		}
	}

EXIT_SHDOFILE:
	if (fp)
	{
		fclose(fp);	// 关闭文件指针fp指向的文件
		fp = NULL;
	}
	if (fpTemp != NULL)
	{
		fclose(fpTemp);	// 关闭文件指针fpTemp指向的文件
		fpTemp = NULL;
	}
	// 在关闭文件后将源文件进行删除
	if (unlink("data.bin") == 0)	// 删除原文件
	{
		// printf("源文件删除成功\r\n");
	}
	// 将用于存储碎片整理后的临时文件更改名字
	if (rename("newdata.bin", "data.bin") == 0)
	{
		printf("整理碎片成功!\r\n");
	}
	return 0;
}