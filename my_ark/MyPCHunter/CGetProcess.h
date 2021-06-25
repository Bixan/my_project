#pragma once


// CMyProcess 对话框

class CGetProcess : public CDialogEx
{
	DECLARE_DYNAMIC(CGetProcess)

public:
	CGetProcess(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGetProcess();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMyProcess };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog(); // 初始化
public:
	CListCtrl m_ListProcess;
	CMenu m_ProcMenu;
	DWORD m_dwPID;
public:
	void SetListColumnWidth(); // 设置列表控件项的宽度
	afx_msg LRESULT OnSetProcessData(WPARAM wParam, LPARAM lParam); // 将进程数据设置到列表控件上
	afx_msg void OnRclickListProcess(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetThreadInfo(); // 获取线程
	afx_msg void OnTerminateProcess(); // 结束进程
};