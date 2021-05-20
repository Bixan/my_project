#pragma once
/*===================================================
*			o(*������*)o
************************************************************************************
*			Project Name��ѧ����Ϣ���� -- ƽ�������
*
*			FileName��MyAVLTree.h
*
*			Start Date��2020/06/11
*
*			Last UpDate��2020/06/15
*
*====================================================*/
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <sstream>
#include "CString.h"
#include "CStack.h"
using namespace std;

template<typename T>
class CAVLTree	
{
private:
	// ���������
	typedef struct tagStuNode
	{
		tagStuNode(T nStuID, CString szStuName, 
			int nBirthYear, 
			int nBirthMonth, 
			int nSex, 
			int nNodeHeight, 
			tagStuNode* pParent, tagStuNode* pLeft, tagStuNode* pRight) :
			m_nStuID(nStuID),
			m_szStuName(szStuName),
			m_nYear(nBirthYear),
			m_nMonth(nBirthMonth),
			m_nSex(nSex),
			m_nNodeHeight(nNodeHeight),
			m_pParent(pParent),
			m_pLeft(pLeft),
			m_pRight(pRight)
		{}
		T	m_nStuID;	// ID
		int	m_nYear;	// �������
		int	m_nMonth;	// �����·�
		int	m_nSex;	// �Ա� 1--�� 2--Ů 3--���� 4--δ֪
		CString m_szStuName;	// ѧ������
		int m_nNodeHeight;	// ���н��ĸ߶�
		tagStuNode* m_pParent;	// ָ�򸸽��
		tagStuNode* m_pLeft;	// ��ǰ��������
		tagStuNode* m_pRight;	// ��ǰ�����Һ���
	}STUNODE, *PSTUNODE;

public:
	CAVLTree();
	CAVLTree(const  CAVLTree& avl);

	CAVLTree& operator=(const CAVLTree& avl);
	virtual ~CAVLTree();

public:
	// ��������
	bool Insert(T nStuID, CString szStuName, int nBirthYear, int nBirthMonth, int nSex);	// ��������
	bool Remove(T nStuID);	// ͨ��IDɾ����Ӧ�������
	bool Updata(T nStuIDSrc, T nStuIDDst);	// ��ԭ������ϵ���ֵ�����µ���ֵ
	bool Change(T nStuID);	// ͨ�� ID �޸�ѧ������
	bool Find(T nStuID);	// ͨ��ID���Ҷ�Ӧ��ѧ������
	bool FindStuName(CString szStuName);	// ��������㣬�����еĽ��������ջ
	bool FindStuName1(T nStuID, CString szStuName);	// ͨ��ѧ�����������ڴ��ж�Ӧ��ѧ����Ϣ
	void Clear();	// ������ĸ������

	// ʹ�õݹ�������ݣ�ǰ�����򡢺�����
	void MLR();	// ǰ��������ݹ飩middle left right
	void LMR();	// ����������ݹ飩 left middle right
	void LRM();	// ����������ݹ飩 left right middle

	int GetNodeSize()const;	// ��ȡ���н�����
	void InFile(PSTUNODE pNode);	// д���ļ�
private:
	void Destroy(PSTUNODE pNode);	// ������
	PSTUNODE FindNode(T nStuID);	// ���ҽ��
	PSTUNODE CopyNewData(PSTUNODE pNode);	// ��������ʱ���������������
	void DelNode(PSTUNODE pNode);	// ���ݴ�ɾ����������λ�ý��ж�Ӧ��ɾ������

	void DelLeaf(PSTUNODE pNode);	// Ҷ�ӽ��
	void DelSingleChildNode(PSTUNODE pNode);	// ����֧���
	void DelDoubleChildNode(PSTUNODE pNode);	// ˫��֧���

	void MLR(PSTUNODE pNode);	// �ݹ� ǰ�� ����
	void LMR(PSTUNODE pNode);	// �ݹ� ���� ����
	void LRM(PSTUNODE pNode);	// �ݹ� ���� ����

public:
	static void ReadAVLTreeStuDataToInFile(CAVLTree<T> &avl);	// ���ļ���ȡ����
	void WirteAVLTreeStuDataToInFile0();	// ����OutFile1�ݹ齫���е�����д�뵽�ļ���
	void WirteAVLTreeStuDataToInFile1(PSTUNODE pNode);		// �ݹ齫���е�����д���ļ�(�ٶ���)
	void WirteAVLTreeStuDataToInFile();	// �����е�����д���ļ�

	void RemoveCopyStuData();	// �޸������ļ���

private:
	void AdjustHeight(PSTUNODE pNode);	// ���ݽ�����Һ��Ӹ߶Ȳ�ж����Ƿ�ƽ�⣬��ƽ��������Ӧ�Ĵ���
	int CalcNodeHeight(PSTUNODE pNode);	// ������߶�
	int GetNodeHeight(PSTUNODE pNode)const;	// ��ȡ���߶�
	void RotateLeft(PSTUNODE pNode);	// ����
	void RotateRight(PSTUNODE pNode);	// �ҵ���


private:
	PSTUNODE m_pRoot;	// ��ǰ���ĸ����
	static int m_pStuSize;	// ����ѧ��������
};


/*=======================================================
*	int CAVLTree<T>::m_pStuSize = 0; -- ��ʼ��ѧ������Ϊ 0
*
*	��������
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/12
*
========================================================*/
template<typename T>
int CAVLTree<T>::m_pStuSize = 0;


/*=======================================================
*	CAVLTree<T>::CAVLTree() -- �޲ι��죬��ʼ����
*
*	��������
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/12
*
========================================================*/
template<typename T>
CAVLTree<T>::CAVLTree()
{
	m_pRoot = nullptr;
}

/*=======================================================
*	CAVLTree<T>::CAVLTree(const CAVLTree& bst) -- ��������
*
*	�������ѳ�ʼ���������
*
*	����ֵ��Nothing
*
*	���棺��
*
*	���£�2020/06/12
*
========================================================*/
template<typename T>
CAVLTree<T>::CAVLTree(const CAVLTree& avl)
{
	*this = avl;
}


/*=======================================================
*	CAVLTree<T>& CAVLTree<T>::operator=(const CAVLTree& bst) -- ��������� =	
*
*	�������ѳ�ʼ���������
*
*	����ֵ��*this
*
*	���棺��
*
*	���£�2020/06/12
*
========================================================*/
template<typename T>
CAVLTree<T>& CAVLTree<T>::operator=(const CAVLTree& avl)
{
	if (this == &avl)
	{
		return *this;	// ����Լ����Լ���ֵ��ʲôҲ����
	}

	Clear();	// ����Լ��ĸ����������

	if (avl.m_pRoot == nullptr)
	{
		return *this;	// �����ֵ��Ϊ�����������и�ֵ
	}

	// ��ֵ
	m_pRoot = CopyNewData(avl.m_pRoot);	// Ϊ��ǰ�������µ�����

	return *this;
}


