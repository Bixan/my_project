#pragma once


// CGetSSDT 对话框

class CGetSSDT : public CDialogEx
{
	DECLARE_DYNAMIC(CGetSSDT)

public:
	CGetSSDT(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGetSSDT();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CGetSSDT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();// 初始化对话框
public:
	CListCtrl m_ListSSDT;
public:
	void SetListColumnWidth(); // 设置列表控件项的宽度
	afx_msg LRESULT OnSetSSDTData(WPARAM wParam, LPARAM lParam); // 设置IDT表数据设置到列表控件上
};
