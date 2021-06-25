// CGetDrvModule.cpp: 实现文件
//

#include "pch.h"
#include "MyPCHunter.h"
#include "CGetDrvModule.h"
#include "afxdialogex.h"
#include "common.h"

// CGetDrvModule 对话框

IMPLEMENT_DYNAMIC(CGetDrvModule, CDialogEx)

CGetDrvModule::CGetDrvModule(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CGetDrvModule, pParent)
{

}

CGetDrvModule::~CGetDrvModule()
{
}

void CGetDrvModule::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListDrvModule);
}


BEGIN_MESSAGE_MAP(CGetDrvModule, CDialogEx)
	ON_MESSAGE(WM_GETDRVMODULE, &CGetDrvModule::OnSetDrvModuleData)
END_MESSAGE_MAP()

DWORD WINAPI DrvModuleThreadProc(_In_ LPVOID lpParameter)
{
	Sleep(500);
	HWND hWnd = (HWND)lpParameter;
	SendMessage(hWnd, WM_GETDRVMODULE, 0, 0);

	return 0;
}
// CGetDrvModule 消息处理程序


BOOL CGetDrvModule::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 设置表头风格
	m_ListDrvModule.SetExtendedStyle(m_ListDrvModule.GetExtendedStyle() |
		LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_TWOCLICKACTIVATE);

	// 设置表头
	m_ListDrvModule.InsertColumn(0, L"序号");
	m_ListDrvModule.InsertColumn(1, L"驱动名");
	m_ListDrvModule.InsertColumn(2, L"基地址");
	m_ListDrvModule.InsertColumn(3, L"大小");
	m_ListDrvModule.InsertColumn(4, L"驱动路径");

	// 设置列表控件项的宽度
	SetListColumnWidth();

	// 创建线程用来处理 SSDT 信息
	CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)DrvModuleThreadProc,
		this->m_hWnd,
		0, 0);

	return TRUE;
}

// 设置列表控件项的宽度
void CGetDrvModule::SetListColumnWidth()
{
	// 获取列表视图控件使用的标题控件的句柄
	CHeaderCtrl* pHdrCtrl = m_ListDrvModule.GetHeaderCtrl();
	// 根据列表内容初始化列表头部的宽度
	for (int i = 0; i < pHdrCtrl->GetItemCount(); i++)
	{
		// 调整列表项的初始化宽度
		m_ListDrvModule.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
	}
}

// 设置驱动模块信息到列表控件上
LRESULT CGetDrvModule::OnSetDrvModuleData(WPARAM wParam, LPARAM lParam)
{
	// 清除控件上的所有数据
	m_ListDrvModule.DeleteAllItems();

	DRVMODULE drv;
	DWORD dwSize = 0;
	// 获取有效的驱动模块项数量
	DeviceIoControl(g_hDev, 
		GETDRVMODULE, 
		NULL, 
		NULL, 
		&drv, 
		sizeof(DRVMODULE), 
		&dwSize, 
		NULL);

	// 根据返回来的大小申请对应的内存空间
	PDRVMODULE pDriver = new DRVMODULE[dwSize]();

	// 获取驱动模块信息
	DeviceIoControl(g_hDev, 
		GETDRVMODULE, 
		pDriver, 
		dwSize, 
		pDriver, 
		dwSize, 
		&dwSize, 
		NULL);

	// 获取有效的驱动模块项数量
	int nCount = dwSize / sizeof(DRVMODULE);
	int i = 0;
	int nIndex = 0;
	while (nCount)
	{
		--nCount;
		CString csTmp = pDriver[i].wszNameBuff;
		if (csTmp.IsEmpty())
		{
			++i;
			continue;
		}
		m_ListDrvModule.InsertItem(nIndex, _T(""));
		CString Temp;
		Temp.Format(L"%d", nIndex + 1);
		m_ListDrvModule.SetItemText(nIndex, 0, Temp);
		m_ListDrvModule.SetItemText(nIndex, 1, csTmp);
		Temp.Format(L"0x%08x", pDriver[i].dwDllBase);
		m_ListDrvModule.SetItemText(nIndex, 2, Temp);
		Temp.Format(L"0x%08x", pDriver[i].dwSize);
		m_ListDrvModule.SetItemText(nIndex, 3, Temp);
		m_ListDrvModule.SetItemText(nIndex, 4, pDriver[i].FullDllName);
		++i;
		++nIndex;
	}

	// 设置列表控件项的宽度
	SetListColumnWidth();

	// 清理内存
	delete[] pDriver;
	return 0;
}