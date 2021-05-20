#pragma once
/*===================================================
*			o(*￣▽￣*)o
************************************************************************************
*			Project Name：学生信息管理 -- 平衡二叉树
*
*			FileName：MyAVLTree.h
*
*			Start Date：2020/06/11
*
*			Last UpDate：2020/06/15
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
	// 树结点数据
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
		int	m_nYear;	// 出生年份
		int	m_nMonth;	// 出生月份
		int	m_nSex;	// 性别 1--男 2--女 3--其它 4--未知
		CString m_szStuName;	// 学生姓名
		int m_nNodeHeight;	// 树中结点的高度
		tagStuNode* m_pParent;	// 指向父结点
		tagStuNode* m_pLeft;	// 当前结点的左孩子
		tagStuNode* m_pRight;	// 当前结点的右孩子
	}STUNODE, *PSTUNODE;

public:
	CAVLTree();
	CAVLTree(const  CAVLTree& avl);

	CAVLTree& operator=(const CAVLTree& avl);
	virtual ~CAVLTree();

public:
	// 基本操作
	bool Insert(T nStuID, CString szStuName, int nBirthYear, int nBirthMonth, int nSex);	// 插入数据
	bool Remove(T nStuID);	// 通过ID删除对应结点数据
	bool Updata(T nStuIDSrc, T nStuIDDst);	// 将原来结点上的数值更改新的数值
	bool Change(T nStuID);	// 通过 ID 修改学生数据
	bool Find(T nStuID);	// 通过ID查找对应的学生数据
	bool FindStuName(CString szStuName);	// 从树根结点，将树中的结点依次入栈
	bool FindStuName1(T nStuID, CString szStuName);	// 通过学生姓名查找内存中对应的学生信息
	void Clear();	// 清空树的各个结点

	// 使用递归遍历数据（前序、中序、后续）
	void MLR();	// 前序遍历（递归）middle left right
	void LMR();	// 中序遍历（递归） left middle right
	void LRM();	// 后序遍历（递归） left right middle

	int GetNodeSize()const;	// 获取树中结点个数
	void InFile(PSTUNODE pNode);	// 写入文件
private:
	void Destroy(PSTUNODE pNode);	// 清理结点
	PSTUNODE FindNode(T nStuID);	// 查找结点
	PSTUNODE CopyNewData(PSTUNODE pNode);	// 拷贝构造时，拷贝各结点数据
	void DelNode(PSTUNODE pNode);	// 根据待删除结点的所处位置进行对应的删除操作

	void DelLeaf(PSTUNODE pNode);	// 叶子结点
	void DelSingleChildNode(PSTUNODE pNode);	// 单分支结点
	void DelDoubleChildNode(PSTUNODE pNode);	// 双分支结点

	void MLR(PSTUNODE pNode);	// 递归 前序 遍历
	void LMR(PSTUNODE pNode);	// 递归 中序 遍历
	void LRM(PSTUNODE pNode);	// 递归 后序 遍历

public:
	static void ReadAVLTreeStuDataToInFile(CAVLTree<T> &avl);	// 从文件读取数据
	void WirteAVLTreeStuDataToInFile0();	// 调用OutFile1递归将树中的数据写入到文件中
	void WirteAVLTreeStuDataToInFile1(PSTUNODE pNode);		// 递归将树中的数据写入文件(速度慢)
	void WirteAVLTreeStuDataToInFile();	// 将树中的数据写入文件

	void RemoveCopyStuData();	// 修改数据文件名

private:
	void AdjustHeight(PSTUNODE pNode);	// 根据结点左右孩子高度差，判断树是否平衡，不平衡做出相应的处理
	int CalcNodeHeight(PSTUNODE pNode);	// 计算结点高度
	int GetNodeHeight(PSTUNODE pNode)const;	// 获取结点高度
	void RotateLeft(PSTUNODE pNode);	// 左单旋
	void RotateRight(PSTUNODE pNode);	// 右单旋


private:
	PSTUNODE m_pRoot;	// 当前树的根结点
	static int m_pStuSize;	// 树中学生结点个数
};


/*=======================================================
*	int CAVLTree<T>::m_pStuSize = 0; -- 初始化学生数量为 0
*
*	参数：无
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/12
*
========================================================*/
template<typename T>
int CAVLTree<T>::m_pStuSize = 0;


/*=======================================================
*	CAVLTree<T>::CAVLTree() -- 无参构造，初始化树
*
*	参数：无
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/12
*
========================================================*/
template<typename T>
CAVLTree<T>::CAVLTree()
{
	m_pRoot = nullptr;
}

/*=======================================================
*	CAVLTree<T>::CAVLTree(const CAVLTree& bst) -- 拷贝构造
*
*	参数：已初始化的类对象
*
*	返回值：Nothing
*
*	警告：无
*
*	更新：2020/06/12
*
========================================================*/
template<typename T>
CAVLTree<T>::CAVLTree(const CAVLTree& avl)
{
	*this = avl;
}


/*=======================================================
*	CAVLTree<T>& CAVLTree<T>::operator=(const CAVLTree& bst) -- 重载运算符 =	
*
*	参数：已初始化的类对象
*
*	返回值：*this
*
*	警告：无
*
*	更新：2020/06/12
*
========================================================*/
template<typename T>
CAVLTree<T>& CAVLTree<T>::operator=(const CAVLTree& avl)
{
	if (this == &avl)
	{
		return *this;	// 如果自己给自己赋值，什么也不做
	}

	Clear();	// 清除自己的各个结点数据

	if (avl.m_pRoot == nullptr)
	{
		return *this;	// 如果赋值方为空树，不进行赋值
	}

	// 赋值
	m_pRoot = CopyNewData(avl.m_pRoot);	// 为当前树赋上新的数据

	return *this;
}


