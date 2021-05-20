#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include "CString.h"
#include <stdarg.h>  //ʹ��va_start

/*=======================================================
*	CString::CString() -- �޲ι���
*
*	��������
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
CString::CString()
{
	SetString(nullptr);	//�����ݳ�Ա���г�ʼ��
}


/*=======================================================
*	CString::~CString() -- ������������������ڽ������Զ������������ͷ�����ռ�
*
*	��������
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
CString::~CString()
{
	Clear();
}

/*=======================================================
*	CString::CString(const char* szStr) -- ���ι��캯��
*
*	������const char* szStr -- �ַ���
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
CString::CString(const char* szStr)
{
	SetString(szStr);
}


/*=======================================================
*	CString::CString(const CString& obj) -- ��������
*	-- ǳ��������ֹ�ظ��ͷţ���Ҫʹ�����á�����
*	������const char* szStr -- �ַ���
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
CString::CString(const CString& obj)
{
	m_pStrBuff = obj.m_pStrBuff;	// ǳ����
	m_nStrLength = obj.m_nStrLength;
	m_nBuffLength = obj.m_nBuffLength;
	m_pnRefCount = obj.m_pnRefCount;
	
	// ��������Ϊ��
	if (m_pnRefCount != nullptr)
	{
		// ��������ֵ +1
		(*m_pnRefCount)++;
	}
}


/*=======================================================
*	CString::CString(int iVal) -- ���ι��캯��
*
*	������int nVal 
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
CString::CString(int nVal)
{
	char szBuf[256] = { 0 };
	sprintf(szBuf, "%d", nVal);
	SetString(szBuf);
}

/*=======================================================
*	CString::CString(float fltVal) -- ���ι��캯��
*
*	������float fltVal
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
CString::CString(float fltVal)
{
	char szBuf[256] = { 0 };
	sprintf(szBuf, "%f", fltVal);
	SetString(szBuf);
}

/*=======================================================
*	CString::CString(double dblVal) -- ���ι��캯��
*
*	������double dblVal
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
CString::CString(double dblVal)
{
	char szBuf[256] = { 0 };
	sprintf(szBuf, "%lf", dblVal);
	SetString(szBuf);
}


/*=======================================================
*	int CString::GetStringLength() -- ��ȡ�ַ����ĳ���
*
*	��������
*
*	����ֵ�����ض�Ӧ�ַ����ĳ���
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
int CString::GetStringLength()
{
	return m_nStrLength;
}


/*=======================================================
*	const char* CString::GetString() -- ��ȡ��Ӧ���ַ���
*
*	��������
*
*	����ֵ�����ش洢�Ķ�Ӧ�ַ�����Ϣ
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
const char* CString::GetString()
{
	return m_pStrBuff;
}


/*=======================================================
*	CString& CString::operator=(const CString& strObj)
*	-- ��������� =	������ұ�Ϊ�����
*	������const CString& strObj -- ��������������
*
*	����ֵ��*this
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
CString& CString::operator=(const CString& strObj)
{
	// ����Լ�ԭ���еĶѶ���
	Clear();

	// ǳ����
	m_pStrBuff = strObj.m_pStrBuff;
	m_pnRefCount = strObj.m_pnRefCount;
	m_nBuffLength = strObj.m_nBuffLength;
	m_nStrLength = strObj.m_nStrLength;

	// ��������1
	(*m_pnRefCount)++;
	return *this;
}


/*=======================================================
*	CString& CString::operator=(const char* szStr)
*	-- ��������� =	������ұ�Ϊ�ַ���
*	������const char* szStr -- ����������ַ���
*
*	����ֵ��*this
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
CString& CString::operator=(const char* szStr)
{
	Clear();	// ����Լ�ԭ���е�����

	SetString(szStr);	// ͨ�������µ��ַ������ﵽ��ֵ����
	return *this;
}


/*=======================================================
*	CString& CString::operator+=(const CString& strObj)
*	-- ���������  +=	������ұ�Ϊ�����
*	������const CString& strObj -- ��������������
*
*	����ֵ��*this
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
CString& CString::operator+=(const CString& strObj)
{
	Append(strObj);
	return *this;
}


/*=======================================================
*	CString& CString::operator+=(const char* szStr)
*	-- ���������  +=	������ұ�Ϊ�ַ���
*	������const char* szStr -- ����������ַ���
*
*	����ֵ��*this
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
CString& CString::operator+=(const char* szStr)
{
	Append(szStr);
	return *this;
}


/*=======================================================
*	bool CString::operator==(const CString& strObj)
*	-- ���������  ==	������ұ�Ϊ�����
*	������const CString& strObj -- ��������������
*
*	����ֵ�����رȽϺ�Ľ��
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
bool CString::operator==(const CString& strObj)
{
	return (*this == strObj.m_pStrBuff);
}


/*=======================================================
*	bool CString::operator==(const char* szStr)
*	-- ���������  ==	������ұ�Ϊ�ַ���
*	������const char* szStr -- ���������ַ���
*
*	����ֵ�����رȽϺ�Ľ��
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
bool CString::operator==(const char* szStr)
{
	return (strcmp(m_pStrBuff, szStr) == 0);
}


/*=======================================================
*	bool CString::operator>(const CString& strObj)
*	-- ���������  > �Ƚ������ַ����Ĵ�С
*	������const CString& strObj -- ��������������
*
*	����ֵ�����ش洢�Ķ�Ӧ�ַ�����Ϣ
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
bool CString::operator>(const CString& strObj)
{
	return (*this > strObj.m_pStrBuff);
}


/*=======================================================
*	bool CString::operator>(const char* szStr)
*	-- ���������  > �Ƚ������ַ����Ĵ�С
*	������const char* szStr -- ����������ַ���
*
*	����ֵ�����رȽϺ�Ľ��
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
bool CString::operator>(const char* szStr)
{
	return (strcmp(m_pStrBuff, szStr) > 0);
}


/*=======================================================
*	bool CString::operator<(const CString& strObj)
*	-- ���������  < �Ƚ������ַ����Ĵ�С
*	������const CString& strObj -- ��������������
*
*	����ֵ�����رȽϺ�Ľ��
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
bool CString::operator<(const CString& strObj)
{
	return (*this < strObj.m_pStrBuff);
}


/*=======================================================
*	bool CString::operator<(const char* szStr)
*	-- ���������  < �Ƚ������ַ����Ĵ�С
*	������const char* szStr -- ����������ַ���
*
*	����ֵ�����رȽϺ�Ľ��
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
bool CString::operator<(const char* szStr)
{
	return (strcmp(m_pStrBuff, szStr) < 0);
}



/*=======================================================
*	bool CString::operator<(const char* szStr)
*	-- ���������  << ����ַ���������
*	������ostream& os -- ���������, const CString& str -- Ҫ������ַ���
*
*	����ֵ������Ҫ����Ľ��
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
ostream& operator<<(ostream& os, const CString& str)
{
	os << str.m_pStrBuff;
	return os;
}

/*=======================================================
*	bool CString::operator<(const char* szStr)
*	-- ���������  >> �����ַ���������
*	������ostream& os -- ���뵽����, const CString& str -- Ҫ������ַ���
*
*	����ֵ������Ҫ����Ľ��
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
istream& operator>>(istream& is, CString& str)
{
	// �ӿ���̨����һ������
	const int nBuffSize = 0xff;
	char szBuff[nBuffSize] = { 0 };
	is >> szBuff;
	str = szBuff;
	return is;
}


/*=======================================================
*	CString& CString::Append(const char* szStr) -- �ַ���ƴ��
*
*	������const char* szStr -- Ҫƴ�ӵ��ַ���
*
*	����ֵ��*this
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
CString& CString::Append(const char* szStr)
{
	// ����Լ�ԭ��û�����ݣ���ֱ�������ַ���������ƴ��
	if (m_pStrBuff == nullptr)
	{
		SetString(szStr);
		return *this;
	}

	// дʱ������Ϊ�Լ������µĻ�����
	int nCatStrLength = strlen(szStr) + m_nStrLength + 1;
	char* pNewBuff = new char[nCatStrLength];	// �����µĻ�����
	int* pnRefCount = new int(1);	// ÿ���µĻ���������һ���µ����ü�����
	strcpy(pNewBuff, m_pStrBuff);	// ��ԭ�������е����ݿ������µĻ�������
	strcat(pNewBuff, szStr);		// ��Ҫƴ�ӵ��ַ���ƴ�ӵ��»�������ȥ


	Clear();	// �ͷ�ԭ���Ļ�����

	
	m_pStrBuff = pNewBuff;	// Ϊ�����ݳ�Ա����ֵ
	// Ϊ���Ա����ֵ��ʱ�򣬽�ԭ���������е����ݸ�ֵ����ǰ������
	m_pnRefCount = pnRefCount;	
	pNewBuff = nullptr;
	m_nBuffLength = nCatStrLength;
	m_nStrLength = nCatStrLength - 1;

	return *this;
}


/*=======================================================
*	CString& CString::Append(const CString& str) -- �ַ���ƴ�ӣ���������Ϊ�����
*
*	������const char* szStr -- Ҫƴ�ӵ��ַ���
*
*	����ֵ������ Append ���������� *this
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
//ʹ�ÿ�����������CString����
CString& CString::Append(const CString& str)
{
	return Append(str.m_pStrBuff);
}

/*=======================================================
*	CString& CString::Append(int nVal) -- ��int���͵�����ת�����ַ���
*
*	������int nVal -- Ҫת��������
*
*	����ֵ������ Append ���������� *this
*
*	���棺��
*
*	���£�2020/06/10
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
*	CString& CString::Append(double nVal) -- ��double���͵�����ת�����ַ���
*
*	������double dblVal -- Ҫת��������
*
*	����ֵ������ Append ���������� *this
*
*	���棺��
*
*	���£�2020/06/10
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
*	CString& CString::Append(char chVal) -- ��char���͵�����ת�����ַ���
*
*	������char chVal -- Ҫת��������
*
*	����ֵ������ Append ���������� *this
*
*	���棺��
*
*	���£�2020/06/10
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
*	CString& CString::Append(float fltVal) -- ��float���͵�����ת�����ַ���
*
*	������float fltVal -- Ҫת��������
*
*	����ֵ������ Append ���������� *this
*
*	���棺��
*
*	���£�2020/06/10
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
*	CString& CString::Append(long lVal) -- ����������ת��Ϊ������Ƶ��ַ���
*
*	������long lVal -- Ҫת��������
*
*	����ֵ������ *this
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
CString& CString::Append(long lVal)
{
	char szStr[0x1000] = { '0' };	//��ʼ��һ�������飬���ڴ洢�ַ���
	/*
	ltoa �ѳ�������ת��Ϊ������Ƶ��ַ����ĺ���
	char *ltoa(long value, char *string, int radix);
	long value Ҫת������ֵ��char *string ת����ָ���ַ�����ָ�룬int radix ת���Ļ���ֵ,
	��2,8,10,16 ���Ƶȣ���СӦ��2-36֮�䡣
	*/
	_ltoa(lVal, szStr, 10);
	Append(szStr);
	return *this;
}


