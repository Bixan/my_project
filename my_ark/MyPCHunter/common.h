#pragma once
#include "pch.h"
#include <winioctl.h>

#define MYCTLCODE(code)CTL_CODE(FILE_DEVICE_UNKNOWN,\
	0x800+(code),\
	METHOD_OUT_DIRECT,\
	FILE_ANY_ACCESS)

// ������
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

// �Զ�����Ϣ
#define WM_GETPROCESS WM_USER + 101
#define WM_GETGDT WM_USER + 102
#define WM_GETIDT WM_USER + 103
#define WM_GETSSDT WM_USER + 104
#define WM_GETDRVMODULE	WM_USER + 105

// ȫ���豸��
extern HANDLE g_hDev;

typedef struct tagProcess
{
	DWORD dwPID; // ����ID
	DWORD dwPPID; // ������ID
	CHAR szProcessName[MAXBYTE]; // ������
	WCHAR wszProcessPath[MAXBYTE]; // ����·��
	struct tagProcess* Next;
}PROCESS, *PPROCESS;

//typedef struct tagProcess2
//{
//	ULONG dwPID; // ����ID
//	ULONG_PTR PageDirectoryBase; // ҳĿ¼���ַ
//	CHAR szProcessName[256*2]; // ������
//	struct tagProcess* Next;
//}PROCESS2, * PPROCESS2;

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

// SSDT
typedef struct tagSSDT
{
	ULONG ulAddr; // ���ػ�ַ
	ULONG ulSysCallIndex; // �������
}SSDT, * PSSDT;

// ��������ģ����Ϣ
typedef struct tagDrvModule
{
	DWORD dwDllBase; // ��ַ
	DWORD dwSize; // ��С
	WCHAR wszNameBuff[256]; // ������
	WCHAR FullDllName[256]; // ·��
	struct tagDrvModule* Next; // ָ����һ������ģ����Ϣ
}DRVMODULE, *PDRVMODULE;

typedef struct tagThread
{
	DWORD dwTID; // �߳�ID
	DWORD dwBasePriority; // ���ȼ�
	DWORD dwStartAddress; // �̵߳���ʼ��ַ
	DWORD dwStatus; // ״̬
	struct _THREAD* Next; // ָ����һ���߳̽ṹ��Ϣ
}THREAD, *PTHREAD;