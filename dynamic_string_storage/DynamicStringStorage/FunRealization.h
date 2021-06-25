#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <Windows.h>


#define USED_BLOCKSPACE -1 //��ʾ��ʹ�õĿ�ռ�
#define UNUSED_BLOCKSPACE 0 //��ʾδʹ�õĿ�ռ�
#define BLOCK_SIZE 100 //�趨�100�ֽڵ��ڴ�ռ�

int StartProgram();	//��������
void ProBeginMenu(); //����ʼ�˵�
void InquireMenu();	//�ַ�����ѯ�˵�
int IncreaseString(); //����ַ���
int DeleteString();	//ɾ��ָ���ַ���
int ReviseString();	//�޸�ָ���ַ���
int InquireString(); //��ѯ�ַ���
int StrPerciseInquire(); //�ַ�����׼��ѯ
int StrFuzzyInquire(); //�ַ���ģ����ѯ
void ShowString(); //��ʾ�ڴ����Ѿ��洢���ַ���
void ShowStringRatio();	//��ʾϵͳ�ڸ��ַ����ִ���������
void ShowMemoryFootprint();	//��ʾ�ڴ��еĿ�ʹ�����
void Defragmentation();	//���ڴ��п��е����������Ƭ����