/*=======================================================
*	int CString::Find(int nIdxBegin, const char* szStr) -- ��ָ��λ�ÿ�ʼ����ָ�����ַ���
*
*	������int nIdxBegin -- ��ʼ���ҵ�λ��, const char* szStr -- ���ҵ��ַ�
*
*	����ֵ������ʧ�� ����-1�����ҳɹ����ض�Ӧ���ַ���
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
int CString::Find(int nIdxBegin, const char* szStr)
{
	// ��������Ƿ�Խ��
	if (nIdxBegin > m_nStrLength || nIdxBegin < 0)
	{
		return -1;
	}
	const char* p = strstr(m_pStrBuff + nIdxBegin, szStr);
	return (p == nullptr) ? -1 : (p - m_pStrBuff);
}


/*=======================================================
*	int CString::Find(int nIdxBegin, const CString& str) -- ��ָ��λ�ÿ�ʼ����ָ���������
*
*	������int nIdxBegin -- ��ʼ���ҵ�λ��, const CString& str -- ���ҵ������
*
*	����ֵ������ Find ��������ʧ�� ����-1�����ҳɹ����ض�Ӧ���ַ���
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
int CString::Find(int nIdxBegin, const CString& str)
{
	return Find(nIdxBegin, str.m_pStrBuff);
}	


/*=======================================================
*	int CString::Find(const char* szStr) -- �ӵ�ǰ�ַ����в���ָ�����ַ���
*
*	������const char* szStr -- ���ҵ��ַ���
*
*	����ֵ������ʧ�ܷ���-1�����ҳɹ����ض�Ӧ������
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
int CString::Find(const char* szStr)
{
	const char* p = strstr(m_pStrBuff, szStr);
	return (p == nullptr) ? -1 : (p - m_pStrBuff);
}


/*=======================================================
*	int CString::Find(const CString& str) -- �ӵ�ǰ�ַ����в���ָ���������
*
*	������const CString& str -- ���ҵ������
*
*	����ֵ������ Find ����������ʧ�ܷ���-1�����ҳɹ����ض�Ӧ������
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
//ʹ�ÿ�����������CString����
int CString::Find(const CString& str)
{
	return Find(str.m_pStrBuff);
}



/*=======================================================
*	int CString::ReverseFind(const char* szStr) -- �ӵ�ǰ�ַ����з�����һ����д洢��ָ���ַ���
*
*	������const CString str��Ҫ���ҵ�ָ���ַ���
*
*	����ֵ������ʧ�ܷ���-1�����ҳɹ����ض�Ӧ������
*
*	���棺��
*
*	���£�2020/06/10
*���ҳɹ������ط��ز��ҵ����ַ�������λ�ã�return -1����ѯʧ��
========================================================*/
int CString::ReverseFind(const char* szStr)
{
	CString strTemp1(m_pStrBuff);
	strTemp1.Reserve();	// ����Reserve�������ַ�������󣬲���ָ�����ַ���
	CString strTemp2(szStr);
	strTemp2.Reserve();

	int nIndex = strTemp1.Find(strTemp2);

	return (nIndex == -1) ? -1 : (m_nStrLength - nIndex - strTemp2.m_nStrLength);
}


