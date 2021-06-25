#pragma once
#include "CMessages.h"
#include "Packet.h"
#include "MyMessages.h"


// CScreenDlg 对话框

class CScreenDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CScreenDlg)

public:
	CScreenDlg(CWnd* pParent = nullptr);	 // 标准构造函数
	virtual ~CScreenDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CScreenDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CMessages* m_pMessages;
public:
	virtual BOOL OnInitDialog(); // 初始化对话框
	afx_msg void OnClose();
	void SetMessages(CMessages* pMessages); // 设置消息
	afx_msg LRESULT OnRecvDataError(WPARAM wParam, LPARAM lParam); // 接收客户端屏幕数据错误
	afx_msg LRESULT OnSendDataError(WPARAM wParam, LPARAM lParam); // 发送数据错误
	afx_msg LRESULT OnGetClientScreenData(WPARAM wParam, LPARAM lParam); // 获取客户端屏幕数据

	// 鼠标左键双击
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
