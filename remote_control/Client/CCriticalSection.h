#pragma once
#include <windows.h>

// �ٽ���
class CCriticalSection
{
public:
	CCriticalSection();	// ��ʼ���ٽ���
	~CCriticalSection();	// �ͷ��ٽ���
	void EnterCS();
	void LeaveCS();
	void DeleteCS();
private:
	CRITICAL_SECTION m_cs;
};
