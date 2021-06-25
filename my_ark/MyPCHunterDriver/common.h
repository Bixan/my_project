#pragma once
#include <ntdef.h>

// GDT
typedef struct tagGDTInfo
{
	UINT16 uGDTlimit; // GDT��Χ
	UINT16 uGDTLowBase; // GDT�ͻ�ַ
	UINT16 uGDTHighBase; // GDT�߻�ַ
}GDTINFO, * PGDTINFO;

// GDT ��ṹ
typedef struct tagGDTEntry
{
	UINT64 Limit0_15 : 16; // ����
	UINT64 base0_23 : 24; // ��ַ1
	UINT64 TYPE : 4; // ����
	UINT64 S : 1; // Ϊ0����ϵͳ�Σ�1�Ǵ���λ������ݶ�
	UINT64 DPL : 2; // ��Ȩ��
	UINT64 P : 1; // ���Ƿ����λ
 	UINT64 Limit16_19 : 4; // ����
	UINT64 AVL : 1; // ϵͳ�������λ
	UINT64 O : 1; // O
	UINT64 D_B : 1; // Ĭ�ϴ�С
	UINT64 G : 1; // G ������
	UINT64 Base24_31 : 8; // ��ַ2
}GDTENTRY, *PGDTENTRY;

// �������д��ַ�����Ϣ
typedef struct tagRWAddr
{
	ULONG dwPID; // ����PID
	PVOID pDestMemAddr; // Ŀ����̵��ڴ��ַ
	CHAR	szRWBuff[256]; // �����ȡ�ڴ����ݵĻ�����
	ULONG ulReadSize; // ��ȡ���ֽڴ�С
}RWADDR, *PRWADDR;


// IDT
typedef struct tagIDTInfo
{
	UINT16 uIdtLimit; // IDT��Χ
	UINT16 uLowIdtBase; // IDT�ͻ�ַ
	UINT16 uHighIdtBase; // IDT�߻�ַ
}IDTINFO, *PIDTINFO;

// IDT ��ṹ
typedef struct tagIDTEntry
{
	UINT16 uOffsetLow; // �͵�ַ
	UINT16 uSelector; // ��ѡ����
	UINT8  uReverse_1; // ����
	UINT8  uType : 4; // �ж�����
	UINT8  StorageSegment : 1; // Ϊ0���ж���
	UINT8  uDpl : 2; // ��Ȩ��
	UINT8  uPresent : 1; // δʹ���жϣ�����Ϊ0
	UINT16 uOffsetHigh; // �ߵ�ַ
}IDTENTRY, *PIDTENTRY;

// ϵͳ������������ SSDT
typedef struct tagSystemServiceDescriptorTable
{
	PVOID ServiceTableBase; // ������ַ����׵�ַ
	PULONG ServiceCounterTableBase; // ��������������ÿ�����������õĴ�����
	ULONG NumberOfService; // �������ĸ���
	ULONG ParamTableBase; // �����������׵�ַ// �����������׵�ַ
}SystemServiceDescriptorTable, *PSystemServiceDescriptorTable;

typedef struct tagSSDT
{
	ULONG ulAddr; // ���ػ�ַ
	ULONG ulSysCallIndex; // �������
}SSDT, *PSSDT;

typedef struct _LDR_DATA_TABLE_ENTRY 
{
	LIST_ENTRY InLoadOrderLinks; //˫������
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


// ��������ģ����Ϣ
typedef struct tagDrvModule
{
	ULONG DllBase; // ��ַ
	ULONG Size; // ��С
	CHAR szNameBuff[256 * 2]; // ������
	CHAR FullDllName[256 * 2]; // ·��
	struct tagDrvModule* Next; // ָ����һ������ģ����Ϣ
}DRVMODULE, *PDRVMODULE;

/*
 ������Ϣ���
	----- begin -----
*/

// ���������Ϣ
typedef struct tagProcess
{
	ULONG dwPID; // ����ID
	ULONG dwPPID; // ������ID
	CHAR szProcessName[256]; // ������
	CHAR szProcessPath[512]; // ����·��
	struct tagProcess* Next;
}PROCESS, * PPROCESS;

typedef struct tagThread
{
	ULONG dwTID; // �߳�ID
	ULONG dwBasePriority; // ���ȼ�
	ULONG dwStartAddress; // �̵߳���ʼ��ַ
	ULONG dwStatus; // ״̬
	struct _THREAD* Next; // ָ����һ���߳̽ṹ��Ϣ
}THREAD, * PTHREAD;

#if 0
typedef struct tagProcess2
{
	ULONG dwPID; // ����ID
	ULONG_PTR PageDirectoryBase; // ҳĿ¼���ַ
	CHAR szProcessName[512]; // ������
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

// ��������(ʹ��δ��������)
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
 ������Ϣ���
	----- end -----
*/
#endif // 0
