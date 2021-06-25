#include "MyPCTools.h"
#include "common.h"


NTKERNELAPI CHAR* PsGetProcessImageFileName(PEPROCESS proc);

// ��ȡGDT��
ULONG GetGDT(PVOID pOutPut, ULONG ulBuffSize)
{
	GDTINFO SGDT1 = { 0,0,0 };
	PGDTENTRY pGDTEntry = NULL;
	PGDTENTRY pOut = (PGDTENTRY)pOutPut;
	ULONG uAddr = 0;

	// ��ȡGDT���ַ
	__asm sgdt SGDT1;

	// ��ȡGDT��������ַ
	ULONG ulNum = SGDT1.uGDTlimit / 8;
	pGDTEntry = (PGDTENTRY)(SGDT1.uGDTLowBase + (SGDT1.uGDTHighBase << 16));
	ULONG ulRetNum = 0;
	for (ULONG i = 0; i < ulNum; ++i)
	{
		// �β�����, �����б���
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
		// �β�����, �����б���
		if (pGDTEntry[i].P == 0)
		{
			continue;
		}
		RtlCopyMemory(&pOut[i], &pGDTEntry[i], sizeof(GDTENTRY));
	}
	return ulRetNum * sizeof(GDTENTRY);
}

// ��ȡ�������еĽ���
ULONG GetProcess(PVOID pOutPut, ULONG ulBuffSize)
{
	NTSTATUS Status;
	Status = STATUS_SUCCESS;
	PPROCESS pProcessHead = ExAllocatePool(PagedPool, sizeof(PROCESS));

	// ��ʼ���ڴ�����
	RtlZeroMemory(pProcessHead, sizeof(PROCESS));
	pProcessHead->Next = NULL;

	// ���������Ϣ
	PPROCESS pProcTmp = pProcessHead;
	ULONG ulCount = 0;
	PEPROCESS pProc = NULL;
	for (int i = 4; i < 100000; i += 4)
	{
		// ���Ի�ȡ����EPROCESS
		Status = PsLookupProcessByProcessId((HANDLE)i, &pProc);

		// �жϽ���״̬
		ULONG* pulObjectTable = (ULONG*)((ULONG)pProc + 0xf4);
		if (!NT_SUCCESS(Status) || !(*pulObjectTable)) 
		{
			continue;
		}
		__try
		{
			// ��ȡ����PID
			ULONG ulPid = (ULONG)PsGetProcessId(pProc);
			pProcTmp->dwPID = ulPid;
			ULONG* PPID = (ULONG*)((ULONG)pProc + 0x140);
			ULONG ulPPid = *PPID;
			pProcTmp->dwPPID = ulPPid;
			// ��ȡ������������
			RtlCopyMemory(pProcTmp->szProcessName, PsGetProcessImageFileName(pProc), strlen(PsGetProcessImageFileName(pProc)));
			PCWSTR FILENAME = (PCWSTR)(*(ULONG*)((ULONG)pProc + 0x1ec) + 8);

			UNICODE_STRING uFILENAME;
			RtlInitUnicodeString(&uFILENAME, FILENAME);
			RtlCopyMemory(pProcTmp->szProcessPath, uFILENAME.Buffer, uFILENAME.Length);
			// �����ڴ�
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

	// ���������ֽ�����������ֽ������������ֽ���Ϊ׼
	// ��֮���Բ�����СΪ׼
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

	// �ͷ��ڴ�
	pProcTmp = pProcessHead;
	for (ULONG i = 0; i <= ulCount; ++i)
	{
		ExFreePool(pProcTmp);
		pProcTmp = pProcTmp->Next;
	}

	// ����������ֽ���
	return OutPutSize;
}


ULONG GetActiveprocesslinksOffset()
{
	NTSTATUS status = STATUS_SUCCESS;

	// ��ȡϵͳ�汾
	ULONG offset = 0;
	RTL_OSVERSIONINFOW os_info = { 0 };
	status = RtlGetVersion(&os_info);
	if (!NT_SUCCESS(status)) {
		return offset;
	}

	// �ж�ϵͳ�汾
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
	// ��ȡActiveProcessLinks�ֶε�ƫ��
	ULONG offset = GetActiveprocesslinksOffset();
	if (offset == 0) {
		return NULL;
	}

	// ��������
	PEPROCESS FirstEProcess = NULL;
	PEPROCESS TraverseEProcess = NULL;
	FirstEProcess = PsGetCurrentProcess();
	TraverseEProcess = FirstEProcess;
	UINT8 found = FALSE;

	do {
		// ��EPROCESS�л�ȡPID
		ULONG ulProcessPID = (ULONG)PsGetProcessId(TraverseEProcess);
		if (ulProcessPID == ulPID) {
			// �ҵ�
			found = TRUE;
			break;
		}

		// ����ƫ�Ƽ�����һ��EPROCESS
		TraverseEProcess = (PEPROCESS)((PUCHAR)(((PLIST_ENTRY)((PUCHAR)TraverseEProcess + offset))->Flink) - offset);
	} while (TraverseEProcess != FirstEProcess);

	if (!found) {
		return NULL;
	}

	return TraverseEProcess;
}

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
	ULONG ulReadSize)
{
	// ��ȡָ�����̵�EPROCESS
	PEPROCESS pEProcess = FindProcessById(ulPID);
	
	// ����ԭCR3
	PVOID pOldCR3 = 0;
	// �ҵ�Ŀ¼���ַ
	PVOID pDirBase = (PVOID)(*(PUINT_PTR)((PUCHAR)pEProcess + 0x18));

	__asm 
	{
		// �����жϣ���ֹ�߳��л�
		cli

		// ����ԭ����cr3
		mov eax, cr3
		mov pOldCR3, eax

		// �޸�cr3
		mov eax, pDirBase
		mov cr3, eax
	}

	// ����ڴ��Ƿ���Ч
	if (MmIsAddressValid(pDestMemAddr)) 
	{
		// �����С����������ҳ
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
		// ��ԭcr3
		mov eax, pOldCR3
		mov cr3, eax

		// �ָ��ж�
		sti
	}

	return ulReadSize;
}

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
	ULONG ulWriteSize)
{
	// ��ȡָ�����̵�EPROCESS
	PEPROCESS pEProcess = FindProcessById(ulPID);
	// ����ԭCR3
	PVOID pOldCR3 = 0;
	// �ҵ�Ŀ¼���ַ
	PVOID pDirBase = (PVOID)(*(PUINT_PTR)((PUCHAR)pEProcess + 0x18));

	__asm 
	{
		// �����жϣ���ֹ�߳��л�
		cli

		// �ر��ڴ�д����
		mov eax, cr0
		and eax, not 10000h
		mov cr0, eax

		// ����ԭ����cr3
		mov eax, cr3
		mov pOldCR3, eax

		// �޸�cr3
		mov eax, pDirBase
		mov cr3, eax
	}

	// ����ڴ��Ƿ���Ч
	if (MmIsAddressValid(pDestMemAddr)) {
		// �����С����������ҳ
		ULONG size = ((ULONG)pDestMemAddr / PAGE_SIZE + 1) * 
			PAGE_SIZE - (ULONG)pDestMemAddr;
		ulWriteSize = ulWriteSize > size ? size : ulWriteSize;
		RtlCopyMemory(pDestMemAddr, pWriteBuff, ulWriteSize);
	}
	else 
	{
		// ��ַ��Ч
		return NULL;
	}

	__asm 
	{
		// ��ԭcr3
		mov eax, pOldCR3
		mov cr3, eax

		// ��ԭ�ڴ�д����
		mov eax, cr0
		or eax, 10000h
		mov cr0, eax

		// �ָ��ж�
		sti
	}

	return ulWriteSize;
}

