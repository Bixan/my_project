#include "MyPCTools.h"
#include "common.h"

#define USTR_DEV_NAME L"\\Device\\MyDrive"
#define USTR_SYMBOLINK L"\\??\\MyPCHunterDev"

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


PRWADDR g_rd;
PDRIVER_OBJECT g_pDriverObj;

// �������
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath)
{
	PAGED_CODE();
	UNREFERENCED_PARAMETER(pRegistryPath);

	NTSTATUS Status = STATUS_SUCCESS;
	g_pDriverObj = pDriverObject;

	// �����豸����
	UNICODE_STRING ustrDevName;
	RtlInitUnicodeString(&ustrDevName, USTR_DEV_NAME);
	PDEVICE_OBJECT pDevObj = NULL;
	Status = IoCreateDevice(pDriverObject, 
		0, 
		&ustrDevName, 
		FILE_DEVICE_UNKNOWN,  // δ֪�豸
		0, 
		FALSE, // �Ƿ����������̲���
		&pDevObj);
	if (Status != STATUS_SUCCESS)
	{
		DbgPrint("[MyDrive]�������豸ʧ�ܣ�������:%08X\n", Status);
		return Status;
	}

	pDevObj->Flags |= DO_DIRECT_IO;
	// Ϊ�豸����һ����������
	UNICODE_STRING DosSymName;
	RtlInitUnicodeString(&DosSymName, USTR_SYMBOLINK);
	IoCreateSymbolicLink(&DosSymName, &ustrDevName);

	// ע������ж�غ���
	pDriverObject->DriverUnload = DriverUnload;

	// ע����ǲ����
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = DispatchCreate;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = DispatchClose;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchControl;

	DbgPrint("[MyDrive]��DriverEntry\n");
	return Status;
}

// ж�ػص�
NTSTATUS DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	PAGED_CODE();
	UNREFERENCED_PARAMETER(pDriverObject);
	
	// ж���豸����
	IoDeleteDevice(pDriverObject->DeviceObject);

	// ɾ����������
	UNICODE_STRING ustrSymbolink;
	RtlInitUnicodeString(&ustrSymbolink, USTR_SYMBOLINK);
	IoDeleteSymbolicLink(&ustrSymbolink);

	DbgPrint("[MyDrive]������ж�سɹ�\n");
	return STATUS_SUCCESS;
}

// �����ص�
NTSTATUS DispatchCreate(PDEVICE_OBJECT pDriverObject, PIRP pIrp)
{
	PAGED_CODE();
	UNREFERENCED_PARAMETER(pDriverObject);
	
	// ֪ͨI/O������������� IoCallDriver(Irp) //�����²�����
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	DbgPrint("[MyDrive]�����������ɹ�\n");
	return STATUS_SUCCESS;
}

// �رջص�
NTSTATUS DispatchClose(PDEVICE_OBJECT pDriverObject, PIRP pIrp)
{
	PAGED_CODE();
	UNREFERENCED_PARAMETER(pDriverObject);
	
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	DbgPrint("[MyDrive]������ֹͣ�ɹ�\n");
	return STATUS_SUCCESS;
}

