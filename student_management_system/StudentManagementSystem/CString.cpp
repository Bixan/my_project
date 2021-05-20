#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include "CString.h"
#include <stdarg.h>  //使用va_start

/*=======================================================
*	CString::CString() -- 无参构造
*
*	参数：无
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString::CString()
{
	SetString(nullptr);	//将数据成员进行初始化
}


/*=======================================================
*	CString::~CString() -- 析构，类对象生命周期结束，自动调用析构，释放自身空间
*
*	参数：无
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString::~CString()
{
	Clear();
}

/*=======================================================
*	CString::CString(const char* szStr) -- 带参构造函数
*
*	参数：const char* szStr -- 字符串
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString::CString(const char* szStr)
{
	SetString(szStr);
}


/*=======================================================
*	CString::CString(const CString& obj) -- 拷贝构造
*	-- 浅拷贝，防止重复释放，需要使用引用·计数
*	参数：const char* szStr -- 字符串
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString::CString(const CString& obj)
{
	m_pStrBuff = obj.m_pStrBuff;	// 浅拷贝
	m_nStrLength = obj.m_nStrLength;
	m_nBuffLength = obj.m_nBuffLength;
	m_pnRefCount = obj.m_pnRefCount;
	
	// 计数器不为空
	if (m_pnRefCount != nullptr)
	{
		// 计数器数值 +1
		(*m_pnRefCount)++;
	}
}


/*=======================================================
*	CString::CString(int iVal) -- 带参构造函数
*
*	参数：int nVal 
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString::CString(int nVal)
{
	char szBuf[256] = { 0 };
	sprintf(szBuf, "%d", nVal);
	SetString(szBuf);
}

/*=======================================================
*	CString::CString(float fltVal) -- 带参构造函数
*
*	参数：float fltVal
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString::CString(float fltVal)
{
	char szBuf[256] = { 0 };
	sprintf(szBuf, "%f", fltVal);
	SetString(szBuf);
}

/*=======================================================
*	CString::CString(double dblVal) -- 带参构造函数
*
*	参数：double dblVal
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString::CString(double dblVal)
{
	char szBuf[256] = { 0 };
	sprintf(szBuf, "%lf", dblVal);
	SetString(szBuf);
}


/*=======================================================
*	int CString::GetStringLength() -- 获取字符串的长度
*
*	参数：无
*
*	返回值：返回对应字符串的长度
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
int CString::GetStringLength()
{
	return m_nStrLength;
}


/*=======================================================
*	const char* CString::GetString() -- 获取对应的字符串
*
*	参数：无
*
*	返回值：返回存储的对应字符串信息
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
const char* CString::GetString()
{
	return m_pStrBuff;
}


/*=======================================================
*	CString& CString::operator=(const CString& strObj)
*	-- 重载运算符 =	运算符右边为类对象
*	参数：const CString& strObj -- 参与运算的类对象
*
*	返回值：*this
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString& CString::operator=(const CString& strObj)
{
	// 清除自己原持有的堆对象
	Clear();

	// 浅拷贝
	m_pStrBuff = strObj.m_pStrBuff;
	m_pnRefCount = strObj.m_pnRefCount;
	m_nBuffLength = strObj.m_nBuffLength;
	m_nStrLength = strObj.m_nStrLength;

	// 计数器加1
	(*m_pnRefCount)++;
	return *this;
}


/*=======================================================
*	CString& CString::operator=(const char* szStr)
*	-- 重载运算符 =	运算符右边为字符串
*	参数：const char* szStr -- 参与运算的字符串
*
*	返回值：*this
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString& CString::operator=(const char* szStr)
{
	Clear();	// 清除自己原持有的数据

	SetString(szStr);	// 通过设置新的字符串，达到赋值操作
	return *this;
}


/*=======================================================
*	CString& CString::operator+=(const CString& strObj)
*	-- 重载运算符  +=	运算符右边为类对象
*	参数：const CString& strObj -- 参与运算的类对象
*
*	返回值：*this
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString& CString::operator+=(const CString& strObj)
{
	Append(strObj);
	return *this;
}


/*=======================================================
*	CString& CString::operator+=(const char* szStr)
*	-- 重载运算符  +=	运算符右边为字符串
*	参数：const char* szStr -- 参与运算的字符串
*
*	返回值：*this
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString& CString::operator+=(const char* szStr)
{
	Append(szStr);
	return *this;
}


/*=======================================================
*	bool CString::operator==(const CString& strObj)
*	-- 重载运算符  ==	运算符右边为类对象
*	参数：const CString& strObj -- 参与运算的类对象
*
*	返回值：返回比较后的结果
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
bool CString::operator==(const CString& strObj)
{
	return (*this == strObj.m_pStrBuff);
}


/*=======================================================
*	bool CString::operator==(const char* szStr)
*	-- 重载运算符  ==	运算符右边为字符串
*	参数：const char* szStr -- 参与运算字符串
*
*	返回值：返回比较后的结果
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
bool CString::operator==(const char* szStr)
{
	return (strcmp(m_pStrBuff, szStr) == 0);
}


/*=======================================================
*	bool CString::operator>(const CString& strObj)
*	-- 重载运算符  > 比较两个字符串的大小
*	参数：const CString& strObj -- 参与运算的类对象
*
*	返回值：返回存储的对应字符串信息
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
bool CString::operator>(const CString& strObj)
{
	return (*this > strObj.m_pStrBuff);
}


/*=======================================================
*	bool CString::operator>(const char* szStr)
*	-- 重载运算符  > 比较两个字符串的大小
*	参数：const char* szStr -- 参与运算的字符串
*
*	返回值：返回比较后的结果
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
bool CString::operator>(const char* szStr)
{
	return (strcmp(m_pStrBuff, szStr) > 0);
}


/*=======================================================
*	bool CString::operator<(const CString& strObj)
*	-- 重载运算符  < 比较两个字符串的大小
*	参数：const CString& strObj -- 参与运算的类对象
*
*	返回值：返回比较后的结果
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
bool CString::operator<(const CString& strObj)
{
	return (*this < strObj.m_pStrBuff);
}


/*=======================================================
*	bool CString::operator<(const char* szStr)
*	-- 重载运算符  < 比较两个字符串的大小
*	参数：const char* szStr -- 参与运算的字符串
*
*	返回值：返回比较后的结果
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
bool CString::operator<(const char* szStr)
{
	return (strcmp(m_pStrBuff, szStr) < 0);
}



/*=======================================================
*	bool CString::operator<(const char* szStr)
*	-- 重载运算符  << 输出字符串到流中
*	参数：ostream& os -- 输出到流中, const CString& str -- 要输出的字符串
*
*	返回值：返回要输出的结果
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
ostream& operator<<(ostream& os, const CString& str)
{
	os << str.m_pStrBuff;
	return os;
}

/*=======================================================
*	bool CString::operator<(const char* szStr)
*	-- 重载运算符  >> 输入字符串到流中
*	参数：ostream& os -- 输入到流中, const CString& str -- 要输入的字符串
*
*	返回值：返回要输出的结果
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
istream& operator>>(istream& is, CString& str)
{
	// 从控制台输入一个单词
	const int nBuffSize = 0xff;
	char szBuff[nBuffSize] = { 0 };
	is >> szBuff;
	str = szBuff;
	return is;
}


/*=======================================================
*	CString& CString::Append(const char* szStr) -- 字符串拼接
*
*	参数：const char* szStr -- 要拼接的字符串
*
*	返回值：*this
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString& CString::Append(const char* szStr)
{
	// 如果自己原先没有数据，则直接设置字符串，不用拼接
	if (m_pStrBuff == nullptr)
	{
		SetString(szStr);
		return *this;
	}

	// 写时拷贝，为自己申请新的缓冲区
	int nCatStrLength = strlen(szStr) + m_nStrLength + 1;
	char* pNewBuff = new char[nCatStrLength];	// 申请新的缓冲区
	int* pnRefCount = new int(1);	// 每个新的缓冲区配置一个新的引用计数器
	strcpy(pNewBuff, m_pStrBuff);	// 将原缓冲区中的数据拷贝到新的缓冲区中
	strcat(pNewBuff, szStr);		// 将要拼接的字符串拼接到新缓冲区中去


	Clear();	// 释放原来的缓冲区

	
	m_pStrBuff = pNewBuff;	// 为类数据成员赋新值
	// 为类成员赋新值的时候，将原来计数器中的数据赋值给当前计数器
	m_pnRefCount = pnRefCount;	
	pNewBuff = nullptr;
	m_nBuffLength = nCatStrLength;
	m_nStrLength = nCatStrLength - 1;

	return *this;
}


/*=======================================================
*	CString& CString::Append(const CString& str) -- 字符串拼接，拷贝对象为类对象
*
*	参数：const char* szStr -- 要拼接的字符串
*
*	返回值：调用 Append 方法，返回 *this
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
//使用拷贝函数传递CString对象
CString& CString::Append(const CString& str)
{
	return Append(str.m_pStrBuff);
}

/*=======================================================
*	CString& CString::Append(int nVal) -- 将int类型的数据转换成字符串
*
*	参数：int nVal -- 要转换的数据
*
*	返回值：调用 Append 方法，返回 *this
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString& CString::Append(int nVal)
{
	CString strFmt;
	strFmt.Format("%d", nVal);
	this->Append(strFmt);

	return *this;
}


/*=======================================================
*	CString& CString::Append(double nVal) -- 将double类型的数据转换成字符串
*
*	参数：double dblVal -- 要转换的数据
*
*	返回值：调用 Append 方法，返回 *this
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString& CString::Append(double dblVal)
{
	CString strFmt;
	strFmt.Format("%lf", dblVal);
	this->Append(strFmt);

	return *this;
}


/*=======================================================
*	CString& CString::Append(char chVal) -- 将char类型的数据转换成字符串
*
*	参数：char chVal -- 要转换的数据
*
*	返回值：调用 Append 方法，返回 *this
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString& CString::Append(char chVal)
{
	CString strFmt;
	strFmt.Format("%c", chVal);
	this->Append(strFmt);

	return *this;
}


/*=======================================================
*	CString& CString::Append(float fltVal) -- 将float类型的数据转换成字符串
*
*	参数：float fltVal -- 要转换的数据
*
*	返回值：调用 Append 方法，返回 *this
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString& CString::Append(float fltVal)
{
	CString strFmt;
	strFmt.Format("%f", fltVal);
	this->Append(strFmt);

	return *this;
}


/*=======================================================
*	CString& CString::Append(long lVal) -- 将长整形数转换为任意进制的字符串
*
*	参数：long lVal -- 要转换的数据
*
*	返回值：返回 *this
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString& CString::Append(long lVal)
{
	char szStr[0x1000] = { '0' };	//初始化一个大数组，用于存储字符串
	/*
	ltoa 把长整形数转换为任意进制的字符串的函数
	char *ltoa(long value, char *string, int radix);
	long value 要转换的数值，char *string 转换后指向字符串的指针，int radix 转换的基数值,
	如2,8,10,16 进制等，大小应在2-36之间。
	*/
	_ltoa(lVal, szStr, 10);
	Append(szStr);
	return *this;
}


