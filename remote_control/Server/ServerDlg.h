
// ServerDlg.h: 头文件
//

#pragma once
#include "CMessages.h"
#include "Packet.h"

// CServerDlg 对话框
class CServerDlg : public CDialogEx
{
// 构造
public:
	CServerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_DIALOG };
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

private:
	void SetListCtrlColumn();	 // 设置客户端信息
	void SetListCtrlText(SOCKET socket, PSOCKADDR_IN PSI);// 客户端连接成功，更新控件

private:
	// 生成的消息映射函数	-- 自定义消息
	afx_msg LRESULT OnCreateScoketErr(WPARAM wParam, LPARAM lParam);	// 创建socket失败
	afx_msg LRESULT OnBindScoketErr(WPARAM wParam, LPARAM lParam);	// 绑定socket失败
	afx_msg LRESULT OnListenScoketErr(WPARAM wParam, LPARAM lParam);	// 监听socket失败
	afx_msg LRESULT OnAcceptSucceed(WPARAM wParam, LPARAM lParam);	// 客户端连接成功
	afx_msg LRESULT OnAcceptFailure(WPARAM wParam, LPARAM lParam);	// 客户端连接失败
	afx_msg LRESULT OnRecvDataError(WPARAM wParam, LPARAM lParam);	// 接受客户端数据失败
	afx_msg LRESULT OnSendDataError(WPARAM wParam, LPARAM lParam);	// 发送数据失败
	afx_msg LRESULT OnClientQuit(WPARAM wParam, LPARAM lParam);	// 客户端退出
public:
	CListCtrl m_ListOnlineClient;	// 在线客户端
	CButton m_btnOpenServer;	// 开启服务器
private:
	CMenu m_Menu;	// 用于加载菜单
	CStatusBar m_StatusBar;	// 用于加载状态条
	CMessages m_Messages;	// 处理消息
	CEdit m_ServerLog;	// 服务器日志控件
	CString m_csServerLog;	// 服务器日志
public:
	afx_msg void OnBnClickedBtnOpenServer();	// 启动服务器
	afx_msg void OnBnClickedBtnCloseServer();	// 关闭服务器
	afx_msg void OnBnClickedBtnOpenCmd();	// 打开客户端命令行
	afx_msg void OnBnClickedBtnOpenDiskFile();	// 打开客户端磁盘文件
	afx_msg void OnBnClickedBtniOpenScreen();		// 查看客户端屏幕
	afx_msg void OnClose();
};
