#pragma once
#include <cassert>

/*=======================================
*			o(*￣▽￣*)o
*****************************************************************
*			Project Name：双向链表
*
*			FileName：MyLinkedList.h
*
*			Start Date：2020/06/02
*
*			Last UpDate：2020/06/16
*
=========================================*/
template<typename T>
class CList
{
private:
	// 双向链表的结点
	typedef struct tagNode
	{
		tagNode(int val) : m_val(val), m_pPre(nullptr), m_pNext(nullptr) {}
		tagNode() : m_pPre(nullptr), m_pNext(nullptr) {}
		int m_val;	// 保存的数据
		tagNode* m_pPre;	// 指向上一个结点的指针，前驱
		tagNode* m_pNext;	// 指向下一个结点的指针，后继
	}NODE, *PNODE;

public:
	// 迭代器
	class iterator
	{
	private:
		friend class CList;
	public:
		// 重载 前置++ 运算符
		iterator& operator++()
		{
			// 判断当前结点 是否是 尾部哨兵结点
			assert(m_pNode != m_pTailGuard);
			m_pNode = m_pNode->m_pNext;

			return *this;
		}

		// 重载 后置++ 运算符
		iterator operator++(int)
		{
			// 判断当前结点 是否是 尾部哨兵结点
			assert(m_pNode != m_pTailGuard);

			m_pNode = m_pNode->m_pNext;

			// 传递修改前的结点
			return iterator(m_pNode->m_pPre, m_pHeadGuard, m_pTailGuard);
		}

		// 重载 前置-- 运算符
		iterator& operator--()
		{
			// 判断当前节点 是否是 头部哨兵结点
			assert(m_pNode->m_pPre != m_pHeadGuard);

			m_pNode = m_pNode->m_pPre;
			return *this;
		}

		// 重载 前置-- 运算符
		iterator operator--(int)
		{
			assert(m_pNode->m_pPre != m_pHeadGuard);
			m_pNode = m_pNode->m_pPre;

			// 返回 -- 之前的结点
			return iterator(m_pNode->m_pNext, m_pHeadGuard, m_pTailGuard);
		}

		// 重载 * 运算符
		int& operator*()
		{
			return m_pNode->m_val;
		}

		// 重载 == 运算符
		bool operator==(const iterator& itr)
		{
			return m_pNode == itr.m_pNode;
		}

		// 重载 != 运算符
		bool operator!=(const iterator& itr)
		{
			return m_pNode != itr.m_pNode;
		}

	private:
		// 不让使用者传递 pNode 指针，将该构造设置为私有
		iterator(PNODE pNode, PNODE pHeadGuard, PNODE pTailGuard) :
			m_pNode(pNode),
			m_pHeadGuard(pHeadGuard),
			m_pTailGuard(pTailGuard)
		{
		}
	private:
		PNODE m_pNode;
		PNODE m_pHeadGuard;	// 头部哨兵
		PNODE m_pTailGuard;	// 尾部哨兵
	};

public:
	iterator begin();	// 正序遍历，从链表头到链表尾
	iterator end();
public:
	iterator rbegin();	// 逆序遍历，从链表尾到链表头
	iterator rend();
public:
	CList();
	/*
	编译器会自动生成拷贝构造以及重载运算符=
	默认生成为浅拷贝，在这里会有问题
	*/
	CList(const CList& list);	// 拷贝构造
	CList& operator=(const CList& list);	// 重载运算符 =
	virtual ~CList();

public:
	// 增加
	bool AddHead(int val);
	bool AddTail(int val);
	bool Insert(iterator pNode, int val);
	// 传递了结点的指针，调用者可以对这个结点上的数据进行随意的修改，破坏了类的封装性

	// 修改
	bool SetVal(iterator pNode, int val);	// 传递了结点的指针，破坏了类的封装性

	// 删除
	bool RemoveHead();
	bool RemoveTail();
	bool Remove(iterator pNode);	// 传递了结点的指针，破坏了类的封装性

	// 查询
	iterator Find(int val);	// 返回值为一个结点的指针 

	// 判断该链表是否是一个空链表
	bool IsNull();

	// 结点的个数
	int GetNodeSize()const;

	// 清空链表
	void Clear();

private:
	void InitList();
private:
	PNODE m_pHeadGuard;	// 保存链表的头部哨兵
	PNODE m_pTailGuard;	// 保存链表的尾部哨兵
	int m_nNodeSize;	// 链表中结点的个数
};