// ��ȡIDT��
ULONG GetIDT(PVOID pOutPut, ULONG ulBuffSize)
{
	IDTINFO SIDT1 = { 0,0,0 };
	PIDTENTRY pIDTEntry = NULL;
	PIDTENTRY pOut = (PIDTENTRY)pOutPut;
	ULONG uAddr = 0;
	
	// ��ȡIDT���ַ
	__asm sidt SIDT1;

	// ��ȡIDT�������ַ
	pIDTEntry = (PIDTENTRY)(SIDT1.uLowIdtBase + (SIDT1.uHighIdtBase << 16));
	if (pIDTEntry != NULL)
	{
		// ��ȡIDT��Ϣ
		for (ULONG i = 0; i < 0x100; ++i)
		{
			pOut[i].uOffsetLow = pIDTEntry[i].uOffsetLow; // �͵�ַ
			pOut[i].uOffsetHigh = pIDTEntry[i].uOffsetHigh; // �ߵ�ַ
			pOut[i].uSelector = pIDTEntry[i].uSelector; // ��ѡ����
			pOut[i].uType = pIDTEntry[i].uType; // ����
			pOut[i].uDpl = pIDTEntry[i].uDpl; // ��Ȩ�ȼ�
		}
		return TRUE;
	}
	return FALSE;
}
// ��ȡSSDT��
int g_Flag = 0;
ULONG GetSSDT(PVOID pOutPut, ULONG ulBuffSize)
{
	PETHREAD pThread = PsGetCurrentThread();
	PSystemServiceDescriptorTable pSystemServiceDescriptor = (PSystemServiceDescriptorTable) * (ULONG*)((ULONG)pThread + 0xbc);
	LONG ulNum = pSystemServiceDescriptor->NumberOfService;
	if (g_Flag == 0)
	{
		//DbgPrint("[MyDrive]��%d\n", ulBuffSize);
		if (ulBuffSize < ulNum * sizeof(SSDT)) 
		{
			//DbgPrint("[MyDrive]��%d\n", ulNum * sizeof(SSDT));
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

// ��ȡ����ģ��
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
		DbgPrint("[MyDrive]��Error: %08x", GetExceptionCode());
	}
	ULONG OutPutSize = ulCount * sizeof(DRVMODULE);
	// �����Ҫ�Ĵ�С�ȴ������Ĵ�С���򷵻���Ҫ�Ĵ�С
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
	// ����ռ�
	Temp = pDriverHead;
	for (ULONG i = 0; i <= ulCount; ++i)
	{
		ExFreePool(Temp);
		Temp = Temp->Next;
	}
	return OutPutSize;
}

// ��ȡ�߳���Ϣ
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
	// ��ֵĿ�����PID
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
		// ��ȡ����EPROCESS
		Status = PsLookupProcessByProcessId((HANDLE)i, &pProc);
		// �ж��Ƿ�Ϊ������
		ULONG* pulObjectTable = (ULONG*)((ULONG)pProc + 0xf4);
		if (!NT_SUCCESS(Status) || !(*pulObjectTable))
		{
			continue;
		}
		ULONG* pulPID = (ULONG*)((ULONG)pProc + 0xB4);
		ULONG ulPID = *pulPID;
		// �ж�PID�Ƿ���ͬ
		if (ulPID != dwInputPID) 
		{
			continue;
		}

		// ��ʼ������ǰ���̵��߳�
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
			// ��ȡ������߳�����
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
			// ��ֵ������
			OutThread[i].dwTID = pThreadTemp->dwTID;
			OutThread[i].dwStartAddress = pThreadTemp->dwStartAddress;
			OutThread[i].dwStatus = pThreadTemp->dwStatus;
			OutThread[i].dwBasePriority = pThreadTemp->dwBasePriority;
			pThreadTemp = pThreadTemp->Next;
		}
	}

	// �ͷſռ�
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