/*=======================================================
*	CAVLTree<T>::~CAVLTree() -- ������������� Clear() �����ͷŶ�Ӧ������
*
*	��������
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/12
*
========================================================*/
template<typename T>
CAVLTree<T>::~CAVLTree()
{
	Clear();
}


/*=======================================================
*	bool CAVLTree<T>::Insert(T nStuID, CString szStuName, int nBirthYear, 
*	int nBirthMonth, int nSex)	-- ��ӵ�ѧ����Ϣ
*
*	������nStuID -- �����ѧ��ID��szStuName -- ѧ��������nBirthYear -- ѧ���������
*	nBirthMonth -- ѧ�������·ݣ�nSex -- ѧ���Ա�
*	����ֵ��true -- ���ݲ���ɹ���false -- ���ݲ���ʧ��
*
*	���棺��
*
*	���£�2020/06/12
*
========================================================*/
template<typename T>
bool CAVLTree<T>::Insert(T nStuID, CString szStuName, int nBirthYear, int nBirthMonth, int nSex)
{
	if (m_pRoot == nullptr)
	{
		// �����Ϊ�գ������������Ϊ����㣬ͬʱ���ڵ�ĸ߶���Ϊ0
		m_pRoot = (PSTUNODE)new STUNODE(nStuID, szStuName, nBirthYear, nBirthMonth, nSex, 0, nullptr, nullptr, nullptr);
		m_pStuSize++;	// ѧ��������1
		//InFile(m_pRoot);
		return true;
	}

	// �����ǰ����Ϊ�գ��ͽ���ǰ���ĸ�������ݸ�ֵ����ʱ�� pTempNode
	PSTUNODE pTempNode = m_pRoot;

	// �������ҿ��Բ����λ�ã����������ĸ�����ʼ
	while (true)
	{
		// ��������������ֵС�ڵ�ǰ��㣬��ȡ�����Ӳ��ֲ��Һ��ʵ�λ��
		if (nStuID < pTempNode->m_nStuID)
		{
			// ����˽��û�����ӣ����²������ֵ��Ϊ��ǰ��������
			if (pTempNode->m_pLeft == nullptr)
			{
				// �²���Ľ��ĸ߶���0����ʱ������ĸ߶ȼ�1
				pTempNode->m_pLeft = new STUNODE(nStuID, szStuName, nBirthYear, nBirthMonth, nSex, 0, pTempNode, nullptr, nullptr);

				// ÿ�β����µĽ�㣬��Ҫ�ж������ִ����Ƿ�ƽ�⣬�����ƽ�⣬������Ӧ�ĵ���
				AdjustHeight(pTempNode);
				m_pStuSize++;
				//InFile(pTempNode->m_pLeft);
				return true;
			}
			else
			{
				// �˽�������ӣ���Ȼ�ý��ָ��ý������ӣ���������Ѱ�ҿ�λ��
				pTempNode = pTempNode->m_pLeft;
			}
		}
		// ��������������ֵ���ڵ�ǰ��㣬��ȡ���Һ����ж�
		else if (nStuID > pTempNode->m_nStuID)
		{
			if (pTempNode->m_pRight == nullptr)
			{
				pTempNode->m_pRight = new STUNODE(nStuID, szStuName, nBirthYear, nBirthMonth, nSex, 0, pTempNode, nullptr, nullptr);

				// ÿ�β����µĽ�㣬��Ҫ�ж������ִ����Ƿ�ƽ�⣬�����ƽ�⣬������Ӧ�ĵ���
				AdjustHeight(pTempNode);
				m_pStuSize++;
				//InFile(pTempNode->m_pRight);
				return true;
			}
			else
			{
				// �˽�����Һ��ӣ���Ȼ�ý��ָ��ý����Һ��ӣ���������Ѱ�ҿ�λ��
				pTempNode = pTempNode->m_pRight;
			}
		}
		else
		{
			cout << "ѧ���Ѵ���" << endl;
			return false;	// ѧ��ID�ظ�����false
		}
	}
	return true;
}


/*=======================================================
*	bool CAVLTree<T>::Remove(T nStuID) -- ɾ��ָ����ѧ����Ϣ
*
*	������nStuID -- ɾ����ѧ��ID
*
*	����ֵ��true -- ����ɾ���ɹ���false -- ����ɾ��ʧ��	
*
*	���棺��
*
*	���£�2020/06/13
*
========================================================*/
template<typename T>
bool CAVLTree<T>::Remove(T nStuID)
{
	PSTUNODE pNodeTodel = FindNode(nStuID);	// ���ҽ�Ҫ��ɾ����ѧ��ID
	if (pNodeTodel == nullptr)
	{
		cout << "�ļ���û���ҵ���Ҫɾ��ѧ�����ݶ�Ӧ��ID" << endl;
		return false;	// pNodeTodel Ϊ�� ����ʧ�� ���� false
	}

	DelNode(pNodeTodel);	// ���ҳɹ������ݽ������������һ������
	cout << "ѧ������ɾ���ɹ�" << endl;
	return true;
}


/*=======================================================
*	bool CAVLTree<T>::Updata(T nStuIDSrc, T nStuIDDst) -- �޸�����
*
*	������nStuIDSrc -- ���޸ĵ�����ֵ��nStuIDDst -- �޸ĺ������ֵ
*
*	����ֵ��false -- ���ݲ���/�޸�ʧ�ܣ�true -- �����޸ĳɹ�
*
*	���棺��
*
*	���£�2020/06/12
*
========================================================*/
// �����д��ڵ���ֵ nStuIDSrc �޸�Ϊ nStuIDDst
template<typename T>
bool CAVLTree<T>::Updata(T nStuIDSrc, T nStuIDDst)
{
	if (m_pRoot == nullptr)
	{
		return false;	// �����ǰ��Ϊ�գ�����false
	}

	PSTUNODE pChangeNode = FindNode(nStuIDSrc);	// �����Ƿ���� nStuIDSrc
	if (pChangeNode == nullptr)
	{
		return false;	// pNodeTodel Ϊ�� ����ʧ�� ���� false
	}

	//pChangeNode->m_nStuID = nStuIDDst;	// ֱ���޸ģ�����ȡ���ᵼ�¸�������һ��������

	Remove(nStuIDSrc);	// ��ֵ���ҳɹ���ɾ�����
	Insert(nStuIDDst);	// ���µ���ֵ���²��뵽������

	return true;
}


