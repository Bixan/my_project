// CCmdDlg.cpp: 实现文件
//

#include "pch.h"
#include "Server.h"
#include "CCmdDlg.h"
#include "afxdialogex.h"
#include "MyMessages.h"

// CCmdDlg 对话框

IMPLEMENT_DYNAMIC(CCmdDlg, CDialogEx)

CCmdDlg::CCmdDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CCmdDlg, pParent)
{

}

CCmdDlg::~CCmdDlg()
{
}

void CCmdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CMD, m_EditCmd);
}


BEGIN_MESSAGE_MAP(CCmdDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_GET_CMD_DATA, &CCmdDlg::OnGetCommandData)
	ON_MESSAGE(WM_RECV_DATA_ERROR, &CCmdDlg::OnRecvDataError)
	ON_MESSAGE(WM_SEND_DATA_ERROR, &CCmdDlg::OnSendDataError)
	ON_EN_CHANGE(IDC_EDIT_CMD, &CCmdDlg::OnChangeEditMyCmd)
END_MESSAGE_MAP()


// CCmdDlg 消息处理程序

// 初始化
BOOL CCmdDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:	在此添加额外的初始化
	// 设置窗口句柄
	m_pMessages->SetHwnd(GetSafeHwnd());

	// 初始化
	PACKETHEAD ph;
	ph.m_nCmd = PK_START_CMD;
	ph.m_nLen = 0;
	// 发送数据
	if (!m_pMessages->SendData((char*)&ph, sizeof(ph)))
	{
		AfxMessageBox("[Server]: SendData Error");
		return 0;
	}

	return TRUE; // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CCmdDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();

	// 关闭窗口，发送退出命令
	PACKETHEAD ph;
	ph.m_nCmd = PK_QUIT_CMD;
	ph.m_nLen = 0;
	// 发送数据
	if (!m_pMessages->SendData((char*)&ph, sizeof(ph)))
	{
		AfxMessageBox("[Server]: SendData Error");
	}
}

void CCmdDlg::SetMessages(CMessages* pMessages)
{
	m_pMessages = pMessages;
}

// 将输入的命令发送给服务器
void CCmdDlg::GetCommandSendServer()
{
	// 获取控件上输入的命令
	CString csBuff;
	m_EditCmd.GetWindowText(csBuff);

	// 提取字符串
	DWORD dwBytes = 0;
	m_csInput = csBuff.Mid(m_csOutPut.GetLength());
	if (m_csInput.IsEmpty())
	{
		return;
	}
	m_csInput += "\r\n";

	// 将输入的命令发送给客户端
	// 发送数据包
	PACKETHEAD ph;
	ph.m_nCmd = PK_INPUT_CMD_ORDER;
	ph.m_nLen = m_csInput.GetLength();

	// 发送数据长度
	if (!m_pMessages->SendData((char*)&ph, sizeof(ph)))
	{
		AfxMessageBox("[Server]: 发送cmd命令长度失败");
		return;
	}

	// 发送数据
	if (!m_pMessages->SendData(m_csInput.GetBuffer(), ph.m_nLen))
	{
		AfxMessageBox("[Server]: 发送cmd命令数据失败");
		return;
	}
}

/*
判断控件上的字符串是否可以删除
返回值:
	TRUE 可以删除
	FALSE 不可以删除
*/
BOOL CCmdDlg::IsStrDel()
{
	// 获取控件上输入的字符串
	CString csBuff;
	m_EditCmd.GetWindowText(csBuff);
	if (csBuff == m_csOutPut)
	{
		// 控件上的数据是输出数据
		return FALSE;
	}
	// 控件上的数据是输入数据
	return TRUE;
}

// 通过输入的命令获取对应的数据
LRESULT CCmdDlg::OnGetCommandData(WPARAM wParam, LPARAM lParam)
{
	char* pBuff = (char*)wParam;
	//判断接收的数据是否与刚输入的数据相同，如果相同，舍弃
	if (strcmp(pBuff, m_csInput.GetBuffer()) == 0)
	{
		delete[] pBuff;	// 释放缓冲区
		return 0;
	}

	// 输入的数据是否为空
	if (!m_csInput.IsEmpty())
	{
		m_EditCmd.ReplaceSel("\r\n");
		m_csInput.Empty(); // 释放CString对象
	}

	// 将接收到的数据设到Edit控件上
	m_EditCmd.ReplaceSel(pBuff);

	// 释放申请的空间
	delete[] pBuff;

	// 获取编辑框中的字符串
	m_EditCmd.GetWindowText(m_csOutPut);

	return 0;
}

LRESULT CCmdDlg::OnRecvDataError(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox("[Server]: 读取数据失败");
	return 0;
}

LRESULT CCmdDlg::OnSendDataError(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox("[Server]: 发送数据失败");
	return 0;
}


BOOL CCmdDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN &&
		pMsg->hwnd == m_EditCmd.GetSafeHwnd())
	{
		switch (pMsg->wParam)
		{
		case VK_BACK:
		{
			// Backspace 按键，输入的数据是否可以删除
			if (!IsStrDel())
			{
				return TRUE;
			}
			break;
		}
		case VK_RETURN:
		{
			// 回车键，获取输入的命令发送给服务器
			GetCommandSendServer();
			return TRUE;
		}
		default:
			break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

// 更新控件数据
void CCmdDlg::OnChangeEditMyCmd()
{
	// 获取输入的命令
	CString csBuff;
	m_EditCmd.GetWindowText(csBuff);
	if (csBuff.GetLength() == HIWORD(m_EditCmd.GetSel()))
	{
		m_EditCmd.SetSel(-1);
		m_EditCmd.SetFocus();
	}
	else
	{
		// 将通过命令获取到的数据添加到控件上
		m_EditCmd.SetWindowText(m_csOutPut);
		m_EditCmd.SetSel(-1);
		m_EditCmd.SetFocus();
	}
}
