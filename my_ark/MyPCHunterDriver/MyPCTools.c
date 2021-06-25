#include "MyPCTools.h"
#include "common.h"


NTKERNELAPI CHAR* PsGetProcessImageFileName(PEPROCESS proc);

// 获取GDT表
ULONG GetGDT(PVOID pOutPut, ULONG ulBuffSize)
{
	GDTINFO SGDT1 = { 0,0,0 };
	PGDTENTRY pGDTEntry = NULL;
	PGDTENTRY pOut = (PGDTENTRY)pOutPut;
	ULONG uAddr = 0;

	// 获取GDT表地址
	__asm sgdt SGDT1;

	// 获取GDT表的数组地址
	ULONG ulNum = SGDT1.uGDTlimit / 8;
	pGDTEntry = (PGDTENTRY)(SGDT1.uGDTLowBase + (SGDT1.uGDTHighBase << 16));
	ULONG ulRetNum = 0;
	for (ULONG i = 0; i < ulNum; ++i)
	{
		// 段不存在, 不进行遍历
		if (pGDTEntry[i].P == 0)
		{
			continue;
		}
		ulRetNum++;
	}
	if (ulRetNum * sizeof(GDTENTRY) > ulBuffSize)
	{
		return ulRetNum * sizeof(GDTENTRY);
	}
	for (ULONG i = 0; i < ulRetNum; ++i)
	{
		// 段不存在, 不进行遍历
		if (pGDTEntry[i].P == 0)
		{
			continue;
		}
		RtlCopyMemory(&pOut[i], &pGDTEntry[i], sizeof(GDTENTRY));
	}
	return ulRetNum * sizeof(GDTENTRY);
}

// 获取正在运行的进程
ULONG GetProcess(PVOID pOutPut, ULONG ulBuffSize)
{
	NTSTATUS Status;
	Status = STATUS_SUCCESS;
	PPROCESS pProcessHead = ExAllocatePool(PagedPool, sizeof(PROCESS));

	// 初始化内存数据
	RtlZeroMemory(pProcessHead, sizeof(PROCESS));
	pProcessHead->Next = NULL;

	// 保存进程信息
	PPROCESS pProcTmp = pProcessHead;
	ULONG ulCount = 0;
	PEPROCESS pProc = NULL;
	for (int i = 4; i < 100000; i += 4)
	{
		// 尝试获取进程EPROCESS
		Status = PsLookupProcessByProcessId((HANDLE)i, &pProc);

		// 判断进程状态
		ULONG* pulObjectTable = (ULONG*)((ULONG)pProc + 0xf4);
		if (!NT_SUCCESS(Status) || !(*pulObjectTable)) 
		{
			continue;
		}
		__try
		{
			// 获取进程PID
			ULONG ulPid = (ULONG)PsGetProcessId(pProc);
			pProcTmp->dwPID = ulPid;
			ULONG* PPID = (ULONG*)((ULONG)pProc + 0x140);
			ULONG ulPPid = *PPID;
			pProcTmp->dwPPID = ulPPid;
			// 获取进程名并保存
			RtlCopyMemory(pProcTmp->szProcessName, PsGetProcessImageFileName(pProc), strlen(PsGetProcessImageFileName(pProc)));
			PCWSTR FILENAME = (PCWSTR)(*(ULONG*)((ULONG)pProc + 0x1ec) + 8);

			UNICODE_STRING uFILENAME;
			RtlInitUnicodeString(&uFILENAME, FILENAME);
			RtlCopyMemory(pProcTmp->szProcessPath, uFILENAME.Buffer, uFILENAME.Length);
			// 分配内存
			PPROCESS pProcessTemp = ExAllocatePool(PagedPool, sizeof(PROCESS));
			RtlZeroMemory(pProcessTemp, sizeof(PROCESS));
			pProcTmp->Next = pProcessTemp;
			pProcessTemp->Next = NULL;
			pProcTmp = pProcessTemp;
			++ulCount;
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			DbgPrint("[51asm] GetProcess(PVOID pOutPut, ULONG ulBuffSize)\n");
		}
		ObDereferenceObject(pProc);
	}

	// 传进来的字节数和所需的字节数大，以所需字节数为准
	// 反之，以参数大小为准
	ULONG OutPutSize = ulCount * sizeof(PROCESS);
	PPROCESS OutProcess = pOutPut;
	if (OutPutSize <= ulBuffSize)
	{
		pProcTmp = pProcessHead;
		for (ULONG i = 0; i < ulCount; ++i)
		{
			RtlCopyMemory(OutProcess[i].szProcessName, pProcTmp->szProcessName, 256);
			OutProcess[i].dwPID = pProcTmp->dwPID;
			OutProcess[i].dwPPID = pProcTmp->dwPPID;
			RtlCopyMemory(OutProcess[i].szProcessPath, pProcTmp->szProcessPath, 256);
			pProcTmp = pProcTmp->Next;
		}
	}

	// 释放内存
	pProcTmp = pProcessHead;
	for (ULONG i = 0; i <= ulCount; ++i)
	{
		ExFreePool(pProcTmp);
		pProcTmp = pProcTmp->Next;
	}

	// 返回输出的字节数
	return OutPutSize;
}