/*=======================================================
*	int CString::Find(int nIdxBegin, const char* szStr) -- 从指定位置开始查找指定的字符串
*
*	参数：int nIdxBegin -- 开始查找的位置, const char* szStr -- 查找的字符
*
*	返回值：查找失败 返回-1，查找成功返回对应的字符串
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
int CString::Find(int nIdxBegin, const char* szStr)
{
	// 检查索引是否越界
	if (nIdxBegin > m_nStrLength || nIdxBegin < 0)
	{
		return -1;
	}
	const char* p = strstr(m_pStrBuff + nIdxBegin, szStr);
	return (p == nullptr) ? -1 : (p - m_pStrBuff);
}


/*=======================================================
*	int CString::Find(int nIdxBegin, const CString& str) -- 从指定位置开始查找指定的类对象
*
*	参数：int nIdxBegin -- 开始查找的位置, const CString& str -- 查找的类对象
*
*	返回值：调用 Find 方法查找失败 返回-1，查找成功返回对应的字符串
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
int CString::Find(int nIdxBegin, const CString& str)
{
	return Find(nIdxBegin, str.m_pStrBuff);
}	


/*=======================================================
*	int CString::Find(const char* szStr) -- 从当前字符串中查找指定的字符串
*
*	参数：const char* szStr -- 查找的字符串
*
*	返回值：查找失败返回-1，查找成功返回对应的数据
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
int CString::Find(const char* szStr)
{
	const char* p = strstr(m_pStrBuff, szStr);
	return (p == nullptr) ? -1 : (p - m_pStrBuff);
}


/*=======================================================
*	int CString::Find(const CString& str) -- 从当前字符串中查找指定的类对象
*
*	参数：const CString& str -- 查找的类对象
*
*	返回值：调用 Find 方法，查找失败返回-1，查找成功返回对应的数据
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
//使用拷贝函数传递CString对象
int CString::Find(const CString& str)
{
	return Find(str.m_pStrBuff);
}



/*=======================================================
*	int CString::ReverseFind(const char* szStr) -- 从当前字符串中反向查找缓存中存储的指定字符串
*
*	参数：const CString str：要查找的指定字符串
*
*	返回值：查找失败返回-1，查找成功返回对应的数据
*
*	警告：无
*
*	更新：2020/06/10
*查找成功，返回返回查找到的字符串所在位置，return -1：查询失败
========================================================*/
int CString::ReverseFind(const char* szStr)
{
	CString strTemp1(m_pStrBuff);
	strTemp1.Reserve();	// 调用Reserve方法将字符串逆序后，查找指定的字符串
	CString strTemp2(szStr);
	strTemp2.Reserve();

	int nIndex = strTemp1.Find(strTemp2);

	return (nIndex == -1) ? -1 : (m_nStrLength - nIndex - strTemp2.m_nStrLength);
}


