.386
.model flat, stdcall
option casemap:none

include windows.inc
include user32.inc
include kernel32.inc
include msvcrt.inc
include dasm.inc

includelib user32.lib
includelib kernel32.lib
includelib msvcrt.lib
includelib dasm.lib

.const
MY_MSG				db	"Test", 0dh, 0ah, 0
MY_OPEN_PROCESS			db	"TestDebug.exe", 0	; 
CREATEPROCESS_ERROR		db	"CreateProcess Error，", 0
CREATEPROCESS_OK		db	"[Tips]：CreateProcess OK", 0dh, 0ah, 0
WAITFORDEBUGEVENT_ERROR	db	"WaitForDebugEvent Error", 0
ERROR_CODES			db 	"[Tips]：%s，%08x", 0dh, 0ah, 0
OPENPROCESS_ERROR	db	"OpenProcess Error", 0
OPENTHREAD_ERROR	db	"OpenThread Error", 0
GETTHREADCONTEXT_ERROR	db	"GetThreadContext Error", 0
SETTHREADCONTEXT_ERROR	db	"SetThreadContext Error", 0
READPROCESSMEMORY_ERROR	db	"ReadProcessMemory Error", 0
WRITEPROCESSMEMORY_ERROR	db	"WriteProcessMemory Error", 0
VIRTUALPROTECTEX_ERROR		db	"VirtualProtectEx Error", 0
MYREADPROCESSMEMORY_ERROR	db	"MyReadProcessMemory Error", 0
CREATEFILE_ERROR		db	"CreateFile Error", 0
WRITEFILE_ERROR			db	"WriteFile Error", 0
READFILE_ERROR			db	"ReadFile Error", 0
MYVIRTUALPROTECTEX_ERROR	db	"MyVirtualProtectEx Error", 0
MYWRITEPROCESSMEMORY_ERROR	db	"MyWriteProcessMemory Error", 0

; 显示字符串
DISPLAY_DISASSEMBLY_CODE	db	"%08x", 09h, "%s", 0dh, 0ah, 0
SZBUFF_FMT			db	"%s", 0dh, 0ah, 0
SZBUFF_FMT1			db	"%d", 0dh, 0ah, 0
SZBUFF_NO_RN_FMT		db	"%s", 0	; 没有回车换行
CREATE_THREAD_FMT		db	"[CREATE_THREAD_DEBUG_EVENT]：hThread：%p，lpStartAddress：%p，lpThreadLocalBase：%p", 0dh, 0ah, 0
CREATE_PROCESS_FMT		db	"[CREATE_PROCESS_DEBUG_EVENT]：hProcess：%p，lpBaseOfImage：%p，lpStartAddress：%p", 0dh, 0ah, 0
EXIT_THREAD_FMT			db	"[EXIT_THREAD_DEBUG_EVENT]：dwExitCode：%p", 0dh, 0ah, 0
EXIT_PROCESS_FMT		db	"[EXIT_PROCESS_DEBUG_EVENT]：dwExitCode：%p", 0dh, 0ah, 0
LOAD_DLL_FMT			db	"基址：%p，路径：%s", 0dh, 0ah, 0	;[LOAD_DLL_DEBUG_EVENT]：
UNLOAD_DLL_FMT			db	"[UNLOAD_DLL_DEBUG_EVENT]：lpBaseOfDll：%p", 0dh, 0ah, 0
OUTPUT_DEBUG_STRING_FMT	db 	"[OUTPUT_DEBUG_STRING_EVENT]：", 0dh, 0ah, 0
EXCEPTION_DEBUG_FMT		db	"[EXCEPTION_DEBUG_EVENT]：ExceptionCode：%p，ExceptionAddress：%p", 0dh, 0ah, 0
SHOW_REGISTER_FMT1		db	"EAX = %p EBX = %p ECX = %p EDX = %p EIP = %p", 0dh, 0ah, 0
SHOW_REGISTER_FMT2		db	"ESI = %p EDI = %p EBP = %p ESP = %p EFLAGS = %p", 0dh, 0ah, 0
SHOW_REGISTER_FLAGS_FMT3	db	"ZF = %2x PF = %2x AF = %2x OF = %2x SF = %2x DF = %2x CF = %2x TF = %2x IF = %2x", 0dh, 0ah, 0
SHOW_HELP	db	"[?] -- 帮助", 0dh, 0ah
			db	"[q] -- 退出调试", 0dh, 0ah
			db	"[r] -- 显示寄存器的值", 0dh, 0ah
			db	"[tt] -- 单步步入", 0dh, 0ah
			db	"[p] -- 单步步过", 0dh, 0ah
			db	"[u/u 地址] -- 显示汇编", 0dh, 0ah
			db	"[gg/gg 地址] -- 运行", 0dh, 0ah
			db	"[bp 地址] -- 软件断点", 0dh, 0ah
			db	"[bm 地址] -- 内存断点", 0dh, 0ah
			db	"[bh 地址] -- 硬件断点", 0dh, 0ah
			db	"[dd 地址] -- 显示内存数据", 0dh, 0ah
			db	"[ml] -- 显示模块列表", 0dh, 0ah
			db	"[es] -- 导出脚本", 0dh, 0ah
			db	"[ls] -- 导入脚本", 0dh, 0ah
			db	"[gr] -- 执行到返回（call指令有效）", 0dh, 0ah
			db	"[er 要修改的寄存器对应的名称 修改的值] -- 修改寄存器值", 0dh, 0ah
			db	"[em 地址 修改后的值] -- 修改指定内存上对应的值", 0dh, 0ah
			db	"[track 起始地址 终止地址] -- 根据起始地址和目标地址进行追踪（指令记录）", 0dh, 0ah
			db	"[dump 文件名] -- 内存 dump PE到文件", 0dh, 0ah, 0
														
REGISTER_EAX	db	"EAX", 0
REGISTER_EBX	db	"EBX", 0
REGISTER_ECX	db	"ECX", 0
REGISTER_EDX	db	"EDX", 0
REGISTER_EIP	db	"EIP", 0
REGISTER_ESI	db	"ESI", 0
REGISTER_EDI	db	"EDI", 0
REGISTER_EBP	db	"EBP", 0
REGISTER_ESP	db	"ESP", 0
REGISTER_EFLAGS	db	"EFLAGS", 0

TIPS_DEBUG_HELPS	db	"[Tips]：输入 <?> 查看调试器命令帮助说明", 0dh, 0ah
			db	"[Tips]：第一次输入 <g> 暂停到被调试程序的入口位置", 0dh, 0ah, 0

INPUT_COMMAND		db	"[cmd]：", 0	; 提示输入命令调试程序
DIVISION		db 	" ", 0			; 分割字符串
SRTTOK_IMPORT_SCRIPT	db	'0dh', 0	; 导入脚本分割		
COMMAND_SCRIPT	db	"script.txt", 0	; 保存输入的命令（脚本功能）
MODULE_FILE	db	"Module_File.txt", 0	; 保存模块信息
INPUT_COMMAND_FMT	db	"%s", 0dh, 0ah, 0

; 显示内存数据相关
SHOW_HEX_MEMORY_DATA	db	"%.2x ", 0	; 显示十六进制数据
SHOW_HEX_DIVISION	db	"- ", 0		; 显示十六进制数据时的分割符
SHOW_HEX_SZBUFF_S_FMT	db	"%s", 0
SHOW_HEX_SZBUFF_P_FMT	db	"%p ", 0
SHOW_HEX_SZBUFF_C_FMT	db	"%c", 0
SHOW_HEX_SZBUFF_POINT_FMT	db	".", 0

; 提示字符串
SCRIPT_EXPORT_OK		db	"[Tips]：脚本导出成功", 0
SCRIPT_IMPORT_OK		db	"[Tips]：脚本导入成功", 0
SCRIPT_IMPORT_TIPS		db	"[Tips]：脚本命令 --> [%s]", 0dh, 0ah, 0
MEMORY_DUMP_OK_TIPS		db	"[Tips]：内存dump成功", 0
MEMORY_DUMP_ERROR_TIPS		db	"[Tips]：内存dump失败", 0
DUMP_ERROR				db	"[Tips]：当前拷贝的节区数据过大，请检缓冲区的大小", 0
EDIT_MEMORY_DATA_OK_TIPS	db	"[Tips]：内存修改成功", 0
EDIT_MEMORY_DATA_ERROR_TIPS	db	"[Tips]：内存修改失败", 0
EDIT_INPUT_MEMORY_ERROR		db	"[Tips]：修改后的内存数据输入格式错误，请输入一个十六进制的内存数据（2个字符）", 0
EDIT_INPUT_REGISTER_NAME_ERR	db	"[Tips]：输入要修改寄存器的名称错误", 0

EXECUTION_TO_RETURN_OK		db	"[Tips]：执行到返回地址保存成功，运行<gg>命令即可运行到返回地址", 0
EXECUTION_TO_RETURN_ERROR	db	"[Tips]：不满足执行到返回地址，当前需要在执行call指令后才可", 0
TRACK_END_ADDR_NULL		db	"[Tips]：跟踪的结束地址为空", 0
TRACK_OK			db	"[Tips]：跟踪完成，数据已保存到 %p-%p.txt 中", 0dh, 0ah, 0
TRACK_FILE_NAME			db	"%p-%p.txt", 0

; debug 命令
COMMAND_DISPLAY_ASSEMBLY	db	"u", 0	; 显示汇编
COMMAND_RUN			db	"gg", 0	; 运行
COMMAND_STEP			db	"tt", 0	; 单步执行一条指令就显示运行结果（单步步入）
COMMAND_P			db	"p", 0		; 单步步过
COMMAND_BREAKPOINT		db	"bp", 0	; 断点
COMMAND_QUIT			db	"q", 0	; 退出调试
COMMAND_SHOW_REGISTER		db	"r", 0	; 显示寄存器的值
COMMAND_MEMORY_BREAKPOINT	db	"bm", 0	; 内存断点
COMMAND_HARDWARE_BREAKPOINT	db	"bh", 0	; 硬件断点
COMMAND_HELP			db	"?", 0	; 帮助
COMMAND_MODULE_LIST		db	"ml", 0	; 查看模块列表
COMMAND_EXPORT_SCRIPT		db	"es", 0	; 导出脚本
COMMAND_IMPORT_SCRIPT		db	"ls", 0	; 导入脚本
COMMAND_MEMORY_DUMP		db	"dump", 0	; 内存 dump PE到文件
COMMAND_DISPLAY_MEMORY_DATA	db	"dd", 0	; 显示内存数据
COMMAND_EDIT_MEMORY_DATA	db	"em", 0	; 修改指定内存上对应的值
COMMADN_EDIT_REGISTER_VALUE	db	"er", 0	; 修改执行寄存器的值
COMMAND_GO_RETURN		db	"gr", 0		; 执行到返回
COMMAND_TRACK			db	"track", 0	; 跟踪（指令记录）


; 硬件断点标志
MY_DR7 struct
	L0 byte ?
	G0 byte ?
	L1 byte ?
	G1 byte ?
	L2 byte ?
	G2 byte ?
	L3 byte ?
	G3 byte ?
	_LE byte ?
	_GE byte ?
	RE1 byte ?
	RTM byte ?
	RE2 byte ?
	GD byte ?
	RE3 WORD ?
	RW0 WORD ?
	LEN0 WORD ?
	RW1 WORD ?
	LEN1 WORD ?
	RW2 WORD ?
	LEN2 WORD ?
	RW3 WORD ?
	LEN3 WORD ?
MY_DR7 ends

; 标志寄存器
FLAGS_REGISTER struct
	CF_ DWORD ?
	PF_ DWORD ?
	AF_ DWORD ?
	ZF_ DWORD ?
	SF_ DWORD ?
	TF_ DWORD ?
	IF_ DWORD ?
	DF_ DWORD ?
	OF_ DWORD ?
FLAGS_REGISTER ends 

; 被调试进程的信息
DEBUG_PROCESS_INFO struct
	hModule HANDLE ?	; 模块句柄
	dwModuleSize DWORD ?	; 模块大小
DEBUG_PROCESS_INFO ends

.data?
;pDR7	MY_DR7<>
.data
g_DebugEv		DEBUG_EVENT {0}
g_context		CONTEXT {0}
g_IsSystemBreakPoint	db	FALSE
g_IsEip			db	FALSE
g_OldAddresss	dd	NULL
g_SetBreakPoint	db	0cch	; 设置单步断点
g_BreakPointOldCode		db	NULL	; 保存下断点前目标程序旧的代码，便于还原代码
g_BreakPointAddress		dd	NULL	; 保存下断点的地址
g_IsBpStep		db	FALSE	; 是否是断点单步
g_IsOldCode		db	FALSE	; 是否一次性断点
g_IsNormalStep	db	FALSE	; 正常单步标志（t）
g_IsMemorStep	db	FALSE	; 是否是内存断点单步
g_IsMemorOnlyOne	dd	FALSE	; 是否是一次性内存断点
g_BMOldProtected	dd	NULL	; 保存旧的内存保护属性
g_BMAddress			dd	NULL	; 保存旧的内存断点地址
g_BMBytes		dd	NULL	; 保存访问的内存字节数
g_IsModule		db	TRUE	; 目标进程加载的模块
g_hFile			dd	NULL	; 保存创建的文件句柄
g_IsUserInput	db	TRUE	; 默认是用户输入的命令
g_IsFirstGo		db	TRUE	; 默认第一次按 "g" 命令
g_lpBaseOfImage	dd	NULL	; 被调试进程的基地址
g_CanDump		db	TRUE	; 是否可以进行内存dump
g_CurrentExceptionAddr		dd	NULL	; 当前产生异常的地址
g_CallNextAdd				dd	NULL	; 保存执行单步步过遇到call指令后的下一条汇编指令的地址
g_IsSaveCmdPCallNextStart	db	FALSE	; 是否保存call指令后的下一条汇编指令的地址
g_CurrentShowMemoryAddr		dd	NULL	; 保存当前显示的内存地址
g_Division_n	db	0dh					; 分割换行字符
g_IsEditRegisterData	db	FALSE	; 是否修改了寄存器数据
g_IsSaveGoReturnAddr	db	FALSE	; 是否保存执行到返回的地址
g_dwGoReturnAddr		dd	NULL	; 保存执行到返回的地址
g_IsAutoTrack			db	FALSE	; 是否开启自动跟踪
g_IsTrack				db	FALSE	; 是否开启跟踪
g_dwTrackBeginAddr		dd	NULL	; 保存跟踪的起始地址
g_dwTrackEndAddr		dd	NULL	; 保存跟踪的终止地址
g_IsSaveTrackAsm		db	FALSE	; 是否保存跟踪时反汇编代码