/*=======================================================
*	CAVLTree<T>::~CAVLTree() -- 程序结束，调用 Clear() 方法释放对应的数据
*
*	参数：无
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/12
*
========================================================*/
template<typename T>
CAVLTree<T>::~CAVLTree()
{
	Clear();
}


/*=======================================================
*	bool CAVLTree<T>::Insert(T nStuID, CString szStuName, int nBirthYear, 
*	int nBirthMonth, int nSex)	-- 添加的学生信息
*
*	参数：nStuID -- 插入的学生ID，szStuName -- 学生姓名，nBirthYear -- 学生出生年份
*	nBirthMonth -- 学生出生月份，nSex -- 学生性别
*	返回值：true -- 数据插入成功，false -- 数据插入失败
*
*	警告：无
*
*	更新：2020/06/12
*
========================================================*/
template<typename T>
bool CAVLTree<T>::Insert(T nStuID, CString szStuName, int nBirthYear, int nBirthMonth, int nSex)
{
	if (m_pRoot == nullptr)
	{
		// 如果树为空，插入的数据作为根结点，同时根节点的高度置为0
		m_pRoot = (PSTUNODE)new STUNODE(nStuID, szStuName, nBirthYear, nBirthMonth, nSex, 0, nullptr, nullptr, nullptr);
		m_pStuSize++;	// 学生个数加1
		//InFile(m_pRoot);
		return true;
	}

	// 如果当前树不为空，就将当前树的根结点数据赋值给临时树 pTempNode
	PSTUNODE pTempNode = m_pRoot;

	// 遍历查找可以插入的位置，遍历从树的根部开始
	while (true)
	{
		// 如果插入的数据数值小于当前结点，则取其左孩子部分查找合适的位置
		if (nStuID < pTempNode->m_nStuID)
		{
			// 如果此结点没有左孩子，则新插入的数值作为当前结点的左孩子
			if (pTempNode->m_pLeft == nullptr)
			{
				// 新插入的结点的高度置0，此时其根结点的高度加1
				pTempNode->m_pLeft = new STUNODE(nStuID, szStuName, nBirthYear, nBirthMonth, nSex, 0, pTempNode, nullptr, nullptr);

				// 每次插入新的结点，都要判断树中现存结点是否平衡，如果不平衡，进行相应的调整
				AdjustHeight(pTempNode);
				m_pStuSize++;
				//InFile(pTempNode->m_pLeft);
				return true;
			}
			else
			{
				// 此结点有左孩子，就然该结点指向该结点的左孩子，继续往下寻找空位置
				pTempNode = pTempNode->m_pLeft;
			}
		}
		// 如果插入的数据数值大于当前结点，则取其右孩子判断
		else if (nStuID > pTempNode->m_nStuID)
		{
			if (pTempNode->m_pRight == nullptr)
			{
				pTempNode->m_pRight = new STUNODE(nStuID, szStuName, nBirthYear, nBirthMonth, nSex, 0, pTempNode, nullptr, nullptr);

				// 每次插入新的结点，都要判断树中现存结点是否平衡，如果不平衡，进行相应的调整
				AdjustHeight(pTempNode);
				m_pStuSize++;
				//InFile(pTempNode->m_pRight);
				return true;
			}
			else
			{
				// 此结点有右孩子，就然该结点指向该结点的右孩子，继续往下寻找空位置
				pTempNode = pTempNode->m_pRight;
			}
		}
		else
		{
			cout << "学号已存在" << endl;
			return false;	// 学生ID重复返回false
		}
	}
	return true;
}


/*=======================================================
*	bool CAVLTree<T>::Remove(T nStuID) -- 删除指定的学生信息
*
*	参数：nStuID -- 删除的学生ID
*
*	返回值：true -- 输入删除成功，false -- 数据删除失败	
*
*	警告：无
*
*	更新：2020/06/13
*
========================================================*/
template<typename T>
bool CAVLTree<T>::Remove(T nStuID)
{
	PSTUNODE pNodeTodel = FindNode(nStuID);	// 查找将要被删除的学生ID
	if (pNodeTodel == nullptr)
	{
		cout << "文件内没有找到你要删除学生数据对应的ID" << endl;
		return false;	// pNodeTodel 为空 查找失败 返回 false
	}

	DelNode(pNodeTodel);	// 查找成功，根据结点的情况进行下一步操作
	cout << "学生数据删除成功" << endl;
	return true;
}


/*=======================================================
*	bool CAVLTree<T>::Updata(T nStuIDSrc, T nStuIDDst) -- 修改数据
*
*	参数：nStuIDSrc -- 被修改的数据值，nStuIDDst -- 修改后的数据值
*
*	返回值：false -- 数据查找/修改失败，true -- 数据修改成功
*
*	警告：无
*
*	更新：2020/06/12
*
========================================================*/
// 将树中存在的数值 nStuIDSrc 修改为 nStuIDDst
template<typename T>
bool CAVLTree<T>::Updata(T nStuIDSrc, T nStuIDDst)
{
	if (m_pRoot == nullptr)
	{
		return false;	// 如果当前树为空，返回false
	}

	PSTUNODE pChangeNode = FindNode(nStuIDSrc);	// 查找是否存在 nStuIDSrc
	if (pChangeNode == nullptr)
	{
		return false;	// pNodeTodel 为空 查找失败 返回 false
	}

	//pChangeNode->m_nStuID = nStuIDDst;	// 直接修改，不可取，会导致该树不是一颗排序树

	Remove(nStuIDSrc);	// 数值查找成功，删除结点
	Insert(nStuIDDst);	// 将新的数值从新插入到该树中

	return true;
}


