#pragma once
#include <windows.h>

// 临界区
class CCriticalSection
{
public:
	CCriticalSection();	// 初始化临界区
	~CCriticalSection();	// 释放临界区
	void EnterCS();
	void LeaveCS();
	void DeleteCS();
private:
	CRITICAL_SECTION m_cs;
};