g_Falgs	FLAGS_REGISTER<1, 1, 1, 1, 1, 1, 1, 1, 1>
g_ModuleInfo		byte	4096	dup (?)	; 保存加载的模块信息
g_ImportCmdScript	byte	1024	dup (?)	; 保存导出的脚本命令
g_ExportCmdScript	byte	1024	dup (?)	; 保存导入的脚本命令
g_szSectionsBuff	byte	200000	dup (?)	; 存储节表数据
g_MemoryData		byte	4096	dup	(?)	; 用于存放代码的缓冲区
g_ShowASM		byte	4096	dup	(?)	; 保存跟踪的反汇编代码
g_SaveSoftwareBP	byte	1024	dup	(?)	; 保存软件断点

.code
;---------------------------------------
; 函数功能：将输入的命令保存到文件中
; 参数1：pBuff -- 指向缓冲区的指针
; 参数2：pFileName -- 执行保存的文件名指针
;---------------------------------------
WRITEFILE proc pBuff:PVOID, pFileName:PVOID
	LOCAL @hFile:HANDLE	; 保存创建的文件句柄
	LOCAL @BytesWrite:byte	; 保存写入的字节数
	LOCAL @BuffLen:dword	; 缓冲区长度
	; 创建文件
	invoke CreateFile, pFileName, \
		GENERIC_WRITE, FILE_SHARE_WRITE, \
		NULL, CREATE_ALWAYS, \
		FILE_ATTRIBUTE_NORMAL, NULL
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset CREATEFILE_ERROR, eax
		ret
	.endif
	mov g_hFile, eax
	
	; 移动文件指针
	invoke SetFilePointer, g_hFile, NULL, NULL, FILE_END

	invoke crt_strlen, pBuff
	mov @BuffLen, eax
	; 写入数据到文件中
	invoke	WriteFile, g_hFile, pBuff, @BuffLen, addr @BytesWrite, NULL
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset WRITEFILE_ERROR, eax
		ret
	.endif
	
	; 关闭文件
	invoke CloseHandle, g_hFile
	mov @hFile, NULL
	ret
WRITEFILE endp

;---------------------------------------
; 函数功能：读取文件数据并显示
; 参数：无
;---------------------------------------
READFILE proc pszFileName:PVOID
	LOCAL @hFile:HANDLE	; 保存创建的文件句柄
	LOCAL @dwFileSize:dword	; 目标文件大小
	LOCAL @szBuff[1024]:byte
	LOCAL @szBuff1[1024]:byte
	LOCAL @RSzie:dword	; 保存写入的字节数
	LOCAL @BuffLen:dword	; 缓冲区长度
	; 打开文件
	invoke CreateFile, pszFileName, \
		GENERIC_READ,	FILE_SHARE_READ,	\
		NULL,	OPEN_EXISTING,	\
		FILE_ATTRIBUTE_NORMAL, NULL
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset CREATEFILE_ERROR, eax
		ret
	.endif
	mov @hFile, eax
	
	invoke GetFileSize, @hFile, NULL
	mov @dwFileSize, eax
	
	; 读取文件数据
	invoke ReadFile, @hFile, addr @szBuff, @dwFileSize, addr @RSzie, NULL
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset READFILE_ERROR, eax
		ret
	.endif
	invoke crt_memcpy, offset g_ExportCmdScript, addr @szBuff, @dwFileSize
	invoke crt_printf, offset g_ExportCmdScript
	
	; 关闭文件
	invoke CloseHandle, @hFile
	ret
READFILE endp

;-----------------------------------
; 函数功能：解析汇编指令
; 参数1：解析汇编的地址
; 参数2：显示的汇编代码行数
;-----------------------------------
DisAsm proc uses ebx ecx dwAddress:dword, dwDisline:dword
	LOCAL @hThread:HANDLE	; 保存打开线程的句柄
	LOCAL @hProcess:HANDLE	; 保存打开进程的句柄
	LOCAL @context:CONTEXT	; 保存寄存器数据的结构体
	LOCAL @szCodeBuff[1024]:byte	; 保存读取的内存数据
	LOCAL @szAsmBuff[1024]:byte	; 保存解析后的反汇编的代码
	LOCAL @eipAddress:dword		; 保存当前EIP的值
	LOCAL @dwReadBytes:dword	; 保存读取的字节数
	LOCAL @dwPrintLine:dword	; 保存显示的反汇编行数
	LOCAL @dwCode:dword				
	LOCAL @dwAsmLength:dword	; 反汇编代码字节长度
	LOCAL @szBuff[256]:byte	; 临时缓冲区，保存拼接后的反汇编代码
	
	; 初始化缓冲区
	invoke crt_memset, addr @szCodeBuff, 0, sizeof @szCodeBuff
	invoke crt_memset, addr @szAsmBuff, 0, sizeof @szAsmBuff
	invoke crt_memset, addr @szBuff, 0, sizeof @szBuff
	
	; 获取寄存器环境
	invoke OpenThread, THREAD_ALL_ACCESS, FALSE, g_DebugEv.dwThreadId
	.if eax == INVALID_HANDLE_VALUE
		xor eax, eax
		ret
	.endif
	mov @hThread, eax
	
		invoke OpenProcess, THREAD_ALL_ACCESS, FALSE, g_DebugEv.dwProcessId
		.if eax == INVALID_HANDLE_VALUE
			invoke CloseHandle,@hThread
		xor eax, eax
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset OPENPROCESS_ERROR, eax
		ret
	.endif
	mov @hProcess, eax
		
	; 获取全部寄存器
	lea ebx, @context
	assume ebx:ptr CONTEXT
	mov [ebx].ContextFlags, CONTEXT_FULL
	invoke GetThreadContext, @hThread, addr @context
	.if eax == 0
	invoke GetLastError
	invoke crt_printf, offset ERROR_CODES, offset GETTHREADCONTEXT_ERROR, eax
	jmp DISASM_END
	.endif
	
	; 修改程序的EIP
	.if g_IsEip == TRUE
		mov eax, @context.regEip
		mov dwAddress, eax
	.else
		.if dwAddress == NULL
			.if g_OldAddresss != NULL
				mov eax, g_OldAddresss
				mov dwAddress, eax
			.else
				mov eax, @context.regEip
				mov dwAddress, eax
			.endif
		.else
			mov eax, 0
			mov g_OldAddresss, eax
		.endif
	.endif
	
	; 读取进程的二进制代码
	invoke ReadProcessMemory,@hProcess, dwAddress, addr @szCodeBuff, sizeof @szCodeBuff, addr @dwReadBytes
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset READPROCESSMEMORY_ERROR, eax
		jmp DISASM_END
	.endif
	
	lea eax, @szCodeBuff
	mov @dwCode, eax
	
	; 读取的反汇编指令行数
	mov ecx, dwDisline			
	mov @dwPrintLine, ecx
	
	; 循环读取反汇编指令
	.while ecx > 0
		invoke Decode2Asm, @dwCode, addr @szAsmBuff, addr @dwAsmLength, dwAddress
		; 将反汇编出来的代码转换为大写
		invoke crt__strupr, addr @szAsmBuff
		invoke crt_printf, offset DISPLAY_DISASSEMBLY_CODE, dwAddress, addr @szAsmBuff
		
		; 判断是否开始保存跟踪的汇编代码
		.if g_IsSaveTrackAsm == TRUE
			; 保存反汇编代码到缓冲区
			invoke wsprintf, addr @szBuff, offset DISPLAY_DISASSEMBLY_CODE, dwAddress, addr @szAsmBuff
			invoke crt_strcat, offset g_ShowASM, addr @szBuff
		.endif
		; 保存call指令后的下一条汇编指令的地址
		.if g_IsSaveCmdPCallNextStart == TRUE
			mov eax, dwAddress
			mov g_CallNextAdd, eax
			mov g_dwGoReturnAddr, eax	; 保存执行到返回的地址
		.endif
		mov ebx, @dwAsmLength
		add @dwCode, ebx
		add dwAddress, ebx
		dec @dwPrintLine
		mov ecx, @dwPrintLine
	.endw
	mov g_IsSaveCmdPCallNextStart, FALSE
	
	; 保存上次的地址
	mov eax, dwAddress
	mov g_OldAddresss, eax
DISASM_END:
	; 关闭句柄
	invoke CloseHandle,@hThread		
	invoke CloseHandle,@hProcess
	
	ret
DisAsm endp

;-----------------------------------
; 函数功能：执行单步功能（t）
; 参数：无
;-----------------------------------
SetStep proc uses ebx
	LOCAL @hThread:HANDLE	; 线程句柄
	LOCAL @context:CONTEXT
	
	; 初始化结构体
	invoke RtlZeroMemory,addr @context,sizeof @context
	
	invoke OpenThread,THREAD_ALL_ACCESS, FALSE, g_DebugEv.dwThreadId
	.if eax == NULL
		xor eax, eax
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset OPENTHREAD_ERROR, eax
		ret
	.endif
	mov @hThread, eax
	 	
	lea ebx, @context
	assume ebx:ptr CONTEXT
	mov [ebx].ContextFlags, CONTEXT_FULL
	; 获取旧的寄存器环境
	invoke GetThreadContext, @hThread,addr @context
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset GETTHREADCONTEXT_ERROR, eax
		jmp SETSTEP_END
	.endif
	
	; 修改新的寄存器环境
	or @context.regFlag, 0100h
	
	; 设置线程上线文环境
	invoke SetThreadContext, @hThread, addr @context
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset SETTHREADCONTEXT_ERROR, eax
	.endif
	
SETSTEP_END:
	invoke CloseHandle, @hThread
	; 正常设置单步，修改标志
	mov g_IsNormalStep, TRUE
	ret
SetStep endp

;-----------------------------------
; 函数功能：设置断点
; 参数：无
;-----------------------------------
SetBreakPoint proc dwAddress:dword
	; 写入int3
	LOCAL @hProcess:HANDLE
	
	; 打开进程
	invoke OpenProcess,THREAD_ALL_ACCESS, FALSE, g_DebugEv.dwProcessId
	.if eax == NULL
		xor eax, eax
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset OPENPROCESS_ERROR, eax
		ret
	.endif
	mov @hProcess, eax
	
	; 保存旧的代码
	invoke ReadProcessMemory, @hProcess, dwAddress, offset g_BreakPointOldCode, sizeof g_BreakPointOldCode, NULL
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset READPROCESSMEMORY_ERROR, eax
		jmp SETBREAKPOINT_END
	.endif
	
	; 修改代码，在入口地址处写入int3
	invoke WriteProcessMemory, @hProcess, dwAddress, offset g_SetBreakPoint, sizeof g_SetBreakPoint, NULL
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset WRITEPROCESSMEMORY_ERROR, eax
		jmp SETBREAKPOINT_END
	.endif
	
SETBREAKPOINT_END:
	invoke CloseHandle,@hProcess
	; 保存下断点的地址
	mov eax, dwAddress
	mov g_BreakPointAddress, eax
	
	mov g_IsOldCode, TRUE
	ret
SetBreakPoint endp

;-----------------------------------
; 函数功能：显示寄存器的值
; 参数：无
;-----------------------------------
ShowRegister proc uses ebx ecx
	LOCAL @hThread:HANDLE
	;LOCAL @context:CONTEXT
	LOCAL @szBuff1[256]:byte
	LOCAL @szBuff2[256]:byte
	LOCAL @szBuff3[256]:byte
	
	invoke OpenThread,THREAD_ALL_ACCESS, FALSE, g_DebugEv.dwThreadId
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset OPENTHREAD_ERROR, eax
		ret
	.endif
	mov @hThread, eax

	assume ebx:ptr CONTEXT
	lea ebx, g_context
	mov [ebx].ContextFlags, CONTEXT_FULL

	; 是否修改了寄存器数据，修改了就不获取旧的寄存器环境
	.if g_IsEditRegisterData == FALSE
		; 获取旧的寄存器环境
		invoke GetThreadContext, @hThread,addr g_context
		.if eax == NULL
			invoke GetLastError
			invoke crt_printf, offset ERROR_CODES, offset GETTHREADCONTEXT_ERROR, eax
		.endif
	.endif
	 
	;invoke MessageBox, NULL, NULL, NULL, NULL
	invoke wsprintf, addr @szBuff1, offset SHOW_REGISTER_FMT1, [ebx].regEax, [ebx].regEbx, [ebx].regEcx, [ebx].regEdx, [ebx].regEip
	invoke crt_printf, addr @szBuff1
	
	invoke wsprintf, addr @szBuff2, offset SHOW_REGISTER_FMT2, [ebx].regEsi, [ebx].regEdi, [ebx].regEbp, [ebx].regEsp, [ebx].regFlag
	invoke crt_printf, addr @szBuff2
	
	;mov ecx, [ebx].regFlag
	;mov dword ptr g_Falgs, ecx
	;invoke wsprintf, addr @szBuff3, offset SHOW_REGISTER_FLAGS_FMT3, g_Falgs.ZF_, g_Falgs.PF_, g_Falgs.AF_, g_Falgs.OF_, g_Falgs.SF_, g_Falgs.DF_, g_Falgs.CF_, g_Falgs.TF_, g_Falgs.IF_
	;invoke crt_printf, addr @szBuff3
	
	invoke CloseHandle, @hThread
	ret
ShowRegister endp

;-----------------------------------
; 函数功能：显示调试器命令帮助说明
; 参数：无
;-----------------------------------
ShowHelp proc
	invoke crt_printf, offset SHOW_HELP
	ret
ShowHelp endp

