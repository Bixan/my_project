#pragma once
/*=======================================================
*		o(*������*)o
*		FileName��StartProgram.h -- ���������ļ�
*		Start Date��2020/06/11
*		Last UpDate��2020/06/17
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
	cout << "���ݶ�ȡ��...";
	clock_t begin0 = clock();
	CAVLTree<int>::ReadAVLTreeStuDataToInFile(StuAVL); // ����ѧ������
	clock_t end0 = clock();

	double TimeComsumption0 = ((double)(end0)-(double)(begin0)) / CLOCKS_PER_SEC;
	cout << endl;
	cout << "���ݶ�ȡ�ɹ�����ʱ��" << TimeComsumption0 << endl;
	system("pause");
#endif // ���Զ�ȡ�ļ�


#if 0
	clock_t begin1 = clock();
	for (int i = 1; i <= 1000000; i++)
	{
		if (i % 2 == 0)
		{
			StuAVL.Insert(i, "������", 2000, 6, 1);
		}
		else if (i % 3 == 0)
		{
			StuAVL.Insert(i, "��С��", 1995, 12, 2);
		}
		else if (i % 5 == 0)
		{
			StuAVL.Insert(i, "����", 2000, 11, 1);
		}
		else if (i % 7 == 0)
		{
			StuAVL.Insert(i, "��ĳĳ", 1997, 8, 1);
		}
		else
		{
			StuAVL.Insert(i, "��С��", 2005, 5, 1);
		}
	}
	StuAVL.WirteAVLTreeStuDataToInFile(); // �����е�����д���ļ�
	clock_t end1 = clock();
	double TimeComsumption1 = ((double)(end1)-(double)(begin1)) / CLOCKS_PER_SEC;
	cout << "100��������д��ɹ�����ʱ��" << TimeComsumption1 << endl;

	cout << "��ǰѧ���������ƣ�" << StuAVL.GetNodeSize() << "λ" << endl;

	system("pause");
#endif // �������100W����

	while (true)
	{
		int nStuID = 0;	// ѧ��ID
		CString szStuName = nullptr; // ѧ������
		int nYear = 0; // ѧ���������
		int nMonth = 0; // ѧ�������·�
		int nSex = 0; // ѧ���Ա�

		int nInputNumber = 0; // ��¼�û����������
		int nFlag = 0; // ��¼scanf����ֵ

		system("cls");
		Menu();
		fflush(stdin);
		nFlag = scanf_s("%d", &nInputNumber); // �������ַ���1����֮����0
		if (nFlag == 0)
		{
			cout << "����������󣬳��򼴽��˳�..." << endl;
			Sleep(2000);
			break;
		}
		else
		{
			if (nInputNumber == 0)
			{
				cout << "���������˳�..." << endl; // ��������Ϊ0�������˳�
				StuAVL.RemoveCopyStuData();
				Sleep(2000);
				break;
			}
			if (nInputNumber < 0 || nInputNumber > 4)
			{
				cout << "����������󣬳��򼴽��˳�..." << endl;
				Sleep(2000);
				break;
			}
			else
			{
				switch (nInputNumber)
				{
				case 1:
				{
					cout << "������Ҫ�洢��ѧ��ID��";
					fflush(stdin);
					scanf_s("%d", &nStuID);
					cout << "������Ҫ�洢��ѧ��������";
					fflush(stdin);
					cin >> szStuName;
					cout << "������Ҫ�洢��ѧ��������ݣ�";
					fflush(stdin);
					cin >> nYear;
					cout << "������Ҫ�洢��ѧ�������·ݣ�";
					fflush(stdin);
					cin >> nMonth;
					cout << "������Ҫ�洢��ѧ���Ա�(1/�У�2/Ů��3/������4/δ֪)��";
					fflush(stdin);
					cin >> nSex;
					if (nStuID < 2147483647 && nStuID > 0 )
					{
						if (!StuAVL.Insert(nStuID, szStuName, nYear, nMonth, nSex))
						{
							system("pause");
							break;
						}
						cout << "ID��" 
							<< nStuID 
							<< " ������" 
							<< szStuName 
							<< " ��ѧ�������Ϣ��ӳɹ�" 
							<< endl;
						cout << "��ǰѧ���������ƣ�" 
							<< StuAVL.GetNodeSize() 
							<< " ��" 
							<< endl;
						StuAVL.WirteAVLTreeStuDataToInFile();
						system("pause");
					}
					else
					{
						cout << "���������ֵ���������ļ���ID��Χ�ڣ�����������" << endl;
						system("pause");
					}
				}
				break;
				case 2:
				{
					cout << "��������Ҫɾ����ѧ��ID��";
					fflush(stdin);
					scanf_s("%d", &nStuID);
					if (nStuID < 2147483647 && nStuID > 0)
					{
						if (!StuAVL.Remove(nStuID))
						{
							system("pause");
							break;
						}
						cout << "ID��" 
							<< nStuID 
							<< " ��ѧ�������Ϣɾ���ɹ�" 
							<< endl;
						cout << "��ǰѧ���������ƣ�" 
							<< StuAVL.GetNodeSize() 
							<< " ��" 
							<< endl;
						StuAVL.WirteAVLTreeStuDataToInFile(); // ���������ļ�
						system("pause");
					}
					else
					{
						cout << "���������ֵ���������ļ���ID��Χ�ڣ�����������" << endl;
						system("pause");
					}
				}
				break;
				case 3:
				{
					cout << "��������Ҫ�޸�ѧ����Ӧ��ID��";
					fflush(stdin);
					scanf_s("%d", &nStuID);
					if (nStuID < 2147483647 && nStuID > 0)
					{
						if (!StuAVL.Change(nStuID))
						{
							system("pause");
							break;
						}
						cout << "ID��" 
							<< nStuID 
							<< " ��ѧ�������Ϣ�޸ĳɹ�" 
							<< endl;
						cout << "��ǰѧ���������ƣ�" 
							<< StuAVL.GetNodeSize() 
							<< " ��" 
							<< endl;
						StuAVL.WirteAVLTreeStuDataToInFile();
						system("pause");
					}
					else
					{
						cout << "���������ֵ���������ļ���ID��Χ�ڣ�����������" << endl;
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
						int nFindStuID = 0; // ��Ҫ��ѯѧ����������Ӧ��ID
						CString szFindStuName = nullptr; // ��Ҫ��ѯѧ����������Ӧ��ѧ������
						int nInputNumber1 = 0; // ��¼�û����������
						int nFlag1 = 0;	// ��¼scanf����ֵ
						
						fflush(stdin);
						nFlag1 = scanf_s("%d", &nInputNumber1);	// �������ַ���1����֮����0
						if (nFlag1 == 0)
						{
							cout << "����������󣬳��򼴽��˳�..." << endl;
							break;
						}
						else
						{
							if (nInputNumber1 == 0)
							{
								cout << "��ѯ���������˳�..." << endl; // ��������Ϊ0�������˳�
								Sleep(2000);
								break;
							}
							if (nInputNumber1 < 0 || nInputNumber1 > 2)
							{
								cout << "����������󣬳��򼴽��˳�..." << endl;
								Sleep(2000);
								break;
							}
							else
							{
								switch (nInputNumber1)
								{
								case 1:
								{
									cout << "��������Ҫ��ѯѧ����Ϣ��Ӧ��ID��";
									fflush(stdin);
									scanf_s("%d", &nFindStuID);
									if (nFindStuID < 2147483647 && nFindStuID > 0)
									{
										StuAVL.Find(nFindStuID);
										system("pause");
									}
									else
									{
										cout << "���������ֵ���������ļ���ID��Χ�ڣ�����������" << endl;
										system("pause");
									}
								}
								break;
								case 2:
								{
									cout << "��������Ҫ��ѯ��ѧ��������";
									fflush(stdin);
									cin >> szFindStuName;
									StuAVL.FindStuName(szFindStuName);
									system("pause");
								}
								break;
								default:
									cout << "���������Ϣ��������������" << endl;
									break;
								}
							}
						}
					}
				}
				break;
				default:
					cout << "���������Ϣ��������������" << endl;
					break;
				}
			}
		}
	}

	system("pause");
}