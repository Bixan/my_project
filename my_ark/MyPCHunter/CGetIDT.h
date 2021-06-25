#pragma once


// CGetIDT 对话框

class CGetIDT : public CDialogEx
{
	DECLARE_DYNAMIC(CGetIDT)

public:
	CGetIDT(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGetIDT();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CGetIDT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListIDT;
public:
	virtual BOOL OnInitDialog(); // 初始化对话框
protected:
	void SetListColumnWidth(); // 设置列表控件项的宽度
	afx_msg LRESULT OnSetIDTData(WPARAM wParam, LPARAM lParam); // 设置IDT表数据设置到列表控件上
};