;-----------------------------------
; 函数功能：设置内存断点	
; 参数1：功能类型（读取、写入等）
; 参数2：内存断点开始的地址
; 参数3：操作的字节数
; 参数4：是否一次性断点
;-----------------------------------
SetMemoryBreakPoint proc dwType:dword, dwAddress:dword, dwBytes:dword, IsOnlyOne:dword
	; 读取、写入、执行、访问（支持一次性，重复断点（需要断点、单步相互配合））
	; dwType == 1（写入），dwType == 2（执行）
	
	LOCAL @hProcess:HANDLE
	.if dwType == 1
		; 目标进程内存写入断点
		; 修改目标进程的内存保护属性，使其产生内存访问异常，这里为一次性断点
		invoke OpenProcess, PROCESS_ALL_ACCESS, FALSE, g_DebugEv.dwProcessId
		.if eax == NULL
			invoke GetLastError
			invoke crt_printf, offset ERROR_CODES, offset OPENTHREAD_ERROR, eax
			ret
		.endif
		mov @hProcess, eax
		invoke VirtualProtectEx, @hProcess, dwAddress, dwBytes, PAGE_EXECUTE_READ, offset g_BMOldProtected
		.if eax == NULL
			invoke GetLastError
			invoke crt_printf, offset ERROR_CODES, offset VIRTUALPROTECTEX_ERROR, eax
		.endif
		mov eax, dwAddress
		mov g_BMAddress, eax
		mov eax, dwBytes
		mov g_BMBytes, eax
		invoke CloseHandle, @hProcess
	.elseif dwType == 2
		; dwType == 2（执行）
		; 目标进程内存执行断点
		invoke OpenProcess, PROCESS_ALL_ACCESS, FALSE, g_DebugEv.dwProcessId
		.if eax == NULL
			invoke GetLastError
			invoke crt_printf, offset ERROR_CODES, offset OPENTHREAD_ERROR, eax
			ret
		.endif
		mov @hProcess, eax
		invoke VirtualProtectEx, @hProcess, dwAddress, dwBytes, PAGE_NOACCESS, offset g_BMOldProtected
		.if eax == NULL
			invoke GetLastError
			invoke crt_printf, offset ERROR_CODES, offset VIRTUALPROTECTEX_ERROR, eax
		.endif
		mov eax, dwAddress
		mov g_BMAddress, eax
		mov eax, dwBytes
		mov g_BMBytes, eax
		invoke CloseHandle, @hProcess
		ret
	.endif
	
	; 保存设置断点的属性（是否是一次性）
	mov eax, IsOnlyOne
		mov g_IsMemorOnlyOne, eax
	
	ret
SetMemoryBreakPoint endp

;-----------------------------------
; 函数功能：设置硬件断点	
; 参数1：目标程序中设置断点的地址
;-----------------------------------
SetHardwareBreakPoint proc uses ebx dwAddress:dword
	LOCAL @hThread:HANDLE
	LOCAL @context:CONTEXT
	LOCAL @pDR7:MY_DR7
	
	invoke OpenThread, THREAD_ALL_ACCESS, FALSE, g_DebugEv.dwThreadId
	 .if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset OPENTHREAD_ERROR, eax
		ret
	.endif
	mov @hThread, eax
	
	; 获取旧的寄存器环境
	invoke GetThreadContext, @hThread, addr @context
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset GETTHREADCONTEXT_ERROR, eax
	.endif
	
	assume ebx:ptr CONTEXT
	lea ebx, @context
	mov [ebx].ContextFlags, CONTEXT_DEBUG_REGISTERS
	
	; 设置断点地址
	mov eax, dwAddress
	mov [ebx].iDr0, eax
	
	mov eax, @context.iDr7
	mov dword ptr @pDR7, eax
	
	; 硬件内存写入断点
	; 设置标志位
	mov @pDR7.L0, 1
	mov @pDR7.G0, 1
	mov @pDR7.RW0, 0
	mov @pDR7.LEN0, 0
	
	; 保存标志位
	invoke SetThreadContext, @hThread, addr @context
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset SETTHREADCONTEXT_ERROR, eax
	.endif
	
	; 关闭句柄
	invoke CloseHandle, @hThread
	ret
SetHardwareBreakPoint endp

GetCommand proto pszCmdScript:PVOID
;-----------------------------------
; 函数功能：导入命令脚本	
; 参数：无
;-----------------------------------
OnCmdImportScript proc uses ebx ecx esi edi edx
	LOCAL @BuffLen:dword	; 保存导入脚本的字节总长度
	LOCAL @ReadBuffLen:dword	; 保存读取的字符长度
	LOCAL @TotalReadBuffSize:dword	; 保存读取的总字节数
	LOCAL @CopySize:dword	; 拷贝字节数
	LOCAL @szBuff[256]:byte	; 临时缓冲区
	LOCAL @szBuff1[256]:byte	; 临时缓冲区
	LOCAL @szBuff2[256]:byte	; 临时缓冲区
	LOCAL @szCmd:dword		; 保存输入的命令
	LOCAL @szParam:dword	; 保存命令后的参数
	
	invoke crt_strcpy, addr @szBuff, offset g_ExportCmdScript
	
	; 获取缓冲区的字节长度
	invoke crt_strlen, addr @szBuff
	mov @BuffLen, eax
	
	; 分割字符串
	xor eax, eax
	xor ebx, ebx
	xor ecx, ecx
	mov @ReadBuffLen, 0
	mov @TotalReadBuffSize, 0
	.while @BuffLen != NULL
		lea ebx, @szBuff
		lea esi, [ebx+ecx]
		mov ax, word ptr [esi]
		.if ax == 0a0dh
			;invoke wsprintf, addr @szBuff1, offset SZBUFF_FMT, [esi]
			pushad	; 保存寄存器环境
			
			lea ebx, @szBuff
			mov edi, @TotalReadBuffSize
			lea esi, [ebx+edi]
			sub ecx, @TotalReadBuffSize
			mov @CopySize, ecx
			
			invoke crt_memcpy, addr @szBuff1, esi, @CopySize
			invoke crt_strlen, addr @szBuff1
			
			; 读取的字节数
			add eax, 2
			mov @ReadBuffLen, eax
			add @TotalReadBuffSize, eax
			
			; 退出循环的条件
			mov eax, @BuffLen
			sub eax, @ReadBuffLen
			mov @BuffLen, eax
			
			; 修改标志
			mov g_IsUserInput, FALSE
			
			; 输出提示
			invoke wsprintf, addr @szBuff2, offset SCRIPT_IMPORT_TIPS, addr @szBuff1
			invoke crt_printf, addr @szBuff2
			
			invoke GetCommand, addr @szBuff1
			mov g_IsUserInput, TRUE
			;invoke crt_printf, addr @szBuff1
			; 格式化字符串
			invoke crt_memset, addr @szBuff1, 0, sizeof @szBuff1
			; 输出读取命令脚本的字节数
			;invoke wsprintf, addr @szBuff2, offset SZBUFF_FMT1, @TotalReadBuffSize
			;invoke crt_printf, addr @szBuff2
			;invoke crt_memset, addr @szBuff2, 0, sizeof @szBuff2 
			popad	; 恢复寄存器环境
		.endif
		inc ecx
		mov eax, @TotalReadBuffSize
	.endw
	
	; 提示脚本导入成功
	invoke wsprintf, addr @szBuff2, offset SZBUFF_FMT, offset SCRIPT_IMPORT_OK
	invoke crt_printf, addr @szBuff2
	ret

OnCmdImportScript endp

;-----------------------------------------------------
; 函数功能：读取进程内存
; 参数：
;	dwProcessPid -- 进程ID
;	lpBaseAddress -- 基地址
;	lpBuffer -- 缓冲区
;	dwReadSize -- 读取字节数
;-----------------------------------------------------
MyReadProcessMemory proc dwProcessPid:dword, lpBaseAddress:PVOID, lpBuffer:PVOID, dwReadSize:dword
	LOCAL	@RetValue:dword	; 返回值
	LOCAL @hProcess:HANDLE	; 保存进程句柄
	
	; 初始化返回值
	mov @RetValue, FALSE
	
	; 打开进程
	invoke OpenProcess, PROCESS_ALL_ACCESS, FALSE, dwProcessPid
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset OPENPROCESS_ERROR, eax
		ret
	.endif
	mov @hProcess, eax
	
	; 读取进程内存
	.if @hProcess != INVALID_HANDLE_VALUE
		invoke ReadProcessMemory, @hProcess, lpBaseAddress, lpBuffer, dwReadSize, NULL
		.if eax !=	NULL
			; 进程句柄有效且内存数据读取成功
			invoke CloseHandle, @hProcess
			mov @RetValue, TRUE
		.else
			; 进程句柄有效且内存数据读取失败
			mov @hProcess, NULL
			invoke CloseHandle, @hProcess
			mov @RetValue, FALSE
			invoke GetLastError
			invoke crt_printf, offset ERROR_CODES, offset READPROCESSMEMORY_ERROR, eax
		.endif
	.endif
	
	mov eax, @RetValue
	ret
MyReadProcessMemory endp

;-----------------------------------------------------
; 函数功能：内存 dump PE到文件
; 参数：pInputCmd -- 指向输入的指令字符串的指针
;-----------------------------------------------------
OnCmdMemoryDump proc uses ebx edi pInputCmd:PVOID
	LOCAL @szInputCmd[256]:byte		; 保存输入的指令
	LOCAL @szDumpFileName[32]:byte	; 保存内存dump后的文件名
	LOCAL @dwInputCmdLen:dword		; 保存导入脚本的字节总长度
	LOCAL @TipsBuff[256]:byte		; 输出提示信息的缓冲区
	LOCAL @DebugProcessPath[MAX_PATH]:byte	; 保存被调试进程的绝对路径
	LOCAL @ImageDosHeader:IMAGE_DOS_HEADER	; DOS头
	LOCAL @ImageNtHeaders:IMAGE_NT_HEADERS	; NT头
	LOCAL @dwNTFileOffset:dword	; NT头文件偏移
	LOCAL @hFile:HANDLE		; 保存创建的文件句柄
	LOCAL @WriteBytes:dword	; 保存写入的字节数
	LOCAL @SectionTablePointer:dword	; 保存第一个节表的位置
	LOCAL @NumberOfSections:dword	; 保存节表的数量
	LOCAL @ImageSectionHeader:IMAGE_SECTION_HEADER	; 保存节表信息的结构体
	LOCAL @dwBaseOfImage:dword	; 保存被调试进程的基地址
	;LOCAL @szSectionsBuff[10000]:byte	; 保存节数据的缓冲区
	LOCAL @SectionsDataPointer:dword	; 保存节数据的地址
	
	; 初始化结构体
	invoke RtlZeroMemory, addr @ImageDosHeader, sizeof @ImageDosHeader
	invoke RtlZeroMemory, addr @ImageNtHeaders, sizeof @ImageNtHeaders
	
	; 初始化缓冲区
	invoke crt_memset, addr @szDumpFileName, 0, sizeof @szDumpFileName
	invoke crt_memset, addr @szInputCmd, 0, sizeof @szInputCmd
	invoke crt_memset, addr @TipsBuff, 0, sizeof @TipsBuff
	
	invoke crt_strcpy, addr @szInputCmd, pInputCmd
	
	; 获取缓冲区的字节长度
	invoke crt_strlen, addr @szInputCmd
	mov @dwInputCmdLen, eax
	
	.if @dwInputCmdLen == NULL
		invoke wsprintf, addr @TipsBuff, offset SZBUFF_FMT, offset MEMORY_DUMP_ERROR_TIPS
		invoke crt_printf, addr @TipsBuff
		ret
	.endif
	
	; 获取文件名
	pushad
	xor ebx, ebx
	xor esi, esi
	xor edx, edx
	
	lea ebx, @szInputCmd
	lea esi, [ebx+5]
	mov edx, @dwInputCmdLen
	sub edx, 5
	
	invoke crt_memcpy, addr @szDumpFileName, esi, edx
	invoke wsprintf, addr @TipsBuff, offset SZBUFF_FMT, addr @szDumpFileName
	;invoke crt_printf, addr @TipsBuff
	popad
	
	mov eax, eax
	; 读取DOS头
	invoke MyReadProcessMemory, g_DebugEv.dwProcessId, g_lpBaseOfImage, addr @ImageDosHeader, sizeof IMAGE_DOS_HEADER
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset MYREADPROCESSMEMORY_ERROR, eax
		ret
	.endif
	
	; 读取NT头
	mov eax, g_lpBaseOfImage
	mov ebx, @ImageDosHeader.e_lfanew
	add eax, ebx
	mov @dwNTFileOffset, eax
	invoke MyReadProcessMemory, g_DebugEv.dwProcessId, @dwNTFileOffset, addr @ImageNtHeaders, sizeof IMAGE_NT_HEADERS
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset MYREADPROCESSMEMORY_ERROR, eax
		ret
	.endif
	
	; 创建文件
	invoke CreateFile, addr @szDumpFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset CREATEFILE_ERROR, eax
		ret
	.endif
	
	; 保存文件句柄
	mov @hFile, eax
	
	; 写入DOS头数据
	mov @WriteBytes, 0
	invoke WriteFile, @hFile, addr @ImageDosHeader, sizeof IMAGE_DOS_HEADER, addr @WriteBytes, NULL
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset WRITEFILE_ERROR, eax
		; 关闭文件句柄
		invoke CloseHandle, @hFile
		ret
	.endif
	
	; 写入NT头数据前移动文件指针
	invoke SetFilePointer, @hFile, @ImageDosHeader.e_lfanew, NULL, FILE_BEGIN
	; 写入NT头数据
	mov @WriteBytes, 0
	invoke WriteFile, @hFile, addr @ImageNtHeaders, sizeof IMAGE_NT_HEADERS, addr @WriteBytes, NULL
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset WRITEFILE_ERROR, eax
		ret
	.endif
	
	; 定位节表的位置
	pushad
	mov eax, @ImageDosHeader.e_lfanew
	add eax, sizeof dword
	add eax, sizeof IMAGE_FILE_HEADER
	xor ebx, ebx
	mov bx, @ImageNtHeaders.FileHeader.SizeOfOptionalHeader
	add eax, ebx
	mov @SectionTablePointer, eax
	popad
	
	; 获取节表数量
	xor eax, eax
	mov ax, @ImageNtHeaders.FileHeader.NumberOfSections
	mov @NumberOfSections, eax
	; 拷贝节表和节数据
	.while @NumberOfSections	!= NULL
		; 初始化保存节表信息的结构体
		invoke RtlZeroMemory, addr @ImageSectionHeader, sizeof @ImageSectionHeader
		
		; 读取节表
		mov eax, g_lpBaseOfImage
		mov ebx, @SectionTablePointer
		add eax, ebx
		mov @dwBaseOfImage, eax
		invoke MyReadProcessMemory, g_DebugEv.dwProcessId, @dwBaseOfImage, addr @ImageSectionHeader, sizeof IMAGE_SECTION_HEADER
		.if eax == NULL
			invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset MYREADPROCESSMEMORY_ERROR, eax
			ret
		.endif
		
		; 移动文件指针
		invoke SetFilePointer, @hFile, @SectionTablePointer, NULL, FILE_BEGIN
		; 写入节表
		mov @WriteBytes, 0
		invoke WriteFile, @hFile, addr @ImageSectionHeader, sizeof IMAGE_SECTION_HEADER, addr @WriteBytes, NULL
		.if eax == NULL
			invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset WRITEFILE_ERROR, eax
			ret
		.endif
		
		; 判断是否是未初始化区
		.if @ImageSectionHeader.SizeOfRawData == NULL
			.continue
		.endif
		
		; 申请空间
		;invoke VirtualAlloc, addr @lpSectionsBuff, @ImageSectionHeader.SizeOfRawData, MEM_COMMIT, PAGE_EXECUTE_READWRITE
		; 读取节数据 -- 缓冲区小
		;invoke crt_memset, addr @szSectionsBuff, 0, sizeof @szSectionsBuff
		
		; 初始化缓冲区
		invoke crt_memset, offset g_szSectionsBuff, 0, sizeof g_szSectionsBuff
		; 判断节区的大小，过大进行提示
		mov eax, sizeof g_szSectionsBuff
		.if @ImageSectionHeader.SizeOfRawData > eax
			invoke wsprintf, addr @TipsBuff, offset SZBUFF_FMT, offset DUMP_ERROR
			invoke crt_printf, addr @TipsBuff
			ret
		.endif
		
		mov eax, g_lpBaseOfImage
		mov ebx, @ImageSectionHeader.VirtualAddress
		add eax, ebx
		mov @SectionsDataPointer, eax
	
		invoke MyReadProcessMemory, g_DebugEv.dwProcessId, @SectionsDataPointer, offset g_szSectionsBuff, @ImageSectionHeader.SizeOfRawData
		.if eax == NULL
			invoke GetLastError
			invoke crt_printf, offset ERROR_CODES, offset MYREADPROCESSMEMORY_ERROR, eax
			ret
		.endif
		
		; 移动文件指针
		invoke SetFilePointer, @hFile, @ImageSectionHeader.PointerToRawData, NULL, FILE_BEGIN
		
		; 写入节数据
		mov @WriteBytes, 0
		invoke WriteFile, @hFile, offset g_szSectionsBuff, @ImageSectionHeader.SizeOfRawData, addr @WriteBytes, NULL
		.if eax == NULL
			invoke GetLastError
			invoke crt_printf, offset ERROR_CODES, offset WRITEFILE_ERROR, eax
			ret
		.endif
		
		; 循环条件
		dec @NumberOfSections
		mov eax, sizeof IMAGE_SECTION_HEADER
		add eax, @SectionTablePointer
		mov @SectionTablePointer, eax
	.endw
	
	invoke wsprintf, addr @TipsBuff, offset SZBUFF_FMT, offset MEMORY_DUMP_OK_TIPS
	invoke crt_printf, addr @TipsBuff
	; 关闭文件句柄
	invoke CloseHandle, @hFile
	ret