/*=======================================================
*	bool CAVLTree<T>::Change(T nStuID) -- 通过学生ID，修改其相应的数据
*
*	参数：nStuIDSrc -- 被修改的数据值，nStuIDDst -- 修改后的数据值
*
*	返回值：false -- 数据查找/修改失败，true -- 数据修改成功
*
*	警告：无
*
*	更新：2020/06/13
*
========================================================*/
template<typename T>
bool CAVLTree<T>::Change(T nStuID)
{
	if (m_pRoot == nullptr)
	{
		return false;	// 树的根结点为空，返回false，不进行修改操作
	}

	// 查询学生ID是否存在，不存在输出对应的提示信息
	PSTUNODE pNode = FindNode(nStuID);
	if (pNode == nullptr)
	{
		cout << "查询的学生ID不存在" << endl;
		return false;
	}

	// 查询对应的学生ID存在，输出对应的信息
	cout << "学生信息查找成功，信息如下：" << endl;
	cout << "ID：" << pNode->m_nStuID << " "
		<< "姓名：" << pNode->m_szStuName << " "
		<< "出生年份：" << pNode->m_nYear << " "
		<< "出生月份" << pNode->m_nMonth << " "
		<< "性别(1/男，2/女，3/其它，4/未知)：" << pNode->m_nSex << endl;

	CString szChangeName = nullptr;	// 存储学生姓名
	int nBirthYear = 0;	// 存储学生出生年份
	int nBirthMonth = 0;	// 存储学生出生月份
	int nSex = 0;	// 存储学生性别

	// 输入修改后的数据，并将对应的数据赋值给原来结点上对应的数据
	cout << "请输入你要修改后的学生姓名：";
	cin >> szChangeName;
	pNode->m_szStuName = szChangeName;
	cout << "请输入你要修改后的学生出生年份：";
	cin >> nBirthYear;
	pNode->m_nYear = nBirthYear;
	cout << "请输入你要修改后的学生出生月份：";
	cin >> nBirthMonth;
	pNode->m_nMonth = nBirthMonth;
	cout << "请输入你要修改后的学生性别(1/男，2/女，3/其它，4/未知)：";
	cin >> nSex;
	pNode->m_nSex = nSex;

	cout << "对应的学生数据修改成功" << endl;
	return true;
}


/*=======================================================
*	bool CAVLTree<T>::Find(T nStuID) -- 查找指定的学生ID
*
*	参数：nStuID -- 要查找的学生ID
*
*	返回值：true -- 数据查找成功，false -- 数据查找失败
*
*	警告：无
*
*	更新：2020/06/13
*
========================================================*/
template<typename T>
bool CAVLTree<T>::Find(T nStuID)
{
	if (m_pRoot == nullptr)
	{
		return false;	// 如果当前树为空，返回false，不进行查询操作
	}

	PSTUNODE pChangeNode = FindNode(nStuID);	// 查找树中是否存在学生ID nStuID
	if (pChangeNode == nullptr)
	{
		cout << "通过ID没有查找到你要查找的学生信息" << endl;
		return false;	// pChangeNode 为空 查找失败 返回 false
	}

	// 找寻到后，输出该ID对应的学生信息
	cout << "学生信息查找成功，信息如下：" << endl;
	cout << "ID：" << pChangeNode->m_nStuID << " "
		<< "姓名：" << pChangeNode->m_szStuName << " "
		<< "出生年份：" << pChangeNode->m_nYear << " "
		<< "出生月份" << pChangeNode->m_nMonth << " "
		<< "性别(1/男，2/女，3/其它，4/未知)：" << pChangeNode->m_nSex << endl;

	return true;
}


/*=======================================================
*	bool CAVLTree<T>::FindStuName(CString szStuName)
*	-- 通过学生姓名查询对应的学生数据
*	参数：CString szStuName -- 要查询的学生姓名
*
*	返回值：true -- 数据查找成功，false -- 结点为空，返回false
*
*	警告：无
*
*	更新：2020/06/16
*
========================================================*/
template<typename T>
bool CAVLTree<T>::FindStuName(CString szStuName)
{
	if (m_pRoot == nullptr)
	{
		return false;	// 如果当前树为空，返回false
	}
	
	PSTUNODE pTempNode = m_pRoot;	// 从该树的根结点开始查找

	CStack<PSTUNODE> stk;	// 初始化一个栈
	PSTUNODE pNode = m_pRoot;	// 将根节点赋值给 pNode

	while (true)
	{
		while (pNode != nullptr)
		{
			stk.push(pNode->m_nStuID);	// 将根结点的学生ID入栈
			pNode = pNode->m_pLeft;	// pNode 指向当前结点的左孩子
		}

		if (stk.empty())
		{
			break;	// 栈空循环结束
		}

		int nTempNum = stk.top();	// 获取栈顶数据
		pNode = FindNode(nTempNum);	// 将栈顶数据对应的结点赋值给 pNode
		if (pNode == nullptr)
		{
			return false;
		}
		stk.pop();	// 从栈顶弹出一个数据
		FindStuName1(nTempNum, szStuName);
		
		pNode = pNode->m_pRight;	// 处理弹出结点数据的右孩子
	}
	return true;
}


