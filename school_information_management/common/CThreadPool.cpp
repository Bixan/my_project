#include "CThreadPool.h"

CThreadPool::CThreadPool()
{
	_maxThreads = 0;
	_threads = NULL;
	_curThreads = 0;
	_idleCount = 0;
	_sem[0] = NULL;
	_sem[1] = NULL;
}

CThreadPool::~CThreadPool()
{
	// �Զ��ͷ���Դ
	Destroy();
}

int CThreadPool::Create(int maxThreads)
{
	// �̲߳�Ϊ�գ������߳�ʱ��Ӧ��������
	if (_threads != NULL)
	{
		Destroy();
	}


	// ��ʼ�� -- �����ź�����֪ͨ��Ӧ���߳�ȥ��������
	_sem[0] = CreateSemaphore(NULL, 0, maxThreads * 10, NULL);
	// ��ʼ��ʧ��
	if (NULL == _sem[0])
		return -1;

	// ͬ�����󣬴���ʱ����� ר������֪ͨ�߳̽���
	_sem[1] = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (_sem[1] == NULL)
		return -1;

	// ��ʼ���ٽ���
	InitializeCriticalSection(&_cs);


	_maxThreads = maxThreads;
	_threads = new HANDLE[maxThreads];
	if (_threads == NULL) 
	{
		return -1; // �����ڴ�ʧ��
	}

	memset(_threads, 0, sizeof(HANDLE) * maxThreads);
	_curThreads = 0;
	_idleCount = 0;
	return 0;
}

int CThreadPool::AddTask(ITask* Task)
{
	/*
	1.������񵽶���
	2.���û���е��̣߳������߳� max��
	3.����п��е��̣߳�֪ͨ��������)	
	*/

	// �ź��� -- ֪ͨ�����̴߳�������
	EnterCriticalSection(&_cs);	// �����ٽ���
	
	// ��������ӵ���������� -- ��ֹ��������ʱ�򣬱�Ľ���������
	_taskQueue.push(Task); // �˳��ٽ���
	
	LeaveCriticalSection(&_cs);

	// ���ڿ��е��߳�����
	if (_idleCount == 0)
	{
		// ��ǰ�߳����� С�� �߳��������
		if (_curThreads < _maxThreads)
		{
			// �����߳�
			DWORD tid;
			_threads[_curThreads] = CreateThread(NULL, 0, ThreadProc, this, 0, &tid);
			printf("create thread pool:%08x id:%d\n", _threads[_curThreads], tid);
			// ʹ���ڴ�������ǰ�߳�����++
			InterlockedIncrement(&_curThreads);
		}
	}

	// �ź����±�Ϊ0 Ͷ���ź�
	ReleaseSemaphore(_sem[0], 1, NULL);

	return 0;
}

int CThreadPool::Destroy()
{
	// û����
	if (_threads == NULL) 
	{
		return 0;
	}

	// ��ָ�����¼���������Ϊ�ź�״̬
	SetEvent(_sem[1]);

	// �ȴ��߳̽������˳�
	WaitForMultipleObjects(_curThreads, _threads, TRUE, INFINITE);

	// �ͷ��ź���
	CloseHandle(_sem[0]);
	CloseHandle(_sem[1]);

	// ����û��ʱ�䴦�� -- ��ֹ�����������������޷����������
	while (!_taskQueue.empty())
	{
		ITask *task = _taskQueue.front();
		task->Abort();
		if (task != NULL)
			delete task;
		_taskQueue.pop();
	}

	// ɾ���ٽ���
	DeleteCriticalSection(&_cs);

	_maxThreads = 0;
	for (int i = 0; i < _curThreads; i++)
	{
		CloseHandle(_threads[i]);
	}

	delete[] _threads;
	_threads = NULL;
	_curThreads = 0;
	_idleCount = 0;

	return 0;
}

DWORD __stdcall CThreadPool::ThreadProc(LPVOID lpParameter)
{
	// ת������ָ��
	CThreadPool* pool = (CThreadPool *)lpParameter;

	while (true)
	{
		// ʹ���ڴ��� -- �����Ӧ��ͬ������
		InterlockedIncrement(&pool->_idleCount); // ����״̬
		// pool->_idleCount++;

		// �ȴ��������
		DWORD index = WaitForMultipleObjects(2, pool->_sem, FALSE, INFINITE);
		// �ź����±�Ϊ0����������Ϊ1 -- ֪ͨ�߳̽���
		if (index == 1)
		{
			break;
		}

		InterlockedDecrement(&pool->_idleCount); // æµ״̬
		 // pool->_idleCount--;

		// ��ȡ���� -- ��������ָ�루���ʹ�ͬ����Դ����ͬ�����⣩
		ITask *task = pool->GetTask();

		// ִ������
		if (task != NULL)
		{
			task->Excetue();
			// ����ִ����ϣ�ɾ������ָ��
			delete task;
		} 
	}

	return 0;
}

// ��ȡ����
ITask* CThreadPool::GetTask()
{
	// �����ٽ���
	EnterCriticalSection(&_cs);

	// �ж���������Ƿ�Ϊ��
	if (_taskQueue.empty())
	{
		// �������û���� 
		// �˳��ٽ�������ֹ����
		LeaveCriticalSection(&_cs);
		return nullptr;
	}

	// ����������л���ͬ������ -- ʹ���ٽ������

	// ����������������������
	ITask*	task = _taskQueue.front();
	_taskQueue.pop();

	// �˳��ٽ���
	LeaveCriticalSection(&_cs);

	return task;
}