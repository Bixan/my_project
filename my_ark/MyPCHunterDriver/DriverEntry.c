#include "MyPCTools.h"
#include "common.h"

#define USTR_DEV_NAME L"\\Device\\MyDrive"
#define USTR_SYMBOLINK L"\\??\\MyPCHunterDev"

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


PRWADDR g_rd;
PDRIVER_OBJECT g_pDriverObj;

// 驱动入口
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath)
{
	PAGED_CODE();
	UNREFERENCED_PARAMETER(pRegistryPath);

	NTSTATUS Status = STATUS_SUCCESS;
	g_pDriverObj = pDriverObject;

	// 创建设备对象
	UNICODE_STRING ustrDevName;
	RtlInitUnicodeString(&ustrDevName, USTR_DEV_NAME);
	PDEVICE_OBJECT pDevObj = NULL;
	Status = IoCreateDevice(pDriverObject, 
		0, 
		&ustrDevName, 
		FILE_DEVICE_UNKNOWN,  // 未知设备
		0, 
		FALSE, // 是否允许多个进程操作
		&pDevObj);
	if (Status != STATUS_SUCCESS)
	{
		DbgPrint("[MyDrive]：创建设备失败，错误码:%08X\n", Status);
		return Status;
	}

	pDevObj->Flags |= DO_DIRECT_IO;
	// 为设备创建一个符号链接
	UNICODE_STRING DosSymName;
	RtlInitUnicodeString(&DosSymName, USTR_SYMBOLINK);
	IoCreateSymbolicLink(&DosSymName, &ustrDevName);

	// 注册驱动卸载函数
	pDriverObject->DriverUnload = DriverUnload;

	// 注册派遣函数
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = DispatchCreate;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = DispatchClose;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchControl;

	DbgPrint("[MyDrive]：DriverEntry\n");
	return Status;
}

// 卸载回调
NTSTATUS DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	PAGED_CODE();
	UNREFERENCED_PARAMETER(pDriverObject);
	
	// 卸载设备对象
	IoDeleteDevice(pDriverObject->DeviceObject);

	// 删除符号链接
	UNICODE_STRING ustrSymbolink;
	RtlInitUnicodeString(&ustrSymbolink, USTR_SYMBOLINK);
	IoDeleteSymbolicLink(&ustrSymbolink);

	DbgPrint("[MyDrive]：驱动卸载成功\n");
	return STATUS_SUCCESS;
}

// 创建回调
NTSTATUS DispatchCreate(PDEVICE_OBJECT pDriverObject, PIRP pIrp)
{
	PAGED_CODE();
	UNREFERENCED_PARAMETER(pDriverObject);
	
	// 通知I/O管理器完成请求 IoCallDriver(Irp) //调用下层驱动
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	DbgPrint("[MyDrive]：驱动创建成功\n");
	return STATUS_SUCCESS;
}

// 关闭回调
NTSTATUS DispatchClose(PDEVICE_OBJECT pDriverObject, PIRP pIrp)
{
	PAGED_CODE();
	UNREFERENCED_PARAMETER(pDriverObject);
	
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	DbgPrint("[MyDrive]：驱动停止成功\n");
	return STATUS_SUCCESS;
}

