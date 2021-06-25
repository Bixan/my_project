#pragma once
#include "pch.h"
#include <winioctl.h>

#define MYCTLCODE(code)CTL_CODE(FILE_DEVICE_UNKNOWN,\
	0x800+(code),\
	METHOD_OUT_DIRECT,\
	FILE_ANY_ACCESS)

// 控制码
typedef enum emMyCtlCodes
{
	GETPID = MYCTLCODE(0),
	GETGDT = MYCTLCODE(1),
	GETPROCESS = MYCTLCODE(2),
	READPROCESSMEMORY = MYCTLCODE(3),
	WRITEPROCESSMEMORY = MYCTLCODE(4),
	GETIDT = MYCTLCODE(5),
	GETSSDT = MYCTLCODE(6),
	GETDRVMODULE = MYCTLCODE(7),
	GETTHREAD = MYCTLCODE(8),
	TERMINATEPROCESS = MYCTLCODE(9),
}MYCTLCODES;

// 自定义消息
#define WM_GETPROCESS WM_USER + 101
#define WM_GETGDT WM_USER + 102
#define WM_GETIDT WM_USER + 103
#define WM_GETSSDT WM_USER + 104
#define WM_GETDRVMODULE	WM_USER + 105

// 全局设备名
extern HANDLE g_hDev;

typedef struct tagProcess
{
	DWORD dwPID; // 进程ID
	DWORD dwPPID; // 父进程ID
	CHAR szProcessName[MAXBYTE]; // 进程名
	WCHAR wszProcessPath[MAXBYTE]; // 进程路径
	struct tagProcess* Next;
}PROCESS, *PPROCESS;

//typedef struct tagProcess2
//{
//	ULONG dwPID; // 进程ID
//	ULONG_PTR PageDirectoryBase; // 页目录表地址
//	CHAR szProcessName[256*2]; // 进程名
//	struct tagProcess* Next;
//}PROCESS2, * PPROCESS2;

typedef struct tagGDTEntry
{
	UINT64 Limit0_15 : 16; // 界限
	UINT64 base0_23 : 24; // 基址1
	UINT64 TYPE : 4; // 类型
	UINT64 S : 1; // 为0则是系统段，1是代码段或者数据段
	UINT64 DPL : 2; // 特权级
	UINT64 P : 1; // 段是否存在位
	UINT64 Limit16_19 : 4; // 界限
	UINT64 AVL : 1; // 系统软件可用位
	UINT64 O : 1; // O
	UINT64 D_B : 1; // 默认大小
	UINT64 G : 1; // G 颗粒度
	UINT64 Base24_31 : 8; // 基址2
}GDTENTRY, *PGDTENTRY;

// 保存读，写地址相关信息
typedef struct tagRWAddr
{
	ULONG dwPID; // 进程PID
	PVOID pDestMemAddr; // 目标进程的内存地址
	CHAR	szRWBuff[256]; // 保存读取内存数据的缓冲区
	ULONG ulReadSize; // 读取的字节大小
}RWADDR, *PRWADDR;

// IDT
typedef struct tagIDTEntry
{
	UINT16 uOffsetLow; // 低地址
	UINT16 uSelector; // 段选择子
	UINT8  uReverse_1; // 保留
	UINT8  uType : 4; // 中断类型
	UINT8  StorageSegment : 1; // 为0是中断门
	UINT8  uDpl : 2; // 特权级
	UINT8  uPresent : 1; // 未使用中断，可置为0
	UINT16 uOffsetHigh; // 高地址
}IDTENTRY, *PIDTENTRY;

// SSDT
typedef struct tagSSDT
{
	ULONG ulAddr; // 加载基址
	ULONG ulSysCallIndex; // 调用序号
}SSDT, * PSSDT;

// 保存驱动模块信息
typedef struct tagDrvModule
{
	DWORD dwDllBase; // 基址
	DWORD dwSize; // 大小
	WCHAR wszNameBuff[256]; // 驱动名
	WCHAR FullDllName[256]; // 路径
	struct tagDrvModule* Next; // 指向下一个驱动模块信息
}DRVMODULE, *PDRVMODULE;

typedef struct tagThread
{
	DWORD dwTID; // 线程ID
	DWORD dwBasePriority; // 优先级
	DWORD dwStartAddress; // 线程的起始地址
	DWORD dwStatus; // 状态
	struct _THREAD* Next; // 指向下一个线程结构信息
}THREAD, *PTHREAD;