ULONG GetActiveprocesslinksOffset()
{
	NTSTATUS status = STATUS_SUCCESS;

	// 获取系统版本
	ULONG offset = 0;
	RTL_OSVERSIONINFOW os_info = { 0 };
	status = RtlGetVersion(&os_info);
	if (!NT_SUCCESS(status)) {
		return offset;
	}

	// 判断系统版本
	switch (os_info.dwMajorVersion) {
	case 6:
		switch (os_info.dwMinorVersion) {
		case 1: // win7
#ifdef _WIN64
			offset = 0x188;
#else
			offset = 0xb8;
#endif
			break;
		}
		break;
	case 10: // win10
#ifdef _WIN64
		offset = 0x2f0;
#else
		offset = 0xb8;
#endif
		break;
	}
	return offset;
}

PEPROCESS FindProcessById(ULONG ulPID)
{
	// 获取ActiveProcessLinks字段的偏移
	ULONG offset = GetActiveprocesslinksOffset();
	if (offset == 0) {
		return NULL;
	}

	// 遍历进程
	PEPROCESS FirstEProcess = NULL;
	PEPROCESS TraverseEProcess = NULL;
	FirstEProcess = PsGetCurrentProcess();
	TraverseEProcess = FirstEProcess;
	UINT8 found = FALSE;

	do {
		// 从EPROCESS中获取PID
		ULONG ulProcessPID = (ULONG)PsGetProcessId(TraverseEProcess);
		if (ulProcessPID == ulPID) {
			// 找到
			found = TRUE;
			break;
		}

		// 根据偏移计算下一个EPROCESS
		TraverseEProcess = (PEPROCESS)((PUCHAR)(((PLIST_ENTRY)((PUCHAR)TraverseEProcess + offset))->Flink) - offset);
	} while (TraverseEProcess != FirstEProcess);

	if (!found) {
		return NULL;
	}

	return TraverseEProcess;
}

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
	ULONG ulReadSize)
{
	// 获取指定进程的EPROCESS
	PEPROCESS pEProcess = FindProcessById(ulPID);
	
	// 保存原CR3
	PVOID pOldCR3 = 0;
	// 找到目录表基址
	PVOID pDirBase = (PVOID)(*(PUINT_PTR)((PUCHAR)pEProcess + 0x18));

	__asm 
	{
		// 屏蔽中断，禁止线程切换
		cli

		// 保存原来的cr3
		mov eax, cr3
		mov pOldCR3, eax

		// 修改cr3
		mov eax, pDirBase
		mov cr3, eax
	}

	// 检查内存是否有效
	if (MmIsAddressValid(pDestMemAddr)) 
	{
		// 计算大小，不允许跨分页
		ULONG size = ((ULONG)pDestMemAddr / PAGE_SIZE + 1) * 
			PAGE_SIZE - (ULONG)pDestMemAddr;
		ulReadSize = ulReadSize > size ? size : ulReadSize;
		RtlCopyMemory(pReadBuff, pDestMemAddr, ulReadSize);
		// RtlCopyMemory(pBuf, pAddress, nLen);
	}
	else
	{
		return NULL;
	}

	__asm 
	{
		// 还原cr3
		mov eax, pOldCR3
		mov cr3, eax

		// 恢复中断
		sti
	}

	return ulReadSize;
}

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
	ULONG ulWriteSize)
{
	// 获取指定进程的EPROCESS
	PEPROCESS pEProcess = FindProcessById(ulPID);
	// 保存原CR3
	PVOID pOldCR3 = 0;
	// 找到目录表基址
	PVOID pDirBase = (PVOID)(*(PUINT_PTR)((PUCHAR)pEProcess + 0x18));

	__asm 
	{
		// 屏蔽中断，禁止线程切换
		cli

		// 关闭内存写保护
		mov eax, cr0
		and eax, not 10000h
		mov cr0, eax

		// 保存原来的cr3
		mov eax, cr3
		mov pOldCR3, eax

		// 修改cr3
		mov eax, pDirBase
		mov cr3, eax
	}

	// 检查内存是否有效
	if (MmIsAddressValid(pDestMemAddr)) {
		// 计算大小，不允许跨分页
		ULONG size = ((ULONG)pDestMemAddr / PAGE_SIZE + 1) * 
			PAGE_SIZE - (ULONG)pDestMemAddr;
		ulWriteSize = ulWriteSize > size ? size : ulWriteSize;
		RtlCopyMemory(pDestMemAddr, pWriteBuff, ulWriteSize);
	}
	else 
	{
		// 地址无效
		return NULL;
	}

	__asm 
	{
		// 还原cr3
		mov eax, pOldCR3
		mov cr3, eax

		// 还原内存写保护
		mov eax, cr0
		or eax, 10000h
		mov cr0, eax

		// 恢复中断
		sti
	}

	return ulWriteSize;
}

