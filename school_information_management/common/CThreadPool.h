#pragma once

#include <windows.h>
#include <queue>

class ITask
{
public:
	virtual int Excetue() = 0; //执行任务	http[]
	virtual int Abort() = 0;	 //终止任务 
};

typedef	std::queue<ITask*> TaskQueue;

class CThreadPool
{
public:
	CThreadPool();
	~CThreadPool();
	int Create(int maxThreads);	 //创建线程池
	int AddTask(ITask *task);	//添加任务
	int Destroy();	//销毁线程池
private:
	static DWORD WINAPI ThreadProc(LPVOID lpParameter);
	ITask* GetTask();	// 获取任务
private:
	HANDLE *		_threads;	// 线程
	unsigned int	_maxThreads;	// 线程最大数量
	unsigned int	_curThreads;	// 当前线程数量
	unsigned int	_idleCount;	// 空闲的线程数量
	TaskQueue	_taskQueue;	// 任务队列
	HANDLE		_sem[2];	// 信号量	//停止信号	
	CRITICAL_SECTION _cs;	// 同步对象 -- 临界区
	HANDLE		_event;	// 同步对象，专门用来通知线程结束
};