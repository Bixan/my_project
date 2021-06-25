#pragma once
#include <ntifs.h>

// �����ص�
NTSTATUS DriverUnload(PDRIVER_OBJECT pDriver);
NTSTATUS DispatchCreate(PDEVICE_OBJECT pDevice, PIRP pIrp);
NTSTATUS DispatchClose(PDEVICE_OBJECT pDevice, PIRP pIrp);
NTSTATUS DispatchControl(PDEVICE_OBJECT pDevice, PIRP pIrp);

// ��ȡGDT��
ULONG GetGDT(PVOID pOutPut, ULONG ulBuffSize);

// ��ȡ�������еĽ���
ULONG GetProcess(PVOID pOutPut, ULONG ulBuffSize);

// ��ȡ_EPROCESS.ActiveProcessLinks��ƫ��
ULONG GetActiveprocesslinksOffset();

// ͨ��PID���Ҷ�Ӧ�Ľ���
PEPROCESS FindProcessById(ULONG ulPID);

/*
	ͨ��PID��ȡָ�����̵��ڴ�
	ulPID: ����PID
	pDestMemAddr: Ŀ����̵��ڴ��ַ
	pReadBuff: �����ȡ�ڴ����ݵĻ�����
	ulReadSize: ��ȡ���ֽڴ�С
	����ֵ: ��ȡ���ֽڴ�С
*/
ULONG_PTR ReadProcessMemory(ULONG ulPID, 
	PVOID pDestMemAddr, 
	PVOID pReadBuff, 
	ULONG ulReadSize);

/*
	ͨ��PIDд�����ݵ�ָ�������ڴ���
	ulPID: ����PID
	pDestMemAddr: Ŀ����̵��ڴ��ַ
	pWriteBuff: ����д���ڴ����ݵĻ�����
	ulWriteSize: д����ֽڴ�С
	����ֵ: д����ֽڴ�С
*/
ULONG_PTR WriteProcessMemory(ULONG ulPID, 
	PVOID pDestMemAddr, 
	PVOID pWriteBuff, 
	ULONG ulWriteSize);

// ��ȡIDT��
ULONG GetIDT(PVOID pOutPut, ULONG ulBuffSize);

// ��ȡSSDT��
ULONG GetSSDT(PVOID pOutPut, ULONG ulBuffSize);

// ��ȡ����ģ��
ULONG GetDrvModule(PVOID pOutPut, ULONG ulBuffSize, PDRIVER_OBJECT pDriver);

// ��ȡ�߳���Ϣ
ULONG GetThreadInfo(PVOID pOutPut, ULONG ulBuffSize, PVOID pPID);

// ��������
void TerminateProcess(PVOID pPID);