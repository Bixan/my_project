// CGetSSDT.cpp: 实现文件
//

#include "pch.h"
#include "MyPCHunter.h"
#include "CGetSSDT.h"
#include "afxdialogex.h"
#include "common.h"

// CGetSSDT 对话框

IMPLEMENT_DYNAMIC(CGetSSDT, CDialogEx)

CGetSSDT::CGetSSDT(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CGetSSDT, pParent)
{

}

CGetSSDT::~CGetSSDT()
{
}

void CGetSSDT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListSSDT);
}


BEGIN_MESSAGE_MAP(CGetSSDT, CDialogEx)
	ON_MESSAGE(WM_GETSSDT, &CGetSSDT::OnSetSSDTData)
END_MESSAGE_MAP()

DWORD WINAPI SSDTThreadProc(_In_ LPVOID lpParameter)
{
	Sleep(500);
	HWND hWnd = (HWND)lpParameter;
	SendMessage(hWnd, WM_GETSSDT, 0, 0);

	return 0;
}

// CGetSSDT 消息处理程序


BOOL CGetSSDT::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 初始化控件
	m_ListSSDT.SetExtendedStyle(m_ListSSDT.GetExtendedStyle() |
		LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_TWOCLICKACTIVATE);

	// 设置表头
	m_ListSSDT.InsertColumn(0, _TEXT("序号"));
	m_ListSSDT.InsertColumn(1, _TEXT("函数地址"));

	// 设置列表控件项的宽度
	SetListColumnWidth();

	// 创建线程用来处理 SSDT 信息
	CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)SSDTThreadProc,
		this->m_hWnd,
		0, 0);

	return TRUE;
}

void CGetSSDT::SetListColumnWidth()
{
	// 获取列表视图控件使用的标题控件的句柄
	CHeaderCtrl* pHdrCtrl = m_ListSSDT.GetHeaderCtrl();
	// 根据列表内容初始化列表头部的宽度
	for (int i = 0; i < pHdrCtrl->GetItemCount(); i++)
	{
		// 调整列表项的初始化宽度
		m_ListSSDT.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
	}
}

// 设置IDT表数据设置到列表控件上
LRESULT CGetSSDT::OnSetSSDTData(WPARAM wParam, LPARAM lParam)
{
	// 清除控件上的所有数据
	m_ListSSDT.DeleteAllItems();

	SSDT ssdt;

	DWORD dwSize = 0;
	DeviceIoControl(g_hDev, 
		GETSSDT, 
		NULL, 
		NULL, 
		&ssdt, 
		sizeof(SSDT), 
		&dwSize, 
		NULL);

	// 获取系统服务描述符表的个数
	int nNum = dwSize / sizeof(SSDT);

	// 获取系统服务描述符表的个数New对应的个数
	PSSDT pSSDT = new SSDT[nNum]();

	// 获取系统服务描述符表
	DeviceIoControl(g_hDev, 
		GETSSDT, 
		NULL, 
		NULL, 
		pSSDT, 
		dwSize, 
		&dwSize, 
		NULL);

	CString csTmp;
	// 将驱动返回的SSDT数据设到列表控件上
	for (int i = 0; i < nNum; ++i)
	{
		m_ListSSDT.InsertItem(i, _T(""));
		csTmp.Format(L"0x%x", pSSDT[i].ulSysCallIndex);
		m_ListSSDT.SetItemText(i, 0, csTmp);
		csTmp.Format(L"0x%08x", pSSDT[i].ulAddr);
		m_ListSSDT.SetItemText(i, 1, csTmp);
	}

	SetListColumnWidth();

	// 清除申请的空间
	delete[] pSSDT;
	
	return 0;
}
