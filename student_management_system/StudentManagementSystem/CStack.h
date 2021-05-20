#pragma once
/*=======================================================
*			o(*￣▽￣*)o
****************************************************************************************
*			Project Name：栈
*
*			FileName：CStack.h
*
*			Start Date：2020/06/16
*
*			Last UpDate：2020/06/16
*
========================================================*/
#include "MyLinkedList.h"
template<typename T>
class CStack
{
public:
	// 约定：链表的头部为栈底，链表的尾部为栈顶

	void push(int val);	// 入栈
	void pop();	// 出栈（之弹出栈顶数据，不访问）
	int top();	// 访问栈顶元素，取出数值
	bool empty();	// 栈空间是否为空
	int StackElementSize();	// 栈内元素的个数
	void clear();	// 清空栈

private:
	CList<T> m_list;
};

/*=======================================================
*	inline void CStack<T>::push(int val) -- 数据入栈
*
*	参数：val -- 入栈的数据
*
*	返回值：无
*
*	警告：无
*
*	更新：2020/06/16
*
========================================================*/
template<typename T>
inline void CStack<T>::push(int val)
{
	m_list.AddTail(val);	// 从链表的尾部存储数据
}


/*=======================================================
*	inline void CStack<T>::pop() -- 从栈顶弹出一个数据
*
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
inline void CStack<T>::pop()
{
	m_list.RemoveTail();	// 从链表的尾部弹出数据
}


/*=======================================================
*	typename int CStack<T>::top() -- 访问栈顶数据
*
*	参数：无
*
*	返回值：返回栈顶元素数据值
*
*	警告：无
*
*	更新：2020/06/16
*
========================================================*/
template<typename T>
typename int CStack<T>::top()
{
	return *(--m_list.end());	// 访问栈顶元素，链表尾部哨兵的前一个结点
}


/*=======================================================
*	inline bool CStack<T>::empty() -- 判断当前栈是否为空
*
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
inline bool CStack<T>::empty()
{
	return m_list.IsNull();		// 栈空间是否为空
}


/*=======================================================
*	inline int CStack<T>::StackElementSize() -- 过去栈空间存在有效元素的个数
*
*	参数：无
*
*	返回值：返回栈空间元素的个数
*
*	警告：无
*
*	更新：2020/06/16
*
========================================================*/
template<typename T>
inline int CStack<T>::StackElementSize()
{
	return m_list.NodeSize();	// 栈空间元素的个数
}

template<typename T>
inline void CStack<T>::clear()
{
	m_list.Clear();	// 清空栈空间数据
}