/*=======================================================
*	bbool CAVLTree<T>::FindStuName1(T nStuID, CString szStuName)
*	-- 根据传入的参数，查找树中存在的所有相同姓名的学生信息
*	参数：T nStuID -- 树结点对应的学生ID，CString szStuName -- 要查询的联系人姓名
*
*	返回值：true -- 数据查找成功，false -- 结点为空，返回false
*
*	警告：无
*
*	更新：2020/06/16
*
========================================================*/
template<typename T>
bool CAVLTree<T>::FindStuName1(T nStuID, CString szStuName)
{
	if (m_pRoot == nullptr)
	{
		return false;	// 如果当前树为空，返回false
	}

	PSTUNODE pChangeNode = FindNode(nStuID);	// 查找树中是否存在数值 nStuID
	if (pChangeNode == nullptr)
	{
		cout << "数据查找失败" << endl;
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
*	void CAVLTree<T>::Clear() -- 清空树中所有有效的结点	
*
*	参数：无
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::Clear()
{
	while (m_pRoot != nullptr)
	{
		DelNode(m_pRoot);	// 树中结点删除完毕后，循环终止
	}
}

/*=======================================================
*	void CAVLTree<T>::MLR() -- 递归 前序 遍历（树不为空，调用MLR()方法）
*
*	参数：无
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::MLR()
{
	if (m_pRoot != nullptr)
	{
		MLR(m_pRoot);	// 调用 前序 遍历
	}
}

/*=======================================================
*	void CAVLTree<T>::LMR() -- 递归 中序 遍历（树不为空，调用LMR()方法）
*
*	参数：无
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::LMR()
{
	if (m_pRoot != nullptr)
	{
		LMR(m_pRoot);	// 调用 中序 遍历
	}
}

/*=======================================================
*	void CAVLTree<T>::LRM() -- 递归 后序 遍历	（树不为空，调用LRM()方法）
*
*	参数：无
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::LRM()
{
	if (m_pRoot != nullptr)
	{
		LRM(m_pRoot);	// 调用 后序 遍历
	}
}


/*=======================================================
*	int CAVLTree<T>::GetNodeSize() const -- 获取当前树中有效的节点数量
*
*	参数：无
*
*	返回值：返回当前树中有效的节点数量
*
*	警告：无
*
*	更新：2020/06/12
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
		cout << "文件打开失败，请检查文件是否存在" << endl;
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
*	void CAVLTree<T>::Destroy(PSTUNODE pNode) -- 使用递归清除树中有效的所有结点
*
*	参数：PSTUNODE pNode -- 树的根结点
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::Destroy(PSTUNODE pNode)
{
	if (pNode != nullptr)
	{
		Destroy(pNode->m_pLeft);	// 销毁根的左孩子
		Destroy(pNode->m_pRight);	// 销毁根的右孩子

		delete pNode;	// 销毁根
		pNode = nullptr;
	}
}


/*=======================================================
*	PSTUNODE CAVLTree<T>::FindNode(T nStuID)		
*	-- 根据输入的数据值查找树中是否存在对应的数据
*	参数：nStuID -- 查找的数据值	
*
*	返回值：pTempNode 返回查到的存储该数据的结点
*
*	警告：无
*
*	更新：2020/06/12
*
========================================================*/
template<typename T>
typename CAVLTree<T>::PSTUNODE CAVLTree<T>::FindNode(T nStuID)
{
	// 从该树的根结点开始查找
	PSTUNODE pTempNode = m_pRoot;

	while (pTempNode != nullptr)
	{
		// 如果查找的数据和当前结点的数值相等，就返回该结点
		if (nStuID == pTempNode->m_nStuID)
		{
			return pTempNode;
		}
		// 如果查找的数据大于当前结点的数值，就去该结点的右部分进行查找
		else if (nStuID > pTempNode->m_nStuID)
		{
			pTempNode = pTempNode->m_pRight;
		}
		else
		{
			// 如果查找的数据小于当前结点的数值，就去该结点的左部分进行查找
			pTempNode = pTempNode->m_pLeft;
		}
	}
	return nullptr;
}


/*=======================================================
*	Ptypename CAVLTree<T>::PSTUNODE CAVLTree<T>::CopyNewData(PSTUNODE pNode)
*	-- 拷贝构造时，拷贝各结点数据
*	参数：PSTUNODE pNode -- 拷贝的结点
*
*	返回值：pTempNode 返回拷贝后的结点数据，nullptr 结点为空，不进行拷贝
*
*	警告：无
*
*	更新：2020/06/15
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
*	-- 删除结点，根据结点类型进行对应的删除
*	参数：PSTUNODE pNode -- 要删除的数据所对应的结点
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::DelNode(PSTUNODE pNode)
{
	// 对要删除的结点进行结点位置判断

	// 叶子结点
	if (pNode->m_pLeft == nullptr && pNode->m_pRight == nullptr)
	{
		DelLeaf(pNode);
		return;
	}
	// 单分支结点
	else if (pNode->m_pLeft == nullptr || pNode->m_pRight == nullptr)
	{
		DelSingleChildNode(pNode);
		return;
	}
	// 双分支结点
	else
	{
		DelDoubleChildNode(pNode);
	}
}


/*=======================================================
*	void CAVLTree<T>::DelLeaf(PSTUNODE pNode)
*	-- 删除的结点为叶子结点（该结点没有左右孩子）
*	参数：PSTUNODE pNode -- 要删除的数据所对应的结点
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::DelLeaf(PSTUNODE pNode)
{
	PSTUNODE pParent = pNode->m_pParent;	// 保存当前结点的父结点

	if (pNode == m_pRoot)	// 如果当前结点是根结点，就删除该结点
	{
		delete m_pRoot;
		m_pRoot = nullptr;
		m_pStuSize--;
		return;
	}

	// 判断要删除的结点是父结点下的 左孩子 还是 右孩子
	if (pParent->m_pLeft == pNode)
	{
		pParent->m_pLeft = nullptr;	// 如果是左孩子，将其左孩子的结点置空
	}
	else
	{
		pParent->m_pRight = nullptr;	// 如果是右孩子，将其右孩子的结点置空
	}

	delete pNode;	// 删除此结点
	m_pStuSize--;
	pNode = nullptr;

	// 结点删除后，从删除结点的父结点开始到根结点进行调整高度
	AdjustHeight(pParent);
}


/*=======================================================
*	void CAVLTree<T>::DelSingleChildNode(PSTUNODE pNode)
*	-- 删除的结点为单分支结点（该结点有一个孩子 左孩子/右孩子）
*	参数：PSTUNODE pNode -- 要删除的数据所对应的结点
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::DelSingleChildNode(PSTUNODE pNode)
{
	PSTUNODE pParent = pNode->m_pParent;	// 保存当前结点的父结点

	PSTUNODE pChild = pNode->m_pLeft;	// 默认保存当前结点的左孩子

	if (pChild == nullptr)
	{
		pChild = pNode->m_pRight;	// 如果左孩子为空，说明该单分支结点只有右孩子，保存有孩子
	}

	// 判断此结点是否是根结点
	if (pNode == m_pRoot)
	{
		m_pRoot = pChild;	// 将该结点的左孩子或者有孩子置为根结点
		pChild->m_pParent = nullptr;	// 置空
		delete pNode;	// 删除该前结点
		m_pStuSize--;
		pNode = nullptr;
		return;
	}

	// 删除的是左孩子
	if (pParent->m_pLeft == pNode)
	{
		pParent->m_pLeft = pChild;
	}
	// 删除的是右孩子
	else
	{
		pParent->m_pRight = pChild;
	}

	// 删除结点的孩子结点指向删除结点的父亲结点
	pChild->m_pParent = pParent;

	// 删出该结点
	delete pNode;
	m_pStuSize--;
	pNode = nullptr;

	// 结点删除后，从删除结点的父结点开始到根结点进行调整高度
	AdjustHeight(pParent);
}


/*=======================================================
*	void CAVLTree<T>::DelSingleChildNode(PSTUNODE pNode)
*	-- 删除的结点为双分支结点（该结点有两个孩子 左孩子和右孩子）
*	参数：PSTUNODE pNode -- 要删除数据对应的结点
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::DelDoubleChildNode(PSTUNODE pNode)
{
	// 查找要删除结点的左字数的最大值和要删除结点的数据值进行交换
	PSTUNODE pTempNode = pNode->m_pLeft;

	while (pTempNode->m_pRight != nullptr)
	{
		// 找到左子树中最右结点，该结点保存的数据为左子树中的值最大的数据
		pTempNode = pTempNode->m_pRight;
	}

	// 找到左子树中的最大值，交换数值
	T nTempNum = pNode->m_nStuID;
	pNode->m_nStuID = pTempNode->m_nStuID;
	pTempNode->m_nStuID = nTempNum;

	// 删除 pTempNode，此时还需要判断交换后的结点处于那种情况
	DelNode(pTempNode);
}


/*=======================================================
*	void CAVLTree<T>::MLR(PSTUNODE pNode) -- 递归 前序 遍历
*
*	参数：PSTUNODE pNode -- 开始遍历的根结点
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::MLR(PSTUNODE pNode)
{
	// 递归的出口
	if (pNode == nullptr)
	{
		return;
	}
	cout << pNode->m_nStuID << " ";	// 先自己
	MLR(pNode->m_pLeft);	// 再左孩子
	MLR(pNode->m_pRight);	// 再右孩子
}


/*=======================================================
*	void CAVLTree<T>::LMR(PSTUNODE pNode) -- 递归 中序 遍历
*
*	参数：PSTUNODE pNode -- 开始遍历的根结点
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::LMR(PSTUNODE pNode)
{
	// 递归的出口
	if (pNode == nullptr)
	{
		return;
	}

	LMR(pNode->m_pLeft);	// 先左孩子
	cout << "ID：" << pNode->m_nStuID << " "
		<< "姓名：" << pNode->m_szStuName << " "
		<< "出生年份：" << pNode->m_nYear << " "
		<< "出生月份" << pNode->m_nMonth << " "
		<< "性别(1/男，2/女，3/其它，4/未知)：" << pNode->m_nSex << endl;
	LMR(pNode->m_pRight);	// 再右孩子
}


/*=======================================================
*	void CAVLTree<T>::LRM(PSTUNODE pNode) -- 递归 后序 遍历
*
*	参数：PSTUNODE pNode -- 开始遍历的根结点
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::LRM(PSTUNODE pNode)
{
	// 递归的出口
	if (pNode == nullptr)
	{
		return;
	}

	LRM(pNode->m_pLeft);	// 先左孩子
	LRM(pNode->m_pRight);	// 再右孩子
	cout << pNode->m_nStuID << " ";	// 再自己
}


/*=======================================================
*	void CAVLTree<T>::ReadAVLTreeStuDataToInFile(CAVLTree<T> &avl)
*	-- 从文件中读取数据到树中对应的个结点
*	参数：CAVLTree<T> &avl -- 传入的类对象
*
*	返回值：文件打开失败，输入错误信息，退出程序
*
*	警告：无
*
*	更新：2020/06/15
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
			cout << "文件打开失败，请检查文件是否存在" << endl;
			exit(-1);
		}
		char szStrTemp[1024];	// 存储读取到的一行临时数据
		while (InFile.getline(szStrTemp, 1024))
		{
			istringstream isStr(szStrTemp);	// 从文件中读取一行数据，并将其分贝赋值给对应的变量
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

		// 文件读取成功后，将原始文件做个备份，放置断电等意外操作
		CopyFile(TEXT("StudentData.bin"), TEXT("StudentCopyData.bin"), false);
		return;
	}

	// 删除源文件，并将备份的文件更改为原来的文件名，用于下次打开使用
	if (_unlink("StudentData.bin") == 0)
	{
		//cout << "源文件删除成功" << endl;
	}

	char szStrTemp[1024];	// 存储读取到的一行临时数据
	while (InFile.getline(szStrTemp, 1024))
	{
		istringstream isStr(szStrTemp);	// 从文件中读取一行数据，并将其分贝赋值给对应的变量
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

	// 文件读取成功后，将原始文件做个备份，放置断电等意外操作
	CopyFile(TEXT("StudentCopyData.bin"), TEXT("StudentData.bin"), false);

}


/*=======================================================
*	void CAVLTree<T>::WirteAVLTreeStuDataToInFile0()
*		-- 调用WirteAVLTreeStuDataToInFile1()方法 使用递归将树中对应的数据写入到文件
*	参数：无
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/16
*
========================================================*/
template<typename T>
void CAVLTree<T>::WirteAVLTreeStuDataToInFile0()
{
	if (m_pRoot != nullptr)
	{
		cout << "学生数据写入中...";
		if (unlink("StudentData.bin") == 0)
		{
			//cout << "源文件删除成功" << endl;
		}
		if (rename("Temp.bin", "StudentData.bin") == 0)
		{
			//cout << "备份数据文件更改名字成功" << endl;
		}
		WirteAVLTreeStuDataToInFile1(m_pRoot);
	}

	// 将更新后的数据写入到备份数据文件内
	CopyFile(TEXT("StudentData.bin"), TEXT("StudentCopyData.bin"), false);	

	// 调用RemoveFileName() 删除源文件，并将备份的文件更改为原来的文件名，用于下次打开使用
	//RemoveFileName();
}


/*=======================================================
*	void CAVLTree<T>::WirteAVLTreeStuDataToInFile1(PSTUNODE pNode)
*	-- 使用递归将树中对应的数据写入到文件
*	参数：无
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/16
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
*	-- 将树中的数据写入对应的数据文件
*	参数：无
*
*	返回值：文件打开失败，输入错误信息，退出程序
*
*	警告：无
*
*	更新：2020/06/16
*
========================================================*/
template<typename T>
void CAVLTree<T>::WirteAVLTreeStuDataToInFile()
{
	cout << "学生数据写入内存中..." << endl;
	ofstream WriteFile;
	WriteFile.open("StudentData.bin");

	if (!WriteFile.is_open())
	{
		cout << "文件打开失败，请检查文件是否存在" << endl;
		exit(-1);
	}
	CStack<PSTUNODE> stk;	// 初始化一个栈
	PSTUNODE pNode = m_pRoot;	// 将根节点赋值给 pNode

	while (true)
	{
		while (pNode != nullptr)
		{
			stk.push(pNode->m_nStuID);	// 将根结点对应的学生ID入栈
			pNode = pNode->m_pLeft;	// pNode 指向其左孩子
		}

		if (stk.empty())
		{
			break;	// 栈空循环结束
		}

		int nTempNum = stk.top();	// 获取栈顶数据
		pNode = FindNode(nTempNum);	// 根据栈顶数据将对应的结点数据赋值给 pNode
		WriteFile << pNode->m_nStuID << " "
			<< pNode->m_szStuName << " "
			<< pNode->m_nYear << " "
			<< pNode->m_nMonth << " "
			<< pNode->m_nSex << endl;
		stk.pop();	// 从栈顶弹出一个数据
		
		pNode = pNode->m_pRight;	// 处理弹出当前结点数据的右孩子
	}

	WriteFile.close();	// 关闭文件

	// 将更新后的数据写入到备份数据文件内
	CopyFile(TEXT("StudentData.bin"), TEXT("StudentCopyData.bin"), false);

	// 调用RemoveFileName() 删除源文件，并将备份的文件更改为原来的文件名，用于下次打开使用
	//RemoveFileName();
	cout << "---学生数据写入内存完成---" << endl;
}


/*=======================================================
*	void CAVLTree<T>::RemoveFileName()
*	-- 删除源文件，并将备份的文件更改为原来的文件名，用于下次打开使用
*	参数：无
*
*	返回值：文件打开失败，输入错误信息，退出程序
*
*	警告：无
*
*	更新：2020/06/15
*
========================================================*/
template<typename T>
void CAVLTree<T>::RemoveCopyStuData()
{
	// 删除源文件，并将备份的文件更改为原来的文件名，用于下次打开使用
	if (_unlink("StudentData.bin") == 0)
	{
		//cout << "源文件删除成功" << endl;
	}
	if (rename("StudentCopyData.bin", "StudentData.bin") == 0)
	{
		//cout << "备份数据文件更改名字成功" << endl;
	}
}


/*=======================================================
*	void CAVLTree<T>::AdjustHeight(PSTUNODE pNode)
*	-- 每次插入数据，判断当前树是否平衡同时调整数结点高度
*	参数：PSTUNODE pNode -- 插入的数据的根结点
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::AdjustHeight(PSTUNODE pNode)
{
	PSTUNODE pTempNode = pNode;	// 将插入的结点数据赋值给临时结点pTempNode

	// 此时需要沿着插入的结点往上判断其根结点（根节点的根节点）是否处于平衡状态
	while (pTempNode != nullptr)
	{
		// 获取当前结点的高度
		pTempNode->m_nNodeHeight = CalcNodeHeight(pTempNode);

		// 判断此结点是否平衡，判断依据，此结点的左孩子的高度减去右孩子的高度差的绝对值不能大于1
		int nTempNodeLeftHeight = GetNodeHeight(pTempNode->m_pLeft);
		int nTempNodeRightHeight = GetNodeHeight(pTempNode->m_pRight);

		// 左旋 AND 左双旋
		if (nTempNodeLeftHeight - nTempNodeRightHeight < -1)
		{
			/*
			1、获取不平衡结点的右子树
			2、当不平衡结点的左子树的左孩子高度小于等于右孩子的高度就 进行左单旋
			3、反之，进行进行左双旋
			*/
			PSTUNODE pTempNodeRightChild = pTempNode->m_pRight;

			if (GetNodeHeight(pTempNodeRightChild->m_pLeft) <=
				GetNodeHeight(pTempNodeRightChild->m_pRight))
			{
				/*
				示例：P --> 根节点 A 的头节点， X 为新插入的结点，可以看出此时结点 A 出现不平衡
								旋转前																	旋转后
									P																			P
									A																			B
							T				B							----->						A				D
										C		D													T		C				X
													X
				注意：旋转前，结点 B 的右孩子 C，是小于 B 结点数据的
						  所以旋转后，B 作为根节点，C 不能在根节点 B 的右部分，可作为结点 A 的右孩子
				*/
				RotateLeft(pTempNode);
			}
			else
			{
				/*
				示例：P --> 根结点 A 的父结点， X 为新插入的结点，可以看出此时结点 A 出现不平衡
								旋转前								右单旋后											左单旋后
									P											P													P
									A											A													C
							T				B							T				C									A				B
										C		D									X			B						T		X					D
									X																	D

				注意：旋转前，结点 A 出现不平衡，判断发现其右孩子的左结点高度高于右结点的高度，
						  此时需要先以上图的 B 结点进行 右单旋，然后再以结点 A 进行左单旋
						  左双旋后，此时当前数处于平衡状态
				*/

				// 获取需要进行旋转的结点
				PSTUNODE pNodeA = pTempNode;
				PSTUNODE pNodeB = pTempNode->m_pRight;

				// 先以结点 pNodeB 进行右单旋，之后再以 pNodeA 进行 左单旋
				RotateRight(pNodeB);
				RotateLeft(pNodeA);
			}
		}
		// 右旋 和 右双旋
		else if (nTempNodeLeftHeight - nTempNodeRightHeight > 1)
		{
			PSTUNODE pTempNodeLiftChild = pTempNode->m_pLeft;
			/*
			1、获取不平衡结点的左子树
			2、当不平衡结点的左子树的左孩子高度大于等于右孩子的高度就 进行右单旋
			3、反之，进行进行右双旋
			*/
			if (GetNodeHeight(pTempNodeLiftChild->m_pLeft) >=
				GetNodeHeight(pTempNodeLiftChild->m_pRight))
			{
				/*
				示例：P --> 根结点 B 的父结点， X 为新插入的结点，可以看出此时结点 B 出现不平衡
								旋转前												旋转后
									P														P
									B														A
							A				D										T				B
						T		C												X				C		D
					X
				注意：旋转前，结点 A 的右孩子 C，是大于 A 结点数据的
						  所以旋转后，A 作为根节点，C 不能在根节点 A 的左部分，应为结点B的左孩子
				*/
				RotateRight(pTempNode);
			}
			else
			{
				/*
				示例：P --> 根结点 B 的父结点， X 为新插入的结点，可以看出此时结点 B 出现不平衡
								旋转前								左单旋后										右单旋后
									P										P													P
									B										B													C
							A				D						C				D									A				B
						T		C								A		X											T				X		D
									X						T

				注意：旋转前，结点 B 出现不平衡，判断发现其左孩子的右结点高度高于左结点的高度，
						  此时需要先以上图的 A 结点进行 左单旋，然后再以结点 B 进行右单旋
						  右双旋后，此时当前数处于平衡状态
				*/

				// 获取旋转的结点
				PSTUNODE pNodeB = pTempNode;
				PSTUNODE pNodeA = pTempNode->m_pLeft;

				RotateLeft(pNodeA);		// 进行单左旋
				RotateRight(pNodeB);	// 进行单右旋
			}
		}
		// 如果当前结点平衡，接着判断其父结点是否处于平衡状态
		pTempNode = pTempNode->m_pParent;
	}
}