/*=======================================================
*	typename CList<T>::iterator CList<T>::begin() -- 迭代器，从头部开始遍历链表中的数据
*
*	参数：已初始化的类对象
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/08
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
*	typename CList<T>::iterator CList<T>::end() -- 迭代器，链表最后一个结点的后继
*
*	参数：已初始化的类对象
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/08
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
*	-- 迭代器，从链表最后一个结点开始反向遍历
*	参数：已初始化的类对象
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/08
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
*	typename CList<T>::iterator CList<T>::rend() -- 迭代器，链表头结点的前驱结点为位置
*
*	参数：已初始化的类对象
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/08
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
*	CList<T>::CList() -- 无参构造，初始化链表
*
*	参数：已初始化的类对象
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/08
*
========================================================*/
template<typename T>
CList<T>::CList()
{
	InitList();
}

/*=======================================================
*	CList<T>::CList(const CList& list) --拷贝构造，
*
*	参数：const CList& list -- 已初始化的类对象
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/08
*
========================================================*/
template<typename T>
CList<T>::CList(const CList& list)
{
	// 需要将哨兵和对应的结点进行连接
	InitList();

	// 调重载运算符 =
	*this = list;
}


/*=======================================================
*	CList<T>& CList<T>::operator=(const CList& list) -- 重载运算符 =
*
*	参数：const CList& list -- 已初始化的类对象
*
*	返回值：如果自己给自己赋值，返回 *this，赋值成功返回 *this
*
*	警告：无
*
*	更新：2020/06/08
*
========================================================*/
template<typename T>
CList<T>& CList<T>::operator=(const CList& list)
{
	// 判断是不是自己给自己赋值
	if (this == &list)
	{
		return *this;
	}

	// 清除掉自己
	Clear();

	// 循环拷贝数据
	PNODE pNode = list.m_pHeadGuard->m_pNext;
	while (pNode != list.m_pTailGuard)
	{
		AddTail(pNode->m_val);	// 将对方的数据添加到自己的链表中

		pNode = pNode->m_pNext;	// 指向下一个结点
	}
	return *this;
}


/*=======================================================
*	CList<T>::~CList() -- 析构 
*	清掉链表中的所有有效结点，此时类对象不在使用，销毁哨兵结点
*	参数：无
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/08
*
========================================================*/
template<typename T>
CList<T>::~CList()
{
	// 清掉链表中的所有有效结点
	Clear();

	// 此时类对象不在使用，销毁哨兵结点
	delete m_pHeadGuard;
	delete m_pTailGuard;
	m_pHeadGuard = nullptr;
	m_pTailGuard = nullptr;
	m_nNodeSize = 0;
}



/*=======================================================
*	bool CList<T>::AddHead(T val) -- 往头结点前添加结点
*
*	参数：val 添加的数据
*
*	返回值：调用 Insert 方法，插入成功返回true
*
*	警告：无
*
*	更新：2020/06/08
*
========================================================*/
template<typename T>
bool CList<T>::AddHead(int val)
{
	// 新的结点插入到头部哨兵的后面
	return Insert(begin(), val);
}

/*=======================================================
*	bool CList<T>::AddHead(T val) -- 往尾结点前添加结点
*
*	参数：val 添加的数据
*
*	返回值：调用 Insert 方法，插入成功返回true
*
*	警告：无
*
*	更新：2020/06/08
*
========================================================*/
template<typename T>
bool CList<T>::AddTail(int val)
{
	// 插入的结点为尾结点，插入到尾部哨兵的前面就可以
	return Insert(end(), val);
}


/*=======================================================
*	bool CList<T>::Insert(iterator itr, T val) -- 往尾结点前添加结点
*
*	参数：pNode -- 要插入的结点位置（插入的位置是该节点的前面）valval 添加的数据
*
*	返回值：插入成功返回 true
*
*	警告：无
*
*	更新：2020/06/08
*
========================================================*/
/*
PNODE pNode：要插入的结点位置（插入的位置是该节点的前面）
*/
template<typename T>
bool CList<T>::Insert(iterator pNode, int val)
{
	// 这里需要判断头结点和尾结点是否为空，需要换个思路
	// 给定两个结点(哨兵结点)，一个放在头节点的前面，一个放在尾结点的后面
	// 这两个结点不会存储数据，插入的数据都存储到这两个结点之间，不用在进行遍历

	// 创建一个新的结点
	PNODE pNewNode = new NODE(val);

	// 插入新的结点
	PNODE pOldPre = pNode.m_pNode->m_pPre;	// 保存插入结点的前一个结点

	// 新结点的下一个结点指向pNode结点
	pNewNode->m_pNext = pNode.m_pNode;
	// 新结点的前一个结点指向pNode结点的前一个结点
	pNewNode->m_pPre = pNode.m_pNode->m_pPre;	

	// 将插入的结点 和 原先的两个结点进行关联
	//pNode->m_pPre->m_pNext = pNewNode;
	//pNode->m_pPre = pNewNode;

	pNode.m_pNode->m_pPre = pNewNode;
	pOldPre->m_pNext = pNewNode;

	// 元素的个数增加
	m_nNodeSize++;

	return true;
}


