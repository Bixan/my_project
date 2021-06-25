#define _CRT_SECURE_NO_DEPRECATE
#include "FunRealization.h"

// 指定数组的大小空间为：1024 * 1024 字节
char g_szBuf[1024 * 1024] = { 0 };

// 记录内存中存储的字符串数量
int g_nCount = 0;

/*
函数功能：启动程序
参数：无
返回值：return 0 程序退出成功
*/
int StartProgram()
{
	int nNumber = 0; // 记录用户输入的数字
	int nStatus = 0; // 记录scanf()的返回值
	while (1)
	{
		system("cls");
		ProBeginMenu();	// 显示功能菜单
		rewind(stdin);
		nStatus = scanf("%d", &nNumber); // 输入是数字返回1，反之返回0
		// 根据scanf返回值作判定，如果输入的数值不是一个整数，提示相应的错误信息
		if (nStatus == 0)
		{
			printf("您的输入有误，程序即将退出...\r\n");
			Sleep(2000);
			break;
		}
		else
		{
			// 用户输入数值为 0，程序退出
			if (nNumber == 0)
			{
				printf("程序正在退出...\r\n");
				Sleep(2000);
				break;
			}
			// 用户输入的数值需要在0~8区间内，输入的数值超过该区间，系统退出
			if (nNumber < 0 || nNumber > 8)
			{
				printf("您的输入有误，程序即将退出...\r\n");
				Sleep(2000);
				break;
			}
			else
			{
				system("cls");
				switch (nNumber)
				{
				case 1:
					// 添加字符串
					IncreaseString();
					break;
				case 2:
					// 删除指定的字符串
					DeleteString();
					break;
				case 3:
					// 修改指定的字符串
					ReviseString();
					break;
				case 4:
					// 查询字符串
					InquireString();
					break;
				case 5:
					// 显示内存中所有的字符串
					ShowString();
					break;
				case 6:
					// 显示内存中字符串出现的次数及比率
					ShowStringRatio();
					break;
				case 7:
					// 显示内存使用分布
					ShowMemoryFootprint();
					break;
				case 8:
					// 碎片整理
					Defragmentation();
					// 显示内存使用分布
					ShowMemoryFootprint();
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
void ProBeginMenu()
{
	printf("\r\n\n");
	printf("\t******************  字符串存储程序  ******************\n");
	printf("\t*\t0.退出程序\r\n");
	printf("\t*\t1.添加字符串\r\n");
	printf("\t*\t2.删除字符串\r\n");
	printf("\t*\t3.修改字符串\r\n");
	printf("\t*\t4.查询字符串\r\n");
	printf("\t*\t5.显示内存中存储的所有字符串\r\n");
	printf("\t*\t6.统计系统内各字符出现次数及比率\r\n");
	printf("\t*\t7.显示内存占用情况\r\n");
	printf("\t*\t8.碎片整理\r\n");
	printf("\t******************************************************\n\n");
	printf("\r\n");
	printf("请输入 0~8 进行相应功能的操作：");
}

/*
该函数的功能为：字符串添加功能
返回值：
	return 0 字符串添加成功
	return -1字符串添加失败
参数：	无
*/
int IncreaseString()
{
	int i = 0;
	int nCount = 1;	// 为字符串添加编号
	char szTempString[100] = { 0 };	// 保存用户输入的临时数据
	printf("请输入你要存储的字符串：");
	rewind(stdin);
	fgets(szTempString, 99, stdin);
	// 处理字符串后面的 '\n'
	if (szTempString[strlen(szTempString) - 1] == '\n')
	{
		szTempString[strlen(szTempString) - 1] = '\0';
	}
	for (i = 0; i < sizeof(g_szBuf); i += (BLOCK_SIZE + 1))
	{
		// 判断当前空间是否使用
		if (g_szBuf[i] == UNUSED_BLOCKSPACE)
		{
			// 将字符串存储到内存中后，做标记，将此块空间标记为已使用
			g_szBuf[i] = USED_BLOCKSPACE;

			if (g_szBuf[i + 1] == g_nCount)
			{
				// 判断当前块的编号位置是否和内存中已经存在字符串的总编号是否相等
				g_nCount = g_nCount + 1;
				g_szBuf[i + 1] = g_nCount;
			}
			if (g_szBuf[i + 1] < g_nCount)
			{
				// 如果当前块的编号位置小于总编号，就说明该位置上的字符串已被删除
				g_nCount = nCount;
				g_szBuf[i + 1] = g_nCount;
			}
			strcpy(&g_szBuf[i + 2], szTempString); // 将输入的字符串拷贝到指定的内存块中去
			printf("字符串 %s 添加成功\r\n", &g_szBuf[i + 2]);
			return 0;
		}
		nCount = nCount + 1; // 循环一次，计数加一
	}
	printf("字符串 %s 添加失败\r\n", szTempString);
	return -1;
}

/*
该函数的功能为：字符串删除功能
返回值：
	return 0 字符串删除成功
	return -1 字符串删除失败
参数：无
*/
int DeleteString()
{
	int i = 0;
	char szTempString[100] = { 0 };	// 保存用户输入的临时数据
	ShowString(); // 显示内存中已存在的字符串
	printf("请输入您要删除的字符串：");
	rewind(stdin);
	fgets(szTempString, 99, stdin);
	// 处理字符串后面的 '\n'
	if (szTempString[strlen(szTempString) - 1] == '\n')
	{
		szTempString[strlen(szTempString) - 1] = '\0';
	}
	for (i = 0; i < sizeof(g_szBuf); i += (BLOCK_SIZE + 1))
	{
		// 判断当前空间是否使用
		if (g_szBuf[i] == USED_BLOCKSPACE)
		{
			// 判断要删除的字符串和内存中存储的所有字符串作比较，判断是否相等
			if (strcmp(&g_szBuf[i + 2], szTempString) == 0)
			{
				// 判断条件成立后，将此块空间标记为未使用
				g_szBuf[i] = UNUSED_BLOCKSPACE;
				// 将该内存块中记录字符串编号的值至0
				g_szBuf[i + 1] = 0;
				printf("经过查询，你要删除的字符串 %s 在该内存中\r\n", &g_szBuf[i + 2]);
				printf("字符串 %s 正在删除中...\r\n", &g_szBuf[i + 2]);
				Sleep(2000);
				printf("字符串 %s 删除成功\r\n", &g_szBuf[i + 2]);
				return 0;
			}
		}
	}
	printf("删除失败，经过查询，字符串 %s 不在改内存中\r\n", szTempString);
	return -1;
}

/*
该函数的功能为：字符串修改功能
返回值：
	return 0  字符串修改成功
	return -1 字符串修改失败
参数：无
*/
int ReviseString()
{
	int i = 0;
	char szTempString[100] = { 0 };	// 保存用户输入的临时数据
	ShowString(); // 显示字符串菜单
	printf("请输入你要修改的字符串：");
	rewind(stdin);
	fgets(szTempString, 99, stdin);
	// 处理字符串后面的'\n'
	if (szTempString[strlen(szTempString) - 1] == '\n')
	{
		szTempString[strlen(szTempString) - 1] = '\0';
	}
	for (i = 0; i < sizeof(g_szBuf); i += (BLOCK_SIZE + 1))
	{
		// 遍历内存块，判断内存块的使用情况
		if (g_szBuf[i] == USED_BLOCKSPACE)
		{
			/*
			如果检测到当前块使用，就将输入的字符串和内存块中存储的字符串进行对比
			如果相等，执行块内的语句
			*/
			if (strcmp(&g_szBuf[i + 2], szTempString) == 0)
			{
				printf("经过查询，你要修改的字符串在该内存中，请输入你要修改后的内容：");
				rewind(stdin);
				fgets(szTempString, 99, stdin);	// 输入要修改后的字符串
				// 同样处理字符串后面的 '\n'
				if (szTempString[strlen(szTempString) - 1] == '\n')
				{
					szTempString[strlen(szTempString) - 1] = '\0';
				}
				// 将更改后的字符串全部内容覆盖到当前块的内存区去
				strcpy(&g_szBuf[i + 2], szTempString);
				printf("字符串修改中...\r\n");
				Sleep(2000);
				printf("字符串修改成功，修改后的字符串为：%s\r\n", &g_szBuf[i + 2]);
				return 0;
			}
		}
	}
	printf("更改失败，经过查询，你要修改的字符串不在该内存中\r\n");
	return -1;
}

/*
该函数的功能为：字符串查询菜单
返回值：无
参数：无
*/
void InquireMenu()
{
	ShowString();	// 显示内存中存储的字符串信息
	printf("\r\n\n");
	printf("*********************  输入对应的编号进行相应的操作  *********************\n");
	printf("*\t0.退出查询\r\n");
	printf("*\t1.精准查询\r\n");
	printf("*\t2.模糊查询\r\n");
	printf("**************************************************************************\n");
}

/*
该函数的功能为：查询系统内的字符串，如果存在输出，反之提示内存中不存在
返回值：
	return 0 程序正常退出
参数：	无
*/
int InquireString()
{
	int nInNumber = 0; // 记录用户输入的数值
	int nRetValue = 0; // 记录scanf函数的返回值
	while (1)
	{
		system("cls");
		InquireMenu(); // 显示功能菜单
		printf("请输入 0~2 进行相应功能的操作：");
		rewind(stdin);
		nRetValue = scanf("%d", &nInNumber);
		// 根据scanf返回值最相应的判断
		if (nRetValue == 0)
		{
			printf("您输入的输入有误，查询程序即将退出...\r\n");
			Sleep(2000);
			break;
		}
		else
		{
			// 输入0退出程序
			if (nInNumber == 0)
			{
				printf("查询程序即将退出...\r\n");
				Sleep(2000);
				break;
			}
			// 输入数值需要在0~2区间
			if (nInNumber < 0 || nInNumber > 2)
			{
				printf("您输入的信息有误，请输入正确的编号\r\n");
				break;
			}
			else
			{
				switch (nInNumber)
				{
				case 1:
					// 精确查找
					StrPerciseInquire();
					break;
				case 2:
					// 模糊查询
					StrFuzzyInquire();
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
该函数的功能为：查询内存中是否存在所要查询的字符串，由于是精确查询，
	所以需要用户输入要查询的字符串全部字符
返回值：
	return 0 查询成功
参数：无
*/
int StrPerciseInquire()
{
	int i = 0;
	char szTempString[100] = { 0 };	// 保存用户输入的字符串，该数组的值是可以随之变得的
	printf("请输入你要查询的字符串：");
	rewind(stdin);
	fgets(szTempString, 99, stdin);
	// 清除输入的字符串结尾的换行符
	if (szTempString[strlen(szTempString) - 1] == '\n')
	{
		szTempString[strlen(szTempString) - 1] = '\0';
	}
	printf("通过精确查询，在内存中找到了你要查询的字符串有：\r\n");
	for (i = 0; i < sizeof(g_szBuf); i += (BLOCK_SIZE + 1))
	{
		if (g_szBuf[i] == USED_BLOCKSPACE)
		{
			// 判断当前内存块中的数据和用户输入的字符串是否相等，相等就输出对应的数据
			if (strcmp(&g_szBuf[i + 2], szTempString) == 0)
			{
				printf("%d: %s\r\n", g_szBuf[i + 1], &g_szBuf[i + 2]);
			}
		}
	}
	return 0;
}

/*
该函数的功能为：模糊查询，输出在内存中含有用户输入指定字符的字符串
返回值：
	return 0 查询成功
参数：无
*/
int StrFuzzyInquire()
{
	int i = 0;
	char* strPosition = NULL; // 用于记录对应的字符在字符串中出现的位置
	char szTempString[100] = { 0 }; // 保存用户输入的字符串，该数组的值时可以随之变得的
	printf("请输入你要查询的字符串：");
	rewind(stdin);
	fgets(szTempString, 99, stdin);
	// 清除输入的字符串结尾的换行符
	if (szTempString[strlen(szTempString) - 1] == '\n')
	{
		szTempString[strlen(szTempString) - 1] = '\0';
	}
	printf("通过模糊查询，内存中存在 字符/字符串 %s 的字符串数据有：\r\n", szTempString);
	for (i = 0; i < sizeof(g_szBuf); i += (BLOCK_SIZE + 1))
	{
		// 判断当前内存块是否使用
		if (g_szBuf[i] == USED_BLOCKSPACE)
		{
			// 记录输出字符在内存块中出现的位置，并将包含该字符串的数据进行输出
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
该函数的功能为：显示内存中已经存储的字符串
返回值：无
参数：无
*/
void ShowString()
{
	int i = 0;
	printf("******************** 当前内存中已保存的所有字符串如下 *******************\r\n");
	for (i = 0; i < sizeof(g_szBuf); i += (BLOCK_SIZE + 1))
	{
		// 遍历内存中存储的所有数据，并将其输出
		if (g_szBuf[i] == USED_BLOCKSPACE)
		{
			printf("%d: %s\r\n", g_szBuf[i + 1], &g_szBuf[i + 2]);
		}
	}
	printf("*************************************************************************\r\n");
}

/*
该函数的功能为：统计输入的字符串内的字母（分大小写）、数字出现的次数，及占总次数的比率
返回值：无
参数：无
*/
void ShowStringRatio()
{
	int i = 0;
	int j = 0;
	int nTotalCount = 0; // 大小写字母及数字出现的总次数
	int nLettersLarge[26] = { 0 }; // 用于存储 26 个大写字母
	int nLettersSmall[26] = { 0 }; // 用于存储 26 个小写字母
	int nNumbers[10] = { 0 };	// 用于存储 10 个数字
	int nLetsLarge = 0;	// 遍历内存中存储的26个大写字母
	int nLetsSmall = 0;	// 遍历内存中存储的26个小写字母
	int nNums = 0; // 遍历内存中存储的10个数字

	for (i = 0; i < sizeof(g_szBuf); i += (BLOCK_SIZE + 1))
	{
		if (g_szBuf[i] == USED_BLOCKSPACE)
		{
			for (j = 0; j < (int)strlen(&g_szBuf[i + 1]); j++)
			{
				if (isupper((unsigned char)g_szBuf[i + j + 1]))
				{
					// 扫描内存中所有数据，将大写字母保存到nLettersLarge内
					nLettersLarge[g_szBuf[i + j + 1] - 'A']++;
					// 总计数加1
					nTotalCount++;
				}
				else if (isalpha((unsigned char)g_szBuf[i + j + 1]))
				{
					// 扫描内存中所有数据，将小写字母保存到nLettersSmall内
					nLettersSmall[g_szBuf[i + j + 1] - 'a']++;	
					// 总计数加1
					nTotalCount++;
				}
				else if (isdigit((unsigned char)g_szBuf[i + j + 1]))
				{
					// 扫描内存中所有数据，将数字保存到nNumbers内
					nNumbers[g_szBuf[i + j + 1] - '0']++;
					// 总计数加1
					nTotalCount++;	
				}
			}
		}
	}
	printf("******************* 内存中存储的字符、数字出现次数及比率 *******************\r\n");
	printf("字符及数字  出现次数  占总数的比率\r\n");

	// 遍历内存中各大写字母出现的次数及所占比率，将内存中大写字母数量大于0的输出
	for (nLetsLarge = 0; nLetsLarge <= 26; nLetsLarge++)
	{
		if (nLettersLarge[nLetsLarge] > 0)
		{
			printf("   %c：\t\t%d  \t%.2lf%%\r\n", 
				'A' + nLetsLarge, 
				nLettersLarge[nLetsLarge],
				((double)nLettersLarge[nLetsLarge] / nTotalCount) * 100);
		}
	}

	// 遍历内存中各小写字母出现的次数及所占比率，将内存中小写字母数量大于0的输出
	for (nLetsSmall = 0; nLetsSmall <= 26; nLetsSmall++)
	{
		if (nLettersSmall[nLetsSmall] > 0)
		{
			printf("   %c：\t\t%d  \t%.2lf%%\r\n", 
				'a' + nLetsSmall, 
				nLettersSmall[nLetsSmall],
				((double)nLettersSmall[nLetsSmall] / nTotalCount) * 100);
		}
	}

	// 遍历内存中各数字出现的次数及所占比率，将内存中数字数量大于0的输出
	for (nNums = 0; nNums <= 9; nNums++)
	{
		if (nNumbers[nNums] > 0)
		{
			printf("   %c：\t\t%d  \t%.2lf%%\r\n", 
				'0' + nNums, 
				nNumbers[nNums],
				((double)nNumbers[nNums] / nTotalCount) * 100);
		}
	}
	printf("****************************************************************************\r\n");
}

/*
该函数的功能为：显示内存空间的占用情况，U(使用)，F(未使用)
返回值：无
参数：无
*/
void ShowMemoryFootprint()
{
	int i = 0;
	printf("\n\n************************* 当前内存中内存块的使用情况如下 **************************\r\n");
	printf("*************************        U(使用)，F(未使用)      **************************\r\n\n");

	for (i = 0; i < sizeof(g_szBuf); i += (BLOCK_SIZE + 1))
	{
		// 遍历内存，查看内存块是否使用
		if (g_szBuf[i] == USED_BLOCKSPACE)
		{
			printf("U"); // 将已使用的内存块用字符 U 表示
		}
		else
		{
			printf("F"); // 将未使用的内存块用字符 F 表示
		}
	}
	printf("\n\n*****************************************************************************\r\n\n");
}

/*
该函数的功能为：对内存中的空位置进行碎片整理，根据内存中存储的数据对其重新编号
返回值：无
参数：无
*/
void Defragmentation()
{
	int i = 0;
	int nNoBlock = 0; // 记录未使用的块
	int nCount = 1;	// 记录循环次数
	for (i = 0; i < sizeof(g_szBuf); i += (BLOCK_SIZE + 1))
	{
		// 遍历内存中当前块是否使用
		if (g_szBuf[i] == USED_BLOCKSPACE)
		{
			// 将块的开始标记和临时块的开始标记作比较
			if (i != nNoBlock)
			{
				/*
				遍历内存中的数据，如果没使用块后面存在使用过的块，
				就将使用的块的内存数据全部拷贝到前面没使用的内存块中
				*/
				memcpy(&g_szBuf[nNoBlock], &g_szBuf[i], (BLOCK_SIZE + 1));
				g_szBuf[i] = UNUSED_BLOCKSPACE;	// 接着就将这块内存标记为未使用
			}
			nNoBlock = nNoBlock + (BLOCK_SIZE + 1);	// 更改nTemp的数值
		}
	}
	// 对内存中剩下的数据进行重新编号
	for (i = 0; i < sizeof(g_szBuf); i += (BLOCK_SIZE + 1))
	{
		// 判断当前空间是否使用
		if (g_szBuf[i] == USED_BLOCKSPACE)
		{
			// 当前块的编号和内存中已经存在字符串的总编号相等，说明第一块没有数据存储
			if (g_szBuf[i + 1] == g_nCount)
			{
				g_nCount = g_nCount + 1;
				g_szBuf[i + 1] = g_nCount;
			}
			// 如果当前块的编号位置大于或大于总编号，就将nCount的值赋值到对应的编号位置上去
			else
			{
				g_nCount = nCount;
				g_szBuf[i + 1] = g_nCount;
			}
		}
		// 记录循环次数，循环一次，计数加一
		nCount = nCount + 1;
	}
	printf("内存碎片正在整理中...\r\n");
	Sleep(2000);
	printf("内存碎片整理成功\r\n");
}