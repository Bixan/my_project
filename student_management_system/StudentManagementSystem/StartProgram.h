#pragma once
/*=======================================================
*		o(*￣▽￣*)o
*		FileName：StartProgram.h -- 程序启动文件
*		Start Date：2020/06/11
*		Last UpDate：2020/06/17
*=======================================================*/


#include <stdlib.h>
#include <time.h>
#include "Menu.h"
#include "MyCAVLTree.h"

using namespace std;

void StartProgram()
{
	CAVLTree<int> StuAVL;
#if 1
	cout << "数据读取中...";
	clock_t begin0 = clock();
	CAVLTree<int>::ReadAVLTreeStuDataToInFile(StuAVL); // 读入学生数据
	clock_t end0 = clock();

	double TimeComsumption0 = ((double)(end0)-(double)(begin0)) / CLOCKS_PER_SEC;
	cout << endl;
	cout << "数据读取成功，耗时：" << TimeComsumption0 << endl;
	system("pause");
#endif // 测试读取文件


#if 0
	clock_t begin1 = clock();
	for (int i = 1; i <= 1000000; i++)
	{
		if (i % 2 == 0)
		{
			StuAVL.Insert(i, "张三三", 2000, 6, 1);
		}
		else if (i % 3 == 0)
		{
			StuAVL.Insert(i, "李小莉", 1995, 12, 2);
		}
		else if (i % 5 == 0)
		{
			StuAVL.Insert(i, "王五", 2000, 11, 1);
		}
		else if (i % 7 == 0)
		{
			StuAVL.Insert(i, "蔡某某", 1997, 8, 1);
		}
		else
		{
			StuAVL.Insert(i, "孙小明", 2005, 5, 1);
		}
	}
	StuAVL.WirteAVLTreeStuDataToInFile(); // 将树中的数据写入文件
	clock_t end1 = clock();
	double TimeComsumption1 = ((double)(end1)-(double)(begin1)) / CLOCKS_PER_SEC;
	cout << "100万条数据写入成功，耗时：" << TimeComsumption1 << endl;

	cout << "当前学生数量共计：" << StuAVL.GetNodeSize() << "位" << endl;

	system("pause");
#endif // 测试添加100W数据

	while (true)
	{
		int nStuID = 0;	// 学生ID
		CString szStuName = nullptr; // 学生姓名
		int nYear = 0; // 学生出生年份
		int nMonth = 0; // 学生出生月份
		int nSex = 0; // 学生性别

		int nInputNumber = 0; // 记录用户输入的数字
		int nFlag = 0; // 记录scanf返回值

		system("cls");
		Menu();
		fflush(stdin);
		nFlag = scanf_s("%d", &nInputNumber); // 输入数字返回1，反之返回0
		if (nFlag == 0)
		{
			cout << "你的输入有误，程序即将退出..." << endl;
			Sleep(2000);
			break;
		}
		else
		{
			if (nInputNumber == 0)
			{
				cout << "程序正在退出..." << endl; // 输入数据为0，程序退出
				StuAVL.RemoveCopyStuData();
				Sleep(2000);
				break;
			}
			if (nInputNumber < 0 || nInputNumber > 4)
			{
				cout << "你的输入有误，程序即将退出..." << endl;
				Sleep(2000);
				break;
			}
			else
			{
				switch (nInputNumber)
				{
				case 1:
				{
					cout << "请输入要存储的学生ID：";
					fflush(stdin);
					scanf_s("%d", &nStuID);
					cout << "请输入要存储的学生姓名：";
					fflush(stdin);
					cin >> szStuName;
					cout << "请输入要存储的学生出生年份：";
					fflush(stdin);
					cin >> nYear;
					cout << "请输入要存储的学生出生月份：";
					fflush(stdin);
					cin >> nMonth;
					cout << "请输入要存储的学生性别(1/男，2/女，3/其它，4/未知)：";
					fflush(stdin);
					cin >> nSex;
					if (nStuID < 2147483647 && nStuID > 0 )
					{
						if (!StuAVL.Insert(nStuID, szStuName, nYear, nMonth, nSex))
						{
							system("pause");
							break;
						}
						cout << "ID：" 
							<< nStuID 
							<< " 姓名：" 
							<< szStuName 
							<< " 的学生相关信息添加成功" 
							<< endl;
						cout << "当前学生数量共计：" 
							<< StuAVL.GetNodeSize() 
							<< " 名" 
							<< endl;
						StuAVL.WirteAVLTreeStuDataToInFile();
						system("pause");
					}
					else
					{
						cout << "你输入的数值不在数据文件的ID范围内，请重新输入" << endl;
						system("pause");
					}
				}
				break;
				case 2:
				{
					cout << "请输入你要删除的学生ID：";
					fflush(stdin);
					scanf_s("%d", &nStuID);
					if (nStuID < 2147483647 && nStuID > 0)
					{
						if (!StuAVL.Remove(nStuID))
						{
							system("pause");
							break;
						}
						cout << "ID：" 
							<< nStuID 
							<< " 的学生相关信息删除成功" 
							<< endl;
						cout << "当前学生数量共计：" 
							<< StuAVL.GetNodeSize() 
							<< " 名" 
							<< endl;
						StuAVL.WirteAVLTreeStuDataToInFile(); // 更新数据文件
						system("pause");
					}
					else
					{
						cout << "你输入的数值不在数据文件的ID范围内，请重新输入" << endl;
						system("pause");
					}
				}
				break;
				case 3:
				{
					cout << "请输入你要修改学生对应的ID：";
					fflush(stdin);
					scanf_s("%d", &nStuID);
					if (nStuID < 2147483647 && nStuID > 0)
					{
						if (!StuAVL.Change(nStuID))
						{
							system("pause");
							break;
						}
						cout << "ID：" 
							<< nStuID 
							<< " 的学生相关信息修改成功" 
							<< endl;
						cout << "当前学生数量共计：" 
							<< StuAVL.GetNodeSize() 
							<< " 名" 
							<< endl;
						StuAVL.WirteAVLTreeStuDataToInFile();
						system("pause");
					}
					else
					{
						cout << "你输入的数值不在数据文件的ID范围内，请重新输入" << endl;
						system("pause");
					}
				}
				break;
				case 4:
				{
					while (true)
					{
						system("cls");
						InquireMenu();
						int nFindStuID = 0; // 需要查询学生数据所对应的ID
						CString szFindStuName = nullptr; // 需要查询学生数据所对应的学生姓名
						int nInputNumber1 = 0; // 记录用户输入的数字
						int nFlag1 = 0;	// 记录scanf返回值
						
						fflush(stdin);
						nFlag1 = scanf_s("%d", &nInputNumber1);	// 输入数字返回1，反之返回0
						if (nFlag1 == 0)
						{
							cout << "你的输入有误，程序即将退出..." << endl;
							break;
						}
						else
						{
							if (nInputNumber1 == 0)
							{
								cout << "查询程序正在退出..." << endl; // 输入数据为0，程序退出
								Sleep(2000);
								break;
							}
							if (nInputNumber1 < 0 || nInputNumber1 > 2)
							{
								cout << "你的输入有误，程序即将退出..." << endl;
								Sleep(2000);
								break;
							}
							else
							{
								switch (nInputNumber1)
								{
								case 1:
								{
									cout << "请输入你要查询学生信息对应的ID：";
									fflush(stdin);
									scanf_s("%d", &nFindStuID);
									if (nFindStuID < 2147483647 && nFindStuID > 0)
									{
										StuAVL.Find(nFindStuID);
										system("pause");
									}
									else
									{
										cout << "你输入的数值不在数据文件的ID范围内，请重新输入" << endl;
										system("pause");
									}
								}
								break;
								case 2:
								{
									cout << "请输入你要查询的学生姓名：";
									fflush(stdin);
									cin >> szFindStuName;
									StuAVL.FindStuName(szFindStuName);
									system("pause");
								}
								break;
								default:
									cout << "你输入的信息有误，请重新输入" << endl;
									break;
								}
							}
						}
					}
				}
				break;
				default:
					cout << "你输入的信息有误，请重新输入" << endl;
					break;
				}
			}
		}
	}

	system("pause");
}