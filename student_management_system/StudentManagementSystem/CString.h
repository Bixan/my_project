#pragma once
#include <iostream>
using namespace std;

/*=======================================================
*			o(*￣▽￣*)o
****************************************************************************************
*			Name：CString
*
*			FileName：CString.h
*
*			Programmer：ileemi
*
*			Start Date：2020/05/12
*
*			Last UpDate：2020/06/11
*
*=======================================================*/
class CString
{
public:
	CString();	// 默认构造函数
	~CString();	// 析构函数
	CString(const char* szStr);	// 带参构造函数
	CString(const CString& obj);	// 拷贝构造

	CString(int nVal);	// 带参数的默认构造
	CString(float fltVal);
	CString(double dblVal);

	int GetStringLength();	// 获取对应字符串的长度
	const char* GetString();	// 获取对应的字符串

public:
	CString& operator=(const CString& strObj);	// 重载运算符 =	运算符右边为类对象
	CString& operator=(const char* szStr);	// 重载运算符 =	运算符右边为字符串
	CString& operator+=(const CString& strObj);	// 重载运算符 += 运算符右边为类对象
	CString& operator+=(const char* szStr);	// 重载运算符 += 运算符右边为字符串
	bool operator==(const CString& strObj);	// 重载运算符 == 运算符右边为类对象
	bool operator==(const char* szStr);	// 重载运算符 == 运算符右边为字符串
	bool operator>(const CString& strObj);	// 重载运算符 > 运算符右边为类对象
	bool operator>(const char* szStr);	// 重载运算符 > 运算符右边为字符串
	bool operator<(const CString& strObj);	// 重载运算符 < 运算符右边为类对象
	bool operator<(const char* szStr);	// 重载运算符 < 运算符右边为字符串

public:
	friend ostream& operator<<(ostream& os, const CString& str);	// 重载运算符 <<
	friend istream& operator>>(istream& is, CString& str);	// 重载运算符 >>

public:
	
	CString& Append(const char* szStr);	// 字符串拼接 ->相当于strcat
	CString& Append(const CString& str);	// 使用拷贝函数传递CString对象
	CString& Append(int nVal);	// 将int类型的数据转换成字符串
	CString& Append(double dblVal);	// 将double类型的数据转换成字符串
	CString& Append(char chVal);	// 将字符 'chVal' 以 字符 "chVal" 的形式输出
	CString& Append(float fVal);	// 将float类型的数据转换成字符串
	CString& Append(long lVal);	// 把长整形数转换为任意进制的字符串
	

public:
	int Find(int nIdxBegin, const char* szStr);	// 正向查找，从指定位置开始查找字符串
	int Find(int nIdxBegin, const CString& str);

	int Find(const char* szStr);	// 查找字符串
	int Find(const CString& str);

	int ReverseFind(const char* szStr);	// 逆向查找字符串
	int ReverseFind(const CString& str);


	// 替换适合 CString&

	// 从指定位置开始查找，将字符串替换 将 szStr 替换成 szDst
	CString& Replace(int nIdxBegin, const char* szSrc, const char* szDst);
	CString& Replace(int nIdxBegin, const CString& strSrc, CString& strDst);

	// 只替换第一个出现的
	CString& Replace(const char* szSrc, const char* szDst);
	CString& Replace(const CString& strSrc, CString& strDst);

	// 指定指定位置替换
	CString& Replace(int nIdxBegin, const char* szDst);
	CString& Replace(int nIdxBegin, const CString& strDst);


	// 字符串拷贝 -> strcpy
	void Copy(const char* szStr);
	void Copy(const CString& str);

	// 提取子串
	// 从索引 nIdx 开始提取nLength个字节为新的字符串	
	CString Mid(int nIdx, int nLength);	// 中间提取
	CString Left(int nLength);	// 从字符串左侧开始提取nLength个字节为新的字符串
	CString Right(int nLength);	// 从字符串右侧开始提取nLength个字节为新的字符串

	void Strip();	// 去除空白字符(strip)：可以从字符串的头部尾部查看是否有空格、tab、回车
	void Format(const char* szFmt, ...);	// 将缓冲区中的数据格式化(浮点，整数等格式化为字符串)

private:
	//封装函数，用于解决重复代码
	void SetString(const char* szStr);	// 初始化缓冲区(字符串)，设置一个新的字符串

	void Clear();	// 释放内存，解决深拷贝问题
	void Reserve();	// 将字符串逆序

	// 根据缓冲区内的空白字符个数，不计空白字符，将字符进行移位
	bool Move(void* pvBuf, int iBufSize, int iStart, int iMoveLen);	

	/*
	Date：2020/05/14
	完善CString类，加入常成员函数，增加以下静态方法
	*/
public:
	static CString ValueOf(bool bVal);	//true ==> "true"
	static CString ValueOf(char chVal);	//a ==> "a"
	static CString ValueOf(double dblVal);	//3.5 ==> "3.5"
	static CString ValueOf(float fltVal);	//4.5f ==> "4.5"
	static CString ValueOf(int nVal);	//123 ==> "123"
	static CString ValueOf(long lVal);	//123 ==> "123"
	static CString ValueOf(short sVal);	//123 ==> "123"
	static CString ValueOf(const char* data);	//char[] = "123" ==> string "123"
	// 返回指定数组中，从指定偏移处开始初始指定大小字节的字符串
	static CString CopyValueOf(const char* pData, int nOffset, int nCount); //char[] = "123" ==> string "123"
	static CString FromFormat(const char* pszFmt, ...);  //("%d", 123) ==> string "123"

	char& operator[](int nindex);	//重载数组下标操作符[], 使之具有判断与处理下标越界的功能

// 数据成员
private:
	char* m_pStrBuff;	// 指向字符串的缓冲区
	int* m_pnRefCount;	// 每次修改缓存中的数据时，为其在堆中申请一个引用计数(记录当前使用该缓存的对象数量)
	int m_nBuffLength;	// 缓冲区的大小
	int m_nStrLength;	// 缓冲区中字符串的大小
};