// 获取IDT表
ULONG GetIDT(PVOID pOutPut, ULONG ulBuffSize)
{
	IDTINFO SIDT1 = { 0,0,0 };
	PIDTENTRY pIDTEntry = NULL;
	PIDTENTRY pOut = (PIDTENTRY)pOutPut;
	ULONG uAddr = 0;
	
	// 获取IDT表地址
	__asm sidt SIDT1;

	// 获取IDT表数组地址
	pIDTEntry = (PIDTENTRY)(SIDT1.uLowIdtBase + (SIDT1.uHighIdtBase << 16));
	if (pIDTEntry != NULL)
	{
		// 获取IDT信息
		for (ULONG i = 0; i < 0x100; ++i)
		{
			pOut[i].uOffsetLow = pIDTEntry[i].uOffsetLow; // 低地址
			pOut[i].uOffsetHigh = pIDTEntry[i].uOffsetHigh; // 高地址
			pOut[i].uSelector = pIDTEntry[i].uSelector; // 段选择子
			pOut[i].uType = pIDTEntry[i].uType; // 类型
			pOut[i].uDpl = pIDTEntry[i].uDpl; // 特权等级
		}
		return TRUE;
	}
	return FALSE;
}
// 获取SSDT表
int g_Flag = 0;
ULONG GetSSDT(PVOID pOutPut, ULONG ulBuffSize)
{
	PETHREAD pThread = PsGetCurrentThread();
	PSystemServiceDescriptorTable pSystemServiceDescriptor = (PSystemServiceDescriptorTable) * (ULONG*)((ULONG)pThread + 0xbc);
	LONG ulNum = pSystemServiceDescriptor->NumberOfService;
	if (g_Flag == 0)
	{
		//DbgPrint("[MyDrive]：%d\n", ulBuffSize);
		if (ulBuffSize < ulNum * sizeof(SSDT)) 
		{
			//DbgPrint("[MyDrive]：%d\n", ulNum * sizeof(SSDT));
			g_Flag = 1;
			return ulNum * sizeof(SSDT);
		}
	}

	PSSDT pSSDT = (PSSDT)pOutPut;
	PLONG Addr = pSystemServiceDescriptor->ServiceTableBase;
	if (Addr != NULL)
	{
		for (int i = 0; i < ulNum; ++i)
		{
			pSSDT[i].ulAddr = Addr[i];
			pSSDT[i].ulSysCallIndex = i;
		}
		g_Flag = 0;
		return ulNum * sizeof(SSDT);
	}
	return FALSE;
}