/*=======================================================
*	int CAVLTree<T>::CalcNodeHeight(PSTUNODE pNode) -- 计算 传入结点的高度
*
*	参数：PSTUNODE pNode -- 需要计算高度的结点
*
*	返回值：返回结点的高度值
*
*	警告：无
*
*	更新：2020/06/12
*
========================================================*/
template<typename T>
int CAVLTree<T>::CalcNodeHeight(PSTUNODE pNode)
{
	/*
	1、获取结点的左孩子的高度，如果为空，将其高度置 -1
	2、获取结点的右孩子的高度，如果为空，将其高度置 -1
	3、取左右孩子结点中高度最大的，然后再加上1
	*/
	int nNodeLeftHeght = pNode->m_pLeft == nullptr ? -1 : pNode->m_pLeft->m_nNodeHeight;
	int nNodeRightHeight = pNode->m_pRight == nullptr ? -1 : pNode->m_pRight->m_nNodeHeight;

	return nNodeLeftHeght > nNodeRightHeight ? (nNodeLeftHeght + 1) : (nNodeRightHeight + 1);
}


/*=======================================================
*	int CAVLTree<T>::GetNodeHeight(PSTUNODE pNode) const -- 获取结点高度
*
*	参数：PSTUNODE pNode -- 需要获取高度的结点
*
*	返回值：返回结点的高度值
*
*	警告：无
*
*	更新：2020/06/12
*
========================================================*/
template<typename T>
int CAVLTree<T>::GetNodeHeight(PSTUNODE pNode) const
{
	if (pNode == nullptr)
	{
		return -1;	// 结点为空，返回 -1
	}
	return pNode->m_nNodeHeight;
}


