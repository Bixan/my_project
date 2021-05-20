#pragma once
/*=======================================================
*			o(*������*)o
****************************************************************************************
*
*			FileName��CStudent.h -- ѧ���ࣨTest��
*
*			Start Date��2020/06/11
*
*			Last UpDate��2020/06/15
*
*=======================================================*/
#include "CString.h"
#include "MyCAVLTree.h"
class CStudent
{
public:
	CStudent();
	CStudent(int nID, int nYear, int nMonth, int nSex, CString szName);
	CStudent(const CStudent& stu);
	CStudent& operator=(const CStudent& stu);
	virtual ~CStudent();
	void ShowStuData();

	template<typename T>
	static CAVLTree<T>& CreateStuIDAVLTree();	// ��ѧ��ID����һ��ƽ�������

	template<typename T>
	static CAVLTree<T>& CreateStuNameAVLTree();	// ��ѧ����������һ��ƽ�������

public:
	int m_nID;	// ѧ��ID
	int m_nYear;	// �������
	int m_nMonth;	// �����·�
	int m_nSex;	// �Ա� 1--�� 2--Ů 3--���� 4--δ֪
	CString m_szName;	// ����
};

template<typename T>
inline CAVLTree<T>& CStudent::CreateStuIDAVLTree()
{
	CAVLTree<T>* StuIDAVLTree = new CAVLTree<T>();
	return *StuIDAVLTree;
}

template<typename T>
inline CAVLTree<T>& CStudent::CreateStuNameAVLTree()
{
	CAVLTree<T>* StuNameAVLTree = new CAVLTree<T>();
	return *StuNameAVLTree;
}