// ���ƻص�
NTSTATUS DispatchControl(PDEVICE_OBJECT pDriverObject, PIRP pIrp)
{
	PAGED_CODE();
	UNREFERENCED_PARAMETER(pDriverObject);
	NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
	ULONG ulRet = 0;

	// ����IRP���״̬
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;

	// ��ȡ��ǰIOջ
	IO_STACK_LOCATION* pIoStack = IoGetCurrentIrpStackLocation(pIrp);

	// ��ȡ������
	ULONG uCtrlCode = pIoStack->Parameters.DeviceIoControl.IoControlCode;

	// ��ȡ���뻺����
	PVOID pInputBuff = pIrp->AssociatedIrp.SystemBuffer;
	PVOID pOutBuff = NULL;
	if (pIrp->MdlAddress && METHOD_FROM_CTL_CODE(uCtrlCode) & METHOD_OUT_DIRECT)
	{
		pOutBuff = MmGetSystemAddressForMdlSafe(pIrp->MdlAddress, 0);
	}
	ULONG ulBuffSize = pIoStack->Parameters.DeviceIoControl.OutputBufferLength;

	// �ж�Ring3���͵Ŀ�����
	switch (uCtrlCode)
	{
		case GETGDT:
		{
			// ����GDT
			DbgPrint("[MyDrive]������SSDTGDT��\n");
			ulRet = GetGDT(pOutBuff, ulBuffSize);
			//EnumProcess(pOutBuff, ulBuffSize);
			ntStatus = STATUS_SUCCESS;
			break;
		}
		case GETPROCESS:
		{
			// ��������
			DbgPrint("[MyDrive]������SSDT����\n");
			ulRet = GetProcess(pOutBuff, ulBuffSize);
			ntStatus = STATUS_SUCCESS;
			break;
		}
		case READPROCESSMEMORY:
		{
			DbgPrint("[MyDrive]�����ڶ�ȡ�ڴ�\n");
			char szBuf[256] = { 0 }; // �����ȡ����ʱ������
			PRWADDR rd = (PRWADDR)pOutBuff;
			if (rd != NULL)
			{
				DbgPrint("[MyDrive]��READPROCESSMEMORY dwPID: %d\n", rd->dwPID);
				DbgPrint("[MyDrive]��READPROCESSMEMORY pDestMemAddr: %p\n", rd->pDestMemAddr);
				DbgPrint("[MyDrive]��READPROCESSMEMORY pReadBuff: %p\n", rd->szRWBuff);
				DbgPrint("[MyDrive]��READPROCESSMEMORY ulReadSize: %d\n", rd->ulReadSize);
				// ��ȡ�ڴ�
				ULONG_PTR ul = ReadProcessMemory(rd->dwPID, rd->pDestMemAddr, (PVOID)szBuf, rd->ulReadSize);
				if (ul == NULL)
				{
					DbgPrint("[MyDrive]��READPROCESSMEMORY ���ڴ��ַ��Ч\n");
					RtlCopyMemory(rd->szRWBuff, "0", rd->ulReadSize);
					ntStatus = STATUS_UNSUCCESSFUL;
					break;
				}
				
				DbgPrint("[MyDrive]��READPROCESSMEMORY szBuf: %s\n", szBuf);

				RtlCopyMemory(rd->szRWBuff, szBuf, rd->ulReadSize);
				DbgPrint("[MyDrive]��READPROCESSMEMORY rd->szRWBuff: %s\n", rd->szRWBuff);
			}
			ntStatus = STATUS_SUCCESS;
			break;
		}
		case WRITEPROCESSMEMORY:
		{
			// д���ڴ�
			DbgPrint("[MyDrive]������д���ڴ�\n");
			PRWADDR rd = (PRWADDR)pOutBuff;
			if (rd != NULL)
			{
				DbgPrint("[MyDrive]��WRITEPROCESSMEMORY dwPID: %d\n", rd->dwPID);
				DbgPrint("[MyDrive]��WRITEPROCESSMEMORY pDestMemAddr: %p\n", rd->pDestMemAddr);
				DbgPrint("[MyDrive]��WRITEPROCESSMEMORY pReadBuff: %s\n", rd->szRWBuff);
				DbgPrint("[MyDrive]��WRITEPROCESSMEMORY ulReadSize: %d\n", rd->ulReadSize);
				// д���ڴ�
				ULONG_PTR ul = WriteProcessMemory(rd->dwPID, rd->pDestMemAddr, rd->szRWBuff, rd->ulReadSize);
				if (ul == NULL)
				{
					DbgPrint("[MyDrive]��READPROCESSMEMORY ���ڴ��ַ��Ч\n");
					ntStatus = STATUS_UNSUCCESSFUL;
					break;
				}
			}
			ntStatus = STATUS_SUCCESS;
			break;
		}
		case GETIDT:
		{
			DbgPrint("[MyDrive]�����ڻ�ȡIDT��\n");
			ulRet = GetIDT(pOutBuff, ulBuffSize);
			if (ulRet == FALSE)
			{
				DbgPrint("[MyDrive]����ȡIDT��ʧ��\n");
				ntStatus = STATUS_UNSUCCESSFUL;
				break;
			}
			ntStatus = STATUS_SUCCESS;
			break;
		}
		case GETSSDT:
		{
			DbgPrint("[MyDrive]�����ڻ�ȡSSDT��\n");
			ulRet = GetSSDT(pOutBuff, ulBuffSize);
			if (ulRet == FALSE)
			{
				DbgPrint("[MyDrive]����ȡSSDT��ʧ��\n");
				ntStatus = STATUS_UNSUCCESSFUL;
				break;
			}
			ntStatus = STATUS_SUCCESS;
			break;
		}
		case GETDRVMODULE:
		{
			DbgPrint("[MyDrive]�����ڻ�ȡ����ģ��\n");
			ulRet = GetDrvModule(pOutBuff, ulBuffSize, g_pDriverObj);
			ntStatus = STATUS_SUCCESS;
			break;
		}
		case GETTHREAD:
		{
			DbgPrint("[MyDrive]�����ڱ����߳�\n");
			ulRet = GetThreadInfo(pOutBuff, ulBuffSize, pInputBuff);
			ntStatus = STATUS_SUCCESS;
			break;
		}
		case TERMINATEPROCESS:
		{
			DbgPrint("[MyDrive]�����ڱ����߳�\n");
			TerminateProcess(pInputBuff);
			break;
		}
	}

	pIrp->IoStatus.Status = ntStatus;
	pIrp->IoStatus.Information = ulRet;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return ntStatus;
}