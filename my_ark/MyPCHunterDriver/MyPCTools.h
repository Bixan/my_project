#pragma once
#include <ntifs.h>

// 驱动回调
NTSTATUS DriverUnload(PDRIVER_OBJECT pDriver);
NTSTATUS DispatchCreate(PDEVICE_OBJECT pDevice, PIRP pIrp);
NTSTATUS DispatchClose(PDEVICE_OBJECT pDevice, PIRP pIrp);
NTSTATUS DispatchControl(PDEVICE_OBJECT pDevice, PIRP pIrp);

// 获取GDT表
ULONG GetGDT(PVOID pOutPut, ULONG ulBuffSize);

// 获取正在运行的进程
ULONG GetProcess(PVOID pOutPut, ULONG ulBuffSize);

// 获取_EPROCESS.ActiveProcessLinks的偏移
ULONG GetActiveprocesslinksOffset();

// 通过PID查找对应的进程
PEPROCESS FindProcessById(ULONG ulPID);

/*
	通过PID读取指定进程的内存
	ulPID: 进程PID
	pDestMemAddr: 目标进程的内存地址
	pReadBuff: 保存读取内存数据的缓冲区
	ulReadSize: 读取的字节大小
	返回值: 读取的字节大小
*/
ULONG_PTR ReadProcessMemory(ULONG ulPID, 
	PVOID pDestMemAddr, 
	PVOID pReadBuff, 
	ULONG ulReadSize);

/*
	通过PID写入数据到指定进程内存中
	ulPID: 进程PID
	pDestMemAddr: 目标进程的内存地址
	pWriteBuff: 保存写入内存数据的缓冲区
	ulWriteSize: 写入的字节大小
	返回值: 写入的字节大小
*/
ULONG_PTR WriteProcessMemory(ULONG ulPID, 
	PVOID pDestMemAddr, 
	PVOID pWriteBuff, 
	ULONG ulWriteSize);

// 获取IDT表
ULONG GetIDT(PVOID pOutPut, ULONG ulBuffSize);

// 获取SSDT表
ULONG GetSSDT(PVOID pOutPut, ULONG ulBuffSize);

// 获取驱动模块
ULONG GetDrvModule(PVOID pOutPut, ULONG ulBuffSize, PDRIVER_OBJECT pDriver);

// 获取线程信息
ULONG GetThreadInfo(PVOID pOutPut, ULONG ulBuffSize, PVOID pPID);

// 结束进程
void TerminateProcess(PVOID pPID);