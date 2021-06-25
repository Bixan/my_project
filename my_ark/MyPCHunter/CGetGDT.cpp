// CMyGDT.cpp: 实现文件
//

#include "pch.h"
#include "MyPCHunter.h"
#include "CGetGDT.h"
#include "afxdialogex.h"
#include "common.h"

// CMyGDT 对话框

IMPLEMENT_DYNAMIC(CGetGDT, CDialogEx)

CGetGDT::CGetGDT(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CMyGDT, pParent)
{

}

CGetGDT::~CGetGDT()
{
}

void CGetGDT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListGDT);
}


BEGIN_MESSAGE_MAP(CGetGDT, CDialogEx)
	ON_MESSAGE(WM_GETGDT, &CGetGDT::OnSetGDTData)
END_MESSAGE_MAP()

DWORD WINAPI GDTThreadProc(_In_ LPVOID lpParameter)
{
	Sleep(500);
	HWND hWnd = (HWND)lpParameter;
	SendMessage(hWnd, WM_GETGDT, 0, 0);
	return 0;
}

// CMyGDT 消息处理程序

BOOL CGetGDT::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 初始化控件
	m_ListGDT.SetExtendedStyle(m_ListGDT.GetExtendedStyle() |
		LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_TWOCLICKACTIVATE);

	// 设置表头
	m_ListGDT.InsertColumn(0, _TEXT("基址"));
	m_ListGDT.InsertColumn(1, _TEXT("界限"));
	m_ListGDT.InsertColumn(3, _TEXT("段粒度"));
	m_ListGDT.InsertColumn(4, _TEXT("段特权级"));
	m_ListGDT.InsertColumn(5, _TEXT("类型"));
	
	// 设置列表控件项的宽度
	SetListColumnWidth();

	// 创建线程用来处理 GDT 消息
	CreateThread(NULL, 0, 
		(LPTHREAD_START_ROUTINE)GDTThreadProc, 
		this->m_hWnd, 
		0, 0);
	return TRUE;
}

// 设置列表控件项的宽度
void CGetGDT::SetListColumnWidth()
{	
	// 获取列表视图控件使用的标题控件的句柄
	CHeaderCtrl* pHdrCtrl = m_ListGDT.GetHeaderCtrl();
	// 根据列表内容初始化列表头部的宽度
	for (int i = 0; i < pHdrCtrl->GetItemCount(); i++)
	{
		// 调整列表项的初始化宽度
		m_ListGDT.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
	}
}

// 设置GDT值到列表控件上
LRESULT CGetGDT::OnSetGDTData(WPARAM wParam, LPARAM lParam)
{
	// 清理所有的列表项
	m_ListGDT.DeleteAllItems();

	GDTENTRY test;
	DWORD dwSize = 0;

	// 发送控制码到驱动中
	DeviceIoControl(g_hDev, GETGDT, 
		NULL, NULL, 
		&test, sizeof(GDTENTRY), 
		&dwSize, NULL);

	// 获取有效的GDT表项的个数
	int nCount = dwSize / sizeof(GDTENTRY);
	PGDTENTRY pGdt = new GDTENTRY[nCount]();
	DeviceIoControl(g_hDev, GETGDT, 
		NULL, NULL, 
		pGdt, dwSize, 
		&dwSize, NULL);

	// 将驱动返回的数据设置到列表控件中
	int i = 0;
	while (nCount)
	{
		--nCount;
		m_ListGDT.InsertItem(i, _T(""));
		CString csBuff;
		LONG Base = (pGdt[i].Base24_31 << 24) + pGdt[i].base0_23;
		csBuff.Format(L"0x%08x", Base);
		m_ListGDT.SetItemText(i, 0, csBuff); // 基址
		LONG Limit = (pGdt[i].Limit16_19 << 16) + pGdt[i].Limit0_15;
		csBuff.Format(L"0x%08x", Limit);
		m_ListGDT.SetItemText(i, 1, csBuff); // 界限
		if (pGdt[i].G == 0) 
		{
			csBuff = L"Byte";
		}
		else if (pGdt[i].G == 1) 
		{
			csBuff = L"Page";
		}
		m_ListGDT.SetItemText(i, 2, csBuff);
		csBuff.Format(L"%d", pGdt[i].DPL);
		m_ListGDT.SetItemText(i, 3, csBuff);
		// 首先判断类型
		if (pGdt[i].S == 0) // 系统段
		{
			csBuff = L"系统段";
		}
		else if (pGdt[i].S == 1)	
		{
			if (pGdt[i].TYPE >= 8)
			{
				csBuff = L"代码段";
			}
			else if (pGdt[i].TYPE < 8) 
			{
				csBuff = L"数据段";
			}
		}
		m_ListGDT.SetItemText(i, 4, csBuff);
		++i;
	}

	// 重新设置列表控件宽
	SetListColumnWidth();

	// 清理申请的堆空间
	delete[] pGdt;
	return 0;
}
