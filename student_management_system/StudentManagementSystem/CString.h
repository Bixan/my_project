#pragma once
#include <iostream>
using namespace std;

/*=======================================================
*			o(*������*)o
****************************************************************************************
*			Name��CString
*
*			FileName��CString.h
*
*			Programmer��ileemi
*
*			Start Date��2020/05/12
*
*			Last UpDate��2020/06/11
*
*=======================================================*/
class CString
{
public:
	CString();	// Ĭ�Ϲ��캯��
	~CString();	// ��������
	CString(const char* szStr);	// ���ι��캯��
	CString(const CString& obj);	// ��������

	CString(int nVal);	// ��������Ĭ�Ϲ���
	CString(float fltVal);
	CString(double dblVal);

	int GetStringLength();	// ��ȡ��Ӧ�ַ����ĳ���
	const char* GetString();	// ��ȡ��Ӧ���ַ���

public:
	CString& operator=(const CString& strObj);	// ��������� =	������ұ�Ϊ�����
	CString& operator=(const char* szStr);	// ��������� =	������ұ�Ϊ�ַ���
	CString& operator+=(const CString& strObj);	// ��������� += ������ұ�Ϊ�����
	CString& operator+=(const char* szStr);	// ��������� += ������ұ�Ϊ�ַ���
	bool operator==(const CString& strObj);	// ��������� == ������ұ�Ϊ�����
	bool operator==(const char* szStr);	// ��������� == ������ұ�Ϊ�ַ���
	bool operator>(const CString& strObj);	// ��������� > ������ұ�Ϊ�����
	bool operator>(const char* szStr);	// ��������� > ������ұ�Ϊ�ַ���
	bool operator<(const CString& strObj);	// ��������� < ������ұ�Ϊ�����
	bool operator<(const char* szStr);	// ��������� < ������ұ�Ϊ�ַ���

public:
	friend ostream& operator<<(ostream& os, const CString& str);	// ��������� <<
	friend istream& operator>>(istream& is, CString& str);	// ��������� >>

public:
	
	CString& Append(const char* szStr);	// �ַ���ƴ�� ->�൱��strcat
	CString& Append(const CString& str);	// ʹ�ÿ�����������CString����
	CString& Append(int nVal);	// ��int���͵�����ת�����ַ���
	CString& Append(double dblVal);	// ��double���͵�����ת�����ַ���
	CString& Append(char chVal);	// ���ַ� 'chVal' �� �ַ� "chVal" ����ʽ���
	CString& Append(float fVal);	// ��float���͵�����ת�����ַ���
	CString& Append(long lVal);	// �ѳ�������ת��Ϊ������Ƶ��ַ���
	

public:
	int Find(int nIdxBegin, const char* szStr);	// ������ң���ָ��λ�ÿ�ʼ�����ַ���
	int Find(int nIdxBegin, const CString& str);

	int Find(const char* szStr);	// �����ַ���
	int Find(const CString& str);

	int ReverseFind(const char* szStr);	// ��������ַ���
	int ReverseFind(const CString& str);


	// �滻�ʺ� CString&

	// ��ָ��λ�ÿ�ʼ���ң����ַ����滻 �� szStr �滻�� szDst
	CString& Replace(int nIdxBegin, const char* szSrc, const char* szDst);
	CString& Replace(int nIdxBegin, const CString& strSrc, CString& strDst);

	// ֻ�滻��һ�����ֵ�
	CString& Replace(const char* szSrc, const char* szDst);
	CString& Replace(const CString& strSrc, CString& strDst);

	// ָ��ָ��λ���滻
	CString& Replace(int nIdxBegin, const char* szDst);
	CString& Replace(int nIdxBegin, const CString& strDst);


	// �ַ������� -> strcpy
	void Copy(const char* szStr);
	void Copy(const CString& str);

	// ��ȡ�Ӵ�
	// ������ nIdx ��ʼ��ȡnLength���ֽ�Ϊ�µ��ַ���	
	CString Mid(int nIdx, int nLength);	// �м���ȡ
	CString Left(int nLength);	// ���ַ�����࿪ʼ��ȡnLength���ֽ�Ϊ�µ��ַ���
	CString Right(int nLength);	// ���ַ����Ҳ࿪ʼ��ȡnLength���ֽ�Ϊ�µ��ַ���

	void Strip();	// ȥ���հ��ַ�(strip)�����Դ��ַ�����ͷ��β���鿴�Ƿ��пո�tab���س�
	void Format(const char* szFmt, ...);	// ���������е����ݸ�ʽ��(���㣬�����ȸ�ʽ��Ϊ�ַ���)

private:
	//��װ���������ڽ���ظ�����
	void SetString(const char* szStr);	// ��ʼ��������(�ַ���)������һ���µ��ַ���

	void Clear();	// �ͷ��ڴ棬����������
	void Reserve();	// ���ַ�������

	// ���ݻ������ڵĿհ��ַ����������ƿհ��ַ������ַ�������λ
	bool Move(void* pvBuf, int iBufSize, int iStart, int iMoveLen);	

	/*
	Date��2020/05/14
	����CString�࣬���볣��Ա�������������¾�̬����
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
	// ����ָ�������У���ָ��ƫ�ƴ���ʼ��ʼָ����С�ֽڵ��ַ���
	static CString CopyValueOf(const char* pData, int nOffset, int nCount); //char[] = "123" ==> string "123"
	static CString FromFormat(const char* pszFmt, ...);  //("%d", 123) ==> string "123"

	char& operator[](int nindex);	//���������±������[], ʹ֮�����ж��봦���±�Խ��Ĺ���

// ���ݳ�Ա
private:
	char* m_pStrBuff;	// ָ���ַ����Ļ�����
	int* m_pnRefCount;	// ÿ���޸Ļ����е�����ʱ��Ϊ���ڶ�������һ�����ü���(��¼��ǰʹ�øû���Ķ�������)
	int m_nBuffLength;	// �������Ĵ�С
	int m_nStrLength;	// ���������ַ����Ĵ�С
};