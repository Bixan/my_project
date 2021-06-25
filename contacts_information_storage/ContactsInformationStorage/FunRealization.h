#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <Windows.h>

#define SIZE sizeof(TAGCONTACTS)

//记录数据在文件内存中的使用情况
typedef enum enDataType
{
	usedData = -1, //已使用的内存
	unusedData //未使用的内存
}ENDATATYPE;

//用于存储联系人信息
typedef struct tagContacts
{
	ENDATATYPE enDataType;
	int nAge; //年龄
	char szName[32]; //姓名
	char szTel[12];	//手机号码
}TAGCONTACTS;

//记录临时文件中保存的汉字
typedef struct tagChineseChar
{
	char szWord[2];
	int nCount;
	struct tagChineseChar* next;
}CHINESECHAR;

//用于临时存储文件中有效联系人的姓名
typedef struct tagTempName
{
	char szName[32];
}TAGTEMPNAME;

int StartProgram();	//启动程序
void Menu(); //程序菜单
int InitializationData(); //将保存联系人信息的文件格式化(初始化)
void initializationChinese(); //格式化存储联系人姓名的文件
int IncreaseData();	 //在指定的文件内存储相应的联系人信息
int DeleteData(); //在指定的文件内删除相应的联系人信息
int ReviseData(); //在指定的文件内修改相应的联系人信息
int ShowData(); //显示文件内的所有有效的联系人数据
void InquireMenu(); //查询联系人数据菜单
int PerciseQueryContactData(); //精确查询文件内指定的有效联系人数据信息
int FuzzyInquireContactData(); //模糊查询文件内指定的有效联系人数据信息
int InquireData(); //查询文件内所有有效的联系人数据
int ShowDataChinese(int g_nCharCount); //显示文件内有效联系人姓名中的汉字数量以及所占比率
int ShowFileDataRatio(); //统计文件内的有效的联系人信息中的汉字、字母（分大小写）、数字出现的次数，及占总次数的比率
int ShowMemoryFootprint(); //显示文件内数据空间的占用情况，U(使用)，F(未使用)
int Defragmentation(); //在不存储数据时，对将文件中存在的空数据进行碎片整理