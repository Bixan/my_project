#pragma once
#include <iostream>
#include <stack>
#include "Packet.h"
#include "CMessages.h"
#include "MyMessages.h"

// CFileDlg 对话框
// 显示客户端计算机中的磁盘文件

class CFileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileDlg)

public:
	CFileDlg(CWnd* pParent = nullptr);	 // 标准构造函数
	virtual ~CFileDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CFileDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	CEdit m_EditFilePath; // 文件路径
	CListCtrl m_ListFile; // 文件列表
public:
	void SetMessages(CMessages* pMessages);
	afx_msg void OnDblclkListFile(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListFile(NMHDR* pNMHDR, LRESULT* pResult);
	void DoubleListCtrl(); // 双击列表控件
	afx_msg void OnDownIsFile(); // 下载的是否是文件
	afx_msg void OnUploadFile(); // 上传文件
public:
	CProgressCtrl m_ProgressDownUp;
	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:
	// 自定义消息
	afx_msg LRESULT OnGetFileDrive(WPARAM wParam, LPARAM lParam); // 获取客户端磁盘驱动
	afx_msg LRESULT OnGetPrevFolder(WPARAM wParam, LPARAM lParam); // 获取当前文件夹的上一层
	afx_msg LRESULT OnGetNextFolder(WPARAM wParam, LPARAM lParam); // 获取当前文件夹的下一层
	afx_msg LRESULT OnHavePrevFile(WPARAM wParam, LPARAM lParam); // 当前文件夹有上一层
	afx_msg LRESULT OnHaveNextFile(WPARAM wParam, LPARAM lParam); // 当前文件夹有下一层
	afx_msg LRESULT OnIsFolder(WPARAM wParam, LPARAM lParam); // 当前点击的文件是文件夹
	afx_msg LRESULT OnIsFile(WPARAM wParam, LPARAM lParam);	// 当前点击的文件是文件
	afx_msg LRESULT OnGetFileSize(WPARAM wParam, LPARAM lParam); // 获取文件大小
	afx_msg LRESULT OnGetFileData(WPARAM wParam, LPARAM lParam); // 获取文件数据
	afx_msg LRESULT OnDownFileDataSucc(WPARAM wParam, LPARAM lParam); // 下载文件数据成功
	afx_msg LRESULT OnUploadFileDataSucc(WPARAM wParam, LPARAM lParam); // 文件数据上传成功
	afx_msg LRESULT OnGetFileNextData(WPARAM wParam, LPARAM lParam); // 获取文件下一条数据
	afx_msg LRESULT OnRecvFileDataError(WPARAM wParam, LPARAM lParam); // 接受数据错误
	afx_msg LRESULT OnSendFileDataError(WPARAM wParam, LPARAM lParam); // 发送数据错误
public:
	std::stack<CString> m_stack; // 存储点击的文件路径
	CString m_csPrevFilePath; // 存储文件的上层路径
	CString m_csDownFilePath; // 存储要下载文件所在的路径
	CString m_csUploadFilePath; // 存储要上传文件所到的路径
	CString m_csSaveDownFilePath; // 保存下载文件的路径

	CMessages* m_pMessages;	// 处理消息
	LARGE_INTEGER m_LIDownFileSize;	// 存储所下载文件的字节大小
	LARGE_INTEGER m_LICurlDownFileSize;	// 存储当前所下载文件的字节大小
	int m_nIdx;	// 存储双击列表控件项的值
};