OnCmdMemoryDump endp

;----------------------------------------------------------
; 函数功能：单步步过
; 参数：pszCmdScript -- 指向导入脚本命令的字符串指针
;----------------------------------------------------------
OnCmdRealizetionP proc
	LOCAL @Codes:byte
	LOCAL @Flag:dword
	
	; 初始化缓冲区
	invoke crt_memset, offset g_MemoryData, 0, sizeof g_MemoryData
	; 读取内存中的数据
	invoke MyReadProcessMemory, g_DebugEv.dwProcessId, g_CurrentExceptionAddr, offset g_MemoryData, sizeof g_MemoryData
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset MYREADPROCESSMEMORY_ERROR, eax
		ret
	.endif
	; 检查当前汇编代码行是否存在 call 指令的操作码
	xor eax, eax
	mov al, byte ptr g_MemoryData
	; 保存操作码
	mov @Codes, al
	; 判断操作码是否是call指令的
	.if @Codes == 0e8h || @Codes == 0ffh || @Codes == 09ah
		; 计算下一条指令的位置
		mov g_IsSaveCmdPCallNextStart, TRUE
		mov g_IsEip, TRUE
		invoke DisAsm,	g_CurrentExceptionAddr, 2
		mov eax, g_CallNextAdd
		
		mov @Flag, TRUE
		mov eax, @Flag
		ret
	.else
		; 设置单步
		mov @Flag, FALSE
		mov eax, @Flag
		ret
	.endif
OnCmdRealizetionP endp

;----------------------------------------------------------
; 函数功能：显示十六进制的内存数据
; 参数：
;	lpShowMemoryDataAddr -- 显示内存数据的起始内存地址
;	pBuff -- 指向数据内存缓冲区的指针
;	dwSize -- 显示的内存字节数
;	dwLine -- 显示内存数据的行数
;----------------------------------------------------------
ShowHexMemoryData proc uses eax ebx ecx lpShowMemoryDataAddr:PVOID, pBuff:PVOID, dwSize:dword, dwLine:dword
	LOCAL @dwPoint:dword	; 移动点
	LOCAL @dwRemainingBytes:dword	; 剩余字节数
	LOCAL @dwShowBytesSize:dword	; 显示的字节数
	LOCAL @dwShowBytesLine:dword	; 显示的行数
	LOCAL @szBuff[32]:byte	; 保存待输出的内存数据
	
	; 初始化
	invoke crt_memset, addr @szBuff, 0, sizeof @szBuff
	mov @dwPoint, 0
	mov @dwRemainingBytes, 0
	mov @dwShowBytesSize, 0
	mov @dwShowBytesLine, 0
	
	; 判断要显示的内存数据字节数是否大于16
	.if dwSize >= 16
		mov @dwShowBytesSize, 16
	.else
		mov eax, dwSize
		mov @dwShowBytesSize, eax
	.endif
	
	; 输出对应行数的内存数据
	mov eax, dwSize
	mov ebx, dwLine
	.while @dwPoint < eax && @dwShowBytesLine < ebx
		
		; 显示异常地址
		invoke wsprintf, addr @szBuff, offset SHOW_HEX_SZBUFF_P_FMT, lpShowMemoryDataAddr
		invoke crt__strupr, addr @szBuff	; 转换成大写
		invoke crt_printf, addr @szBuff
		
		; 显示十六进制数据
		xor ebx, ebx
		.while ebx < @dwShowBytesSize
			.if ebx == 8
				; 显示分隔符 "-"
				invoke wsprintf, addr @szBuff, offset SHOW_HEX_SZBUFF_S_FMT, offset SHOW_HEX_DIVISION
				invoke crt__strupr, addr @szBuff	; 转换成大写
				invoke crt_printf, addr @szBuff
			.endif
			; 显示数据
			mov eax, @dwPoint
			mov ecx, eax
			add ecx, ebx
			mov al, byte ptr g_MemoryData[ecx]
			invoke wsprintf, addr @szBuff, offset SHOW_HEX_MEMORY_DATA, al
			invoke crt__strupr, addr @szBuff	; 转换成大写
			invoke crt_printf, addr @szBuff
			inc ebx
		.endw
		
		; 显示对应的ASCII
		xor ebx, ebx
		.while ebx < @dwShowBytesSize
			; 将一定范围的ASCII进行显示
			mov eax, @dwPoint
			mov ecx, eax
			add ecx, ebx
			mov al, byte ptr g_MemoryData[ecx]
			.if al >= 32 && al <= 128
				invoke wsprintf, addr @szBuff, offset SHOW_HEX_SZBUFF_C_FMT, al
				invoke crt_printf, addr @szBuff
			.else
				; 不在该分为的显示 "."
				invoke wsprintf, addr @szBuff, offset SHOW_HEX_SZBUFF_S_FMT, offset SHOW_HEX_SZBUFF_POINT_FMT
				invoke crt_printf, addr @szBuff
			.endif
			inc ebx
		.endw
		
		; 回车换行
		invoke wsprintf, addr @szBuff, offset SZBUFF_FMT, NULL
		invoke crt_printf, addr @szBuff
		
		; 移动点移动到未显示的内存数据中
		mov eax, @dwShowBytesSize
		add @dwPoint, eax
		
		; 计算剩余字节数
		mov eax, dwSize
		sub eax, @dwPoint
		mov @dwRemainingBytes, eax
		
		; 检查剩余数据的大小是否大于16
		.if @dwRemainingBytes >= 16
			mov @dwShowBytesSize, 16
		.else
			mov eax, @dwRemainingBytes
			mov @dwShowBytesSize, eax
		.endif
		
		; 异常地址++
		add lpShowMemoryDataAddr, 16
		
		; 行数++
		inc @dwShowBytesLine
		mov ebx, dwLine
	.endw
	ret
ShowHexMemoryData endp

;----------------------------------------------------------
; 函数功能：显示内存数据
; 参数：pszCmdScript -- 指向导入脚本命令的字符串指针
;----------------------------------------------------------
OnCmdDisplayMemoryData proc pShowMemortDataAddr:PVOID
	LOCAL @dwShowMemoryData:dword	; 显示的内存起始地址
	
	; 判断显示地址内存数据的地址是否为空
	.if pShowMemortDataAddr == NULL
		; 为空，根据异常地址读取数据并按照指定的格式进行显示
		mov eax, g_CurrentExceptionAddr
		mov @dwShowMemoryData, eax
	.else
		; 不为空，按照参数地址进行显示内存数据
		mov eax, pShowMemortDataAddr
		mov @dwShowMemoryData, eax
	.endif
	
	; 初始化缓冲区
	invoke crt_memset, offset g_MemoryData, 0, sizeof g_MemoryData
	; 读取内存中的数据
	invoke MyReadProcessMemory, g_DebugEv.dwProcessId, @dwShowMemoryData, offset g_MemoryData, sizeof g_MemoryData
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset MYREADPROCESSMEMORY_ERROR, eax
		ret
	.endif
	
	; 按照指定的格式进行显示（16进制显示）
	invoke ShowHexMemoryData, @dwShowMemoryData, offset g_MemoryData, sizeof g_MemoryData, 5
	
	; 异常地址加上显示的内存字节数
	add g_CurrentExceptionAddr, 50h
	ret

OnCmdDisplayMemoryData endp

;-----------------------------------------------------
; 函数功能：修改内存保存属性
; 参数：
;	dwProcessPid -- 进程ID
;	pEditAddr -- 修改的内存地址
;	dwSize -- 修改的字节数
;	flNewProtect -- 期望的访问保护属性
;-----------------------------------------------------
MyVirtualProtectEx proc dwProcessPid:dword, pEditAddr:PVOID, dwSize:dword, flNewProtect:dword
	LOCAL	@RetValue:dword	; 返回值
	LOCAL @hProcess:HANDLE	; 保存进程句柄
	LOCAL @dwOld:dword	; 旧的保护属性
	
	; 初始化返回值
	mov @RetValue, FALSE
	
	; 打开进程
	invoke OpenProcess, PROCESS_ALL_ACCESS, FALSE, dwProcessPid
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset OPENPROCESS_ERROR, eax
		ret
	.endif
	mov @hProcess, eax
	
	; 读取进程内存
	.if @hProcess != INVALID_HANDLE_VALUE
		invoke VirtualProtectEx, @hProcess, pEditAddr, dwSize, flNewProtect, addr @dwOld
		.if eax !=	NULL
			; 进程句柄有效且内存数据读取成功
			invoke CloseHandle, @hProcess
			mov @RetValue, TRUE
		.else
			; 进程句柄有效且内存数据读取失败
			mov @hProcess, NULL
			invoke CloseHandle, @hProcess
			mov @RetValue, FALSE
			invoke GetLastError
			invoke crt_printf, offset ERROR_CODES, offset VIRTUALPROTECTEX_ERROR, eax
		.endif
	.endif
	
	mov eax, @RetValue
	ret
MyVirtualProtectEx endp


;-----------------------------------------------------
; 函数功能：写入数据到进程的内存中
; 参数：
;	dwProcessPid -- 进程ID
;	pEditAddr -- 修改的内存地址
;	pEditValueAddr -- 修改的数据地址
;	dwEditValueSize -- 写入字节数据的字节数
;-----------------------------------------------------
MyWriteProcessMemory proc dwProcessPid:dword, pEditAddr:PVOID, pEditValueAddr:PVOID, dwEditValueSize:dword
	LOCAL	@RetValue:dword	; 返回值
	LOCAL @hProcess:HANDLE	; 保存进程句柄
	
	; 初始化返回值
	mov @RetValue, FALSE
	
	; 打开进程
	invoke OpenProcess, PROCESS_ALL_ACCESS, FALSE, dwProcessPid
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset OPENPROCESS_ERROR, eax
		ret
	.endif
	mov @hProcess, eax
	
	; 读取进程内存
	.if @hProcess != INVALID_HANDLE_VALUE
		invoke WriteProcessMemory, @hProcess, pEditAddr, pEditValueAddr, dwEditValueSize, NULL
		.if eax !=	NULL
			; 进程句柄有效且内存数据读取成功
			invoke CloseHandle, @hProcess
			mov @RetValue, TRUE
		.else
			; 进程句柄有效且内存数据写入失败
			mov @hProcess, NULL
			invoke CloseHandle, @hProcess
			mov @RetValue, FALSE
			invoke GetLastError
			invoke crt_printf, offset ERROR_CODES, offset WRITEPROCESSMEMORY_ERROR, eax
		.endif
	.endif
	
	mov eax, @RetValue
	ret
MyWriteProcessMemory endp