/*=======================================================
*	int CString::ReverseFind(const CString& str) 
*	-- ʹ�ÿ�����������CString���󣬷�����һ����д洢��ָ���ַ���
*	������const CString& str��Ҫ���ҵ�CString����ָ�����ַ���
*
*	����ֵ�����ҳɹ������ط��ز��ҵ����ַ�������λ�ã�return -1����ѯʧ��
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
int CString::ReverseFind(const CString& str)
{
	return ReverseFind(str.m_pStrBuff);
}


/*=======================================================
*	CString& CString::Replace(int nIdxBegin, const char* szSrc, const char* szDst)
*	-- ��nIdxBeginλ�ÿ�ʼ����szStr�ַ����滻�� szDst
*	������nIdxBegin ��ʼ�滻��λ�ã�szSrc ���滻���ַ�����szDst���滻���ַ���
*
*	����ֵ��*this
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
CString& CString::Replace(int nIdxBegin, const char* szSrc, const char* szDst)
{
	// ����
	int nIndex = Find(nIdxBegin, szSrc);
	if (nIndex == -1)
	{
		return *this;
	}

	// �ҵ��������滻
	// "HelloWorld" W --> hahaha
	// ��ȡ�ִ� "Hello" "orld"
	// ����ȡ�������ִ� �� �޸ĺ���ַ�������ƴ��
	CString strBengin = Mid(0, nIndex);
	int nEnd = nIndex + strlen(szSrc);
	CString strEnd = Right(m_nStrLength - nEnd);

	// ƴ��
	*this = strBengin;
	*this += szDst;
	*this += strEnd;

	// ����ж���������滻
	// return Replace(szStr, szDst);

	return *this;
}


/*=======================================================
*	CString& CString::Replace(int nIdxBegin, const CString& strSrc, CString& strDst)
*	-- ��nIdxBeginλ�ÿ�ʼ����CString����ָ�����ַ���szStr�ַ��� 
*	�滻��CString����ָ�����ַ��� szDst
*
*	������nIdxBegin ��ʼ�滻��λ�ã�szSrc ���滻���ַ�����szDst���滻���ַ���
*
*	����ֵ������ Replace ������*this
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
CString& CString::Replace(int nIdxBegin, const CString& strSrc, CString& strDst)
{
	return Replace(nIdxBegin, strSrc.m_pStrBuff, strDst.m_pStrBuff);
}


/*=======================================================
*	CString& CString::Replace(const char* szSrc, const char* szDst)
*	-- ���ַ���ͷ����ʼѰ�ң�����һ�γ���szSrc���ַ����滻��szDst
*	������szSrc ���滻���ַ�����szDst���滻���ַ���
*
*	����ֵ������ Replace ������*this
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
// ֻ�滻�ҵ��ĵ�һ��
CString& CString::Replace(const char* szSrc, const char* szDst)
{
	return Replace(0, szSrc, szDst);
}

/*=======================================================
*	CString& CString::Replace(const CString& strSrc, CString& strDst)
*	-- �����������Ļ������ַ��������滻
*	������szSrc ���滻���ַ�����szDst���滻���ַ���
*
*	����ֵ������ Replace ������*this
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
// ʹ�ÿ�����������CStringָ�����������
CString& CString::Replace(const CString& strSrc, CString& strDst)
{
	return Replace(strSrc.m_pStrBuff, strDst.m_pStrBuff);
}


/*=======================================================
*	CString& CString::Replace(int nIdxBegin, const char* szDst)
*	-- �ӻ������е��±�nIdxBegin��ʼ������ʼ����ַ��������滻
*	������nIdxBegin ��ʼ�滻��λ�ã�szDst���滻���ַ���
*
*	����ֵ��*this
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
//ָ������(����ָ��λ��)���滻
CString& CString::Replace(int nIdxBegin, const char* szDst)
{
	// Խ�磬ʲôҲ����
	if (nIdxBegin > m_nStrLength)
	{
		return *this;
	}

	// �滻�����ַ��������nIdxBeginλ�ÿ�ʼ����nIdxBegin������ַ��������滻
	CString strTemp = Left(nIdxBegin);
	strTemp += szDst;

	*this = strTemp;
	return *this;
}

/*=======================================================
*	CString& CString::Replace(int nIdxBegin, const CString& strDst)
*	-- �ӻ������е��±�nIdxBegin��ʼ������ʼ����ַ��������滻
*	������nIdxBegin ��ʼ�滻��λ�ã�szDst��������Ӧ��Ҫ�滻���ַ���
*
*	����ֵ��*this
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
// ָ��λ�����滻��ͬʱ���ݿ�����������CStringָ��Ķ���
CString& CString::Replace(int nIdxBegin, const CString& strDst)
{
	return Replace(nIdxBegin, strDst.m_pStrBuff);
}


/*=======================================================
*	void CString::Copy(const char* szStr)	-- �ַ������������ַ���szStr��������ǰ������
*
*	������const char* szStr -- �������ַ���
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
// �ַ������� -> strcpy
void CString::Copy(const char* szStr)
{
	*this = szStr;
}


/*=======================================================
*	void CString::Copy(const CString& str) 
*			-- �ַ������������ַ�����Ӧ�������Ļ������洢���ַ�����������ǰ����
*	������const char* szStr -- ����������󻺳����е��ַ���
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
void CString::Copy(const CString& str)
{
	*this = str;
}



/*=======================================================
*	CString CString::Mid(int nIdx, int nLength)
*	-- ������ nIdx ��λ�ÿ�ʼ����ȡ����ΪnLength���ַ���
*	������int nIdx -- ��ʼ��ȡ�ַ�����λ��, int nLength -- ��ȡ���ַ�������
*
*	����ֵ���±�Խ�磬���ص�ǰ �������ȡ�ɹ����أ���ȡ���ַ�������
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
CString CString::Mid(int nIdx, int nLength)
{
	// �ж��Ƿ�Խ��
	if (nIdx + nLength > m_nStrLength)
	{
		return CString();
	}

	// ������ʱ�����������ȡ���ַ���

	char* pTempBuff = new char[nLength + 1];
	memcpy(pTempBuff, &m_pStrBuff[nIdx], nLength);
	
	// �ַ��������һ��Ԫ����0
	pTempBuff[nLength] = '\0';
	CString strRet(pTempBuff);
	delete[] pTempBuff;

	return strRet;
}


/*=======================================================
*	CString CString::Left(int nLength)
*	-- �ӻ�����������࣬��ʼ��ȡ����ΪnLength���ַ���
*	������int nLength -- ��ȡ���ַ�������
*
*	����ֵ������ MId �������±�Խ�磬���ص�ǰ �������ȡ�ɹ����أ���ȡ���ַ�������
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
CString CString::Left(int nLength)
{
	return Mid(0, nLength);
}


/*=======================================================
*	CString CString::Right(int nLength)
*	-- �ӻ����������Ҳ࣬��ʼ��ȡ����ΪnLength���ַ���
*	������int nLength -- ��ȡ���ַ�������
*
*	����ֵ������ MId �������±�Խ�磬���ص�ǰ �������ȡ�ɹ����أ���ȡ���ַ�������
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
CString CString::Right(int nLength)
{
	return Mid(m_nStrLength - nLength, nLength);
}


/*=======================================================
*	void CString::Strip()
*	-- ȥ���հ��ַ�(strip)�����Դ��ַ�����ͷ��β���鿴�Ƿ��пո�tab���س�
*	��������
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
void CString::Strip()
{
	int nBlankCount = 0;

	//�����������洢���ַ�����ͷ����¼�ո�tab���س��ĸ���
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
			//���Ʋ���i��ǿ������ѭ��
			i = m_nStrLength;
			break;
		}
	}
	//���������������洢���ַ���ĩβ����¼�ո�tab���س��ĸ���
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
			//���Ʋ���i��ǿ������ѭ��
			i = -1;
			break;
		}
	}
	Move(this->m_pStrBuff, this->m_nStrLength, nBlankCount, 0 - nBlankCount);
}

/*=======================================================
*	void CString::Format(const char* szFmt, ...)
*	-- ���������е����ݸ�ʽ��(���㣬�����ȸ�ʽ��Ϊ�ַ���)vsprintf
*	������const char* szFmt, ...  --��Ҫ���и�ʽ���Ĳ����б�
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
void CString::Format(const char* szFmt, ...)
{
	char szBuff[0x1000] = {0};	//��ʼ��һ��������

	va_list aptr;		//ָ��÷���������һ��ָ��
	va_start(aptr, szFmt);	//ʹ�ú�va_start ��ʼ������
	int ret;
	ret = vsprintf(szBuff, szFmt, aptr);
	va_end(aptr);	//va_end�����ɱ�����Ļ�ȡ

	*this = szBuff;
	//SetString(szBuff);	//ʹ��SetString������������ʼ����������
}


/*=======================================================
*	void CString::SetString(const char* szStr)
*	-- ��ʼ���ַ���
*	������const char* szStr -- ��Ҫ��ʼ�����ַ���
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
void CString::SetString(const char* szStr)
{
	//�ж���ӵ��ַ����Ƿ�Ϊ��
	if (szStr == nullptr)
	{
		m_pStrBuff = nullptr;
		m_pnRefCount = nullptr;
		m_nBuffLength = 0;
		m_nStrLength = 0;
	}
	else
	{
		m_nStrLength = strlen(szStr);	//�洢�ַ����ĳ���
		m_nBuffLength = m_nStrLength + 1;	//��������Ҫ�洢 '\0'����Ҫ���㻺������С��ʱ�򣬶��ַ����ĳ��ȼ�1
		m_pStrBuff = new char[m_nBuffLength];	//���ݻ������Ĵ�С�������Ӧ�Ķѿռ�
		
		m_pnRefCount = new int(1);	//�ڶ�������int��С�Ŀռ�洢��������ֵ
		strcpy(m_pStrBuff, szStr);	//���ַ�������������Ķѿռ���ȥ
	}
}


/*=======================================================
*	void CString::SetString(const char* szStr) -- �ͷ��������ڴ�ռ�
*
*	��������
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
void CString::Clear()
{
	if (m_pStrBuff == nullptr && m_pnRefCount == nullptr)
	{
		return;
	}

	// ���ü���
	(*m_pnRefCount)--;	// ÿ����һ�Σ���������ֵ��1
	
	// ������ΪֵΪ0����ʱֻ���Լ���������ѿռ䣬�����ͷŵ�
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
*	void CString::SetString(const char* szStr) -- ���ַ�����������
*	
*	��������
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
void CString::Reserve()
{
	_strrev(m_pStrBuff);
}

/*=======================================================
*	bool CString::Move(void* pTempBuff, 
*		int nTempBuffLength, int nBlankCount, int nMoveLen)
*	-- ���ݻ������ڵĿհ��ַ����������ƿհ��ַ������ַ�������λ
*	������
*		oid* pTempBuff����Ҫ����λ�ƵĻ�����
*		int nTempBuffLength������λ�Ƶ���ʼλ��
*		int nBlankCount���հ�(�ո�tab���س�)�ַ��ĸ���
*		int nMoveLen����Ҫ����λ�Ƶĳ���
*
*	����ֵ����λ�ɹ����� true
*
*	���棺��
*
*	���£�2020/06/10
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
Date��2020/05/14
����CString�࣬���볣��Ա�������������¾�̬����
*/
/*=======================================================
*	CString CString::ValueOf(bool bVal)
*	-- �ݲ������͵� b��ֵ������ת���ɶ�Ӧ���ַ��������
*	������bool bVal -- boolֵ
*
*	����ֵ������ true ���� �ַ���false����������ַ��� true
*
*	���棺��
*
*	���£�2020/06/10
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
*	-- ���ַ� 'a' �� �ַ��� "a" ����ʽ���
*	������char chVal -- �ַ�����ʽ�������ֵ
*
*	����ֵ������ Append() ����
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
CString CString::ValueOf(char chVal)
{
	return CString().Append(chVal);
}


/*=======================================================
*	CString CString::ValueOf(double dblVal)
*	-- �� double ��ֵ �� ��Ӧ��ֵ���ַ�����ʽ���
*	������char chVal -- �ַ�����ʽ�������ֵ
*
*	����ֵ������ Append() ����
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
CString CString::ValueOf(double dblVal)
{
	return CString().Append(dblVal);
}


/*=======================================================
*	CString CString::ValueOf(float dblVal)
*	-- �� float ��ֵ �� ��Ӧ��ֵ���ַ�����ʽ���
*	������float fltVal -- �ַ�����ʽ�������ֵ
*
*	����ֵ������ Append() ����
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
CString CString::ValueOf(float fltVal)
{
	return CString().Append(fltVal);
}

/*=======================================================
*	CString CString::ValueOf(int dblVal)
*	-- �� int ��ֵ �� ��Ӧ��ֵ���ַ�����ʽ���
*	������int nVal -- �ַ�����ʽ�������ֵ
*
*	����ֵ������ Append() ����
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
CString CString::ValueOf(int nVal)
{
	return CString().Append(nVal);
}

/*=======================================================
*	CString CString::ValueOf(long lVal)
*	-- �� long ��ֵ �� ��Ӧ��ֵ���ַ�����ʽ���
*	������long lVal --�ַ�����ʽ�������ֵ
*
*	����ֵ������ Append() ����
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
CString CString::ValueOf(long lVal)
{
	return CString().Append(lVal);
}


/*=======================================================
*	CString CString::ValueOf(short sVal)
*	-- �� short ��ֵ �� ��Ӧ��ֵ���ַ�����ʽ���
*	������short sVal --�ַ�����ʽ�������ֵ
*
*	����ֵ������ Append() ����
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
CString CString::ValueOf(short sVal)
{
	return CString().Append((int)sVal);
}


/*=======================================================
*	CString CString::ValueOf(const char* data)
*	-- �� short ��ֵ �� ��Ӧ��ֵ���ַ�����ʽ���
*	������const char* data --�ַ�����ʽ�������ֵ
*
*	����ֵ������ Append() ����
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
CString CString::ValueOf(const char* data)
{
	return CString().Append(data);
}


/*=======================================================
*	CString CString::CopyValueOf(const char* pData, int nOffset, int nCount)
*	-- ����ָ�������У���ָ��ƫ�ƴ���ʼ��ʼָ����С�ֽڵ��ַ���
*	������
*		const char* pData -- ���ص��ַ�����nOffset -- ƫ�ƣ�nCount--ָ����С�ֽڵ��ַ���
*	����ֵ��Խ�緵��-1���ɹ������ض�Ӧ�������
*
*	���棺��
*
*	���£�2020/06/10
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
		//��������Ļ������������ ������޷����ַ� 0
		memset(pNewBuff, 0, nStrDataLength);

		//��ԭ������ָ����ʼλ����ȡָ����С���ַ������µĻ�����ȥ
		memcpy(pNewBuff, &pData[nOffset], nCount);
		return CString(pNewBuff);
	}
}

/*=======================================================
*	CString CString::FromFormat(const char* pszFmt, ...)
*	-- ���������е����ݸ�ʽ��(���㣬�����ȸ�ʽ��Ϊ�ַ���)vsprintf
*	������const char* szFmt, ...  --��Ҫ���и�ʽ���Ĳ����б�
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/10
*
========================================================*/
CString CString::FromFormat(const char* pszFmt, ...)
{
	CString str;
	char szBuff[0x1000];	//��ʼ��һ��������
	va_list aptr;		//ָ��÷���������һ��ָ��
	va_start(aptr, pszFmt);	//ʹ�ú�va_start ��ʼ������
	int ret;
	ret = vsprintf(szBuff, pszFmt, aptr);
	va_end(aptr);	//va_end�����ɱ�����Ļ�ȡ

	str.SetString(szBuff);
	return str;
}


// 5.�ڵ�4��Ļ����ϣ����������±������[], ʹ֮�����ж��봦���±�Խ��Ĺ��ܡ�
char& CString::operator[](int nindex)
{
    // ע���±�С��0
	if (nindex < 0 || nindex > strlen(m_pStrBuff))
	{
		throw "overflow";
	}
	return m_pStrBuff[nindex];
}
