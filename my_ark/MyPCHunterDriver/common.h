#pragma once
#include <ntdef.h>

// GDT
typedef struct tagGDTInfo
{
	UINT16 uGDTlimit; // GDT范围
	UINT16 uGDTLowBase; // GDT低基址
	UINT16 uGDTHighBase; // GDT高基址
}GDTINFO, * PGDTINFO;

// GDT 表结构
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
typedef struct tagIDTInfo
{
	UINT16 uIdtLimit; // IDT范围
	UINT16 uLowIdtBase; // IDT低基址
	UINT16 uHighIdtBase; // IDT高基址
}IDTINFO, *PIDTINFO;

// IDT 表结构
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

// 系统服务描述符表 SSDT
typedef struct tagSystemServiceDescriptorTable
{
	PVOID ServiceTableBase; // 函数地址表的首地址
	PULONG ServiceCounterTableBase; // 保留（函数表中每个函数被调用的次数）
	ULONG NumberOfService; // 服务函数的个数
	ULONG ParamTableBase; // 参数个数表首地址// 参数个数表首地址
}SystemServiceDescriptorTable, *PSystemServiceDescriptorTable;

typedef struct tagSSDT
{
	ULONG ulAddr; // 加载基址
	ULONG ulSysCallIndex; // 调用序号
}SSDT, *PSSDT;

typedef struct _LDR_DATA_TABLE_ENTRY 
{
	LIST_ENTRY InLoadOrderLinks; //双向链表
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	ULONG Flags;
	USHORT LoadCount;
	USHORT TlsIndex;
	union 
	{
		LIST_ENTRY HashLinks;
		struct 
		{
			PVOID SectionPointer;
			ULONG CheckSum;
		}s1;
	}u1;
	union 
	{
		struct 
		{
			ULONG TimeDateStamp;
		}s2;
		struct 
		{
			PVOID LoadedImports;
		}s3;
	}u2;
}LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;


// 保存驱动模块信息
typedef struct tagDrvModule
{
	ULONG DllBase; // 基址
	ULONG Size; // 大小
	CHAR szNameBuff[256 * 2]; // 驱动名
	CHAR FullDllName[256 * 2]; // 路径
	struct tagDrvModule* Next; // 指向下一个驱动模块信息
}DRVMODULE, *PDRVMODULE;

/*
 进程信息相关
	----- begin -----
*/

// 进程相关信息
typedef struct tagProcess
{
	ULONG dwPID; // 进程ID
	ULONG dwPPID; // 父进程ID
	CHAR szProcessName[256]; // 进程名
	CHAR szProcessPath[512]; // 进程路径
	struct tagProcess* Next;
}PROCESS, * PPROCESS;

typedef struct tagThread
{
	ULONG dwTID; // 线程ID
	ULONG dwBasePriority; // 优先级
	ULONG dwStartAddress; // 线程的起始地址
	ULONG dwStatus; // 状态
	struct _THREAD* Next; // 指向下一个线程结构信息
}THREAD, * PTHREAD;

#if 0
typedef struct tagProcess2
{
	ULONG dwPID; // 进程ID
	ULONG_PTR PageDirectoryBase; // 页目录表地址
	CHAR szProcessName[512]; // 进程名
	struct tagProcess* Next;
}PROCESS2, * PPROCESS2;