;----------------------------------------------------------
; 函数功能：获取命令并执行
; 参数：
;	pEditAddr -- 修改的内存地址
;	bEditValue -- 修改的值
;----------------------------------------------------------
OnCmdEditMemoryData proc pEditAddr:PVOID, bEditValue:byte
	LOCAL @bEditValue:byte	; 保存修改的值
	LOCAL @szBuff[256]:byte	; 临时缓冲区
	mov al, bEditValue
	mov @bEditValue, al
	; 修改内存保护属性
	invoke MyVirtualProtectEx, g_DebugEv.dwProcessId, pEditAddr,1, PAGE_EXECUTE_READWRITE
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset MYVIRTUALPROTECTEX_ERROR, eax
		ret
	.endif
	
	; 写入内存数据
	invoke MyWriteProcessMemory, g_DebugEv.dwProcessId, pEditAddr, addr @bEditValue, sizeof @bEditValue
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset MYWRITEPROCESSMEMORY_ERROR, eax
		invoke wsprintf, addr @szBuff, offset SZBUFF_FMT, offset EDIT_MEMORY_DATA_ERROR_TIPS
	 	invoke crt_printf, addr @szBuff
		ret
	 .else
	 	invoke wsprintf, addr @szBuff, offset SZBUFF_FMT, offset EDIT_MEMORY_DATA_OK_TIPS
	 	invoke crt_printf, addr @szBuff	
	.endif
	
	ret
OnCmdEditMemoryData endp


;----------------------------------------------------------
; 函数功能：获取命令并执行
; 参数：
;	pEditAddr -- 修改的内存地址
;	bEditValue -- 修改的值
;----------------------------------------------------------
OnCmdEditRegisterData proc pEditRegisterValName:PVOID, dwEditRegisterValue:dword
	; 判断要修改的寄存器名是否有效
	LOCAL	@TipsBuff[64]:byte	; 保存提示信息
	
	assume ebx:ptr CONTEXT
	lea ebx, g_context
	mov [ebx].ContextFlags, CONTEXT_FULL

	invoke crt_strcmp, offset REGISTER_EAX, pEditRegisterValName
	.if eax == 0
		; 相等
		mov g_IsEditRegisterData, TRUE
		mov eax, dwEditRegisterValue
		mov [ebx].regEax, eax
		ret
	.endif
	invoke crt_strcmp, offset REGISTER_EBX, pEditRegisterValName
	.if eax == 0
		; 相等
		mov g_IsEditRegisterData, TRUE
		mov eax, dwEditRegisterValue
		mov [ebx].regEbx, eax
		ret
	.endif
	invoke crt_strcmp, offset REGISTER_ECX, pEditRegisterValName
	.if eax == 0
		; 相等
		mov g_IsEditRegisterData, TRUE
		mov eax, dwEditRegisterValue
		mov [ebx].regEcx, eax
		ret
	.endif
	invoke crt_strcmp, offset REGISTER_EDX, pEditRegisterValName
	.if eax == 0
		; 相等
		mov g_IsEditRegisterData, TRUE
		mov eax, dwEditRegisterValue
		mov [ebx].regEdx, eax
		ret
	.endif
	invoke crt_strcmp, offset REGISTER_EIP, pEditRegisterValName
	.if eax == 0
		; 相等
		mov g_IsEditRegisterData, TRUE
		mov eax, dwEditRegisterValue
		mov [ebx].regEip, eax
		ret
	.endif
	invoke crt_strcmp, offset REGISTER_ESI, pEditRegisterValName
	.if eax == 0
		; 相等
		mov g_IsEditRegisterData, TRUE
		mov eax, dwEditRegisterValue
		mov [ebx].regEsi, eax
		ret
	.endif
	invoke crt_strcmp, offset REGISTER_EDI, pEditRegisterValName
	.if eax == 0
		; 相等
		mov g_IsEditRegisterData, TRUE
		mov eax, dwEditRegisterValue
		mov [ebx].regEdi, eax
		ret
	.endif
	invoke crt_strcmp, offset REGISTER_EBP, pEditRegisterValName
	.if eax == 0
		; 相等
		mov g_IsEditRegisterData, TRUE
		mov eax, dwEditRegisterValue
		mov [ebx].regEbp, eax
		ret
	.endif
	invoke crt_strcmp, offset REGISTER_ESP, pEditRegisterValName
	.if eax == 0
		; 相等
		mov g_IsEditRegisterData, TRUE
		mov eax, dwEditRegisterValue
		mov [ebx].regEsp, eax
		ret
	.endif
	invoke crt_strcmp, offset REGISTER_EFLAGS, pEditRegisterValName
	.if eax == 0
		; 相等
		mov g_IsEditRegisterData, TRUE
		mov eax, dwEditRegisterValue
		mov [ebx].regFlag, eax
		ret
	.endif
	
	; 修改的寄存器名称不匹配，进行提示
	invoke wsprintf, addr @TipsBuff, offset SZBUFF_FMT, offset EDIT_INPUT_REGISTER_NAME_ERR
	invoke crt_printf, addr @TipsBuff	
	ret
OnCmdEditRegisterData endp

;----------------------------------------------------------
; 函数功能：获取命令并执行
; 参数：
;		dwTrackBeginAddr -- 保存跟踪的起始地址
;		dwTrackEndAddr -- 保存跟踪的终止地址
;----------------------------------------------------------
OnCmdTrack proc dwTrackBeginAddr:dword, dwTrackEndAddr:dword
	; 保存起始、终止地址
	mov eax, dwTrackBeginAddr
	mov g_dwTrackBeginAddr, eax
	
	mov eax, dwTrackEndAddr
	mov g_dwTrackEndAddr, eax
	
	; 开启跟踪
	mov g_IsAutoTrack, TRUE
	mov g_IsTrack, TRUE
	
	; 设置断点
	;invoke SetBreakPoint, dwTrackBeginAddr
	invoke SetBreakPoint, dwTrackEndAddr
	ret
OnCmdTrack endp

;-----------------------------------
; 函数功能：设置内存断点	
; 参数：
;	dwType -- 功能类型（读取、写入等）
;	dwAddress -- 内存断点开始的地址
;	dwBytes -- 操作的字节数
;	IsOnlyOne -- 是否一次性断点
;-----------------------------------
OnCmdBM proc dwType:dword, dwAddress:dword, dwBytes:dword, IsOnlyOne:dword
	
	; dwType == 1（写入）
	LOCAL @hProcess:HANDLE
	.if dwType == 1
		; 目标进程内存写入断点
		; 修改目标进程的内存保护属性，使其产生内存访问异常，这里为一次性断点
		invoke OpenProcess, PROCESS_ALL_ACCESS, FALSE, g_DebugEv.dwProcessId
		.if eax == NULL
			invoke GetLastError
			invoke crt_printf, offset ERROR_CODES, offset OPENTHREAD_ERROR, eax
			ret
		.endif
		mov @hProcess, eax
		invoke VirtualProtectEx, @hProcess, dwAddress, dwBytes, PAGE_EXECUTE_READ, offset g_BMOldProtected
		.if eax == NULL
			invoke GetLastError
			invoke crt_printf, offset ERROR_CODES, offset VIRTUALPROTECTEX_ERROR, eax
		.endif
		mov eax, dwAddress
		mov g_BMAddress, eax
		mov eax, dwBytes
		mov g_BMBytes, eax
		invoke CloseHandle, @hProcess
	.elseif dwType == 2
		; dwType == 2（执行）
		; 目标进程内存执行断点
		invoke OpenProcess, PROCESS_ALL_ACCESS, FALSE, g_DebugEv.dwProcessId
		.if eax == NULL
			invoke GetLastError
			invoke crt_printf, offset ERROR_CODES, offset OPENTHREAD_ERROR, eax
			ret
		.endif
		mov @hProcess, eax
		invoke VirtualProtectEx, @hProcess, dwAddress, dwBytes, PAGE_NOACCESS, offset g_BMOldProtected
		.if eax == NULL
			invoke GetLastError
			invoke crt_printf, offset ERROR_CODES, offset VIRTUALPROTECTEX_ERROR, eax
		.endif
		mov eax, dwAddress
		mov g_BMAddress, eax
		mov eax, dwBytes
		mov g_BMBytes, eax
		invoke CloseHandle, @hProcess
		ret
	.endif
	
	; 保存设置断点的属性（是否是一次性）
	mov eax, IsOnlyOne
	mov g_IsMemorOnlyOne, eax
	
	ret
OnCmdBM endp

;-----------------------------------
; 函数功能：设置硬件断点	
; 参数：
;	dwAddress -- 目标程序中设置断点的地址
;-----------------------------------
OnCmdBH proc proc uses ebx dwAddress:dword
	LOCAL @hThread:HANDLE
	LOCAL @context:CONTEXT
	LOCAL @pDR7:MY_DR7
	
	invoke OpenThread, THREAD_ALL_ACCESS, FALSE, g_DebugEv.dwThreadId
	 .if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset OPENTHREAD_ERROR, eax
		ret
	.endif
	mov @hThread, eax
	
	; 获取旧的寄存器环境
	invoke GetThreadContext, @hThread, addr @context
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset GETTHREADCONTEXT_ERROR, eax
	.endif
	
	assume ebx:ptr CONTEXT
	lea ebx, @context
	mov [ebx].ContextFlags, CONTEXT_DEBUG_REGISTERS
	
	; 设置断点地址
	mov eax, dwAddress
	mov [ebx].iDr0, eax
	
	mov eax, @context.iDr7
	mov dword ptr @pDR7, eax
	
	; 硬件内存写入断点
	; 设置标志位
	mov @pDR7.L0, 1
	mov @pDR7.G0, 1
	mov @pDR7.RW0, 0
	mov @pDR7.LEN0, 0
	
	; 保存标志位
	invoke SetThreadContext, @hThread, addr @context
	.if eax == NULL
		invoke GetLastError
			invoke crt_printf, offset ERROR_CODES, offset SETTHREADCONTEXT_ERROR, eax
	.endif
	
	; 关闭句柄
	invoke CloseHandle, @hThread
	ret

OnCmdBH endp

;-----------------------------------
; 函数功能：设置软件断点
; 参数：
;	dwAddress -- 设置的地址
;-----------------------------------
OnCmdSetBP proc dwAddress:dword
	; 写入int3
	LOCAL @hProcess:HANDLE
	
	; 打开进程
	invoke OpenProcess,THREAD_ALL_ACCESS, FALSE, g_DebugEv.dwProcessId
	.if eax == NULL
		xor eax, eax
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset OPENPROCESS_ERROR, eax
		ret
	.endif
	mov @hProcess, eax
	
	; 保存旧的代码
	invoke ReadProcessMemory, @hProcess, dwAddress, offset g_BreakPointOldCode, sizeof g_BreakPointOldCode, NULL
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset READPROCESSMEMORY_ERROR, eax
		jmp SETBREAKPOINT_END
	.endif
	
	; 修改代码，在入口地址处写入int3
	invoke WriteProcessMemory, @hProcess, dwAddress, offset g_SetBreakPoint, sizeof g_SetBreakPoint, NULL
	.if eax == NULL
			invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset WRITEPROCESSMEMORY_ERROR, eax
		jmp SETBREAKPOINT_END
	.endif
	
SETBREAKPOINT_END:
	invoke CloseHandle,@hProcess
	; 保存下断点的地址
	 	mov eax, dwAddress
	mov g_BreakPointAddress, eax
	
	mov g_IsOldCode, TRUE
	ret
OnCmdSetBP endp

;----------------------------------------------------------
; 函数功能：设置软件断点（普通断点）
; 参数：
;	dwAddress -- 设置断点的地址
;----------------------------------------------------------
OnCmdBP proc dwAddress:dword

	invoke SetBreakPoint, dwAddress
	ret
OnCmdBP endp

;----------------------------------------------------------
; 函数功能：运行到指定地址
; 参数：
;	dwAddress -- 运行到的地址
;----------------------------------------------------------
OnCmdGG proc dwAddress:dword
	invoke SetBreakPoint, dwAddress
	ret
OnCmdGG endp

;----------------------------------------------------------
; 函数功能：单步步过
; 参数：无
;----------------------------------------------------------
OnCmdP proc
	invoke OnCmdRealizetionP
	.if eax == TRUE
		; 跳过call指令
	 		invoke SetBreakPoint, g_CallNextAdd
	 	.else
	 		; 设置断点
	 		invoke SetStep
	.endif
	ret
OnCmdP endp

