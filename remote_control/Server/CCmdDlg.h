#pragma once
#include "CMessages.h"
#include "CMyEdit.h"
// CCmdDlg 对话框

class CCmdDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCmdDlg)

public:
	CCmdDlg(CWnd* pParent = nullptr); // 标准构造函数
	virtual ~CCmdDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CCmdDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();

public:
	void SetMessages(CMessages* pMessages);	// 设置消息
	void GetCommandSendServer(); // 将输入的命令发送给服务器
	BOOL IsStrDel(); // 判断控件上的字符串是否可以删除
public:
	// 生成的消息映射函数
	afx_msg LRESULT OnGetCommandData(WPARAM wParam, LPARAM lParam); // 获取命令数据
	afx_msg LRESULT OnRecvDataError(WPARAM wParam, LPARAM lParam); // 获取数据错误
	afx_msg LRESULT OnSendDataError(WPARAM wParam, LPARAM lParam); // 发送数据错误
public:
	// 处理按键消息
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnChangeEditMyCmd();
public:
	CMyEdit m_EditCmd;
private:
	CMessages* m_pMessages; // 收发消息
	CString m_csInput; // 存储输入的数据
	CString m_csOutPut; // 存储输出的数据
};
