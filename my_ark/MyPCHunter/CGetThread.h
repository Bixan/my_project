#pragma once


// CGetThread 对话框

class CGetThread : public CDialogEx
{
	DECLARE_DYNAMIC(CGetThread)

public:
	CGetThread(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGetThread();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CGetThread };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void GetThreadInfo();
	void SetListColumnWidth();
public:
	DWORD m_dwPID;
	CListCtrl m_ListThread;
};
