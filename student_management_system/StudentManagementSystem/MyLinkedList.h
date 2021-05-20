#pragma once
#include <cassert>

/*=======================================
*			o(*������*)o
*****************************************************************
*			Project Name��˫������
*
*			FileName��MyLinkedList.h
*
*			Start Date��2020/06/02
*
*			Last UpDate��2020/06/16
*
=========================================*/
template<typename T>
class CList
{
private:
	// ˫������Ľ��
	typedef struct tagNode
	{
		tagNode(int val) : m_val(val), m_pPre(nullptr), m_pNext(nullptr) {}
		tagNode() : m_pPre(nullptr), m_pNext(nullptr) {}
		int m_val;	// ���������
		tagNode* m_pPre;	// ָ����һ������ָ�룬ǰ��
		tagNode* m_pNext;	// ָ����һ������ָ�룬���
	}NODE, *PNODE;

public:
	// ������
	class iterator
	{
	private:
		friend class CList;
	public:
		// ���� ǰ��++ �����
		iterator& operator++()
		{
			// �жϵ�ǰ��� �Ƿ��� β���ڱ����
			assert(m_pNode != m_pTailGuard);
			m_pNode = m_pNode->m_pNext;

			return *this;
		}

		// ���� ����++ �����
		iterator operator++(int)
		{
			// �жϵ�ǰ��� �Ƿ��� β���ڱ����
			assert(m_pNode != m_pTailGuard);

			m_pNode = m_pNode->m_pNext;

			// �����޸�ǰ�Ľ��
			return iterator(m_pNode->m_pPre, m_pHeadGuard, m_pTailGuard);
		}

		// ���� ǰ��-- �����
		iterator& operator--()
		{
			// �жϵ�ǰ�ڵ� �Ƿ��� ͷ���ڱ����
			assert(m_pNode->m_pPre != m_pHeadGuard);

			m_pNode = m_pNode->m_pPre;
			return *this;
		}

		// ���� ǰ��-- �����
		iterator operator--(int)
		{
			assert(m_pNode->m_pPre != m_pHeadGuard);
			m_pNode = m_pNode->m_pPre;

			// ���� -- ֮ǰ�Ľ��
			return iterator(m_pNode->m_pNext, m_pHeadGuard, m_pTailGuard);
		}

		// ���� * �����
		int& operator*()
		{
			return m_pNode->m_val;
		}

		// ���� == �����
		bool operator==(const iterator& itr)
		{
			return m_pNode == itr.m_pNode;
		}

		// ���� != �����
		bool operator!=(const iterator& itr)
		{
			return m_pNode != itr.m_pNode;
		}

	private:
		// ����ʹ���ߴ��� pNode ָ�룬���ù�������Ϊ˽��
		iterator(PNODE pNode, PNODE pHeadGuard, PNODE pTailGuard) :
			m_pNode(pNode),
			m_pHeadGuard(pHeadGuard),
			m_pTailGuard(pTailGuard)
		{
		}
	private:
		PNODE m_pNode;
		PNODE m_pHeadGuard;	// ͷ���ڱ�
		PNODE m_pTailGuard;	// β���ڱ�
	};

public:
	iterator begin();	// ���������������ͷ������β
	iterator end();
public:
	iterator rbegin();	// ���������������β������ͷ
	iterator rend();
public:
	CList();
	/*
	���������Զ����ɿ��������Լ����������=
	Ĭ������Ϊǳ�������������������
	*/
	CList(const CList& list);	// ��������
	CList& operator=(const CList& list);	// ��������� =
	virtual ~CList();

public:
	// ����
	bool AddHead(int val);
	bool AddTail(int val);
	bool Insert(iterator pNode, int val);
	// �����˽���ָ�룬�����߿��Զ��������ϵ����ݽ���������޸ģ��ƻ�����ķ�װ��

	// �޸�
	bool SetVal(iterator pNode, int val);	// �����˽���ָ�룬�ƻ�����ķ�װ��

	// ɾ��
	bool RemoveHead();
	bool RemoveTail();
	bool Remove(iterator pNode);	// �����˽���ָ�룬�ƻ�����ķ�װ��

	// ��ѯ
	iterator Find(int val);	// ����ֵΪһ������ָ�� 

	// �жϸ������Ƿ���һ��������
	bool IsNull();

	// ���ĸ���
	int GetNodeSize()const;

	// �������
	void Clear();

private:
	void InitList();
private:
	PNODE m_pHeadGuard;	// ���������ͷ���ڱ�
	PNODE m_pTailGuard;	// ���������β���ڱ�
	int m_nNodeSize;	// �����н��ĸ���
};

/*=======================================================
*	typename CList<T>::iterator CList<T>::begin() -- ����������ͷ����ʼ���������е�����
*
*	�������ѳ�ʼ���������
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/08
*
========================================================*/
template<typename T>
typename CList<T>::iterator CList<T>::begin()
{
	return iterator(m_pHeadGuard->m_pNext, 
		m_pHeadGuard, 
		m_pTailGuard);
}


/*=======================================================
*	typename CList<T>::iterator CList<T>::end() -- ���������������һ�����ĺ��
*
*	�������ѳ�ʼ���������
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/08
*
========================================================*/
template<typename T>
typename CList<T>::iterator CList<T>::end()
{
	return iterator(m_pTailGuard, 
		m_pHeadGuard, 
		m_pTailGuard);
}

/*=======================================================
*	typename CList<T>::iterator CList<T>::rbegin() 
*	-- �����������������һ����㿪ʼ�������
*	�������ѳ�ʼ���������
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/08
*
========================================================*/
template<typename T>
typename CList<T>::iterator CList<T>::rbegin()
{
	return iterator(m_pTailGuard->m_pPre, 
		m_pTailGuard->m_pPre, 
		m_pHeadGuard->m_pPre);
}


/*=======================================================
*	typename CList<T>::iterator CList<T>::rend() -- ������������ͷ����ǰ�����Ϊλ��
*
*	�������ѳ�ʼ���������
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/08
*
========================================================*/
template<typename T>
typename CList<T>::iterator CList<T>::rend()
{
	return iterator(m_pHeadGuard,
		m_pTailGuard->m_pPre, 
		m_pHeadGuard);
}


/*=======================================================
*	CList<T>::CList() -- �޲ι��죬��ʼ������
*
*	�������ѳ�ʼ���������
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/08
*
========================================================*/
template<typename T>
CList<T>::CList()
{
	InitList();
}

/*=======================================================
*	CList<T>::CList(const CList& list) --�������죬
*
*	������const CList& list -- �ѳ�ʼ���������
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/08
*
========================================================*/
template<typename T>
CList<T>::CList(const CList& list)
{
	// ��Ҫ���ڱ��Ͷ�Ӧ�Ľ���������
	InitList();

	// ����������� =
	*this = list;
}


/*=======================================================
*	CList<T>& CList<T>::operator=(const CList& list) -- ��������� =
*
*	������const CList& list -- �ѳ�ʼ���������
*
*	����ֵ������Լ����Լ���ֵ������ *this����ֵ�ɹ����� *this
*
*	���棺��
*
*	���£�2020/06/08
*
========================================================*/
template<typename T>
CList<T>& CList<T>::operator=(const CList& list)
{
	// �ж��ǲ����Լ����Լ���ֵ
	if (this == &list)
	{
		return *this;
	}

	// ������Լ�
	Clear();

	// ѭ����������
	PNODE pNode = list.m_pHeadGuard->m_pNext;
	while (pNode != list.m_pTailGuard)
	{
		AddTail(pNode->m_val);	// ���Է���������ӵ��Լ���������

		pNode = pNode->m_pNext;	// ָ����һ�����
	}
	return *this;
}


/*=======================================================
*	CList<T>::~CList() -- ���� 
*	��������е�������Ч��㣬��ʱ�������ʹ�ã������ڱ����
*	��������
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/08
*
========================================================*/
template<typename T>
CList<T>::~CList()
{
	// ��������е�������Ч���
	Clear();

	// ��ʱ�������ʹ�ã������ڱ����
	delete m_pHeadGuard;
	delete m_pTailGuard;
	m_pHeadGuard = nullptr;
	m_pTailGuard = nullptr;
	m_nNodeSize = 0;
}