/*=======================================================
*	bool CAVLTree<T>::Change(T nStuID) -- ͨ��ѧ��ID���޸�����Ӧ������
*
*	������nStuIDSrc -- ���޸ĵ�����ֵ��nStuIDDst -- �޸ĺ������ֵ
*
*	����ֵ��false -- ���ݲ���/�޸�ʧ�ܣ�true -- �����޸ĳɹ�
*
*	���棺��
*
*	���£�2020/06/13
*
========================================================*/
template<typename T>
bool CAVLTree<T>::Change(T nStuID)
{
	if (m_pRoot == nullptr)
	{
		return false;	// ���ĸ����Ϊ�գ�����false���������޸Ĳ���
	}

	// ��ѯѧ��ID�Ƿ���ڣ������������Ӧ����ʾ��Ϣ
	PSTUNODE pNode = FindNode(nStuID);
	if (pNode == nullptr)
	{
		cout << "��ѯ��ѧ��ID������" << endl;
		return false;
	}

	// ��ѯ��Ӧ��ѧ��ID���ڣ������Ӧ����Ϣ
	cout << "ѧ����Ϣ���ҳɹ�����Ϣ���£�" << endl;
	cout << "ID��" << pNode->m_nStuID << " "
		<< "������" << pNode->m_szStuName << " "
		<< "������ݣ�" << pNode->m_nYear << " "
		<< "�����·�" << pNode->m_nMonth << " "
		<< "�Ա�(1/�У�2/Ů��3/������4/δ֪)��" << pNode->m_nSex << endl;

	CString szChangeName = nullptr;	// �洢ѧ������
	int nBirthYear = 0;	// �洢ѧ���������
	int nBirthMonth = 0;	// �洢ѧ�������·�
	int nSex = 0;	// �洢ѧ���Ա�

	// �����޸ĺ�����ݣ�������Ӧ�����ݸ�ֵ��ԭ������϶�Ӧ������
	cout << "��������Ҫ�޸ĺ��ѧ��������";
	cin >> szChangeName;
	pNode->m_szStuName = szChangeName;
	cout << "��������Ҫ�޸ĺ��ѧ��������ݣ�";
	cin >> nBirthYear;
	pNode->m_nYear = nBirthYear;
	cout << "��������Ҫ�޸ĺ��ѧ�������·ݣ�";
	cin >> nBirthMonth;
	pNode->m_nMonth = nBirthMonth;
	cout << "��������Ҫ�޸ĺ��ѧ���Ա�(1/�У�2/Ů��3/������4/δ֪)��";
	cin >> nSex;
	pNode->m_nSex = nSex;

	cout << "��Ӧ��ѧ�������޸ĳɹ�" << endl;
	return true;
}


/*=======================================================
*	bool CAVLTree<T>::Find(T nStuID) -- ����ָ����ѧ��ID
*
*	������nStuID -- Ҫ���ҵ�ѧ��ID
*
*	����ֵ��true -- ���ݲ��ҳɹ���false -- ���ݲ���ʧ��
*
*	���棺��
*
*	���£�2020/06/13
*
========================================================*/
template<typename T>
bool CAVLTree<T>::Find(T nStuID)
{
	if (m_pRoot == nullptr)
	{
		return false;	// �����ǰ��Ϊ�գ�����false�������в�ѯ����
	}

	PSTUNODE pChangeNode = FindNode(nStuID);	// ���������Ƿ����ѧ��ID nStuID
	if (pChangeNode == nullptr)
	{
		cout << "ͨ��IDû�в��ҵ���Ҫ���ҵ�ѧ����Ϣ" << endl;
		return false;	// pChangeNode Ϊ�� ����ʧ�� ���� false
	}

	// ��Ѱ���������ID��Ӧ��ѧ����Ϣ
	cout << "ѧ����Ϣ���ҳɹ�����Ϣ���£�" << endl;
	cout << "ID��" << pChangeNode->m_nStuID << " "
		<< "������" << pChangeNode->m_szStuName << " "
		<< "������ݣ�" << pChangeNode->m_nYear << " "
		<< "�����·�" << pChangeNode->m_nMonth << " "
		<< "�Ա�(1/�У�2/Ů��3/������4/δ֪)��" << pChangeNode->m_nSex << endl;

	return true;
}


/*=======================================================
*	bool CAVLTree<T>::FindStuName(CString szStuName)
*	-- ͨ��ѧ��������ѯ��Ӧ��ѧ������
*	������CString szStuName -- Ҫ��ѯ��ѧ������
*
*	����ֵ��true -- ���ݲ��ҳɹ���false -- ���Ϊ�գ�����false
*
*	���棺��
*
*	���£�2020/06/16
*
========================================================*/
template<typename T>
bool CAVLTree<T>::FindStuName(CString szStuName)
{
	if (m_pRoot == nullptr)
	{
		return false;	// �����ǰ��Ϊ�գ�����false
	}
	
	PSTUNODE pTempNode = m_pRoot;	// �Ӹ����ĸ���㿪ʼ����

	CStack<PSTUNODE> stk;	// ��ʼ��һ��ջ
	PSTUNODE pNode = m_pRoot;	// �����ڵ㸳ֵ�� pNode

	while (true)
	{
		while (pNode != nullptr)
		{
			stk.push(pNode->m_nStuID);	// ��������ѧ��ID��ջ
			pNode = pNode->m_pLeft;	// pNode ָ��ǰ��������
		}

		if (stk.empty())
		{
			break;	// ջ��ѭ������
		}

		int nTempNum = stk.top();	// ��ȡջ������
		pNode = FindNode(nTempNum);	// ��ջ�����ݶ�Ӧ�Ľ�㸳ֵ�� pNode
		if (pNode == nullptr)
		{
			return false;
		}
		stk.pop();	// ��ջ������һ������
		FindStuName1(nTempNum, szStuName);
		
		pNode = pNode->m_pRight;	// ������������ݵ��Һ���
	}
	return true;
}


/*=======================================================
*	bbool CAVLTree<T>::FindStuName1(T nStuID, CString szStuName)
*	-- ���ݴ���Ĳ������������д��ڵ�������ͬ������ѧ����Ϣ
*	������T nStuID -- ������Ӧ��ѧ��ID��CString szStuName -- Ҫ��ѯ����ϵ������
*
*	����ֵ��true -- ���ݲ��ҳɹ���false -- ���Ϊ�գ�����false
*
*	���棺��
*
*	���£�2020/06/16
*
========================================================*/
template<typename T>
bool CAVLTree<T>::FindStuName1(T nStuID, CString szStuName)
{
	if (m_pRoot == nullptr)
	{
		return false;	// �����ǰ��Ϊ�գ�����false
	}

	PSTUNODE pChangeNode = FindNode(nStuID);	// ���������Ƿ������ֵ nStuID
	if (pChangeNode == nullptr)
	{
		cout << "���ݲ���ʧ��" << endl;
		return false;
	}

	if (pChangeNode->m_szStuName == szStuName)
	{
		Find(nStuID);
		return true;
	}
	return true;
}


