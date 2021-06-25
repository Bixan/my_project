#pragma once


// CMyGDT 对话框

class CGetGDT : public CDialogEx
{
	DECLARE_DYNAMIC(CGetGDT)

public:
	CGetGDT(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGetGDT();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMyGDT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog(); // 初始化对话框
public:
	CListCtrl m_ListGDT;
protected:
	void SetListColumnWidth(); // 设置列表控件项的宽度
	afx_msg LRESULT OnSetGDTData(WPARAM wParam, LPARAM lParam); // 设置GDT值到列表控件上
};