/*=======================================================
*	bool CList<T>::AddHead(T val) -- ��ͷ���ǰ��ӽ��
*
*	������val ��ӵ�����
*
*	����ֵ������ Insert ����������ɹ�����true
*
*	���棺��
*
*	���£�2020/06/08
*
========================================================*/
template<typename T>
bool CList<T>::AddHead(int val)
{
	// �µĽ����뵽ͷ���ڱ��ĺ���
	return Insert(begin(), val);
}

/*=======================================================
*	bool CList<T>::AddHead(T val) -- ��β���ǰ��ӽ��
*
*	������val ��ӵ�����
*
*	����ֵ������ Insert ����������ɹ�����true
*
*	���棺��
*
*	���£�2020/06/08
*
========================================================*/
template<typename T>
bool CList<T>::AddTail(int val)
{
	// ����Ľ��Ϊβ��㣬���뵽β���ڱ���ǰ��Ϳ���
	return Insert(end(), val);
}


/*=======================================================
*	bool CList<T>::Insert(iterator itr, T val) -- ��β���ǰ��ӽ��
*
*	������pNode -- Ҫ����Ľ��λ�ã������λ���Ǹýڵ��ǰ�棩valval ��ӵ�����
*
*	����ֵ������ɹ����� true
*
*	���棺��
*
*	���£�2020/06/08
*
========================================================*/
/*
PNODE pNode��Ҫ����Ľ��λ�ã������λ���Ǹýڵ��ǰ�棩
*/
template<typename T>
bool CList<T>::Insert(iterator pNode, int val)
{
	// ������Ҫ�ж�ͷ����β����Ƿ�Ϊ�գ���Ҫ����˼·
	// �����������(�ڱ����)��һ������ͷ�ڵ��ǰ�棬һ������β���ĺ���
	// ��������㲻��洢���ݣ���������ݶ��洢�����������֮�䣬�����ڽ��б���

	// ����һ���µĽ��
	PNODE pNewNode = new NODE(val);

	// �����µĽ��
	PNODE pOldPre = pNode.m_pNode->m_pPre;	// ����������ǰһ�����

	// �½�����һ�����ָ��pNode���
	pNewNode->m_pNext = pNode.m_pNode;
	// �½���ǰһ�����ָ��pNode����ǰһ�����
	pNewNode->m_pPre = pNode.m_pNode->m_pPre;	

	// ������Ľ�� �� ԭ�ȵ����������й���
	//pNode->m_pPre->m_pNext = pNewNode;
	//pNode->m_pPre = pNewNode;

	pNode.m_pNode->m_pPre = pNewNode;
	pOldPre->m_pNext = pNewNode;

	// Ԫ�صĸ�������
	m_nNodeSize++;

	return true;
}


/*=======================================================
*	bool CList<T>::SetVal(iterator pNode, T val) -- ��β���ǰ��ӽ��
*
*	������pNode -- �޸ĵĽ�� ��valval �޸ĺ������
*
*	����ֵ���޸ĳɹ����� true
*
*	���棺��
*
*	���£�2020/06/08
*
========================================================*/
template<typename T>
bool CList<T>::SetVal(iterator pNode, int val)
{
	// ��ֱ���޸ĸýڵ��ϵ����ݣ�����������ǰ������Ч��飬�������Ѿ�����
	pNode.m_pNode->m_val = val;
	return true;
}


/*=======================================================
*	bool CList<T>::RemoveHead() -- ɾ��ͷ���
*
*	��������
*
*	����ֵ������Remove������ɾ���ɹ�����true
*
*	���棺��
*
*	���£�2020/06/08
*
========================================================*/
template<typename T>
bool CList<T>::RemoveHead()
{
	return Remove(begin());
}


/*=======================================================
*	bool CList<T>::RemoveTail() -- ɾ��β���
*
*	��������
*
*	����ֵ������Remove������ɾ���ɹ�����true
*
*	���棺��
*
*	���£�2020/06/08
*
========================================================*/
template<typename T>
bool CList<T>::RemoveTail()
{
	return Remove(--end());	//end()Ϊβ�ڱ����
}