/*=======================================================
*	void CAVLTree<T>::Clear() -- �������������Ч�Ľ��	
*
*	��������
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::Clear()
{
	while (m_pRoot != nullptr)
	{
		DelNode(m_pRoot);	// ���н��ɾ����Ϻ�ѭ����ֹ
	}
}

/*=======================================================
*	void CAVLTree<T>::MLR() -- �ݹ� ǰ�� ����������Ϊ�գ�����MLR()������
*
*	��������
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::MLR()
{
	if (m_pRoot != nullptr)
	{
		MLR(m_pRoot);	// ���� ǰ�� ����
	}
}

/*=======================================================
*	void CAVLTree<T>::LMR() -- �ݹ� ���� ����������Ϊ�գ�����LMR()������
*
*	��������
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::LMR()
{
	if (m_pRoot != nullptr)
	{
		LMR(m_pRoot);	// ���� ���� ����
	}
}

/*=======================================================
*	void CAVLTree<T>::LRM() -- �ݹ� ���� ����	������Ϊ�գ�����LRM()������
*
*	��������
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::LRM()
{
	if (m_pRoot != nullptr)
	{
		LRM(m_pRoot);	// ���� ���� ����
	}
}


/*=======================================================
*	int CAVLTree<T>::GetNodeSize() const -- ��ȡ��ǰ������Ч�Ľڵ�����
*
*	��������
*
*	����ֵ�����ص�ǰ������Ч�Ľڵ�����
*
*	���棺��
*
*	���£�2020/06/12
*
========================================================*/
template<typename T>
int CAVLTree<T>::GetNodeSize() const
{
	return m_pStuSize;
}

template<typename T>
void CAVLTree<T>::InFile(PSTUNODE pNode)
{
	fstream InFile;
	InFile.open("StudentData.bin", ios_base::out | ios_base::in | ios_base::binary);
	if (!InFile.is_open())
	{
		cout << "�ļ���ʧ�ܣ������ļ��Ƿ����" << endl;
		exit(-1);
	}
	InFile.seekp(0, ios_base::end);
	InFile << pNode->m_nStuID << " "
		<< pNode->m_szStuName << " "
		<< pNode->m_nYear << " "
		<< pNode->m_nMonth << " "
		<< pNode->m_nSex << endl;
	InFile.close();
}


/*=======================================================
*	void CAVLTree<T>::Destroy(PSTUNODE pNode) -- ʹ�õݹ����������Ч�����н��
*
*	������PSTUNODE pNode -- ���ĸ����
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::Destroy(PSTUNODE pNode)
{
	if (pNode != nullptr)
	{
		Destroy(pNode->m_pLeft);	// ���ٸ�������
		Destroy(pNode->m_pRight);	// ���ٸ����Һ���

		delete pNode;	// ���ٸ�
		pNode = nullptr;
	}
}


/*=======================================================
*	PSTUNODE CAVLTree<T>::FindNode(T nStuID)		
*	-- �������������ֵ���������Ƿ���ڶ�Ӧ������
*	������nStuID -- ���ҵ�����ֵ	
*
*	����ֵ��pTempNode ���ز鵽�Ĵ洢�����ݵĽ��
*
*	���棺��
*
*	���£�2020/06/12
*
========================================================*/
template<typename T>
typename CAVLTree<T>::PSTUNODE CAVLTree<T>::FindNode(T nStuID)
{
	// �Ӹ����ĸ���㿪ʼ����
	PSTUNODE pTempNode = m_pRoot;

	while (pTempNode != nullptr)
	{
		// ������ҵ����ݺ͵�ǰ������ֵ��ȣ��ͷ��ظý��
		if (nStuID == pTempNode->m_nStuID)
		{
			return pTempNode;
		}
		// ������ҵ����ݴ��ڵ�ǰ������ֵ����ȥ�ý����Ҳ��ֽ��в���
		else if (nStuID > pTempNode->m_nStuID)
		{
			pTempNode = pTempNode->m_pRight;
		}
		else
		{
			// ������ҵ�����С�ڵ�ǰ������ֵ����ȥ�ý����󲿷ֽ��в���
			pTempNode = pTempNode->m_pLeft;
		}
	}
	return nullptr;
}


/*=======================================================
*	Ptypename CAVLTree<T>::PSTUNODE CAVLTree<T>::CopyNewData(PSTUNODE pNode)
*	-- ��������ʱ���������������
*	������PSTUNODE pNode -- �����Ľ��
*
*	����ֵ��pTempNode ���ؿ�����Ľ�����ݣ�nullptr ���Ϊ�գ������п���
*
*	���棺��
*
*	���£�2020/06/15
*
========================================================*/
template<typename T>
typename CAVLTree<T>::PSTUNODE CAVLTree<T>::CopyNewData(PSTUNODE pNode)
{
	if (pNode != nullptr)
	{
		PSTUNODE pTempRoot = nullptr;
		pTempRoot = new STUNODE(pNode->m_nStuID, pNode->m_szStuName, pNode->m_nYear, 
			pNode->m_nMonth, pNode->m_nSex, pNode->m_nNodeHeight, pNode->m_pParent, 
			pNode->m_pLeft, pNode->m_pRight);
		return pTempRoot;
	}
	return nullptr;
}


