// CMyProcess.cpp: 实现文件
//

#include "pch.h"
#include "MyPCHunter.h"
#include "CGetProcess.h"
#include "afxdialogex.h"
#include "common.h"
#include "resource.h"
#include "CGetThread.h"

// CMyProcess 对话框

IMPLEMENT_DYNAMIC(CGetProcess, CDialogEx)

CGetProcess::CGetProcess(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CMyProcess, pParent)
{

}

CGetProcess::~CGetProcess()
{
}

void CGetProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROCESS, m_ListProcess);
}


BEGIN_MESSAGE_MAP(CGetProcess, CDialogEx)
	ON_MESSAGE(WM_GETPROCESS, &CGetProcess::OnSetProcessData)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_PROCESS, &CGetProcess::OnRclickListProcess)
	ON_COMMAND(ID_32781, &CGetProcess::OnGetThreadInfo)
	ON_COMMAND(ID_32782, &CGetProcess::OnTerminateProcess)
END_MESSAGE_MAP()

DWORD WINAPI ProcessThreadProc(_In_ LPVOID lpParameter)
{
	Sleep(500);
	HWND hWnd = (HWND)lpParameter;
	SendMessage(hWnd, WM_GETPROCESS, 0, 0);
	return 0;
}
// CMyProcess 消息处理程序


BOOL CGetProcess::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 设置表头风格
	m_ListProcess.SetExtendedStyle(m_ListProcess.GetExtendedStyle() |
		LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_TWOCLICKACTIVATE);

	// 设置表头
	m_ListProcess.InsertColumn(0, L"映像名称");
	m_ListProcess.InsertColumn(1, L"进程ID");
	m_ListProcess.InsertColumn(2, L"父进程ID");
	m_ListProcess.InsertColumn(3, L"映像路径名称");

	// 设置列表控件项的宽度
	SetListColumnWidth();

	// 加载右键菜单
	m_ProcMenu.LoadMenuW(IDR_MENU_PROCESS);

	// 创建线程发送消息
	CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)ProcessThreadProc,
		this->m_hWnd,
		0, 0);

	return TRUE;
}

// 设置列表控件项的宽度
void CGetProcess::SetListColumnWidth()
{
	// 获取列表视图控件使用的标题控件的句柄
	CHeaderCtrl* pHdrCtrl = m_ListProcess.GetHeaderCtrl();
	// 根据列表内容初始化列表头部的宽度
	for (int i = 0; i < pHdrCtrl->GetItemCount(); i++)
	{
		// 调整列表项的初始化宽度
		m_ListProcess.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
	}
}

// 将进程数据设置到列表控件上
LRESULT CGetProcess::OnSetProcessData(WPARAM wParam, LPARAM lParam)
{
	// 清除列表中所有数据
	m_ListProcess.DeleteAllItems();

	PROCESS ps;
	DWORD dwSize = 0; // 保存返回的大小

	// 先发出一次请求，返回所需要的大小。
	DeviceIoControl(g_hDev,
		GETPROCESS,
		NULL, NULL,
		&ps, sizeof(PROCESS),
		&dwSize, NULL);

	// 根据返回来的大小重新申请空间
	PPROCESS pProcess = new PROCESS[dwSize]();
	DeviceIoControl(g_hDev,
		GETPROCESS,
		pProcess, dwSize,
		pProcess, dwSize,
		&dwSize, NULL);

	// 获取有效的进程数量
	int nCount = dwSize / sizeof(PROCESS);
	int i = 0;
	int nIdx = 0;
	while (nCount)
	{
		--nCount;
		USES_CONVERSION;
		// 转换字符
		CString csBuff = A2W(pProcess[i].szProcessName);
		if (csBuff.IsEmpty())
		{
			++i;
			continue;
		}
		m_ListProcess.InsertItem(nIdx, _T(""));
		CString csTmp;
		m_ListProcess.SetItemText(nIdx, 0, csBuff);
		csTmp.Format(L"%d", pProcess[i].dwPID);
		m_ListProcess.SetItemText(nIdx, 1, csTmp);
		csTmp.Format(L"%d", pProcess[i].dwPPID);
		m_ListProcess.SetItemText(nIdx, 2, csTmp);
		if (csBuff == L"System")
		{
			m_ListProcess.SetItemText(nIdx, 3, L"System");
		}
		else
		{
			// 将 \Device\HarddiskVolume2 替换为 "C:\"
			CString csTmp = pProcess[i].wszProcessPath;
			csTmp.Replace(L"\\Device\\HarddiskVolume2\\", L"C:\\");
			m_ListProcess.SetItemText(nIdx, 3, csTmp);
		}
		++nIdx;
		++i;
	}

	SetListColumnWidth();

	// 清除申请的空间
	delete[] pProcess;
	return 0;
}

// 右键列表控件弹出菜单
void CGetProcess::OnRclickListProcess(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// 获取所选进程 PID
	CString csTmp = m_ListProcess.GetItemText(pNMItemActivate->iItem, 1);
	m_dwPID = _wtoi(csTmp.GetBuffer());

#if 0
	CString cs;
	cs.Format(L"%d", m_dwPID);
	MessageBox(cs);
#endif // 0

	//获取鼠标坐标
	CPoint ptCurSel(0);
	GetCursorPos(&ptCurSel);

	// 获取子菜单
	CMenu* SubMenu = m_ProcMenu.GetSubMenu(0);

	// 弹出菜单窗口
	SubMenu->TrackPopupMenu(TPM_LEFTALIGN, ptCurSel.x, ptCurSel.y, this);
	*pResult = 0;
}

// 显示线程信息
void CGetProcess::OnGetThreadInfo()
{
	CGetThread ThreadObj;
	ThreadObj.m_dwPID = m_dwPID;
	ThreadObj.DoModal();
}

// 结束进程
void CGetProcess::OnTerminateProcess()
{
	DWORD dwSize = 0;
	DeviceIoControl(g_hDev, 
		TERMINATEPROCESS, 
		&m_dwPID, 
		sizeof(DWORD), 
		NULL, 
		NULL, 
		&dwSize, 
		NULL);
}
