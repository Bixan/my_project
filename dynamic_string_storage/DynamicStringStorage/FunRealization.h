#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <Windows.h>


#define USED_BLOCKSPACE -1 //表示已使用的块空间
#define UNUSED_BLOCKSPACE 0 //表示未使用的块空间
#define BLOCK_SIZE 100 //设定额定100字节的内存空间

int StartProgram();	//启动程序
void ProBeginMenu(); //程序开始菜单
void InquireMenu();	//字符串查询菜单
int IncreaseString(); //添加字符串
int DeleteString();	//删除指定字符串
int ReviseString();	//修改指定字符串
int InquireString(); //查询字符串
int StrPerciseInquire(); //字符串精准查询
int StrFuzzyInquire(); //字符串模糊查询
void ShowString(); //显示内存中已经存储的字符串
void ShowStringRatio();	//显示系统内各字符出现次数及比率
void ShowMemoryFootprint();	//显示内存中的块使用情况
void Defragmentation();	//对内存中空闲的区域进行碎片整理