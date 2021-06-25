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
	// 自动释放资源
	Destroy();
}

int CThreadPool::Create(int maxThreads)
{
	// 线程不为空，创建线程时，应将其销毁
	if (_threads != NULL)
	{
		Destroy();
	}


	// 初始化 -- 创建信号量，通知对应的线程去处理任务
	_sem[0] = CreateSemaphore(NULL, 0, maxThreads * 10, NULL);
	// 初始化失败
	if (NULL == _sem[0])
		return -1;

	// 同步对象，创建时间对象 专门用来通知线程结束
	_sem[1] = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (_sem[1] == NULL)
		return -1;

	// 初始化临界区
	InitializeCriticalSection(&_cs);


	_maxThreads = maxThreads;
	_threads = new HANDLE[maxThreads];
	if (_threads == NULL) 
	{
		return -1; // 申请内存失败
	}

	memset(_threads, 0, sizeof(HANDLE) * maxThreads);
	_curThreads = 0;
	_idleCount = 0;
	return 0;
}

int CThreadPool::AddTask(ITask* Task)
{
	/*
	1.添加任务到队列
	2.如果没空闲的线程（创建线程 max）
	3.如果有空闲的线程（通知处理任务)	
	*/

	// 信号量 -- 通知所需线程处理任务
	EnterCriticalSection(&_cs);	// 进入临界区
	
	// 将任务添加到任务队列中 -- 防止添加任务的时候，别的进程拿任务
	_taskQueue.push(Task); // 退出临界区
	
	LeaveCriticalSection(&_cs);

	// 存在空闲的线程数量
	if (_idleCount == 0)
	{
		// 当前线程数量 小于 线程最大数量
		if (_curThreads < _maxThreads)
		{
			// 创建线程
			DWORD tid;
			_threads[_curThreads] = CreateThread(NULL, 0, ThreadProc, this, 0, &tid);
			printf("create thread pool:%08x id:%d\n", _threads[_curThreads], tid);
			// 使用内存锁，当前线程数量++
			InterlockedIncrement(&_curThreads);
		}
	}

	// 信号量下标为0 投递信号
	ReleaseSemaphore(_sem[0], 1, NULL);

	return 0;
}

int CThreadPool::Destroy()
{
	// 没创建
	if (_threads == NULL) 
	{
		return 0;
	}

	// 将指定的事件对象设置为信号状态
	SetEvent(_sem[1]);

	// 等待线程结束，退出
	WaitForMultipleObjects(_curThreads, _threads, TRUE, INFINITE);

	// 释放信号量
	CloseHandle(_sem[0]);
	CloseHandle(_sem[1]);

	// 任务没有时间处理 -- 终止任务，清除任务队列中无法处理的任务
	while (!_taskQueue.empty())
	{
		ITask *task = _taskQueue.front();
		task->Abort();
		if (task != NULL)
			delete task;
		_taskQueue.pop();
	}

	// 删除临界区
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
	// 转换函数指针
	CThreadPool* pool = (CThreadPool *)lpParameter;

	while (true)
	{
		// 使用内存锁 -- 解决对应的同步问题
		InterlockedIncrement(&pool->_idleCount); // 空闲状态
		// pool->_idleCount++;

		// 等待多个任务
		DWORD index = WaitForMultipleObjects(2, pool->_sem, FALSE, INFINITE);
		// 信号量下标为0，处理任务，为1 -- 通知线程结束
		if (index == 1)
		{
			break;
		}

		InterlockedDecrement(&pool->_idleCount); // 忙碌状态
		 // pool->_idleCount--;

		// 获取任务 -- 返回任务指针（访问共同的资源存在同步问题）
		ITask *task = pool->GetTask();

		// 执行任务
		if (task != NULL)
		{
			task->Excetue();
			// 任务执行完毕，删除任务指针
			delete task;
		} 
	}

	return 0;
}

// 获取任务
ITask* CThreadPool::GetTask()
{
	// 进入临界区
	EnterCriticalSection(&_cs);

	// 判断任务队列是否为空
	if (_taskQueue.empty())
	{
		// 任务队列没任务 
		// 退出临界区，防止死锁
		LeaveCriticalSection(&_cs);
		return nullptr;
	}

	// 操作任务队列会有同步问题 -- 使用临界区解决

	// 任务队列有任务，任务出队列
	ITask*	task = _taskQueue.front();
	_taskQueue.pop();

	// 退出临界区
	LeaveCriticalSection(&_cs);

	return task;
}