// 获取驱动模块
ULONG GetDrvModule(PVOID pOutPut, ULONG ulBuffSize, PDRIVER_OBJECT pDriver)
{
	PLDR_DATA_TABLE_ENTRY pLdr = (PLDR_DATA_TABLE_ENTRY)pDriver->DriverSection;
	PLDR_DATA_TABLE_ENTRY pBegin = pLdr;
	PDRVMODULE pDriverHead = ExAllocatePool(PagedPool, sizeof(DRVMODULE));
	if (pDriverHead == NULL)
	{
		return FALSE;
	}
	RtlZeroMemory(pDriverHead, sizeof(DRVMODULE));
	pDriverHead->Next = NULL;
	PDRVMODULE Temp = pDriverHead;
	ULONG ulCount = 0;
	__try {
		do
		{
			RtlCopyMemory(Temp->szNameBuff, pLdr->BaseDllName.Buffer, pLdr->BaseDllName.Length);
			Temp->Size = pLdr->SizeOfImage;
			Temp->DllBase = (ULONG)pLdr->DllBase;
			RtlCopyMemory(Temp->FullDllName, pLdr->FullDllName.Buffer, pLdr->FullDllName.Length);
			pLdr = (PLDR_DATA_TABLE_ENTRY)(pLdr->InLoadOrderLinks.Flink);
			PDRVMODULE pEnumDrivertemp;
			pEnumDrivertemp = ExAllocatePool(PagedPool, sizeof(DRVMODULE));
			if (pEnumDrivertemp == NULL)
			{
				return FALSE;
			}
			RtlZeroMemory(pEnumDrivertemp, sizeof(DRVMODULE));
			Temp->Next = pEnumDrivertemp;
			pEnumDrivertemp->Next = NULL;
			Temp = pEnumDrivertemp;
			ulCount++;
		} while (pBegin != pLdr);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("[MyDrive]：Error: %08x", GetExceptionCode());
	}
	ULONG OutPutSize = ulCount * sizeof(DRVMODULE);
	// 如果需要的大小比传进来的大小大，则返回需要的大小
	PDRVMODULE OutDriver = pOutPut;
	if (OutPutSize <= ulBuffSize)
	{
		Temp = pDriverHead;
		for (ULONG i = 0; i < ulCount; ++i)
		{
			RtlCopyMemory(OutDriver[i].szNameBuff, Temp->szNameBuff, 256 * 2);
			OutDriver[i].DllBase = Temp->DllBase;
			OutDriver[i].Size = Temp->Size;
			RtlCopyMemory(OutDriver[i].FullDllName, Temp->FullDllName, 256 * 2);
			Temp = Temp->Next;
		}
	}
	// 清理空间
	Temp = pDriverHead;
	for (ULONG i = 0; i <= ulCount; ++i)
	{
		ExFreePool(Temp);
		Temp = Temp->Next;
	}
	return OutPutSize;
}