;----------------------------------------------------------
; 函数功能：获取命令并执行
; 参数：
;	pszCmdScript -- 指向导入脚本命令的字符串指针
;----------------------------------------------------------
GetCommand proc pszCmdScript:PVOID
	LOCAL @szInputBuff[256]:byte	; 保存输入的字符串
	LOCAL @szBuff[256]:byte		; 临时缓冲区
	LOCAL @TipsBuff[265]:byte	; 输出提示信息的缓冲区
	LOCAL @szImportCmdScriptBuff[256]:byte	; 保存导出的命令
	LOCAL @szCmd:dword		; 保存输入的命令
	LOCAL @szParam:dword	; 保存命令后的参数1
	LOCAL @szParam2:dword	; 保存命令后的参数2 （用于保存修改内存数据的数据）
	LOCAL @dwAddress:dword	; 保存转换后的地址
	LOCAL @szInputBuffNotSplit[256]:byte	; 保存未拆分的输入命令
	LOCAL @szEditMemoryDataBuff[32]:byte	; 保存修改内存数据的数据
	LOCAL @bEditValue:byte		; 保存修改的内存值
	LOCAL @szEditRegisterValNameBuff[32]:byte	; 保存修改的寄存器名
	LOCAL @dwEditRegisterValue:dword	; 保存修改后的寄存器值的值
	LOCAL @dwTrackBeginAddr:dword		; 保存跟踪的起始地址
	LOCAL @dwTrackEndAddr:dword			; 保存跟踪的终止地址
	LOCAL @szSaveSoftwareBP[32]:byte	; 中转进行保存软件断点
	
	.while TRUE
		; 初始化缓冲区
		invoke crt_memset, addr @szInputBuff, 0, sizeof @szInputBuff
		invoke crt_memset, addr @szBuff, 0, sizeof @szBuff
		invoke crt_memset, addr @szInputBuffNotSplit, 0, sizeof @szInputBuffNotSplit
		invoke crt_memset, addr @szEditMemoryDataBuff, 0, sizeof @szEditMemoryDataBuff
		invoke crt_memset, addr @szEditRegisterValNameBuff, 0, sizeof @szEditRegisterValNameBuff
		invoke crt_memset, addr @TipsBuff, 0, sizeof @TipsBuff
		
		; 判断是否是用户输入的命令，TRUE--用户输入的命令
		.if g_IsUserInput == TRUE
			; 输入命令
			invoke crt_printf, offset INPUT_COMMAND
			invoke crt_gets, addr @szInputBuff
			
			invoke crt_strlen, addr @szInputBuff
			.if eax == NULL
				.continue	; 输入命令为空时，重新输入
			.endif
		.else
			invoke crt_strcpy, addr @szInputBuff, pszCmdScript
		.endif
		
		; 将输入的字符串转为小写
		invoke crt__strlwr, addr @szInputBuff
		
		invoke crt_strcpy, addr @szInputBuffNotSplit, addr @szInputBuff
		
		; 保存输入的命令到脚本文件中
		;invoke wsprintf, addr @szBuff, offset INPUT_COMMAND_FMT, addr @szInputBuff
			;invoke WRITEFILE, addr @szBuff, offset COMMAND_SCRIPT
			; 判断是否第一次执行 "g" 命令
			invoke crt_strncmp,	addr @szInputBuff, offset COMMAND_RUN, 1
			.if eax != NULL
				mov g_IsFirstGo, FALSE
			.endif
			
			; 第一次执行 "g" 命令后将输入的命令保存到脚本缓冲区
			.if g_IsFirstGo == FALSE
				; 保存输入的命令到脚本缓冲区中
				invoke wsprintf, addr @szImportCmdScriptBuff, offset INPUT_COMMAND_FMT, addr @szInputBuff
				; 判断是否是导出脚本命令
				invoke crt_strncmp,	addr @szInputBuff, offset COMMAND_EXPORT_SCRIPT, 2
				.if eax != NULL
					invoke crt_strcat, offset g_ImportCmdScript, addr @szImportCmdScriptBuff
				.endif
			.endif
			
			; 分割字符串
			invoke crt_strtok, addr @szInputBuff, offset DIVISION
			mov @szCmd, eax
			invoke crt_strtok, NULL, offset DIVISION
			mov @szParam, eax
			
			; 分割第三个参数
			invoke crt_strtok, NULL, offset g_Division_n
			mov @szParam2, eax
			
		; 判断并执行命令
		; 显示反汇编（"u 参数"）
		invoke crt_strncmp, addr @szInputBuff, offset COMMAND_DISPLAY_ASSEMBLY, 1
		.if eax == NULL
			; 显示指定地址上的反汇编代码
			.if @szParam != NULL
				; 转换为16进制的地址
				invoke crt_strtoul, @szParam, NULL, 16
				mov @dwAddress, eax
				mov g_IsEip, FALSE
				invoke DisAsm, @dwAddress, 7
				invoke ShowRegister
			.else
				mov g_IsEip, FALSE
				invoke DisAsm, NULL, 7
				invoke ShowRegister
			.endif
			
			.if g_IsUserInput == FALSE
				ret
			.endif
			.continue	; 跳过后面的汇编代码
		.endif
		; 运行（gg）
		invoke crt_strncmp, addr @szInputBuff, offset COMMAND_RUN, 2
		.if eax == NULL
			; 关闭修改寄存器数据标志
			mov g_IsEditRegisterData, FALSE
			
			; 判断是否带有参数
			;int 3
			.if @szParam != NULL
				invoke crt_strtoul, @szParam, NULL, 16
				mov @dwAddress, eax
				mov g_IsOldCode, FALSE	; 一次性断点，防止下次重复设置断点
				;mov g_IsModule, FALSE
				invoke SetBreakPoint, @dwAddress
				;mov g_IsModule, FALSE
			.endif
			.break
		.endif
		; 单步步入（t）
		invoke crt_strncmp, addr @szInputBuff, offset COMMAND_STEP, 2
		.if eax == NULL
			; 关闭修改寄存器数据标志
			mov g_IsEditRegisterData, FALSE
			
			; 单步执行，修改TF标志位
			invoke SetStep
			.if g_IsUserInput == FALSE
				mov g_IsNormalStep, FALSE
				mov g_IsBpStep, TRUE	; 断步配合
				; 显示汇编指令
				mov g_IsEip, TRUE
				invoke DisAsm, NULL, 1
				invoke ShowRegister
			.endif
			.break
		.endif
		; 单步步过（p） 
		invoke crt_strncmp, addr @szInputBuff, offset COMMAND_P, 2
		.if eax == NULL
			; 关闭修改寄存器数据标志
			mov g_IsEditRegisterData, FALSE
			;int 3
			.if g_IsUserInput == FALSE
				invoke OnCmdRealizetionP
				.if eax == TRUE
					; 跳过call指令
			 			invoke SetBreakPoint, g_CallNextAdd
			 		.else
			 			; 设置断点
			 			invoke SetStep
			 			 .if g_IsUserInput == FALSE
						mov g_IsNormalStep, FALSE
						mov g_IsBpStep, TRUE	; 断步配合
						; 显示汇编指令
						mov g_IsEip, TRUE
						invoke DisAsm, NULL, 1
						invoke ShowRegister
					.endif
				.endif
				.break
			.endif
			; 单步步过
			;int 3
			invoke OnCmdRealizetionP
			.if eax == TRUE
				; 跳过call指令
			 		invoke SetBreakPoint, g_CallNextAdd
			 	.else
			 		; 设置断点
			 		invoke SetStep
			.endif
			 	.break
		.endif
		; 断点（bp），设置多个断点时，可以将断点保存到数据结构中
		invoke crt_strncmp, addr @szInputBuff, offset COMMAND_BREAKPOINT, 2
		.if eax == NULL
			; 关闭修改寄存器数据标志
			mov g_IsEditRegisterData, FALSE
			.if g_IsUserInput == FALSE
				ret
			.endif
			; 根据地址在对应的位置下断点
			.if @szParam != NULL
				invoke crt_strtoul, @szParam, NULL, 16
				mov @dwAddress, eax
				invoke SetBreakPoint, @dwAddress
			.endif
		.endif
		; 显示寄存器的值（r）
		invoke crt_strncmp, addr @szInputBuff, offset COMMAND_SHOW_REGISTER, 1
		.if eax == NULL
			.if g_IsUserInput == FALSE
				invoke ShowRegister
				ret
			.endif
		 		invoke ShowRegister
		.endif
		; 内存断点（bm）
		invoke crt_strncmp, addr @szInputBuff, offset COMMAND_MEMORY_BREAKPOINT, 2
		.if eax == NULL
			; 关闭修改寄存器数据标志
			mov g_IsEditRegisterData, FALSE
			.if g_IsUserInput == FALSE
				ret
			.endif
			; 读取、写入、执行、访问
			; 内存写入断点格式：bm w 00401000 8（内存范围）
			.if @szParam != NULL
				invoke crt_strtoul, @szParam, NULL, 16
				mov @dwAddress, eax
				; 目标进程内存写入断点
				invoke SetMemoryBreakPoint, 1, @dwAddress, 4, TRUE
			.endif
		.endif
		; 硬件断点（bh）COMMAND_HARDWARE_BREAKPOINT
		 invoke crt_strncmp, addr @szInputBuff, offset COMMAND_HARDWARE_BREAKPOINT, 2
		.if eax == NULL
			; 关闭修改寄存器数据标志
			mov g_IsEditRegisterData, FALSE
			.if g_IsUserInput == FALSE
				ret
			.endif
			.if @szParam != NULL
				invoke crt_strtoul, @szParam, NULL, 16
				mov @dwAddress, eax
				invoke SetBreakPoint, @dwAddress
			.endif
		.endif
		; 退出调试（q）
		invoke crt_strncmp, addr @szInputBuff, offset COMMAND_QUIT, 1
		.if eax == NULL
			invoke ExitProcess, 0
		.endif
		; 显示命令帮助说明（?）
		invoke crt_strncmp, addr @szInputBuff, offset COMMAND_HELP, 1
		.if eax == NULL
			.if g_IsUserInput == FALSE
				invoke ShowHelp
				ret
			.endif
			invoke ShowHelp
		.endif
		; 显示模块列表（ml）
		invoke crt_strncmp, addr @szInputBuff, offset COMMAND_MODULE_LIST, 2
		.if eax == NULL
			.if g_IsUserInput == FALSE
				invoke crt_printf, offset g_ModuleInfo
				ret
			.endif
			invoke crt_printf, offset g_ModuleInfo
			;invoke READFILE, offset MODULE_FILE1
		.endif
		; 导出脚本（es）
		invoke crt_strncmp, addr @szInputBuff, offset COMMAND_EXPORT_SCRIPT, 2
		.if eax == NULL
			; 保存输入的命令到脚本文件中
			invoke WRITEFILE, offset g_ImportCmdScript, offset COMMAND_SCRIPT
			invoke wsprintf, addr @TipsBuff, offset SZBUFF_FMT, offset SCRIPT_EXPORT_OK
			invoke crt_printf, addr @TipsBuff
		.endif
		; 导入脚本（ls）
		invoke crt_strncmp, addr @szInputBuff, offset COMMAND_IMPORT_SCRIPT, 2
		.if eax == NULL
			; 关闭修改寄存器数据标志
			mov g_IsEditRegisterData, FALSE
			
			; 读取脚本文件
			invoke READFILE, offset COMMAND_SCRIPT
			;int 3
			invoke OnCmdImportScript
		.endif
		 ; 显示内存数据 （dd）
		invoke crt_strncmp, addr @szInputBuff, offset COMMAND_DISPLAY_MEMORY_DATA, 2
		.if eax == NULL
			;int 3
			.if @szParam != NULL
				.if g_IsUserInput == FALSE
					; 按照执行的地址显示内存
					invoke crt_strtoul, @szParam, NULL, 16
					mov @dwAddress, eax
					invoke OnCmdDisplayMemoryData, @dwAddress
					ret
				.endif
				; 按照执行的地址显示内存
				invoke crt_strtoul, @szParam, NULL, 16
				mov @dwAddress, eax
				invoke OnCmdDisplayMemoryData, @dwAddress
			.else
				; 没有指定地址进行显示内存
				.if g_IsUserInput == FALSE
					invoke OnCmdDisplayMemoryData, NULL
					ret
				.endif
				; 没有指定地址进行显示内存
				invoke OnCmdDisplayMemoryData, NULL
			.endif
			.continue
		.endif
		; 内存 dump PE到文件 (dump 文件名)
		invoke crt_strncmp, addr @szInputBuff, offset COMMAND_MEMORY_DUMP, 4
		.if eax == NULL
			; 判断dump的文件名是否为空
			.if @szParam != NULL
				; 文件名不为空，开始内存dump
				;int 3
				.if g_IsUserInput == FALSE
					ret
				.endif
				; 判断是否可以进行内存dump
				.if g_CanDump == TRUE
					invoke OnCmdMemoryDump, addr @szInputBuffNotSplit
				.else
					invoke wsprintf, addr @TipsBuff, offset SZBUFF_FMT, offset MEMORY_DUMP_ERROR_TIPS
					invoke crt_printf, addr @TipsBuff
				.endif
			.else
				invoke wsprintf, addr @TipsBuff, offset SZBUFF_FMT, offset MEMORY_DUMP_ERROR_TIPS
				invoke crt_printf, addr @TipsBuff
			.endif
			.continue
		.endif
		
		; 修改指定内存上对应的值 (em)
		invoke crt_strncmp, addr @szInputBuff, offset COMMAND_EDIT_MEMORY_DATA, 2
		.if eax == NULL
			;int 3
			.if @szParam != NULL
				; 禁止内存dump
				mov g_CanDump, FALSE
				
				.if g_IsUserInput == FALSE
					ret
				.endif
				
				; 转换为16进制的地址
				invoke crt_strtoul, @szParam, NULL, 16
				mov @dwAddress, eax
				
				; 判断修改后的内存数据的字节长度要小于一个字节（不能超过2个字符）
				invoke wsprintf, addr @szEditMemoryDataBuff, offset SZBUFF_NO_RN_FMT, @szParam2
				invoke crt_strlen, addr @szEditMemoryDataBuff
				.if eax < 3
					; 修改后的内存数据值有效
					invoke crt_strtoul, @szParam2, NULL, 16
					mov @bEditValue, al
					invoke OnCmdEditMemoryData, @dwAddress, @bEditValue
				.else
					; 提示修改的内存数据值的字节长度（最好16进制，一个字节）
					invoke wsprintf, addr @TipsBuff, offset SZBUFF_FMT, offset EDIT_INPUT_MEMORY_ERROR
					invoke crt_printf, addr @TipsBuff
				.endif
			.endif
			.continue
		.endif
		; 修改指定寄存器的值 （er xxx xxx）
		invoke crt_strncmp, addr @szInputBuff, offset COMMADN_EDIT_REGISTER_VALUE, 2
		.if eax == NULL
			;int 3
			.if @szParam != NULL
				; 禁止内存dump
				mov g_CanDump, FALSE
				
				.if g_IsUserInput == FALSE
					ret
				.endif
				
				; 保存要修改的寄存器的名
				invoke wsprintf, addr @szEditRegisterValNameBuff, offset SZBUFF_NO_RN_FMT, @szParam
				
				; 将反汇编出来的代码转换为大写
				invoke crt__strupr, addr @szEditRegisterValNameBuff
				
				; 将修改后的寄存器的值进行转换并保存，如果为空默认将寄存器的值改为0
				invoke crt_strtoul, @szParam2, NULL, 16
					mov @dwEditRegisterValue, eax
					
					; 调用函数对寄存器的值进行修改
					invoke OnCmdEditRegisterData, addr @szEditRegisterValNameBuff, @dwEditRegisterValue
			.endif
		.endif
		; 执行到返回（gr）
		invoke crt_strncmp, addr @szInputBuff, offset COMMAND_GO_RETURN, 2
		.if eax == NULL
			; 满足执行到返回（执行到返回的标志以保存）
			;int 3
			.if g_IsUserInput == FALSE
				ret
			.endif
			.if g_IsSaveGoReturnAddr == TRUE
				; 在执行到返回的地址上下断点
				invoke SetBreakPoint, g_dwGoReturnAddr
				
				; 执行后关闭标志位
				mov g_IsSaveGoReturnAddr, FALSE
				
				; 输出提示信息
				invoke wsprintf, addr @TipsBuff, offset SZBUFF_FMT, offset EXECUTION_TO_RETURN_OK
				invoke crt_printf, addr @TipsBuff
			.else
				; 输出提示信息
				invoke wsprintf, addr @TipsBuff, offset SZBUFF_FMT, offset EXECUTION_TO_RETURN_ERROR
				invoke crt_printf, addr @TipsBuff
			.endif
		.endif
		; 指令记录（跟踪）（track）
		invoke crt_strncmp, addr @szInputBuff, offset COMMAND_TRACK, 5
		.if eax == NULL
			;int 3
			.if @szParam != NULL
				; 禁止内存dump
				mov g_CanDump, FALSE
				
				.if g_IsUserInput == FALSE
					ret
				.endif
				
				; 获取起始地址并 转换为16进制的地址进行保存
				invoke crt_strtoul, @szParam, NULL, 16
				mov @dwTrackBeginAddr, eax
				
				; 判断结束地址是否为空
				.if @szParam2 == NULL
					; 输出提示信息
					invoke wsprintf, addr @TipsBuff, offset SZBUFF_FMT, offset TRACK_END_ADDR_NULL
					invoke crt_printf, addr @TipsBuff
					.continue
				.endif
				
				; 获取终止地址
				invoke crt_strtoul, @szParam2, NULL, 16
				mov @dwTrackEndAddr, eax
				
				invoke OnCmdTrack, @dwTrackBeginAddr, @dwTrackEndAddr
			.endif
		.endif
	.endw
	ret
