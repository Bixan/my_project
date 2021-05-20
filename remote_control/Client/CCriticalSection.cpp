#include "CCriticalSection.h"

// 初始化临界区
CCriticalSection::CCriticalSection()
{
	InitializeCriticalSection(&m_cs);
}

// 释放临界区
CCriticalSection::~CCriticalSection()
{
	DeleteCS();
}

// 等待指定临界区对象
void CCriticalSection::EnterCS()
{
	EnterCriticalSection(&m_cs);
}

// 释放临界区对象
void CCriticalSection::LeaveCS()
{
	LeaveCriticalSection(&m_cs);
}

// 释放无主临界区对象
void CCriticalSection::DeleteCS()
{
	DeleteCriticalSection(&m_cs);
}