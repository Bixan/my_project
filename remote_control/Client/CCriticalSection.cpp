#include "CCriticalSection.h"

// ��ʼ���ٽ���
CCriticalSection::CCriticalSection()
{
	InitializeCriticalSection(&m_cs);
}

// �ͷ��ٽ���
CCriticalSection::~CCriticalSection()
{
	DeleteCS();
}

// �ȴ�ָ���ٽ�������
void CCriticalSection::EnterCS()
{
	EnterCriticalSection(&m_cs);
}

// �ͷ��ٽ�������
void CCriticalSection::LeaveCS()
{
	LeaveCriticalSection(&m_cs);
}

// �ͷ������ٽ�������
void CCriticalSection::DeleteCS()
{
	DeleteCriticalSection(&m_cs);
}