// CGetIDT.cpp: 实现文件
//

#include "pch.h"
#include "MyPCHunter.h"
#include "CGetIDT.h"
#include "afxdialogex.h"
#include "common.h"


// CGetIDT 对话框

IMPLEMENT_DYNAMIC(CGetIDT, CDialogEx)

CGetIDT::CGetIDT(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CGetIDT, pParent)
{

}

CGetIDT::~CGetIDT()
{
}

void CGetIDT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListIDT);
}


BEGIN_MESSAGE_MAP(CGetIDT, CDialogEx)
	ON_MESSAGE(WM_GETIDT, &CGetIDT::OnSetIDTData)
END_MESSAGE_MAP()



DWORD WINAPI IDTThreadProc(_In_ LPVOID lpParameter)
{
	Sleep(500);
	HWND hWnd = (HWND)lpParameter;
	SendMessage(hWnd, WM_GETIDT, 0, 0);
	return 0;
}

// CGetIDT 消息处理程序

// 初始化IDT窗口
BOOL CGetIDT::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化

	// 初始化控件
	m_ListIDT.SetExtendedStyle(m_ListIDT.GetExtendedStyle() |
		LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_TWOCLICKACTIVATE);

	// 设置表头
	m_ListIDT.InsertColumn(0, _TEXT("序号"));
	m_ListIDT.InsertColumn(1, _TEXT("中断地址"));
	m_ListIDT.InsertColumn(3, _TEXT("段选择子"));
	m_ListIDT.InsertColumn(4, _TEXT("类型"));
	m_ListIDT.InsertColumn(5, _TEXT("段特权级"));

	// 设置列表控件项的宽度
	SetListColumnWidth();

	// 创建线程用来处理 IDT 信息
	CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)IDTThreadProc,
		this->m_hWnd,
		0, 0);

	return TRUE;
}


void CGetIDT::SetListColumnWidth()
{
	// 获取列表视图控件使用的标题控件的句柄
	CHeaderCtrl* pHdrCtrl = m_ListIDT.GetHeaderCtrl();
	// 根据列表内容初始化列表头部的宽度
	for (int i = 0; i < pHdrCtrl->GetItemCount(); i++)
	{
		// 调整列表项的初始化宽度
		m_ListIDT.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
	}
}

// 设置GDT值到列表控件上
LRESULT CGetIDT::OnSetIDTData(WPARAM wParam, LPARAM lParam)
{
	// 清除控件上的所有数据
	m_ListIDT.DeleteAllItems();
	
	// 初始化中断表256项
	PIDTENTRY pIdt = new IDTENTRY[0x100]();
	DWORD dwSize = 0;
	
	// 发送控制码
	DeviceIoControl(g_hDev, 
		GETIDT, 
		NULL, NULL, 
		pIdt, 
		0x100 * sizeof(IDTENTRY), 
		&dwSize, NULL);

	// 将驱动返回的值设置到控件上进行显示
	for (size_t i = 0; i < 0x100; i++)
	{
		m_ListIDT.InsertItem(i, _T(""));
		CString csTmp;
		csTmp.Format(L"0x%02x", i);
		m_ListIDT.SetItemText(i, 0, csTmp); // 中断序号
		ULONG ulIDTAddr = MAKELONG(pIdt[i].uOffsetLow, pIdt[i].uOffsetHigh);
		csTmp.Format(L"0x%08x", ulIDTAddr);
		m_ListIDT.SetItemText(i, 1, csTmp);	// 中断地址
		csTmp.Format(L"%d", pIdt[i].uSelector);
		m_ListIDT.SetItemText(i, 2, csTmp);	// 段选择子
		csTmp.Format(L"%d", pIdt[i].uType);
		m_ListIDT.SetItemText(i, 3, csTmp);	// 类型
		csTmp.Format(L"%d", pIdt[i].uDpl);
		m_ListIDT.SetItemText(i, 4, csTmp); // 特权等级
	}

	// 重新设置列表控件宽
	SetListColumnWidth();

	// 清理申请的堆空间
	delete[] pIdt;

	return 0;
}
