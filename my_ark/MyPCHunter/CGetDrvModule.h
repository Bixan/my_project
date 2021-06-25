#pragma once


// CGetDrvModule 对话框

class CGetDrvModule : public CDialogEx
{
	DECLARE_DYNAMIC(CGetDrvModule)

public:
	CGetDrvModule(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGetDrvModule();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CGetDrvModule };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog(); // 初始化
public:
	CListCtrl m_ListDrvModule;
protected:
	void SetListColumnWidth(); // 设置列表控件项的宽度
	afx_msg LRESULT OnSetDrvModuleData(WPARAM wParam, LPARAM lParam); // 设置驱动模块信息到列表控件上
};
