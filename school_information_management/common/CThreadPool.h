#pragma once

#include <windows.h>
#include <queue>

class ITask
{
public:
	virtual int Excetue() = 0; //ִ������	http[]
	virtual int Abort() = 0;	 //��ֹ���� 
};

typedef	std::queue<ITask*> TaskQueue;

class CThreadPool
{
public:
	CThreadPool();
	~CThreadPool();
	int Create(int maxThreads);	 //�����̳߳�
	int AddTask(ITask *task);	//�������
	int Destroy();	//�����̳߳�
private:
	static DWORD WINAPI ThreadProc(LPVOID lpParameter);
	ITask* GetTask();	// ��ȡ����
private:
	HANDLE *		_threads;	// �߳�
	unsigned int	_maxThreads;	// �߳��������
	unsigned int	_curThreads;	// ��ǰ�߳�����
	unsigned int	_idleCount;	// ���е��߳�����
	TaskQueue	_taskQueue;	// �������
	HANDLE		_sem[2];	// �ź���	//ֹͣ�ź�	
	CRITICAL_SECTION _cs;	// ͬ������ -- �ٽ���
	HANDLE		_event;	// ͬ������ר������֪ͨ�߳̽���
};