/*=======================================================
*	void CAVLTree<T>::DelNode(PSTUNODE pNode)
*	-- ɾ����㣬���ݽ�����ͽ��ж�Ӧ��ɾ��
*	������PSTUNODE pNode -- Ҫɾ������������Ӧ�Ľ��
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::DelNode(PSTUNODE pNode)
{
	// ��Ҫɾ���Ľ����н��λ���ж�

	// Ҷ�ӽ��
	if (pNode->m_pLeft == nullptr && pNode->m_pRight == nullptr)
	{
		DelLeaf(pNode);
		return;
	}
	// ����֧���
	else if (pNode->m_pLeft == nullptr || pNode->m_pRight == nullptr)
	{
		DelSingleChildNode(pNode);
		return;
	}
	// ˫��֧���
	else
	{
		DelDoubleChildNode(pNode);
	}
}


/*=======================================================
*	void CAVLTree<T>::DelLeaf(PSTUNODE pNode)
*	-- ɾ���Ľ��ΪҶ�ӽ�㣨�ý��û�����Һ��ӣ�
*	������PSTUNODE pNode -- Ҫɾ������������Ӧ�Ľ��
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::DelLeaf(PSTUNODE pNode)
{
	PSTUNODE pParent = pNode->m_pParent;	// ���浱ǰ���ĸ����

	if (pNode == m_pRoot)	// �����ǰ����Ǹ���㣬��ɾ���ý��
	{
		delete m_pRoot;
		m_pRoot = nullptr;
		m_pStuSize--;
		return;
	}

	// �ж�Ҫɾ���Ľ���Ǹ�����µ� ���� ���� �Һ���
	if (pParent->m_pLeft == pNode)
	{
		pParent->m_pLeft = nullptr;	// ��������ӣ��������ӵĽ���ÿ�
	}
	else
	{
		pParent->m_pRight = nullptr;	// ������Һ��ӣ������Һ��ӵĽ���ÿ�
	}

	delete pNode;	// ɾ���˽��
	m_pStuSize--;
	pNode = nullptr;

	// ���ɾ���󣬴�ɾ�����ĸ���㿪ʼ���������е����߶�
	AdjustHeight(pParent);
}


/*=======================================================
*	void CAVLTree<T>::DelSingleChildNode(PSTUNODE pNode)
*	-- ɾ���Ľ��Ϊ����֧��㣨�ý����һ������ ����/�Һ��ӣ�
*	������PSTUNODE pNode -- Ҫɾ������������Ӧ�Ľ��
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::DelSingleChildNode(PSTUNODE pNode)
{
	PSTUNODE pParent = pNode->m_pParent;	// ���浱ǰ���ĸ����

	PSTUNODE pChild = pNode->m_pLeft;	// Ĭ�ϱ��浱ǰ��������

	if (pChild == nullptr)
	{
		pChild = pNode->m_pRight;	// �������Ϊ�գ�˵���õ���֧���ֻ���Һ��ӣ������к���
	}

	// �жϴ˽���Ƿ��Ǹ����
	if (pNode == m_pRoot)
	{
		m_pRoot = pChild;	// ���ý������ӻ����к�����Ϊ�����
		pChild->m_pParent = nullptr;	// �ÿ�
		delete pNode;	// ɾ����ǰ���
		m_pStuSize--;
		pNode = nullptr;
		return;
	}

	// ɾ����������
	if (pParent->m_pLeft == pNode)
	{
		pParent->m_pLeft = pChild;
	}
	// ɾ�������Һ���
	else
	{
		pParent->m_pRight = pChild;
	}

	// ɾ�����ĺ��ӽ��ָ��ɾ�����ĸ��׽��
	pChild->m_pParent = pParent;

	// ɾ���ý��
	delete pNode;
	m_pStuSize--;
	pNode = nullptr;

	// ���ɾ���󣬴�ɾ�����ĸ���㿪ʼ���������е����߶�
	AdjustHeight(pParent);
}


/*=======================================================
*	void CAVLTree<T>::DelSingleChildNode(PSTUNODE pNode)
*	-- ɾ���Ľ��Ϊ˫��֧��㣨�ý������������ ���Ӻ��Һ��ӣ�
*	������PSTUNODE pNode -- Ҫɾ�����ݶ�Ӧ�Ľ��
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::DelDoubleChildNode(PSTUNODE pNode)
{
	// ����Ҫɾ�����������������ֵ��Ҫɾ����������ֵ���н���
	PSTUNODE pTempNode = pNode->m_pLeft;

	while (pTempNode->m_pRight != nullptr)
	{
		// �ҵ������������ҽ�㣬�ý�㱣�������Ϊ�������е�ֵ��������
		pTempNode = pTempNode->m_pRight;
	}

	// �ҵ��������е����ֵ��������ֵ
	T nTempNum = pNode->m_nStuID;
	pNode->m_nStuID = pTempNode->m_nStuID;
	pTempNode->m_nStuID = nTempNum;

	// ɾ�� pTempNode����ʱ����Ҫ�жϽ�����Ľ�㴦���������
	DelNode(pTempNode);
}


/*=======================================================
*	void CAVLTree<T>::MLR(PSTUNODE pNode) -- �ݹ� ǰ�� ����
*
*	������PSTUNODE pNode -- ��ʼ�����ĸ����
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::MLR(PSTUNODE pNode)
{
	// �ݹ�ĳ���
	if (pNode == nullptr)
	{
		return;
	}
	cout << pNode->m_nStuID << " ";	// ���Լ�
	MLR(pNode->m_pLeft);	// ������
	MLR(pNode->m_pRight);	// ���Һ���
}


/*=======================================================
*	void CAVLTree<T>::LMR(PSTUNODE pNode) -- �ݹ� ���� ����
*
*	������PSTUNODE pNode -- ��ʼ�����ĸ����
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::LMR(PSTUNODE pNode)
{
	// �ݹ�ĳ���
	if (pNode == nullptr)
	{
		return;
	}

	LMR(pNode->m_pLeft);	// ������
	cout << "ID��" << pNode->m_nStuID << " "
		<< "������" << pNode->m_szStuName << " "
		<< "������ݣ�" << pNode->m_nYear << " "
		<< "�����·�" << pNode->m_nMonth << " "
		<< "�Ա�(1/�У�2/Ů��3/������4/δ֪)��" << pNode->m_nSex << endl;
	LMR(pNode->m_pRight);	// ���Һ���
}


/*=======================================================
*	void CAVLTree<T>::LRM(PSTUNODE pNode) -- �ݹ� ���� ����
*
*	������PSTUNODE pNode -- ��ʼ�����ĸ����
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::LRM(PSTUNODE pNode)
{
	// �ݹ�ĳ���
	if (pNode == nullptr)
	{
		return;
	}

	LRM(pNode->m_pLeft);	// ������
	LRM(pNode->m_pRight);	// ���Һ���
	cout << pNode->m_nStuID << " ";	// ���Լ�
}


/*=======================================================
*	void CAVLTree<T>::ReadAVLTreeStuDataToInFile(CAVLTree<T> &avl)
*	-- ���ļ��ж�ȡ���ݵ����ж�Ӧ�ĸ����
*	������CAVLTree<T> &avl -- ����������
*
*	����ֵ���ļ���ʧ�ܣ����������Ϣ���˳�����
*
*	���棺��
*
*	���£�2020/06/15
*
========================================================*/
template<typename T>
void CAVLTree<T>::ReadAVLTreeStuDataToInFile(CAVLTree<T> &avl)
{
	ifstream InFile;
	InFile.open("StudentCopyData.bin");
	if (!InFile.is_open())
	{
		InFile.open("StudentData.bin");
		if (!InFile.is_open())
		{
			cout << "�ļ���ʧ�ܣ������ļ��Ƿ����" << endl;
			exit(-1);
		}
		char szStrTemp[1024];	// �洢��ȡ����һ����ʱ����
		while (InFile.getline(szStrTemp, 1024))
		{
			istringstream isStr(szStrTemp);	// ���ļ��ж�ȡһ�����ݣ�������ֱ���ֵ����Ӧ�ı���
			int stuID;
			int stuYear;
			int stuMonth;
			int stuSex;
			CString stuName;

			isStr >> stuID;
			isStr >> stuName;
			isStr >> stuYear;
			isStr >> stuMonth;
			isStr >> stuSex;

			avl.Insert(stuID, stuName, stuYear, stuMonth, stuSex);
		}
		InFile.close();

		// �ļ���ȡ�ɹ��󣬽�ԭʼ�ļ��������ݣ����öϵ���������
		CopyFile(TEXT("StudentData.bin"), TEXT("StudentCopyData.bin"), false);
		return;
	}

	// ɾ��Դ�ļ����������ݵ��ļ�����Ϊԭ�����ļ����������´δ�ʹ��
	if (_unlink("StudentData.bin") == 0)
	{
		//cout << "Դ�ļ�ɾ���ɹ�" << endl;
	}

	char szStrTemp[1024];	// �洢��ȡ����һ����ʱ����
	while (InFile.getline(szStrTemp, 1024))
	{
		istringstream isStr(szStrTemp);	// ���ļ��ж�ȡһ�����ݣ�������ֱ���ֵ����Ӧ�ı���
		int stuID;
		int stuYear;
		int stuMonth;
		int stuSex;
		CString stuName;

		isStr >> stuID;
		isStr >> stuName;
		isStr >> stuYear;
		isStr >> stuMonth;
		isStr >> stuSex;

		avl.Insert(stuID, stuName, stuYear, stuMonth, stuSex);
	}
	InFile.close();

	// �ļ���ȡ�ɹ��󣬽�ԭʼ�ļ��������ݣ����öϵ���������
	CopyFile(TEXT("StudentCopyData.bin"), TEXT("StudentData.bin"), false);

}