/*=======================================================
*	bool CList<T>::SetVal(iterator pNode, T val) -- 往尾结点前添加结点
*
*	参数：pNode -- 修改的结点 ，valval 修改后的数据
*
*	返回值：修改成功返回 true
*
*	警告：无
*
*	更新：2020/06/08
*
========================================================*/
template<typename T>
bool CList<T>::SetVal(iterator pNode, int val)
{
	// 可直接修改该节点上的数据，不用在做当前结点的有效检查，迭代器已经做了
	pNode.m_pNode->m_val = val;
	return true;
}


/*=======================================================
*	bool CList<T>::RemoveHead() -- 删除头结点
*
*	参数：无
*
*	返回值：调用Remove方法，删除成功返回true
*
*	警告：无
*
*	更新：2020/06/08
*
========================================================*/
template<typename T>
bool CList<T>::RemoveHead()
{
	return Remove(begin());
}


/*=======================================================
*	bool CList<T>::RemoveTail() -- 删除尾结点
*
*	参数：无
*
*	返回值：调用Remove方法，删除成功返回true
*
*	警告：无
*
*	更新：2020/06/08
*
========================================================*/
template<typename T>
bool CList<T>::RemoveTail()
{
	return Remove(--end());	//end()为尾哨兵结点
}


/*=======================================================
*	bool CList<T>::Remove(iterator pNode) -- 删除指定结点
*
*	参数：iterator pNode -- 要删除的结点
*
*	返回值：删除成功返回true
*
*	警告：无
*
*	更新：2020/06/08
*
========================================================*/
// 删除指定结点
template<typename T>
bool CList<T>::Remove(iterator pNode)
{
	// 修改前驱和后继结点
	PNODE pOldPre = pNode.m_pNode->m_pPre;
	PNODE pOldNext = pNode.m_pNode->m_pNext;

	pOldPre->m_pNext = pOldNext;	// 该结点前驱结点的后继结点 指向 该结点的后继结点
	pOldNext->m_pPre = pOldPre;	// 该结点后继结点的前驱结点 指向 该结点的前驱结点

	delete pNode.m_pNode;	// 删除该结点
	m_nNodeSize--;	// 结点个数减1

	return true;
}


/*=======================================================
*	typename CList<T>::iterator CList<T>::Find(T val) -- 在该链表中查找指定的数据
*
*	参数：val -- 查找的数据
*
*	返回值：查找成功，返回对应的结点，查找失败 返回尾哨兵
*
*	警告：无
*
*	更新：2020/06/08
*
========================================================*/
template<typename T>
typename CList<T>::iterator CList<T>::Find(int val)
{
	// 定义一个指向头节点的临时结点
	PNODE pNewTemp = m_pHeadGuard->m_pNext;
	// 遍历整个链表，从头节点到尾结点
	while (pNewTemp != m_pTailGuard)
	{
		if (pNewTemp->m_val == val)
		{
			return iterator(pNewTemp, m_pHeadGuard, m_pTailGuard);	// 找到返回该结点
		}
		// 匹配失败，指向下一个结点
		pNewTemp = pNewTemp->m_pNext;
	}

	return iterator(m_pTailGuard, m_pHeadGuard, m_pTailGuard);
}


/*=======================================================
*	bool CList<T>::IsNull() -- 判断链表是否为空
*
*	参数：无
*
*	返回值：如果链表中有效的节点数量为0 返回false，返回true
*
*	警告：无
*
*	更新：2020/06/08
*
========================================================*/
template<typename T>
bool CList<T>::IsNull()
{
	return m_nNodeSize == 0;
}


/*=======================================================
*	int CList<T>::NodeSize() -- 链表节点个数
*
*	参数：无
*
*	返回值：返回链表中有效的节点数量
*
*	警告：无
*
*	更新：2020/06/08
*
========================================================*/
template<typename T>
int CList<T>::GetNodeSize()const
{
	return m_nNodeSize;
}


/*=======================================================
*	void CList<T>::Clear()-- 清空链表(哨兵结点暂时不需要进行释放销毁)
*
*	参数：无
*
*	返回值：返回链表中有效的节点数量
*
*	警告：无
*
*	更新：2020/06/08
*
========================================================*/
// 对象销毁时，应将哨兵也进行销毁
template<typename T>
void CList<T>::Clear()
{
	while (!IsNull())
	{
		RemoveHead();	// 这里结点数量不再需要进行 减1操作
	}
}


/*=======================================================
*	void CList<T>::InitList() -- 初始化链表 
*
*	参数：无
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/08
*
========================================================*/
template<typename T>
void CList<T>::InitList()
{
	m_pHeadGuard = new NODE();
	m_pTailGuard = new NODE();

	// 将两个占位的哨兵互相指向
	m_pHeadGuard->m_pNext = m_pTailGuard;
	m_pTailGuard->m_pPre = m_pHeadGuard;
	m_nNodeSize = 0;
}