// 获取线程信息
ULONG GetThreadInfo(PVOID pOutPut, ULONG ulBuffSize, PVOID pPID)
{
	PETHREAD pEThread;
	PKTHREAD pKThread;
	PLIST_ENTRY LClink;
	PLIST_ENTRY LNink;
	CLIENT_ID* ci;
	char BasePriority;
	UCHAR State;
	ULONG ThreadID = 0;
	ULONG ThreadStartAddress;
	// 赋值目标进程PID
	ULONG dwInputPID = *(ULONG*)pPID;
	NTSTATUS Status = STATUS_SUCCESS;
	PEPROCESS pProc;
	ULONG ulCount = 0;
	PTHREAD pThreadHead = ExAllocatePool(PagedPool, sizeof(THREAD));
	RtlZeroMemory(pThreadHead, sizeof(THREAD));
	pThreadHead->Next = NULL;
	PTHREAD pThreadTemp = pThreadHead;
	for (int i = 4; i < 90000; i += 4)
	{
		// 获取进程EPROCESS
		Status = PsLookupProcessByProcessId((HANDLE)i, &pProc);
		// 判断是否为死进程
		ULONG* pulObjectTable = (ULONG*)((ULONG)pProc + 0xf4);
		if (!NT_SUCCESS(Status) || !(*pulObjectTable))
		{
			continue;
		}
		ULONG* pulPID = (ULONG*)((ULONG)pProc + 0xB4);
		ULONG ulPID = *pulPID;
		// 判断PID是否相同
		if (ulPID != dwInputPID) 
		{
			continue;
		}

		// 开始遍历当前进程的线程
		LClink = (PLIST_ENTRY)((ULONG)pProc + 0x188);
		LNink = LClink->Flink;
		while (LNink != LClink)
		{
			pEThread = (PETHREAD)((ULONG)LNink - 0x268);
			pKThread = (PKTHREAD)pEThread;
			ci = (PCLIENT_ID)((ULONG)pEThread + 0x22C);
			ThreadID = (ULONG)(ci->UniqueThread);
			ThreadStartAddress = *(ULONG*)((ULONG)pEThread + 0x260);
			BasePriority = *(char*)((ULONG)pKThread + 0x57);
			State = *(UCHAR*)((ULONG)pKThread + 0x68);

			pThreadTemp->dwTID = ThreadID;
			pThreadTemp->dwStartAddress = ThreadStartAddress;
			pThreadTemp->dwBasePriority = BasePriority;
			pThreadTemp->dwStatus = State;

			PTHREAD pThreadTemp2 = ExAllocatePool(PagedPool, sizeof(THREAD));
			// memset(pThreadTemp2, 0, sizeof(THREAD));
			RtlZeroMemory(pThreadTemp2, sizeof(THREAD));
			pThreadTemp2->Next = NULL;
			pThreadTemp->Next = pThreadTemp2;
			pThreadTemp = pThreadTemp2;
			LNink = LNink->Flink;
			// 获取到这个线程数量
			ulCount++;
		}
		ObDereferenceObject(pProc);
		break;
	}
	ULONG OutPutSize = ulCount * sizeof(THREAD);

	PTHREAD OutThread = pOutPut;
	if (OutPutSize <= ulBuffSize)
	{
		pThreadTemp = pThreadHead;
		for (ULONG i = 0; i < ulCount; ++i)
		{
			// 赋值并传出
			OutThread[i].dwTID = pThreadTemp->dwTID;
			OutThread[i].dwStartAddress = pThreadTemp->dwStartAddress;
			OutThread[i].dwStatus = pThreadTemp->dwStatus;
			OutThread[i].dwBasePriority = pThreadTemp->dwBasePriority;
			pThreadTemp = pThreadTemp->Next;
		}
	}

	// 释放空间
	pThreadTemp = pThreadHead;
	for (ULONG i = 0; i <= ulCount; ++i)
	{
		ExFreePool(pThreadTemp);
		pThreadTemp = pThreadTemp->Next;
	}

	return OutPutSize;
}

void TerminateProcess(PVOID pPID)
{
	HANDLE hProcess;
	ULONG ulInputPID = *(ULONG*)pPID;
	OBJECT_ATTRIBUTES ObjectAttributes;
	CLIENT_ID ClientId;
	InitializeObjectAttributes(&ObjectAttributes, 
		0, 
		OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, 
		0, 
		0);
	
	ClientId.UniqueProcess = (HANDLE)ulInputPID;
	ClientId.UniqueThread = 0;
	ZwOpenProcess(&hProcess, PROCESS_ALL_ACCESS, &ObjectAttributes, &ClientId);
	ZwTerminateProcess(hProcess, 0);
}