/*=======================================================
*	void CAVLTree<T>::WirteAVLTreeStuDataToInFile0()
*		-- ����WirteAVLTreeStuDataToInFile1()���� ʹ�õݹ齫���ж�Ӧ������д�뵽�ļ�
*	��������
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/16
*
========================================================*/
template<typename T>
void CAVLTree<T>::WirteAVLTreeStuDataToInFile0()
{
	if (m_pRoot != nullptr)
	{
		cout << "ѧ������д����...";
		if (unlink("StudentData.bin") == 0)
		{
			//cout << "Դ�ļ�ɾ���ɹ�" << endl;
		}
		if (rename("Temp.bin", "StudentData.bin") == 0)
		{
			//cout << "���������ļ��������ֳɹ�" << endl;
		}
		WirteAVLTreeStuDataToInFile1(m_pRoot);
	}

	// �����º������д�뵽���������ļ���
	CopyFile(TEXT("StudentData.bin"), TEXT("StudentCopyData.bin"), false);	

	// ����RemoveFileName() ɾ��Դ�ļ����������ݵ��ļ�����Ϊԭ�����ļ����������´δ�ʹ��
	//RemoveFileName();
}


/*=======================================================
*	void CAVLTree<T>::WirteAVLTreeStuDataToInFile1(PSTUNODE pNode)
*	-- ʹ�õݹ齫���ж�Ӧ������д�뵽�ļ�
*	��������
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/16
*
========================================================*/
template<typename T>
void CAVLTree<T>::WirteAVLTreeStuDataToInFile1(PSTUNODE pNode)
{
	ofstream WriteFile;
	WriteFile.open("StudentData.bin", ios_base::app);

	if (pNode != nullptr)
	{
		WirteAVLTreeStuDataToInFile1(pNode->m_pLeft);
		WriteFile << pNode->m_nStuID << " "
			<< pNode->m_szStuName << " "
			<< pNode->m_nYear << " "
			<< pNode->m_nMonth << " "
			<< pNode->m_nSex << endl;
		WirteAVLTreeStuDataToInFile1(pNode->m_pRight);
	}

	WriteFile.close();
}


/*=======================================================
*	void CAVLTree<T>::WirteAVLTreeStuDataToInFile()
*	-- �����е�����д���Ӧ�������ļ�
*	��������
*
*	����ֵ���ļ���ʧ�ܣ����������Ϣ���˳�����
*
*	���棺��
*
*	���£�2020/06/16
*
========================================================*/
template<typename T>
void CAVLTree<T>::WirteAVLTreeStuDataToInFile()
{
	cout << "ѧ������д���ڴ���..." << endl;
	ofstream WriteFile;
	WriteFile.open("StudentData.bin");

	if (!WriteFile.is_open())
	{
		cout << "�ļ���ʧ�ܣ������ļ��Ƿ����" << endl;
		exit(-1);
	}
	CStack<PSTUNODE> stk;	// ��ʼ��һ��ջ
	PSTUNODE pNode = m_pRoot;	// �����ڵ㸳ֵ�� pNode

	while (true)
	{
		while (pNode != nullptr)
		{
			stk.push(pNode->m_nStuID);	// ��������Ӧ��ѧ��ID��ջ
			pNode = pNode->m_pLeft;	// pNode ָ��������
		}

		if (stk.empty())
		{
			break;	// ջ��ѭ������
		}

		int nTempNum = stk.top();	// ��ȡջ������
		pNode = FindNode(nTempNum);	// ����ջ�����ݽ���Ӧ�Ľ�����ݸ�ֵ�� pNode
		WriteFile << pNode->m_nStuID << " "
			<< pNode->m_szStuName << " "
			<< pNode->m_nYear << " "
			<< pNode->m_nMonth << " "
			<< pNode->m_nSex << endl;
		stk.pop();	// ��ջ������һ������
		
		pNode = pNode->m_pRight;	// ��������ǰ������ݵ��Һ���
	}

	WriteFile.close();	// �ر��ļ�

	// �����º������д�뵽���������ļ���
	CopyFile(TEXT("StudentData.bin"), TEXT("StudentCopyData.bin"), false);

	// ����RemoveFileName() ɾ��Դ�ļ����������ݵ��ļ�����Ϊԭ�����ļ����������´δ�ʹ��
	//RemoveFileName();
	cout << "---ѧ������д���ڴ����---" << endl;
}


/*=======================================================
*	void CAVLTree<T>::RemoveFileName()
*	-- ɾ��Դ�ļ����������ݵ��ļ�����Ϊԭ�����ļ����������´δ�ʹ��
*	��������
*
*	����ֵ���ļ���ʧ�ܣ����������Ϣ���˳�����
*
*	���棺��
*
*	���£�2020/06/15
*
========================================================*/
template<typename T>
void CAVLTree<T>::RemoveCopyStuData()
{
	// ɾ��Դ�ļ����������ݵ��ļ�����Ϊԭ�����ļ����������´δ�ʹ��
	if (_unlink("StudentData.bin") == 0)
	{
		//cout << "Դ�ļ�ɾ���ɹ�" << endl;
	}
	if (rename("StudentCopyData.bin", "StudentData.bin") == 0)
	{
		//cout << "���������ļ��������ֳɹ�" << endl;
	}
}


