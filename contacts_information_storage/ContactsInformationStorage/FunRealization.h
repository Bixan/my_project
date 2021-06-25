#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <Windows.h>

#define SIZE sizeof(TAGCONTACTS)

//��¼�������ļ��ڴ��е�ʹ�����
typedef enum enDataType
{
	usedData = -1, //��ʹ�õ��ڴ�
	unusedData //δʹ�õ��ڴ�
}ENDATATYPE;

//���ڴ洢��ϵ����Ϣ
typedef struct tagContacts
{
	ENDATATYPE enDataType;
	int nAge; //����
	char szName[32]; //����
	char szTel[12];	//�ֻ�����
}TAGCONTACTS;

//��¼��ʱ�ļ��б���ĺ���
typedef struct tagChineseChar
{
	char szWord[2];
	int nCount;
	struct tagChineseChar* next;
}CHINESECHAR;

//������ʱ�洢�ļ�����Ч��ϵ�˵�����
typedef struct tagTempName
{
	char szName[32];
}TAGTEMPNAME;

int StartProgram();	//��������
void Menu(); //����˵�
int InitializationData(); //��������ϵ����Ϣ���ļ���ʽ��(��ʼ��)
void initializationChinese(); //��ʽ���洢��ϵ���������ļ�
int IncreaseData();	 //��ָ�����ļ��ڴ洢��Ӧ����ϵ����Ϣ
int DeleteData(); //��ָ�����ļ���ɾ����Ӧ����ϵ����Ϣ
int ReviseData(); //��ָ�����ļ����޸���Ӧ����ϵ����Ϣ
int ShowData(); //��ʾ�ļ��ڵ�������Ч����ϵ������
void InquireMenu(); //��ѯ��ϵ�����ݲ˵�
int PerciseQueryContactData(); //��ȷ��ѯ�ļ���ָ������Ч��ϵ��������Ϣ
int FuzzyInquireContactData(); //ģ����ѯ�ļ���ָ������Ч��ϵ��������Ϣ
int InquireData(); //��ѯ�ļ���������Ч����ϵ������
int ShowDataChinese(int g_nCharCount); //��ʾ�ļ�����Ч��ϵ�������еĺ��������Լ���ռ����
int ShowFileDataRatio(); //ͳ���ļ��ڵ���Ч����ϵ����Ϣ�еĺ��֡���ĸ���ִ�Сд�������ֳ��ֵĴ�������ռ�ܴ����ı���
int ShowMemoryFootprint(); //��ʾ�ļ������ݿռ��ռ�������U(ʹ��)��F(δʹ��)
int Defragmentation(); //�ڲ��洢����ʱ���Խ��ļ��д��ڵĿ����ݽ�����Ƭ����