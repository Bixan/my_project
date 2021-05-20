#pragma once
/*=======================================================
*			o(*￣▽￣*)o
****************************************************************************************
*
*			FileName：CStudent.h -- 学生类（Test）
*
*			Start Date：2020/06/11
*
*			Last UpDate：2020/06/15
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
	static CAVLTree<T>& CreateStuIDAVLTree();	// 以学生ID创建一颗平衡二叉树

	template<typename T>
	static CAVLTree<T>& CreateStuNameAVLTree();	// 以学生姓名创建一颗平衡二叉树

public:
	int m_nID;	// 学生ID
	int m_nYear;	// 出生年份
	int m_nMonth;	// 出生月份
	int m_nSex;	// 性别 1--男 2--女 3--其它 4--未知
	CString m_szName;	// 姓名
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
