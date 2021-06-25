// CRWProcessMem.cpp: 实现文件
//

#include "pch.h"
#include "MyPCHunter.h"
#include "CRWProcessMem.h"
#include "afxdialogex.h"
#include "common.h"

// CRWProcessMem 对话框

IMPLEMENT_DYNAMIC(CRWProcessMem, CDialogEx)

CRWProcessMem::CRWProcessMem(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CRWProcessMem, pParent)
	, m_csRWBuff(_T(""))
{

}

CRWProcessMem::~CRWProcessMem()
{
}

void CRWProcessMem::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, EDIT_PID, m_PID);
	DDX_Control(pDX, EDIT_RWADDR, m_RWAddr);
	DDX_Control(pDX, EDIT_BYTESIZE, m_ByteSize);
	DDX_Text(pDX, EDIT_RW_BUFF, m_csRWBuff);
}


BEGIN_MESSAGE_MAP(CRWProcessMem, CDialogEx)
	ON_BN_CLICKED(BTN_READ, &CRWProcessMem::OnBnClickedRead)
	ON_BN_CLICKED(BTN_WRITE, &CRWProcessMem::OnBnClickedWrite)
END_MESSAGE_MAP()


// CRWProcessMem 消息处理程序


BOOL CRWProcessMem::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;
}


// 读取内存数据
void CRWProcessMem::OnBnClickedRead()
{
	// TODO: 在此处添加实现代码.
	UpdateData(TRUE);

	RWADDR rd;
	DWORD dwSize = 0; // 保存返回的大小

	// 获取PID
	CString csTmp;
	m_PID.GetWindowText(csTmp);
	m_dwPID = _wtoi(csTmp.GetBuffer());
	rd.dwPID = m_dwPID;

	// 获取要操作的地址
	CString csRAddr;
	m_RWAddr.GetWindowText(csRAddr);
	char* pBuff = CStringToChar(csRAddr);
	char szBuff[32] = { 0 };
	strcpy_s(szBuff, pBuff);
	int nAddr = strtol(szBuff, NULL, 16);
	rd.pDestMemAddr = (PVOID)nAddr;

	// 获取操作的字节大小
	CString csByteSize;
	m_ByteSize.GetWindowText(csByteSize);
	m_dwByteSize = _wtoi(csByteSize.GetBuffer());
	rd.ulReadSize = m_dwByteSize;

	// 初始化读写缓冲区
	memset(rd.szRWBuff, 0, sizeof(rd.szRWBuff));

	//CString cs;
	//cs.Format(L"PID: %d, addr: %p, size: %d, %s\r\n", 
	//	rd.dwPID, 
	//	rd.pDestMemAddr, 
	//	rd.ulReadSize, 
	//	rd.szRWBuff);
	//MessageBox(cs);

	// 发送读取内存请求
	DeviceIoControl(g_hDev,
		READPROCESSMEMORY,
		&rd, sizeof(RWADDR),
		&rd, sizeof(RWADDR),
		&dwSize, NULL);

	// 将返回到的数据（读取到的数据）更新到控件中
	USES_CONVERSION;
	m_csRWBuff = A2W(rd.szRWBuff);

	UpdateData(FALSE);
	MessageBox(L"内存读取成功");

}

// 写入内存数据
void CRWProcessMem::OnBnClickedWrite()
{
	UpdateData(TRUE);

	RWADDR rd;
	DWORD dwSize = 0; // 保存返回的大小

	// 获取PID
	CString csTmp;
	m_PID.GetWindowText(csTmp);
	m_dwPID = _wtoi(csTmp.GetBuffer());
	rd.dwPID = m_dwPID;

	// 获取读取的字节大小
	CString csByteSize;
	m_ByteSize.GetWindowText(csByteSize);
	m_dwByteSize = _wtoi(csByteSize.GetBuffer());
	rd.ulReadSize = m_dwByteSize;

	// 获取要写入数据的内存地址
	CString csWAddr;
	m_RWAddr.GetWindowText(csWAddr);

	char* pTmp = CStringToChar(csWAddr);
	char szBuff[32] = { 0 };
	strcpy_s(szBuff, pTmp);
	int nAddr = strtol(szBuff, NULL, 16);
	rd.pDestMemAddr = (PVOID)nAddr;

	// 初始化读写数据缓冲区
	memset(rd.szRWBuff, 0, sizeof(rd.szRWBuff));

	// 获取要写入的数据
	char* pBuff = CStringToChar(m_csRWBuff);
	strcpy_s(rd.szRWBuff, pBuff);

	// 发送读取内存请求
	DeviceIoControl(g_hDev,
		WRITEPROCESSMEMORY,
		&rd, sizeof(RWADDR),
		&rd, sizeof(RWADDR),
		&dwSize, NULL);

	MessageBox(L"内存写入成功");
}

// CString 转换 Char
char* CRWProcessMem::CStringToChar(CString cs)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, cs, -1, NULL, 0, NULL, NULL);
	char* pBuff = new char[nLen + 1];
	WideCharToMultiByte(CP_ACP, 0, cs, -1, pBuff, nLen, NULL, NULL);
	return pBuff;
}

// Char 转换 CString
CString CRWProcessMem::CharToCString(char* pBuff)
{
	int npBuffLen = strlen(pBuff);
	// 按字符计算多字节字符的大小
	int nLen = MultiByteToWideChar(CP_ACP, 0, pBuff, npBuffLen, NULL, 0);
	// 为宽字节字符数组申请空间，数组大小为按字节计算的多字节字符大小
	TCHAR* pwszBuff = new TCHAR[nLen + 1];
	// 多字节编码转换成宽字节编码
	MultiByteToWideChar(CP_ACP, 0, pBuff, npBuffLen, pwszBuff, nLen);

	pwszBuff[nLen] = '\0';  //添加字符串结尾，注意不是len+1

	// 将TCHAR数组转换为CString	
	CString csTemp;
	csTemp.Append(pwszBuff);

	// 删除缓冲区
	delete[]pwszBuff;

	return csTemp;
}