/*=======================================================
*	int CString::ReverseFind(const CString& str) 
*	-- 使用拷贝函数传递CString对象，反向查找缓存中存储的指定字符串
*	参数：const CString& str：要查找的CString对象指定的字符串
*
*	返回值：查找成功，返回返回查找到的字符串所在位置，return -1：查询失败
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
int CString::ReverseFind(const CString& str)
{
	return ReverseFind(str.m_pStrBuff);
}


/*=======================================================
*	CString& CString::Replace(int nIdxBegin, const char* szSrc, const char* szDst)
*	-- 从nIdxBegin位置开始，将szStr字符串替换成 szDst
*	参数：nIdxBegin 开始替换的位置，szSrc 被替换的字符串，szDst，替换的字符串
*
*	返回值：*this
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString& CString::Replace(int nIdxBegin, const char* szSrc, const char* szDst)
{
	// 查找
	int nIndex = Find(nIdxBegin, szSrc);
	if (nIndex == -1)
	{
		return *this;
	}

	// 找到，进行替换
	// "HelloWorld" W --> hahaha
	// 提取字串 "Hello" "orld"
	// 将提取出来的字串 和 修改后的字符串进行拼接
	CString strBengin = Mid(0, nIndex);
	int nEnd = nIndex + strlen(szSrc);
	CString strEnd = Right(m_nStrLength - nEnd);

	// 拼接
	*this = strBengin;
	*this += szDst;
	*this += strEnd;

	// 如果有多个，继续替换
	// return Replace(szStr, szDst);

	return *this;
}


/*=======================================================
*	CString& CString::Replace(int nIdxBegin, const CString& strSrc, CString& strDst)
*	-- 从nIdxBegin位置开始，将CString对象指定的字符串szStr字符串 
*	替换成CString对象指定的字符串 szDst
*
*	参数：nIdxBegin 开始替换的位置，szSrc 被替换的字符串，szDst，替换的字符串
*
*	返回值：调用 Replace 方法，*this
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString& CString::Replace(int nIdxBegin, const CString& strSrc, CString& strDst)
{
	return Replace(nIdxBegin, strSrc.m_pStrBuff, strDst.m_pStrBuff);
}


/*=======================================================
*	CString& CString::Replace(const char* szSrc, const char* szDst)
*	-- 从字符串头部开始寻找，将第一次出现szSrc的字符串替换成szDst
*	参数：szSrc 被替换的字符串，szDst，替换的字符串
*
*	返回值：调用 Replace 方法，*this
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
// 只替换找到的第一个
CString& CString::Replace(const char* szSrc, const char* szDst)
{
	return Replace(0, szSrc, szDst);
}

/*=======================================================
*	CString& CString::Replace(const CString& strSrc, CString& strDst)
*	-- 将两个类对象的缓冲区字符串进行替换
*	参数：szSrc 被替换的字符串，szDst，替换的字符串
*
*	返回值：调用 Replace 方法，*this
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
// 使用拷贝函数传递CString指向的两个对象
CString& CString::Replace(const CString& strSrc, CString& strDst)
{
	return Replace(strSrc.m_pStrBuff, strDst.m_pStrBuff);
}


/*=======================================================
*	CString& CString::Replace(int nIdxBegin, const char* szDst)
*	-- 从缓冲区中的下标nIdxBegin开始，将开始后的字符串进行替换
*	参数：nIdxBegin 开始替换的位置，szDst，替换的字符串
*
*	返回值：*this
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
//指定长度(或者指定位置)来替换
CString& CString::Replace(int nIdxBegin, const char* szDst)
{
	// 越界，什么也不干
	if (nIdxBegin > m_nStrLength)
	{
		return *this;
	}

	// 替换，从字符串的左端nIdxBegin位置开始，将nIdxBegin后面的字符串进行替换
	CString strTemp = Left(nIdxBegin);
	strTemp += szDst;

	*this = strTemp;
	return *this;
}

/*=======================================================
*	CString& CString::Replace(int nIdxBegin, const CString& strDst)
*	-- 从缓冲区中的下标nIdxBegin开始，将开始后的字符串进行替换
*	参数：nIdxBegin 开始替换的位置，szDst，类对象对应的要替换的字符串
*
*	返回值：*this
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
// 指定位置来替换，同时传递拷贝函数传递CString指向的对象
CString& CString::Replace(int nIdxBegin, const CString& strDst)
{
	return Replace(nIdxBegin, strDst.m_pStrBuff);
}


/*=======================================================
*	void CString::Copy(const char* szStr)	-- 字符串拷贝，将字符串szStr拷贝给当前缓冲区
*
*	参数：const char* szStr -- 拷贝的字符串
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
// 字符串拷贝 -> strcpy
void CString::Copy(const char* szStr)
{
	*this = szStr;
}


/*=======================================================
*	void CString::Copy(const CString& str) 
*			-- 字符串拷贝，将字符串对应的类对象的缓冲区存储的字符串拷贝给当前对象
*	参数：const char* szStr -- 拷贝的类对象缓冲区中的字符串
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
void CString::Copy(const CString& str)
{
	*this = str;
}



/*=======================================================
*	CString CString::Mid(int nIdx, int nLength)
*	-- 从索引 nIdx 的位置开始，提取长度为nLength的字符串
*	参数：int nIdx -- 开始提取字符串的位置, int nLength -- 提取的字符串长度
*
*	返回值：下标越界，返回当前 类对象，提取成功返回，提取的字符串对象
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString CString::Mid(int nIdx, int nLength)
{
	// 判断是否越界
	if (nIdx + nLength > m_nStrLength)
	{
		return CString();
	}

	// 申请临时缓冲区存放提取的字符串

	char* pTempBuff = new char[nLength + 1];
	memcpy(pTempBuff, &m_pStrBuff[nIdx], nLength);
	
	// 字符数组最后一个元素置0
	pTempBuff[nLength] = '\0';
	CString strRet(pTempBuff);
	delete[] pTempBuff;

	return strRet;
}


/*=======================================================
*	CString CString::Left(int nLength)
*	-- 从缓冲区的最左侧，开始提取长度为nLength的字符串
*	参数：int nLength -- 提取的字符串长度
*
*	返回值：调用 MId 方法，下标越界，返回当前 类对象，提取成功返回，提取的字符串对象
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString CString::Left(int nLength)
{
	return Mid(0, nLength);
}


/*=======================================================
*	CString CString::Right(int nLength)
*	-- 从缓冲区的最右侧，开始提取长度为nLength的字符串
*	参数：int nLength -- 提取的字符串长度
*
*	返回值：调用 MId 方法，下标越界，返回当前 类对象，提取成功返回，提取的字符串对象
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString CString::Right(int nLength)
{
	return Mid(m_nStrLength - nLength, nLength);
}


/*=======================================================
*	void CString::Strip()
*	-- 去除空白字符(strip)：可以从字符串的头部尾部查看是否有空格、tab、回车
*	参数：无
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
void CString::Strip()
{
	int nBlankCount = 0;

	//遍历缓冲区存储的字符串开头，记录空格、tab、回车的个数
	for (int i = 0; i < this->m_nStrLength; i++)
	{
		switch (this->m_pStrBuff[i])
		{
		case ' ':
		case 9:	//Tab
		case '\r':
		case '\n':
			nBlankCount++;
			break;
		default:
			//控制步长i，强制跳出循环
			i = m_nStrLength;
			break;
		}
	}
	//遍历遍历缓冲区存储的字符串末尾，记录空格、tab、回车的个数
	for (int i = this->m_nStrLength - 1; i >= 0; i--)
	{
		switch (this->m_pStrBuff[i])
		{
		case ' ':
		case 9:	//Tab
		case '\r':
		case '\n':
			this->m_pStrBuff[i] = '\0';
			break;
		default:
			//控制步长i，强制跳出循环
			i = -1;
			break;
		}
	}
	Move(this->m_pStrBuff, this->m_nStrLength, nBlankCount, 0 - nBlankCount);
}

/*=======================================================
*	void CString::Format(const char* szFmt, ...)
*	-- 将缓冲区中的数据格式化(浮点，整数等格式化为字符串)vsprintf
*	参数：const char* szFmt, ...  --需要进行格式化的参数列表
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
void CString::Format(const char* szFmt, ...)
{
	char szBuff[0x1000] = {0};	//初始化一个大数组

	va_list aptr;		//指向该方法参数的一个指针
	va_start(aptr, szFmt);	//使用宏va_start 初始化变量
	int ret;
	ret = vsprintf(szBuff, szFmt, aptr);
	va_end(aptr);	//va_end结束可变参数的获取

	*this = szBuff;
	//SetString(szBuff);	//使用SetString方法将参数初始化到缓冲区
}


/*=======================================================
*	void CString::SetString(const char* szStr)
*	-- 初始化字符串
*	参数：const char* szStr -- 需要初始化的字符串
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
void CString::SetString(const char* szStr)
{
	//判断添加的字符串是否为空
	if (szStr == nullptr)
	{
		m_pStrBuff = nullptr;
		m_pnRefCount = nullptr;
		m_nBuffLength = 0;
		m_nStrLength = 0;
	}
	else
	{
		m_nStrLength = strlen(szStr);	//存储字符串的长度
		m_nBuffLength = m_nStrLength + 1;	//缓冲区需要存储 '\0'，需要计算缓冲区大小的时候，对字符串的长度加1
		m_pStrBuff = new char[m_nBuffLength];	//根据缓冲区的大小，申请对应的堆空间
		
		m_pnRefCount = new int(1);	//在堆中申请int大小的空间存储计数器数值
		strcpy(m_pStrBuff, szStr);	//将字符串拷贝到申请的堆空间内去
	}
}


/*=======================================================
*	void CString::SetString(const char* szStr) -- 释放类对象的内存空间
*
*	参数：无
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
void CString::Clear()
{
	if (m_pStrBuff == nullptr && m_pnRefCount == nullptr)
	{
		return;
	}

	// 引用计数
	(*m_pnRefCount)--;	// 每调用一次，计数器数值减1
	
	// 当计数为值为0，此时只有自己持有这个堆空间，可以释放掉
	if ((*m_pnRefCount) == 0)
	{
		delete[] m_pStrBuff;
		delete m_pnRefCount;
		m_pStrBuff = nullptr;
		m_pnRefCount = nullptr;

		m_nBuffLength = 0;
		m_nStrLength = 0;
	}
}

/*=======================================================
*	void CString::SetString(const char* szStr) -- 将字符串进行逆序
*	
*	参数：无
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
void CString::Reserve()
{
	_strrev(m_pStrBuff);
}

/*=======================================================
*	bool CString::Move(void* pTempBuff, 
*		int nTempBuffLength, int nBlankCount, int nMoveLen)
*	-- 根据缓冲区内的空白字符个数，不计空白字符，将字符进行移位
*	参数：
*		oid* pTempBuff：需要进行位移的缓冲区
*		int nTempBuffLength：进行位移的起始位置
*		int nBlankCount：空白(空格、tab、回车)字符的个数
*		int nMoveLen：需要进行位移的长度
*
*	返回值：移位成功返回 true
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
bool CString::Move(void* pTempBuff, int nTempBuffLength, int nBlankCount, int nMoveLen)
{
	if (nMoveLen == 0)
	{
		return true;
	}
	else if (nMoveLen < 0)
	{
		for (int i = nBlankCount; i <= nTempBuffLength; i++)
		{
			((char*)pTempBuff)[i + nMoveLen] = ((char*)pTempBuff)[i];
		}
	}
	else
	{
		for (int i = nTempBuffLength - 1; i >= nBlankCount + nMoveLen; i--)
		{
			((char*)pTempBuff)[i] = ((char*)pTempBuff)[i - nMoveLen];
		}
	}
	return true;
}

/*
Date：2020/05/14
完善CString类，加入常成员函数，增加以下静态方法
*/
/*=======================================================
*	CString CString::ValueOf(bool bVal)
*	-- 据布尔类型的 b的值，将其转换成对应的字符出并输出
*	参数：bool bVal -- bool值
*
*	返回值：输入 true 返回 字符串false，返回输出字符串 true
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString CString::ValueOf(bool bVal)
{
	if (bVal == true)
	{
		return CString("ture");
	}
	return CString("false");
}

/*=======================================================
*	CString CString::ValueOf(char chVal)
*	-- 将字符 'a' 以 字符串 "a" 的形式输出
*	参数：char chVal -- 字符串形式输出的数值
*
*	返回值：调用 Append() 方法
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString CString::ValueOf(char chVal)
{
	return CString().Append(chVal);
}


/*=======================================================
*	CString CString::ValueOf(double dblVal)
*	-- 将 double 数值 以 对应数值的字符串形式输出
*	参数：char chVal -- 字符串形式输出的数值
*
*	返回值：调用 Append() 方法
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString CString::ValueOf(double dblVal)
{
	return CString().Append(dblVal);
}


/*=======================================================
*	CString CString::ValueOf(float dblVal)
*	-- 将 float 数值 以 对应数值的字符串形式输出
*	参数：float fltVal -- 字符串形式输出的数值
*
*	返回值：调用 Append() 方法
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString CString::ValueOf(float fltVal)
{
	return CString().Append(fltVal);
}

/*=======================================================
*	CString CString::ValueOf(int dblVal)
*	-- 将 int 数值 以 对应数值的字符串形式输出
*	参数：int nVal -- 字符串形式输出的数值
*
*	返回值：调用 Append() 方法
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString CString::ValueOf(int nVal)
{
	return CString().Append(nVal);
}

/*=======================================================
*	CString CString::ValueOf(long lVal)
*	-- 将 long 数值 以 对应数值的字符串形式输出
*	参数：long lVal --字符串形式输出的数值
*
*	返回值：调用 Append() 方法
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString CString::ValueOf(long lVal)
{
	return CString().Append(lVal);
}


/*=======================================================
*	CString CString::ValueOf(short sVal)
*	-- 将 short 数值 以 对应数值的字符串形式输出
*	参数：short sVal --字符串形式输出的数值
*
*	返回值：调用 Append() 方法
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString CString::ValueOf(short sVal)
{
	return CString().Append((int)sVal);
}


/*=======================================================
*	CString CString::ValueOf(const char* data)
*	-- 将 short 数值 以 对应数值的字符串形式输出
*	参数：const char* data --字符串形式输出的数值
*
*	返回值：调用 Append() 方法
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString CString::ValueOf(const char* data)
{
	return CString().Append(data);
}


/*=======================================================
*	CString CString::CopyValueOf(const char* pData, int nOffset, int nCount)
*	-- 返回指定数组中，从指定偏移处开始初始指定大小字节的字符串
*	参数：
*		const char* pData -- 返回的字符串，nOffset -- 偏移，nCount--指定大小字节的字符串
*	返回值：越界返回-1，成功，返回对应的类对象
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString CString::CopyValueOf(const char* pData, int nOffset, int nCount)
{
	int nStrDataLength = strlen(pData) + 1;

	if (nCount <= 0)
	{
		return -1;
	}
	else
	{
		char* pNewBuff = new char[nStrDataLength];
		//将新申请的缓冲区进行填充 ，填充无符号字符 0
		memset(pNewBuff, 0, nStrDataLength);

		//从原缓冲区指定开始位置提取指定大小的字符串到新的缓冲区去
		memcpy(pNewBuff, &pData[nOffset], nCount);
		return CString(pNewBuff);
	}
}

/*=======================================================
*	CString CString::FromFormat(const char* pszFmt, ...)
*	-- 将缓冲区中的数据格式化(浮点，整数等格式化为字符串)vsprintf
*	参数：const char* szFmt, ...  --需要进行格式化的参数列表
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/10
*
========================================================*/
CString CString::FromFormat(const char* pszFmt, ...)
{
	CString str;
	char szBuff[0x1000];	//初始化一个大数组
	va_list aptr;		//指向该方法参数的一个指针
	va_start(aptr, pszFmt);	//使用宏va_start 初始化变量
	int ret;
	ret = vsprintf(szBuff, pszFmt, aptr);
	va_end(aptr);	//va_end结束可变参数的获取

	str.SetString(szBuff);
	return str;
}


// 5.在第4题的基础上，重载数组下标操作符[], 使之具有判断与处理下标越界的功能。
char& CString::operator[](int nindex)
{
    // 注意下标小于0
	if (nindex < 0 || nindex > strlen(m_pStrBuff))
	{
		throw "overflow";
	}
	return m_pStrBuff[nindex];
}