typedef enum _SYSTEM_INFORMATION_CLASS {
	SystemBasicInformation,
	SystemProcessorInformation, // obsolete...delete
	SystemPerformanceInformation,
	SystemTimeOfDayInformation,
	SystemPathInformation,
	SystemProcessInformation,
	SystemCallCountInformation,
	SystemDeviceInformation,
	SystemProcessorPerformanceInformation,
	SystemFlagsInformation,
	SystemCallTimeInformation,
	SystemModuleInformation,
	SystemLocksInformation,
	SystemStackTraceInformation,
	SystemPagedPoolInformation,
	SystemNonPagedPoolInformation,
	SystemHandleInformation,
	SystemObjectInformation,
	SystemPageFileInformation,
	SystemVdmInstemulInformation,
	SystemVdmBopInformation,
	SystemFileCacheInformation,
	SystemPoolTagInformation,
	SystemInterruptInformation,
	SystemDpcBehaviorInformation,
	SystemFullMemoryInformation,
	SystemLoadGdiDriverInformation,
	SystemUnloadGdiDriverInformation,
	SystemTimeAdjustmentInformation,
	SystemSummaryMemoryInformation,
	SystemMirrorMemoryInformation,
	SystemPerformanceTraceInformation,
	SystemObsolete0,
	SystemExceptionInformation,
	SystemCrashDumpStateInformation,
	SystemKernelDebuggerInformation,
	SystemContextSwitchInformation,
	SystemRegistryQuotaInformation,
	SystemExtendServiceTableInformation,
	SystemPrioritySeperation,
	SystemVerifierAddDriverInformation,
	SystemVerifierRemoveDriverInformation,
	SystemProcessorIdleInformation,
	SystemLegacyDriverInformation,
	SystemCurrentTimeZoneInformation,
	SystemLookasideInformation,
	SystemTimeSlipNotification,
	SystemSessionCreate,
	SystemSessionDetach,
	SystemSessionInformation,
	SystemRangeStartInformation,
	SystemVerifierInformation,
	SystemVerifierThunkExtend,
	SystemSessionProcessInformation,
	SystemLoadGdiDriverInSystemSpace,
	SystemNumaProcessorMap,
	SystemPrefetcherInformation,
	SystemExtendedProcessInformation,
	SystemRecommendedSharedDataAlignment,
	SystemComPlusPackage,
	SystemNumaAvailableMemory,
	SystemProcessorPowerInformation,
	SystemEmulationBasicInformation,
	SystemEmulationProcessorInformation,
	SystemExtendedHandleInformation,
	SystemLostDelayedWriteInformation,
	SystemBigPoolInformation,
	SystemSessionPoolTagInformation,
	SystemSessionMappedViewInformation,
	SystemHotpatchInformation,
	SystemObjectSecurityMode,
	SystemWatchdogTimerHandler,
	SystemWatchdogTimerInformation,
	SystemLogicalProcessorInformation,
	SystemWow64SharedInformation,
	SystemRegisterFirmwareTableInformationHandler,
	SystemFirmwareTableInformation,
	SystemModuleInformationEx,
	SystemVerifierTriageInformation,
	SystemSuperfetchInformation,
	SystemMemoryListInformation,
	SystemFileCacheInformationEx,
	MaxSystemInfoClass  // MaxSystemInfoClass should always be the last enum
} SYSTEM_INFORMATION_CLASS;

// 函数声明(使用未公开函数)
NTSTATUS NTAPI ZwQuerySystemInformation(
	__in SYSTEM_INFORMATION_CLASS SystemInformationClass,
	__out_bcount_opt(SystemInformationLength) PVOID SystemInformation,
	__in ULONG SystemInformationLength,
	__out_opt PULONG ReturnLength
);

typedef struct _SYSTEM_PROCESS_INFORMATION {
	ULONG NextEntryOffset;
	ULONG NumberOfThreads;
	LARGE_INTEGER SpareLi1;
	LARGE_INTEGER SpareLi2;
	LARGE_INTEGER SpareLi3;
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER KernelTime;
	UNICODE_STRING ImageName;
	KPRIORITY BasePriority;
	HANDLE UniqueProcessId;
	HANDLE InheritedFromUniqueProcessId;
	ULONG HandleCount;
	ULONG SessionId;
	ULONG_PTR PageDirectoryBase;
	SIZE_T PeakVirtualSize;
	SIZE_T VirtualSize;
	ULONG PageFaultCount;
	SIZE_T PeakWorkingSetSize;
	SIZE_T WorkingSetSize;
	SIZE_T QuotaPeakPagedPoolUsage;
	SIZE_T QuotaPagedPoolUsage;
	SIZE_T QuotaPeakNonPagedPoolUsage;
	SIZE_T QuotaNonPagedPoolUsage;
	SIZE_T PagefileUsage;
	SIZE_T PeakPagefileUsage;
	SIZE_T PrivatePageCount;
	LARGE_INTEGER ReadOperationCount;
	LARGE_INTEGER WriteOperationCount;
	LARGE_INTEGER OtherOperationCount;
	LARGE_INTEGER ReadTransferCount;
	LARGE_INTEGER WriteTransferCount;
	LARGE_INTEGER OtherTransferCount;
} SYSTEM_PROCESS_INFORMATION, * PSYSTEM_PROCESS_INFORMATION;

/*
 进程信息相关
	----- end -----
*/
#endif // 0
