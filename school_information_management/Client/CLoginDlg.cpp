// CLoginDlg.cpp: 实现文件
//

#include "pch.h"
#include "Client.h"
#include "CLoginDlg.h"
#include "afxdialogex.h"
#include "ClientDlg.h"

// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLoginDlg, pParent)
	, m_csUserName(_T(""))
	, m_csPassword(_T(""))
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDE_USER_NAME, m_csUserName);
	DDX_Text(pDX, IDE_PASSWORD, m_csPassword);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLoginDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_SHOW_PASSWORD, &CLoginDlg::OnBnClickedCheckShowPassword)
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序


// 登录
void CLoginDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (m_csUserName.IsEmpty() || m_csPassword.IsEmpty()) {
		AfxMessageBox("账号或者密码不能为空");
		return;
	}

	// 检查账号密码 -- 获取父窗口
	CClientDlg* pParent = (CClientDlg*)GetParent();

	CString csSQL;
	csSQL.Format("SELECT * FROM T_USER WHERE USER_NAME = '%s' AND PASSWORD ='%s'",
		m_csUserName, m_csPassword);

	if (mysql_real_query(&pParent->m_mysql, 
		csSQL.GetBuffer(0), 
		csSQL.GetLength() + 1) != 0)
	{
		AfxMessageBox(mysql_error(&pParent->m_mysql));
		return;
	}

	// 保存结果
	MYSQL_RES* result = mysql_store_result(&pParent->m_mysql);
	if (mysql_num_rows(result) != 1)
	{
		AfxMessageBox("账号或者密码错误");
		mysql_free_result(result);
		return;
	}
	// 释放
	mysql_free_result(result);
	CDialogEx::OnOK();
}


void CLoginDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

// 显示密码
void CLoginDlg::OnBnClickedCheckShowPassword()
{
	if (IsDlgButtonChecked(IDC_CHECK_SHOW_PASSWORD) == BST_CHECKED)
	{
		UpdateData(true);
		CEdit* pEdit = (CEdit*)(this)->GetDlgItem(IDE_PASSWORD);
		pEdit->SetPasswordChar(0);
		SetDlgItemText(IDE_PASSWORD, m_csPassword);
		// 显示输入的密码
		UpdateData(false);
	}
	else
	{
		// 恢复
		UpdateData(true);
		CEdit* pEdit = (CEdit*)(this)->GetDlgItem(IDE_PASSWORD);
		pEdit->SetPasswordChar('*');
		SetDlgItemText(IDE_PASSWORD, m_csPassword);
		UpdateData(false);
	}
	GetDlgItem(IDE_PASSWORD)->SetFocus();
}