/*=======================================================
*	void CAVLTree<T>::RotateLeft(PSTUNODE pNode) --  左单旋，调整树的平衡
*
*	参数：PSTUNODE pNode -- 需要进行旋转的结点
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::RotateLeft(PSTUNODE pNode)
{
	/*
	此时从当前结点处开始不平衡
	示例：P --> 根节点 A 的头节点， X 为新插入的结点，可以看出此时结点 A 出现不平衡
	当结点左孩子的高度减去右孩子的高度差的绝对值小于 -1 时，需要进行左旋转（从不平衡点开始旋转）
					旋转前																	旋转后
						P																			P
						A																			B
				T				B							----->						A				D
							C		D													T		C				X
										X
	注意：旋转前，结点 B 的右孩子 C，是小于 B 结点数据的
			  所以旋转后，B 作为根节点，C 不能在根节点 B 的右部分
	*/

	// 将旋转时受影响的结点取出
	PSTUNODE pNodeA = pNode;
	PSTUNODE pParent = pNode->m_pParent;
	PSTUNODE pNodeB = pNodeA->m_pRight;
	PSTUNODE pNodeC = pNodeB->m_pLeft;

	// 修改各个受影响的结点
	if (pParent == nullptr)
	{
		m_pRoot = pNodeB;	// 此时结点 B 作为新的根结点
		pNodeB->m_pParent = nullptr;	// 此时结点 B 的头结点为 nullptr
	}
	// 不是根节点
	else
	{
		/*
		1)	如果旋转前根结点是其父结点的左孩子，就将旋转后的根结点 pNodeB
			做为 pNodeA 父结点的左孩子，反之作为右孩子
		2)	旋转后，修改旋转上来的结点的父结点
			pNode->m_pRight->m_pParent = pNode->m_pParent;
		*/
		pParent->m_pRight == pNodeA ? pParent->m_pRight = pNodeB : pParent->m_pLeft = pNodeB;
		pNodeB->m_pParent = pParent;
	}

	// 如果结点 pNodeB 存在左孩子，让其左孩子向原pNodeB的根结点pNodeA的右边靠拢
	pNodeA->m_pRight = pNodeC;
	if (pNodeC != nullptr)
	{
		pNodeC->m_pParent = pNodeA;
	}

	pNodeA->m_pParent = pNodeB;
	pNodeB->m_pLeft = pNodeA;

	// 修改结点高度
	pNodeA->m_nNodeHeight = CalcNodeHeight(pNodeA);
	pNodeB->m_nNodeHeight = CalcNodeHeight(pNodeB);
}


