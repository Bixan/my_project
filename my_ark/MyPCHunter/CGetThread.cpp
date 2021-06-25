// CGetThread.cpp: 实现文件
//

#include "pch.h"
#include "MyPCHunter.h"
#include "CGetThread.h"
#include "afxdialogex.h"
#include "common.h"

// CGetThread 对话框

IMPLEMENT_DYNAMIC(CGetThread, CDialogEx)

CGetThread::CGetThread(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CGetThread, pParent)
{

}

CGetThread::~CGetThread()
{
}

void CGetThread::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListThread);
}


BEGIN_MESSAGE_MAP(CGetThread, CDialogEx)
END_MESSAGE_MAP()


// CGetThread 消息处理程序


BOOL CGetThread::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 设置表头风格
	m_ListThread.SetExtendedStyle(m_ListThread.GetExtendedStyle() |
		LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_TWOCLICKACTIVATE);

	// 设置表头
	m_ListThread.InsertColumn(0, L"序号");
	m_ListThread.InsertColumn(1, L"TID");
	m_ListThread.InsertColumn(2, L"优先级");
	m_ListThread.InsertColumn(3, L"线程起始地址");
	m_ListThread.InsertColumn(4, L"线程状态");

	// 设置列表控件项的宽度
	SetListColumnWidth();

	GetThreadInfo();
	return TRUE;
}

// 根据状态值转换对应字符
WCHAR* NumToStatus(INT nNum)
{
	WCHAR* Buffer = L"NULL";
	switch (nNum)
	{
	case 0:
		Buffer = L"已初始化";
		break;
	case 1:
		Buffer = L"准备";
		break;
	case 2:
		Buffer = L"运行中";
		break;
	case 3:
		Buffer = L"Standby";
		break;
	case 4:
		Buffer = L"终止";
		break;
	case 5:
		Buffer = L"等待";
		break;
	case 6:
		Buffer = L"Transition";
		break;
	case 7:
		Buffer = L"DeferredReady";
		break;
	case 8:
		Buffer = L"GateWait";
		break;
	default:
		break;
	}
	return Buffer;
}

void CGetThread::GetThreadInfo()
{
	m_ListThread.DeleteAllItems();
	PTHREAD test;
	DWORD dwSize = 0;
	// 获取线程数量
	DeviceIoControl(g_hDev, 
		GETTHREAD, 
		&m_dwPID, 
		sizeof(DWORD), 
		&test, 
		sizeof(THREAD), 
		&dwSize, 
		NULL);
	// 根据线程数据申请对应的空间
	PTHREAD pThread = new THREAD[dwSize]();

	// 获取线程数据
	DeviceIoControl(g_hDev, 
		GETTHREAD, 
		&m_dwPID, 
		dwSize, 
		pThread, 
		dwSize, 
		&dwSize, 
		NULL);
	// 获取线程个数
	int nCount = dwSize / sizeof(THREAD);
	int i = 0;
	int nIndex = 0;
	while (nCount)
	{
		--nCount;
		m_ListThread.InsertItem(nIndex, _T(""));
		CString Temp;
		Temp.Format(L"%d", nIndex + 1);
		m_ListThread.SetItemText(nIndex, 0, Temp);
		Temp.Format(L"%d", pThread[i].dwTID);
		m_ListThread.SetItemText(nIndex, 1, Temp);
		Temp.Format(L"%d", pThread[i].dwBasePriority);
		m_ListThread.SetItemText(nIndex, 2, Temp);
		Temp.Format(L"0x%08x", pThread[i].dwStartAddress);
		m_ListThread.SetItemText(nIndex, 3, Temp);
		m_ListThread.SetItemText(nIndex, 4, NumToStatus(pThread[i].dwStatus));
		++i;
		++nIndex;
	}
	delete[] pThread;

	SetListColumnWidth();
}


// 设置列表控件项的宽度
void CGetThread::SetListColumnWidth()
{
	// 获取列表视图控件使用的标题控件的句柄
	CHeaderCtrl* pHdrCtrl = m_ListThread.GetHeaderCtrl();
	// 根据列表内容初始化列表头部的宽度
	for (int i = 0; i < pHdrCtrl->GetItemCount(); i++)
	{
		// 调整列表项的初始化宽度
		m_ListThread.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
	}
}