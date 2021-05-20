
// ClientDlg.h: 头文件
//

#pragma once

#include "../common/CUMTSocket.h"

// CClientDlg 对话框
class CClientDlg : public CDialogEx
{
// 构造
public:
	CClientDlg(CWnd* pParent = nullptr);	// 标准构造函数
	MYSQL m_mysql;
	int InitMySQL();	// 初始化MySQL
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
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

public:
	afx_msg void OnDestroy();
	void UpdateListInfo();	// 更新列表信息
	void ShowTableData(const char* pszBuff);
	void InitSocket();	// 初始化socket
	void ShowQueryData(CString cs);
public:
	// 学生表
	CString m_csStuID;	// 学生ID
	CEdit m_Edit_StuID;
	CString m_csStuName;	// 学生姓名
	CEdit m_Edit_StuName;
	CString m_csClass;	// 学生所在班级
	CEdit m_Edit_StuAtClassID;

	CListCtrl m_ListShowQueryInfo;	// 显示查询的信息列表
	CComboBox m_Tables;
	void InitTables();	// 初始化combo tables 控件
public:
	CUMTSocket m_socket;	// UDP coket
	afx_msg void OnBnClickedIncrease();	// 增加
	afx_msg void OnBnClickedDelete();	// 删除
	afx_msg void OnBnClickedRevise();	// 修改
	afx_msg void OnBnClickedQuery();	// 查询
	afx_msg void OnDblclkIdlInfo(NMHDR* pNMHDR, LRESULT* pResult);
public:
	// 班级
	CString m_csClassID;	// 班级ID
	CEdit m_Edit_ClassID;
	CString m_csClassName;	// 班级名称
	CEdit m_Edit_ClassName;
public:
	// 课程
	CString m_csCourseID;	// 课程ID
	CEdit m_Edit_CourseID;
	CString m_csCourseName;	// 课程名
	CEdit m_Edit_CourseName;
public:
	// 选课表
	CString m_csSelectStuID;	// 选课表_学生ID
	CEdit m_Edit_SelectStuID;
	CString m_csSelectCourseID;	// 选课表_课程ID
	CEdit m_Edit_SelectCourseID;
	CString m_csNewCourseID;	// 新课程 ID
	afx_msg void OnClose();
};