// 控制回调
NTSTATUS DispatchControl(PDEVICE_OBJECT pDriverObject, PIRP pIrp)
{
	PAGED_CODE();
	UNREFERENCED_PARAMETER(pDriverObject);
	NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
	ULONG ulRet = 0;

	// 设置IRP完成状态
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;

	// 获取当前IO栈
	IO_STACK_LOCATION* pIoStack = IoGetCurrentIrpStackLocation(pIrp);

	// 获取控制码
	ULONG uCtrlCode = pIoStack->Parameters.DeviceIoControl.IoControlCode;

	// 获取输入缓冲区
	PVOID pInputBuff = pIrp->AssociatedIrp.SystemBuffer;
	PVOID pOutBuff = NULL;
	if (pIrp->MdlAddress && METHOD_FROM_CTL_CODE(uCtrlCode) & METHOD_OUT_DIRECT)
	{
		pOutBuff = MmGetSystemAddressForMdlSafe(pIrp->MdlAddress, 0);
	}
	ULONG ulBuffSize = pIoStack->Parameters.DeviceIoControl.OutputBufferLength;

	// 判断Ring3发送的控制码
	switch (uCtrlCode)
	{
		case GETGDT:
		{
			// 遍历GDT
			DbgPrint("[MyDrive]：正在SSDTGDT表\n");
			ulRet = GetGDT(pOutBuff, ulBuffSize);
			//EnumProcess(pOutBuff, ulBuffSize);
			ntStatus = STATUS_SUCCESS;
			break;
		}
		case GETPROCESS:
		{
			// 遍历进程
			DbgPrint("[MyDrive]：正在SSDT进程\n");
			ulRet = GetProcess(pOutBuff, ulBuffSize);
			ntStatus = STATUS_SUCCESS;
			break;
		}
		case READPROCESSMEMORY:
		{
			DbgPrint("[MyDrive]：正在读取内存\n");
			char szBuf[256] = { 0 }; // 保存读取的临时缓冲区
			PRWADDR rd = (PRWADDR)pOutBuff;
			if (rd != NULL)
			{
				DbgPrint("[MyDrive]：READPROCESSMEMORY dwPID: %d\n", rd->dwPID);
				DbgPrint("[MyDrive]：READPROCESSMEMORY pDestMemAddr: %p\n", rd->pDestMemAddr);
				DbgPrint("[MyDrive]：READPROCESSMEMORY pReadBuff: %p\n", rd->szRWBuff);
				DbgPrint("[MyDrive]：READPROCESSMEMORY ulReadSize: %d\n", rd->ulReadSize);
				// 读取内存
				ULONG_PTR ul = ReadProcessMemory(rd->dwPID, rd->pDestMemAddr, (PVOID)szBuf, rd->ulReadSize);
				if (ul == NULL)
				{
					DbgPrint("[MyDrive]：READPROCESSMEMORY 该内存地址无效\n");
					RtlCopyMemory(rd->szRWBuff, "0", rd->ulReadSize);
					ntStatus = STATUS_UNSUCCESSFUL;
					break;
				}
				
				DbgPrint("[MyDrive]：READPROCESSMEMORY szBuf: %s\n", szBuf);

				RtlCopyMemory(rd->szRWBuff, szBuf, rd->ulReadSize);
				DbgPrint("[MyDrive]：READPROCESSMEMORY rd->szRWBuff: %s\n", rd->szRWBuff);
			}
			ntStatus = STATUS_SUCCESS;
			break;
		}
		case WRITEPROCESSMEMORY:
		{
			// 写入内存
			DbgPrint("[MyDrive]：正在写入内存\n");
			PRWADDR rd = (PRWADDR)pOutBuff;
			if (rd != NULL)
			{
				DbgPrint("[MyDrive]：WRITEPROCESSMEMORY dwPID: %d\n", rd->dwPID);
				DbgPrint("[MyDrive]：WRITEPROCESSMEMORY pDestMemAddr: %p\n", rd->pDestMemAddr);
				DbgPrint("[MyDrive]：WRITEPROCESSMEMORY pReadBuff: %s\n", rd->szRWBuff);
				DbgPrint("[MyDrive]：WRITEPROCESSMEMORY ulReadSize: %d\n", rd->ulReadSize);
				// 写入内存
				ULONG_PTR ul = WriteProcessMemory(rd->dwPID, rd->pDestMemAddr, rd->szRWBuff, rd->ulReadSize);
				if (ul == NULL)
				{
					DbgPrint("[MyDrive]：READPROCESSMEMORY 该内存地址无效\n");
					ntStatus = STATUS_UNSUCCESSFUL;
					break;
				}
			}
			ntStatus = STATUS_SUCCESS;
			break;
		}
		case GETIDT:
		{
			DbgPrint("[MyDrive]：正在获取IDT表\n");
			ulRet = GetIDT(pOutBuff, ulBuffSize);
			if (ulRet == FALSE)
			{
				DbgPrint("[MyDrive]：获取IDT表失败\n");
				ntStatus = STATUS_UNSUCCESSFUL;
				break;
			}
			ntStatus = STATUS_SUCCESS;
			break;
		}
		case GETSSDT:
		{
			DbgPrint("[MyDrive]：正在获取SSDT表\n");
			ulRet = GetSSDT(pOutBuff, ulBuffSize);
			if (ulRet == FALSE)
			{
				DbgPrint("[MyDrive]：获取SSDT表失败\n");
				ntStatus = STATUS_UNSUCCESSFUL;
				break;
			}
			ntStatus = STATUS_SUCCESS;
			break;
		}
		case GETDRVMODULE:
		{
			DbgPrint("[MyDrive]：正在获取驱动模块\n");
			ulRet = GetDrvModule(pOutBuff, ulBuffSize, g_pDriverObj);
			ntStatus = STATUS_SUCCESS;
			break;
		}
		case GETTHREAD:
		{
			DbgPrint("[MyDrive]：正在遍历线程\n");
			ulRet = GetThreadInfo(pOutBuff, ulBuffSize, pInputBuff);
			ntStatus = STATUS_SUCCESS;
			break;
		}
		case TERMINATEPROCESS:
		{
			DbgPrint("[MyDrive]：正在遍历线程\n");
			TerminateProcess(pInputBuff);
			break;
		}
	}

	pIrp->IoStatus.Status = ntStatus;
	pIrp->IoStatus.Information = ulRet;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return ntStatus;
}