/*=======================================================
*	void CAVLTree<T>::AdjustHeight(PSTUNODE pNode)
*	-- ÿ�β������ݣ��жϵ�ǰ���Ƿ�ƽ��ͬʱ���������߶�
*	������PSTUNODE pNode -- ��������ݵĸ����
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::AdjustHeight(PSTUNODE pNode)
{
	PSTUNODE pTempNode = pNode;	// ������Ľ�����ݸ�ֵ����ʱ���pTempNode

	// ��ʱ��Ҫ���Ų���Ľ�������ж������㣨���ڵ�ĸ��ڵ㣩�Ƿ���ƽ��״̬
	while (pTempNode != nullptr)
	{
		// ��ȡ��ǰ���ĸ߶�
		pTempNode->m_nNodeHeight = CalcNodeHeight(pTempNode);

		// �жϴ˽���Ƿ�ƽ�⣬�ж����ݣ��˽������ӵĸ߶ȼ�ȥ�Һ��ӵĸ߶Ȳ�ľ���ֵ���ܴ���1
		int nTempNodeLeftHeight = GetNodeHeight(pTempNode->m_pLeft);
		int nTempNodeRightHeight = GetNodeHeight(pTempNode->m_pRight);

		// ���� AND ��˫��
		if (nTempNodeLeftHeight - nTempNodeRightHeight < -1)
		{
			/*
			1����ȡ��ƽ�����������
			2������ƽ����������������Ӹ߶�С�ڵ����Һ��ӵĸ߶Ⱦ� ��������
			3����֮�����н�����˫��
			*/
			PSTUNODE pTempNodeRightChild = pTempNode->m_pRight;

			if (GetNodeHeight(pTempNodeRightChild->m_pLeft) <=
				GetNodeHeight(pTempNodeRightChild->m_pRight))
			{
				/*
				ʾ����P --> ���ڵ� A ��ͷ�ڵ㣬 X Ϊ�²���Ľ�㣬���Կ�����ʱ��� A ���ֲ�ƽ��
								��תǰ																	��ת��
									P																			P
									A																			B
							T				B							----->						A				D
										C		D													T		C				X
													X
				ע�⣺��תǰ����� B ���Һ��� C����С�� B ������ݵ�
						  ������ת��B ��Ϊ���ڵ㣬C �����ڸ��ڵ� B ���Ҳ��֣�����Ϊ��� A ���Һ���
				*/
				RotateLeft(pTempNode);
			}
			else
			{
				/*
				ʾ����P --> ����� A �ĸ���㣬 X Ϊ�²���Ľ�㣬���Կ�����ʱ��� A ���ֲ�ƽ��
								��תǰ								�ҵ�����											������
									P											P													P
									A											A													C
							T				B							T				C									A				B
										C		D									X			B						T		X					D
									X																	D

				ע�⣺��תǰ����� A ���ֲ�ƽ�⣬�жϷ������Һ��ӵ�����߶ȸ����ҽ��ĸ߶ȣ�
						  ��ʱ��Ҫ������ͼ�� B ������ �ҵ�����Ȼ�����Խ�� A ��������
						  ��˫���󣬴�ʱ��ǰ������ƽ��״̬
				*/

				// ��ȡ��Ҫ������ת�Ľ��
				PSTUNODE pNodeA = pTempNode;
				PSTUNODE pNodeB = pTempNode->m_pRight;

				// ���Խ�� pNodeB �����ҵ�����֮������ pNodeA ���� ����
				RotateRight(pNodeB);
				RotateLeft(pNodeA);
			}
		}
		// ���� �� ��˫��
		else if (nTempNodeLeftHeight - nTempNodeRightHeight > 1)
		{
			PSTUNODE pTempNodeLiftChild = pTempNode->m_pLeft;
			/*
			1����ȡ��ƽ�����������
			2������ƽ����������������Ӹ߶ȴ��ڵ����Һ��ӵĸ߶Ⱦ� �����ҵ���
			3����֮�����н�����˫��
			*/
			if (GetNodeHeight(pTempNodeLiftChild->m_pLeft) >=
				GetNodeHeight(pTempNodeLiftChild->m_pRight))
			{
				/*
				ʾ����P --> ����� B �ĸ���㣬 X Ϊ�²���Ľ�㣬���Կ�����ʱ��� B ���ֲ�ƽ��
								��תǰ												��ת��
									P														P
									B														A
							A				D										T				B
						T		C												X				C		D
					X
				ע�⣺��תǰ����� A ���Һ��� C���Ǵ��� A ������ݵ�
						  ������ת��A ��Ϊ���ڵ㣬C �����ڸ��ڵ� A ���󲿷֣�ӦΪ���B������
				*/
				RotateRight(pTempNode);
			}
			else
			{
				/*
				ʾ����P --> ����� B �ĸ���㣬 X Ϊ�²���Ľ�㣬���Կ�����ʱ��� B ���ֲ�ƽ��
								��תǰ								������										�ҵ�����
									P										P													P
									B										B													C
							A				D						C				D									A				B
						T		C								A		X											T				X		D
									X						T

				ע�⣺��תǰ����� B ���ֲ�ƽ�⣬�жϷ��������ӵ��ҽ��߶ȸ�������ĸ߶ȣ�
						  ��ʱ��Ҫ������ͼ�� A ������ ������Ȼ�����Խ�� B �����ҵ���
						  ��˫���󣬴�ʱ��ǰ������ƽ��״̬
				*/

				// ��ȡ��ת�Ľ��
				PSTUNODE pNodeB = pTempNode;
				PSTUNODE pNodeA = pTempNode->m_pLeft;

				RotateLeft(pNodeA);		// ���е�����
				RotateRight(pNodeB);	// ���е�����
			}
		}
		// �����ǰ���ƽ�⣬�����ж��丸����Ƿ���ƽ��״̬
		pTempNode = pTempNode->m_pParent;
	}
}


/*=======================================================
*	int CAVLTree<T>::CalcNodeHeight(PSTUNODE pNode) -- ���� ������ĸ߶�
*
*	������PSTUNODE pNode -- ��Ҫ����߶ȵĽ��
*
*	����ֵ�����ؽ��ĸ߶�ֵ
*
*	���棺��
*
*	���£�2020/06/12
*
========================================================*/
template<typename T>
int CAVLTree<T>::CalcNodeHeight(PSTUNODE pNode)
{
	/*
	1����ȡ�������ӵĸ߶ȣ����Ϊ�գ�����߶��� -1
	2����ȡ�����Һ��ӵĸ߶ȣ����Ϊ�գ�����߶��� -1
	3��ȡ���Һ��ӽ���и߶����ģ�Ȼ���ټ���1
	*/
	int nNodeLeftHeght = pNode->m_pLeft == nullptr ? -1 : pNode->m_pLeft->m_nNodeHeight;
	int nNodeRightHeight = pNode->m_pRight == nullptr ? -1 : pNode->m_pRight->m_nNodeHeight;

	return nNodeLeftHeght > nNodeRightHeight ? (nNodeLeftHeght + 1) : (nNodeRightHeight + 1);
}