/*=======================================================
*	void CAVLTree<T>::RotateRight(PSTUNODE pNode) --  右单旋，调整树的平衡	
*
*	参数：PSTUNODE pNode -- 需要进行旋转的结点
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/12
*
========================================================*/
template<typename T>
void CAVLTree<T>::RotateRight(PSTUNODE pNode)
{
	/*
	此时从当前结点处开始不平衡
	示例：P --> 根节点 B 的头节点， X 为新插入的结点，可以看出此时结点 B 出现不平衡
	当结点左孩子的高度减去右孩子的高度差的绝对值小于 -1 时，需要进行左旋转（从不平衡点开始旋转）
					旋转前												旋转后
						P														P
						B														A
				A				D										T				B
			T		C												X				C		D
		X
	注意：旋转前，结点 A 的右孩子 C，是大于 A 结点数据的
			  所以旋转后，A 作为根节点，C 不能在根节点 A 的左部分，应为结点B的左孩子

	*/

	// 将旋转时会受到影响的的结点取出
	PSTUNODE pNodeB = pNode;
	PSTUNODE pParent = pNodeB->m_pParent;
	PSTUNODE pNodeA = pNodeB->m_pLeft;
	PSTUNODE pNodeC = pNodeA->m_pRight;

	// 修改受影响的各个结点

	if (pParent == nullptr)
	{
		m_pRoot = pNodeA;	// 结点 pNodeA 做为根结点
		pNodeA->m_pParent = nullptr;	// 根结点 pNodeA 的父结点为 nullptr
	}
	else
	{
		/*
		1)	如果旋转前根结点是其父结点的左孩子，就将旋转后的根结点 pNodeA
			做为pNodeB父结点的左孩子，反之作为右孩子
		2)	其左孩子的父结点 修改为 当前结点的父结点
			pNode->m_pLeft->m_pParent = pNode->m_pParent;
		*/
		pParent->m_pLeft == pNodeB ? pParent->m_pLeft = pNodeA : pParent->m_pRight = pNodeA;
		pNodeA->m_pParent = pParent;
	}

	// 如果结点 pNodeA 存在右孩子，旋转后让其右孩子 pNodeC 作为 pNodeB 的左孩子
	pNodeB->m_pLeft = pNodeC;
	if (pNodeC != nullptr)
	{
		pNodeC->m_pParent = pNodeB;
	}

	pNodeB->m_pParent = pNodeA;
	pNodeA->m_pRight = pNodeB;

	// 重新计算受影响的结点高度值
	pNodeA->m_nNodeHeight = CalcNodeHeight(pNodeA);
	pNodeB->m_nNodeHeight = CalcNodeHeight(pNodeB);
}