/*=======================================================
*	bool CList<T>::Remove(iterator pNode) -- ɾ��ָ�����
*
*	������iterator pNode -- Ҫɾ���Ľ��
*
*	����ֵ��ɾ���ɹ�����true
*
*	���棺��
*
*	���£�2020/06/08
*
========================================================*/
// ɾ��ָ�����
template<typename T>
bool CList<T>::Remove(iterator pNode)
{
	// �޸�ǰ���ͺ�̽��
	PNODE pOldPre = pNode.m_pNode->m_pPre;
	PNODE pOldNext = pNode.m_pNode->m_pNext;

	pOldPre->m_pNext = pOldNext;	// �ý��ǰ�����ĺ�̽�� ָ�� �ý��ĺ�̽��
	pOldNext->m_pPre = pOldPre;	// �ý���̽���ǰ����� ָ�� �ý���ǰ�����

	delete pNode.m_pNode;	// ɾ���ý��
	m_nNodeSize--;	// ��������1

	return true;
}


/*=======================================================
*	typename CList<T>::iterator CList<T>::Find(T val) -- �ڸ������в���ָ��������
*
*	������val -- ���ҵ�����
*
*	����ֵ�����ҳɹ������ض�Ӧ�Ľ�㣬����ʧ�� ����β�ڱ�
*
*	���棺��
*
*	���£�2020/06/08
*
========================================================*/
template<typename T>
typename CList<T>::iterator CList<T>::Find(int val)
{
	// ����һ��ָ��ͷ�ڵ����ʱ���
	PNODE pNewTemp = m_pHeadGuard->m_pNext;
	// ��������������ͷ�ڵ㵽β���
	while (pNewTemp != m_pTailGuard)
	{
		if (pNewTemp->m_val == val)
		{
			return iterator(pNewTemp, m_pHeadGuard, m_pTailGuard);	// �ҵ����ظý��
		}
		// ƥ��ʧ�ܣ�ָ����һ�����
		pNewTemp = pNewTemp->m_pNext;
	}

	return iterator(m_pTailGuard, m_pHeadGuard, m_pTailGuard);
}


/*=======================================================
*	bool CList<T>::IsNull() -- �ж������Ƿ�Ϊ��
*
*	��������
*
*	����ֵ�������������Ч�Ľڵ�����Ϊ0 ����false������true
*
*	���棺��
*
*	���£�2020/06/08
*
========================================================*/
template<typename T>
bool CList<T>::IsNull()
{
	return m_nNodeSize == 0;
}


/*=======================================================
*	int CList<T>::NodeSize() -- ����ڵ����
*
*	��������
*
*	����ֵ��������������Ч�Ľڵ�����
*
*	���棺��
*
*	���£�2020/06/08
*
========================================================*/
template<typename T>
int CList<T>::GetNodeSize()const
{
	return m_nNodeSize;
}


/*=======================================================
*	void CList<T>::Clear()-- �������(�ڱ������ʱ����Ҫ�����ͷ�����)
*
*	��������
*
*	����ֵ��������������Ч�Ľڵ�����
*
*	���棺��
*
*	���£�2020/06/08
*
========================================================*/
// ��������ʱ��Ӧ���ڱ�Ҳ��������
template<typename T>
void CList<T>::Clear()
{
	while (!IsNull())
	{
		RemoveHead();	// ����������������Ҫ���� ��1����
	}
}


/*=======================================================
*	void CList<T>::InitList() -- ��ʼ������ 
*
*	��������
*
*	����ֵ����
*
*	���棺��
*
*	���£�2020/06/08
*
========================================================*/
template<typename T>
void CList<T>::InitList()
{
	m_pHeadGuard = new NODE();
	m_pTailGuard = new NODE();

	// ������ռλ���ڱ�����ָ��
	m_pHeadGuard->m_pNext = m_pTailGuard;
	m_pTailGuard->m_pPre = m_pHeadGuard;
	m_nNodeSize = 0;
}