/*=======================================================
*	int CAVLTree<T>::GetNodeHeight(PSTUNODE pNode) const -- ��ȡ���߶�
*
*	������PSTUNODE pNode -- ��Ҫ��ȡ�߶ȵĽ��
*
*	����ֵ�����ؽ��ĸ߶�ֵ
*
*	���棺��
*
*	���£�2020/06/12
*
========================================================*/
template<typename T>
int CAVLTree<T>::GetNodeHeight(PSTUNODE pNode) const
{
	if (pNode == nullptr)
	{
		return -1;	// ���Ϊ�գ����� -1
	}
	return pNode->m_nNodeHeight;
}


/*=======================================================
*	void CAVLTree<T>::RotateLeft(PSTUNODE pNode) --  ��������������ƽ��
*
*	������PSTUNODE pNode -- ��Ҫ������ת�Ľ��
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::RotateLeft(PSTUNODE pNode)
{
	/*
	��ʱ�ӵ�ǰ��㴦��ʼ��ƽ��
	ʾ����P --> ���ڵ� A ��ͷ�ڵ㣬 X Ϊ�²���Ľ�㣬���Կ�����ʱ��� A ���ֲ�ƽ��
	��������ӵĸ߶ȼ�ȥ�Һ��ӵĸ߶Ȳ�ľ���ֵС�� -1 ʱ����Ҫ��������ת���Ӳ�ƽ��㿪ʼ��ת��
					��תǰ																	��ת��
						P																			P
						A																			B
				T				B							----->						A				D
							C		D													T		C				X
										X
	ע�⣺��תǰ����� B ���Һ��� C����С�� B ������ݵ�
			  ������ת��B ��Ϊ���ڵ㣬C �����ڸ��ڵ� B ���Ҳ���
	*/

	// ����תʱ��Ӱ��Ľ��ȡ��
	PSTUNODE pNodeA = pNode;
	PSTUNODE pParent = pNode->m_pParent;
	PSTUNODE pNodeB = pNodeA->m_pRight;
	PSTUNODE pNodeC = pNodeB->m_pLeft;

	// �޸ĸ�����Ӱ��Ľ��
	if (pParent == nullptr)
	{
		m_pRoot = pNodeB;	// ��ʱ��� B ��Ϊ�µĸ����
		pNodeB->m_pParent = nullptr;	// ��ʱ��� B ��ͷ���Ϊ nullptr
	}
	// ���Ǹ��ڵ�
	else
	{
		/*
		1)	�����תǰ��������丸�������ӣ��ͽ���ת��ĸ���� pNodeB
			��Ϊ pNodeA ���������ӣ���֮��Ϊ�Һ���
		2)	��ת���޸���ת�����Ľ��ĸ����
			pNode->m_pRight->m_pParent = pNode->m_pParent;
		*/
		pParent->m_pRight == pNodeA ? pParent->m_pRight = pNodeB : pParent->m_pLeft = pNodeB;
		pNodeB->m_pParent = pParent;
	}

	// ������ pNodeB �������ӣ�����������ԭpNodeB�ĸ����pNodeA���ұ߿�£
	pNodeA->m_pRight = pNodeC;
	if (pNodeC != nullptr)
	{
		pNodeC->m_pParent = pNodeA;
	}

	pNodeA->m_pParent = pNodeB;
	pNodeB->m_pLeft = pNodeA;

	// �޸Ľ��߶�
	pNodeA->m_nNodeHeight = CalcNodeHeight(pNodeA);
	pNodeB->m_nNodeHeight = CalcNodeHeight(pNodeB);
}


/*=======================================================
*	void CAVLTree<T>::RotateRight(PSTUNODE pNode) --  �ҵ�������������ƽ��	
*
*	������PSTUNODE pNode -- ��Ҫ������ת�Ľ��
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::RotateRight(PSTUNODE pNode)
{
	/*
	��ʱ�ӵ�ǰ��㴦��ʼ��ƽ��
	ʾ����P --> ���ڵ� B ��ͷ�ڵ㣬 X Ϊ�²���Ľ�㣬���Կ�����ʱ��� B ���ֲ�ƽ��
	��������ӵĸ߶ȼ�ȥ�Һ��ӵĸ߶Ȳ�ľ���ֵС�� -1 ʱ����Ҫ��������ת���Ӳ�ƽ��㿪ʼ��ת��
					��תǰ												��ת��
						P														P
						B														A
				A				D										T				B
			T		C												X				C		D
		X
	ע�⣺��תǰ����� A ���Һ��� C���Ǵ��� A ������ݵ�
			  ������ת��A ��Ϊ���ڵ㣬C �����ڸ��ڵ� A ���󲿷֣�ӦΪ���B������

	*/

	// ����תʱ���ܵ�Ӱ��ĵĽ��ȡ��
	PSTUNODE pNodeB = pNode;
	PSTUNODE pParent = pNodeB->m_pParent;
	PSTUNODE pNodeA = pNodeB->m_pLeft;
	PSTUNODE pNodeC = pNodeA->m_pRight;

	// �޸���Ӱ��ĸ������

	if (pParent == nullptr)
	{
		m_pRoot = pNodeA;	// ��� pNodeA ��Ϊ�����
		pNodeA->m_pParent = nullptr;	// ����� pNodeA �ĸ����Ϊ nullptr
	}
	else
	{
		/*
		1)	�����תǰ��������丸�������ӣ��ͽ���ת��ĸ���� pNodeA
			��ΪpNodeB���������ӣ���֮��Ϊ�Һ���
		2)	�����ӵĸ���� �޸�Ϊ ��ǰ���ĸ����
			pNode->m_pLeft->m_pParent = pNode->m_pParent;
		*/
		pParent->m_pLeft == pNodeB ? pParent->m_pLeft = pNodeA : pParent->m_pRight = pNodeA;
		pNodeA->m_pParent = pParent;
	}

	// ������ pNodeA �����Һ��ӣ���ת�������Һ��� pNodeC ��Ϊ pNodeB ������
	pNodeB->m_pLeft = pNodeC;
	if (pNodeC != nullptr)
	{
		pNodeC->m_pParent = pNodeB;
	}

	pNodeB->m_pParent = pNodeA;
	pNodeA->m_pRight = pNodeB;

	// ���¼�����Ӱ��Ľ��߶�ֵ
	pNodeA->m_nNodeHeight = CalcNodeHeight(pNodeA);
	pNodeB->m_nNodeHeight = CalcNodeHeight(pNodeB);
}