GetCommand endp

;-----------------------------------
; 函数功能：处理断点异常	
; 参数：无
;-----------------------------------
OnExecBreakPoint proc uses ebx
	LOCAL @pInfo:dword
	LOCAL @hProcess:HANDLE
	LOCAL @hThread:HANDLE
	LOCAL @context:CONTEXT
	LOCAL @dwContinueStatus:dword	; 异常处理返回值
	
	mov @dwContinueStatus, DBG_EXCEPTION_NOT_HANDLED
	
	; 初始化
	invoke RtlZeroMemory, addr @context, sizeof @context
	
	; 系统断点
	.if g_IsSystemBreakPoint == FALSE
		;mov g_IsEip, TRUE
		invoke DisAsm, NULL, 1
		invoke ShowRegister
		; 输入命令
		invoke GetCommand, NULL
		mov g_IsSystemBreakPoint, TRUE
		mov eax, DBG_CONTINUE
		ret
	.else
		; 判断异常，由调试器产生的异常就交给调试器去处理
		; 程序本身产生的异常就应该将异常交还给程序自己去处理
		assume ebx:ptr EXCEPTION_DEBUG_INFO
		mov ebx, offset g_DebugEv.u.Exception
		mov @pInfo, ebx
		mov eax, g_BreakPointAddress
		.if [ebx].pExceptionRecord.ExceptionAddress == eax
			; 还原代码
			invoke OpenProcess,THREAD_ALL_ACCESS, FALSE, g_DebugEv.dwProcessId
			.if eax == NULL
				invoke GetLastError
				invoke crt_printf, offset ERROR_CODES, offset OPENPROCESS_ERROR, eax
				jmp ONEXCEBREAKPOINT_END
				ret
			.endif
			mov @hProcess, eax
			
			; 修改代码，在入口地址处写入int3
			invoke WriteProcessMemory, @hProcess, g_BreakPointAddress, offset g_BreakPointOldCode, sizeof g_BreakPointOldCode, NULL
			.if eax == NULL
				invoke GetLastError
				invoke crt_printf, offset ERROR_CODES, offset WRITEPROCESSMEMORY_ERROR, eax
				jmp ONEXCEBREAKPOINT_END
			.endif
			
			; 还原EIP
			 invoke OpenThread, THREAD_ALL_ACCESS, FALSE, g_DebugEv.dwThreadId
			.if eax == NULL
				invoke GetLastError
				invoke crt_printf, offset ERROR_CODES, offset OPENTHREAD_ERROR, eax
				jmp ONEXCEBREAKPOINT_END
				ret
			.endif
			mov @hThread, eax
			
			lea ebx, @context
			assume ebx:ptr CONTEXT
			mov [ebx].ContextFlags, CONTEXT_FULL
			
			; 获取旧的寄存器环境
			invoke GetThreadContext, @hThread,addr @context
			.if eax == NULL
				invoke GetLastError
				invoke crt_printf, offset ERROR_CODES, offset GETTHREADCONTEXT_ERROR, eax
				jmp ONEXCEBREAKPOINT_END
			.endif
			
			mov eax, sizeof g_BreakPointOldCode
			sub [ebx].regEip, eax
			; 保存标志寄存器
			invoke SetThreadContext, @hThread, addr @context
			.if eax == NULL
				invoke GetLastError
					invoke crt_printf, offset ERROR_CODES, offset SETTHREADCONTEXT_ERROR, eax
					jmp ONEXCEBREAKPOINT_END
			.endif
			
			invoke CloseHandle, @hProcess
			invoke CloseHandle, @hThread
			
			; 通过调试器产生的异常和软件内部产生的异常一样时，要防止程序进入递归。
			; 调试器产生的异常处理完毕后，应将保存异常地址的成员置空。
			;mov g_BreakPointAddress, NULL
			
			; 同地址上的断点不是一次性的，就需要断点、单步配合，上面的地址就不应该置空
			; 同时进制调试器在目标进程int3汇编代码上设置断点（bp）
			; 设置单步，产生异常
			.if g_IsOldCode == TRUE
				invoke SetStep	; 还原代码
				mov g_IsNormalStep, FALSE
				mov g_IsBpStep, TRUE	; 断步配合
			.endif
			
			; 显示汇编指令
			mov g_IsEip, TRUE
			invoke DisAsm, NULL, 1
			invoke ShowRegister
			
			; 输入命令
			invoke GetCommand, NULL
			mov eax, DBG_CONTINUE
			ret
		.endif
	.endif
	
ONEXCEBREAKPOINT_END:
	mov eax, @dwContinueStatus
	ret
OnExecBreakPoint endp

;-----------------------------------
; 函数功能：处理单步、断点异常
; 参数：无
;-----------------------------------
OnExecSingleStep proc uses ebx
	LOCAL	@hProcess:HANDLE
	LOCAL @hThread:HANDLE
	LOCAL @context:CONTEXT
	LOCAL @pDR7:MY_DR7
	LOCAL @dwContinueStatus:dword	; 异常处理返回值
	LOCAL @TipsBuff[265]:byte		; 输出提示信息的缓冲区
	LOCAL @TrackASMDataFileName[256]:byte	; 保存文件名称（该文件存储跟踪时的反汇编代码）
	
	; 初始化返回值
	mov @dwContinueStatus, DBG_EXCEPTION_NOT_HANDLED
	
	; 初始化缓冲区
	invoke crt_memset, addr @TipsBuff, 0, sizeof @TipsBuff
	
	; 打开线程，获取寄存器值
	invoke OpenThread, THREAD_ALL_ACCESS, FALSE, g_DebugEv.dwThreadId
	 .if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset OPENTHREAD_ERROR, eax
		ret
	.endif
	mov @hThread, eax
	
	; 获取旧的寄存器环境
	invoke GetThreadContext, @hThread, addr @context
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset GETTHREADCONTEXT_ERROR, eax
	.endif
	
	assume ebx:ptr CONTEXT
	lea ebx, @context
	mov [ebx].ContextFlags, CONTEXT_DEBUG_REGISTERS
	
	mov eax, [ebx].iDr7
	mov dword ptr @pDR7, eax
	
	; 通过硬件断点寄存器Dr6判断是否硬件断点异常
	mov eax, [ebx].iDr6
	and eax, 0fh
	.if eax == TRUE
		mov @pDR7.L0, 0
		; 保存标志位
		invoke SetThreadContext, @hThread, addr @context
		.if eax == NULL
			invoke GetLastError
				invoke crt_printf, offset ERROR_CODES, offset SETTHREADCONTEXT_ERROR, eax
		.endif
		invoke CloseHandle, @hThread
		mov g_IsEip, TRUE
		invoke DisAsm, NULL, 1
		invoke ShowRegister
		invoke GetCommand, NULL
		mov @dwContinueStatus, DBG_CONTINUE
	.endif
	
	invoke CloseHandle, @hThread
	
	; 是否是断点单步 g_IsBpStep == FALSE，正常显示
	.if g_IsBpStep == TRUE
		; 将代码重新设置为 int3，在一个地址重复设置断点
		mov g_SetBreakPoint, 0cch
		invoke OpenProcess,THREAD_ALL_ACCESS, FALSE, g_DebugEv.dwProcessId
		.if eax == NULL
			xor eax, eax
			invoke GetLastError
			invoke crt_printf, offset ERROR_CODES, offset OPENTHREAD_ERROR, eax
			ret
		.endif
		mov @hProcess, eax
		
		invoke WriteProcessMemory, @hProcess, g_BreakPointAddress, offset g_SetBreakPoint, sizeof g_SetBreakPoint, NULL
		.if eax == NULL
			invoke GetLastError
			invoke crt_printf, offset ERROR_CODES, offset WRITEPROCESSMEMORY_ERROR, eax
		.endif
		
		invoke CloseHandle, @hProcess
		mov g_IsBpStep, FALSE
		mov @dwContinueStatus, DBG_CONTINUE
	.endif
	
	; 处理正常单步
	.if g_IsNormalStep == TRUE
		; 正常单步，显示一条汇编指令
		mov g_IsEip, TRUE
		invoke DisAsm, NULL, 1
		invoke ShowRegister
		
		 ; 检测下一行反汇编指令是否是call指令
		invoke OnCmdP
	 	.if eax == TRUE
			; 满足执行到返回
			mov g_IsSaveGoReturnAddr, TRUE
		.endif
		
		; 修改标志
		mov g_IsNormalStep, FALSE
		
		; 输入命令
		invoke GetCommand, NULL
		mov @dwContinueStatus, DBG_CONTINUE
	.endif
	
	; 处理内存断点单步
		.if g_IsMemorStep == TRUE	
		; 重新设置内存属性
		invoke SetMemoryBreakPoint, 1, g_BMAddress, g_BMBytes, g_IsMemorOnlyOne
		mov g_IsMemorStep, FALSE
		mov @dwContinueStatus, DBG_CONTINUE
		.endif
		
		; 检查自动跟踪是否开启
		.if g_IsAutoTrack == TRUE
			; 跟踪是否开启
			 .if g_IsTrack == TRUE
			 	; 判断异常地址是否是跟踪的结束地址，不是继续跟踪
			 	mov eax, g_dwTrackEndAddr
			 	.if g_CurrentExceptionAddr != eax
			 		; 设置单步断点
			 		invoke SetStep
			 		; 开启保存汇编代码
					mov g_IsSaveTrackAsm, TRUE
			 		mov @dwContinueStatus, DBG_CONTINUE
			 	.else
			 		; 停止跟踪
			 		mov g_IsAutoTrack, FALSE
			 		mov g_IsTrack, FALSE
			 		mov g_IsSaveTrackAsm, FALSE
			 		invoke wsprintf, addr @TipsBuff, offset TRACK_OK, g_dwTrackBeginAddr, g_dwTrackEndAddr
				invoke crt_printf, addr @TipsBuff
				
				; 获取文件名
				invoke wsprintf, addr @TrackASMDataFileName, offset TRACK_FILE_NAME, g_dwTrackBeginAddr, g_dwTrackEndAddr
				
				; 将跟踪的反汇编代码进行保存
				invoke WRITEFILE, offset g_ShowASM, addr @TrackASMDataFileName
			 	.endif
			 .endif
		.endif
		
	mov eax, @dwContinueStatus
	ret
OnExecSingleStep endp

;-----------------------------------
; 函数功能：处理内存访问异常
; 参数：无
;-----------------------------------
OnExecAccess proc uses ebx ecx
	LOCAL @hProcess:HANDLE
	LOCAL @InfoPoint:dword
	LOCAL @ExecAddress:dword
	LOCAL @dwOld:dword	; 保存旧的内存保护属性
	LOCAL @dwContinueStatus:dword	; 异常处理返回值
	
	mov @dwContinueStatus, DBG_EXCEPTION_NOT_HANDLED
	assume ebx:ptr EXCEPTION_DEBUG_INFO
	mov ebx, offset g_DebugEv.u.Exception
	mov @InfoPoint, ebx
	
	; 获取目标进程产生异常的地址
	mov eax, [[ebx].pExceptionRecord.ExceptionInformation + 4]
	mov @ExecAddress, eax
	mov eax, g_BMAddress
	mov ecx, eax
	add ecx, g_BMBytes
	.if @ExecAddress >= eax && @ExecAddress < ecx
		; 处理内存异常，恢复内存属性
		invoke OpenProcess, PROCESS_ALL_ACCESS, FALSE, g_DebugEv.dwProcessId
		.if eax == NULL
			invoke GetLastError
			invoke crt_printf, offset ERROR_CODES, offset OPENTHREAD_ERROR, eax
			jmp ONEXCEACCESS_END
		.endif
		mov @hProcess, eax
		invoke VirtualProtectEx, @hProcess, g_BMAddress, g_BMBytes, g_BMOldProtected, addr @dwOld;
		.if eax == NULL
			invoke GetLastError
			invoke crt_printf, offset ERROR_CODES, offset VIRTUALPROTECTEX_ERROR, eax
			jmp ONEXCEACCESS_END
		.endif
		
		invoke CloseHandle, @hProcess
		; 内存断点多次有效，需要重复修改内存的保护属性（断点、单步配合）
		; 判断是否是一次性断点
		.if g_IsMemorOnlyOne == FALSE
			mov g_IsMemorStep, TRUE
			invoke SetStep
			mov g_IsNormalStep, FALSE
		.endif
		
		mov @dwContinueStatus, DBG_CONTINUE
	.endif
	
ONEXCEACCESS_END:
	mov eax, @dwContinueStatus
	ret
OnExecAccess endp

