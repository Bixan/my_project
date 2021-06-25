
// MyPCHunterDlg.h: 头文件
//

#pragma once

#include "CMyTab.h"
#include <winsvc.h>

// CMyPCHunterDlg 对话框
class CMyPCHunterDlg : public CDialogEx
{
// 构造
public:
	CMyPCHunterDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYPCHUNTER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
public:
	void LoadDriver(); // 加载驱动
	void StartDriver(); // 启动驱动
	void StopDriver(); // 停止驱动
	void UnLoadDriver(); // 卸载驱动
public:
	CMyTab m_TabCtrl;
	CString m_Path; // 保存服务路径
	SC_HANDLE m_hSCManager; // 保存服务管理器句柄
	SC_HANDLE m_hService; // 保存创建服务对象的句柄
	DWORD m_dwLastError; // 保存错误码
	afx_msg void OnDestroy();
};