;-----------------------------------
; 函数功能：创建调试对象
; 参数：无
;-----------------------------------
OnDebugEvent proc uses ebx
	LOCAL @szBuff[256]:byte	; 缓冲区
	LOCAL @InfoPoint:dword
	LOCAL @dwContinueStatus:dword	; 异常处理返回值
	
	mov @dwContinueStatus, DBG_EXCEPTION_NOT_HANDLED
	
	assume ebx:ptr EXCEPTION_DEBUG_INFO
	mov ebx, offset g_DebugEv.u.Exception
	mov @InfoPoint, ebx
	
	invoke wsprintf, addr @szBuff, offset EXCEPTION_DEBUG_FMT, [ebx].pExceptionRecord.ExceptionCode, [ebx].pExceptionRecord.ExceptionAddress
	; 保存当前异常的地址
	mov eax, [ebx].pExceptionRecord.ExceptionAddress
	mov g_CurrentExceptionAddr, eax
	
	; 输出异常信息
	invoke crt_printf, addr @szBuff
	
	.if [ebx].pExceptionRecord.ExceptionCode == STATUS_PRIVILEGED_INSTRUCTION
		; 特权指令异常
		mov g_IsEip, TRUE
		invoke DisAsm, NULL, 1
		invoke ShowRegister
		invoke GetCommand, NULL
	.elseif [ebx].pExceptionRecord.ExceptionCode == EXCEPTION_ACCESS_VIOLATION
		; 内存访问异常
		mov g_IsEip, TRUE
		invoke DisAsm, NULL, 1
		invoke ShowRegister
		invoke OnExecAccess
		; 输入命令
		mov @dwContinueStatus, eax
		invoke GetCommand, NULL
	.elseif [ebx].pExceptionRecord.ExceptionCode == EXCEPTION_BREAKPOINT
		; 断点异常
		invoke OnExecBreakPoint
		mov @dwContinueStatus, eax
	.elseif [ebx].pExceptionRecord.ExceptionCode == EXCEPTION_SINGLE_STEP
		; 单步异常
		invoke OnExecSingleStep
		mov @dwContinueStatus, eax
	.elseif [ebx].pExceptionRecord.ExceptionCode == DBG_CONTROL_C
		; Ctrl+C 强制结束任务
	.endif
	
	mov eax, @dwContinueStatus
	ret
OnDebugEvent endp

;-----------------------------------
; 函数功能：创建线程调试对象	
; 参数：无
;-----------------------------------
OnCreateThread proc
	LOCAL @szBuff[256]:byte
	invoke wsprintf, addr @szBuff, offset CREATE_THREAD_FMT, \
		g_DebugEv.u.CreateThread.hThread, \
		g_DebugEv.u.CreateThread.lpStartAddress, \
		g_DebugEv.u.CreateThread.lpThreadLocalBase
	;invoke crt_printf, addr @szBuff
	mov eax, DBG_CONTINUE
	ret
OnCreateThread endp

;-----------------------------------
; 函数功能：获取指定字符在字符串中最后的下标位置
; 参数：
;	pszFilePath -- 文件路径
;	char -- 分割符
;-----------------------------------
GetCharIdx proc uses ebx, pszFilePath:PVOID, char:byte
	LOCAL @BuffLen:dword	; 文件路径的字符长度
	LOCAL @ch:byte
	invoke crt_strlen, pszFilePath
	
	mov @BuffLen, eax
	mov ebx, 0
	.while eax != @BuffLen
		mov eax, [pszFilePath + ebx]
		mov dword ptr @ch, eax
		 invoke crt_strncmp,	addr @ch, addr char, 1
		inc ebx
	.endw
	ret
GetCharIdx endp

;-----------------------------------
; 函数功能：创建进程调试对象	
; 参数：无
;-----------------------------------
OnCreateProcess proc
	LOCAL @szBuff[256]:byte	; 缓冲区
	LOCAL @DebugProcessPath[MAX_PATH]:byte	; 保存被调试进程的绝对路径
	;assume ecx:ptr CREATE_PROCESS_DEBUG_INFO
	;lea ecx, g_DebugEv.u.CreateProcessInfo
	
	invoke wsprintf, addr @szBuff, offset CREATE_PROCESS_FMT, \
		g_DebugEv.u.CreateProcessInfo.hProcess, \
		g_DebugEv.u.CreateProcessInfo.lpBaseOfImage, \
		g_DebugEv.u.CreateProcessInfo.lpStartAddress
	invoke crt_printf, addr @szBuff
	
	mov eax, g_DebugEv.u.CreateProcessInfo.lpBaseOfImage
	mov g_lpBaseOfImage, eax
	
	; 删除已存在文件
	;invoke DeleteFile, offset COMMAND_SCRIPT
	;invoke DeleteFile, offset MODULE_FILE
	
	; 获取被调试进程的信息
	invoke GetFullPathName, offset MY_OPEN_PROCESS, 260, addr @DebugProcessPath, NULL
	invoke wsprintf, addr @szBuff, offset LOAD_DLL_FMT, g_DebugEv.u.CreateProcessInfo.lpBaseOfImage, addr @DebugProcessPath
	invoke crt_strcat, offset	g_ModuleInfo, addr @szBuff
	
	
	; 提示命令帮助说明
	invoke crt_printf, offset TIPS_DEBUG_HELPS
	
	; 在目标程序的入口地址处设置一个断点，方便调试目标程序代码
	
	invoke SetBreakPoint, g_DebugEv.u.CreateProcessInfo.lpStartAddress
	
	mov eax, DBG_CONTINUE
	ret
OnCreateProcess endp

;-----------------------------------
; 函数功能：退出线程调试对象	
; 参数：无
;-----------------------------------
OnExitThread proc
	LOCAL @szBuff[256]:byte
	
	invoke wsprintf, addr @szBuff, offset EXIT_THREAD_FMT, g_DebugEv.u.ExitThread.dwExitCode
	invoke crt_printf, addr @szBuff
	mov eax, DBG_CONTINUE
	ret
OnExitThread endp

;-----------------------------------
; 函数功能：退出进程调试对象	
; 参数：无
;-----------------------------------
OnExitProcess proc
	LOCAL @szBuff[256]:byte
	invoke wsprintf, addr @szBuff, offset EXIT_PROCESS_FMT, g_DebugEv.u.ExitProcess.dwExitCode
	invoke crt_printf, addr @szBuff
	mov eax, DBG_CONTINUE
	ret
OnExitProcess endp

;-----------------------------------
; 函数功能：加载模块
; 参数：无
;-----------------------------------
OnLoadDll proc
	LOCAL @szBuff[MAX_PATH]:byte
	LOCAL @szModuleName[MAX_PATH]:byte
	LOCAL @szTest[MAX_PATH]:byte
	LOCAL @lpBaseAddress:LPVOID
	LOCAL @hProcess:HANDLE
	LOCAL @dwContinueStatus:dword	; 异常处理返回值
	
	mov @dwContinueStatus, DBG_EXCEPTION_NOT_HANDLED
	;invoke crt_printf, offset MY_MSG
	
	invoke OpenProcess, PROCESS_ALL_ACCESS, FALSE, g_DebugEv.dwProcessId
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset OPENTHREAD_ERROR, eax
		jmp ONLOADDLL_END
	.endif
	mov @hProcess, eax
	.if @hProcess != INVALID_HANDLE_VALUE
		invoke ReadProcessMemory, @hProcess, \
			g_DebugEv.u.LoadDll.lpImageName, \
			addr @lpBaseAddress, \
			sizeof LPVOID, \
			NULL
		.if eax !=	NULL
			invoke ReadProcessMemory, @hProcess, \
				@lpBaseAddress, addr @szModuleName, \
				sizeof @szBuff, NULL
			.if g_DebugEv.u.LoadDll.fUnicode == 0
				; 非Unicode
				invoke wsprintf, addr @szBuff, \
					offset LOAD_DLL_FMT, \
					g_DebugEv.u.LoadDll.lpBaseOfDll, \
					addr @szModuleName
				;invoke crt_printf, addr @szBuff
				invoke crt_strcat, offset	g_ModuleInfo, addr @szBuff
				;invoke WRITEFILE, addr @szBuff, offset MODULE_FILE
			.else
				; Unicode
				invoke WideCharToMultiByte, CP_ACP, \
					NULL, addr @szModuleName, \
					-1, addr @szTest, \
					sizeof @szModuleName, \
					NULL, NULL
				invoke wsprintf, addr @szBuff, offset LOAD_DLL_FMT, g_DebugEv.u.LoadDll.lpBaseOfDll, addr @szTest
				;invoke crt_printf, addr @szBuff
				invoke crt_strcat, offset	g_ModuleInfo, addr @szBuff
				;invoke WRITEFILE, addr @szBuff, offset MODULE_FILE
			.endif
		.endif
	.endif
	
	invoke CloseHandle, @hProcess
	mov @dwContinueStatus, DBG_CONTINUE
ONLOADDLL_END:
	mov eax, @dwContinueStatus
	ret
OnLoadDll endp

;-----------------------------------
; 函数功能：卸载模块
; 参数：无
;-----------------------------------
OnUnLoadDll proc
	LOCAL @szBuff[256]:byte
	invoke wsprintf, addr @szBuff, \
		offset UNLOAD_DLL_FMT, \
		g_DebugEv.u.UnloadDll.lpBaseOfDll
	invoke crt_printf, addr @szBuff
	mov eax, DBG_CONTINUE
	ret
OnUnLoadDll endp

;---------------------------------------------------
; 函数功能：将一个字符串发送给调试器以进行显示
; 参数：无
;---------------------------------------------------
OnOutputDebugString proc
	LOCAL @szBuff[1000]:byte
	LOCAL @szModuleName[256]:byte
	LOCAL @szTest[256]:byte
	LOCAL @hProcess:HANDLE
	
	invoke OpenProcess, PROCESS_ALL_ACCESS, FALSE, g_DebugEv.dwProcessId
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset OPENTHREAD_ERROR, eax
	.endif
	mov @hProcess, eax
	.if @hProcess != INVALID_HANDLE_VALUE
		invoke ReadProcessMemory, @hProcess, \
			g_DebugEv.u.DebugString.lpDebugStringData, \
			addr @szModuleName, \
			g_DebugEv.u.DebugString.nDebugStringiLength, \
			NULL
		.if g_DebugEv.u.DebugString.fUnicode == 0
			; 非Unicode
			invoke wsprintf, addr @szBuff, offset OUTPUT_DEBUG_STRING_FMT, addr @szModuleName
			invoke crt_printf, addr @szBuff
		.else
			; Unicode
			invoke WideCharToMultiByte, CP_ACP, \
				NULL, \
				addr @szModuleName, \
				-1, addr @szTest, \
				sizeof @szModuleName, \
				NULL, NULL
			invoke wsprintf, addr @szBuff, offset OUTPUT_DEBUG_STRING_FMT, addr @szTest
			invoke crt_printf, addr @szBuff
		.endif
	.endif
	
	invoke CloseHandle, @hProcess
	mov eax, DBG_CONTINUE
	ret
OnOutputDebugString endp


;-----------------------------------
; 函数功能：开始调试
; 参数：无
;-----------------------------------
BeginDebug proc uses ebx
	LOCAL @si:STARTUPINFO
	LOCAL @pi:PROCESS_INFORMATION
	LOCAL @szBuff[256]:byte	; 缓冲区
	;LOCAL @DebugEv:DEBUG_EVENT		; 保存调试事件的信息
	LOCAL @dwContinueStatus:DWORD	; 设置异常条件（事件是否处理）
	
	mov @dwContinueStatus, DBG_CONTINUE	; 默认处理
	
	invoke crt_memset, addr @si, 0, sizeof @si
	mov @si.cb, sizeof @si
	invoke crt_memset, addr @pi, 0, sizeof @pi
	
	; 创建进程
	invoke CreateProcess, offset MY_OPEN_PROCESS, \
		NULL, NULL, NULL, FALSE, \
		DEBUG_PROCESS, NULL, NULL, addr @si, addr @pi
	.if eax == NULL
		invoke GetLastError
		invoke crt_printf, offset ERROR_CODES, offset CREATEPROCESS_ERROR, eax
		ret
	.endif
	invoke crt_printf, offset CREATEPROCESS_OK
	
	; DBG_EXCEPTION_NOT_HANDLED（不处理，将异常交给软件本身）	DBG_CONTINUE（处理异常）
	; 循环调试处理事件
	.while TRUE
		; 等待调试事件, INFINITE -- 无限等待
		invoke WaitForDebugEvent, addr g_DebugEv, INFINITE
		.if eax == NULL
			invoke GetLastError
			invoke crt_printf, offset ERROR_CODES, offset WAITFORDEBUGEVENT_ERROR, eax
			ret
		.endif
		; 将结构体的异常代码给ebx，然后下方通过ebx判断是哪个事件
		mov ebx, g_DebugEv.dwDebugEventCode
		.if ebx == EXCEPTION_DEBUG_EVENT
			invoke OnDebugEvent
			mov @dwContinueStatus, eax
		.elseif ebx == CREATE_THREAD_DEBUG_EVENT
			invoke OnCreateThread
			mov @dwContinueStatus, eax
		.elseif ebx == CREATE_PROCESS_DEBUG_EVENT
			invoke OnCreateProcess
			mov @dwContinueStatus, eax
		.elseif ebx == EXIT_THREAD_DEBUG_EVENT
			invoke OnExitThread
			mov @dwContinueStatus, eax
		.elseif ebx == EXIT_PROCESS_DEBUG_EVENT
			invoke OnExitProcess
			mov @dwContinueStatus, eax
		.elseif ebx == LOAD_DLL_DEBUG_EVENT
			invoke OnLoadDll
			mov @dwContinueStatus, eax
		.elseif ebx == UNLOAD_DLL_DEBUG_EVENT
			invoke OnUnLoadDll
			mov @dwContinueStatus, eax
		.elseif ebx == OUTPUT_DEBUG_STRING_EVENT
			invoke OnOutputDebugString
			mov @dwContinueStatus, eax
		.endif
		
		; 允许调试器继续先前报告调试事件的线程(继续执行，DBG_EXCEPTION_NOT_HANDLED（不处理）、DBG_CONTINUE（处理）)
		invoke ContinueDebugEvent, g_DebugEv.dwProcessId, g_DebugEv.dwThreadId, @dwContinueStatus
	.endw
	ret
BeginDebug endp
; 程序入口
START:
	invoke BeginDebug
	invoke ExitProcess, 0
	ret
end START