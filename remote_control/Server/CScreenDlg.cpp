// CScreenDlg.cpp: 实现文件
//
#include "pch.h"
#include "Server.h"
#include "CScreenDlg.h"
#include "afxdialogex.h"
#include "zlib.h"
#pragma comment(lib, "zlib.lib")

CONST DWORD g_KeyMask = 0x80000000;
// CScreenDlg 对话框

IMPLEMENT_DYNAMIC(CScreenDlg, CDialogEx)

CScreenDlg::CScreenDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CScreenDlg, pParent)
{

}

CScreenDlg::~CScreenDlg()
{
}

void CScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CScreenDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_RECV_DATA_ERROR, &CScreenDlg::OnRecvDataError)
	ON_MESSAGE(WM_SEND_DATA_ERROR, &CScreenDlg::OnSendDataError)
	ON_MESSAGE(WM_CLIENT_SCREEN_DATA, &CScreenDlg::OnGetClientScreenData)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CScreenDlg 消息处理程序


BOOL CScreenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:	在此添加额外的初始化
	m_pMessages->SetHwnd(GetSafeHwnd());

	// 获取客户端屏幕数据
	PACKETHEAD ph;
	ph.m_nCmd = PK_CLIENT_SCREEN_DATA;
	ph.m_nLen = 0;

	if (!m_pMessages->SendData((char*)&ph, sizeof(ph)))
	{
		MessageBox("发送数据失败", "提示", MB_OK);
		return 0;
	}
	return TRUE;	// return TRUE unless you set the focus to a control
					// 异常: OCX 属性页应返回 FALSE
}


void CScreenDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}

void CScreenDlg::SetMessages(CMessages* pMessages)
{
	m_pMessages = pMessages;
}

// 接收屏幕数据错误
LRESULT CScreenDlg::OnRecvDataError(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox("[Server]: 接收屏幕数据失败");
	return 0;
}

// 发送数据错误
LRESULT CScreenDlg::OnSendDataError(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox("[Server]: 发送数据失败");
	return 0;
}

// 获取客户端屏幕数据
LRESULT CScreenDlg::OnGetClientScreenData(WPARAM wParam, LPARAM lParam)
{
	PSCREENINFO pScreenInfo = (PSCREENINFO)wParam;
	char* pScreenData = (char*)lParam;

	// 解压文件
	uLongf outSize;
	char* pUncompress = new char[pScreenInfo->m_SrcSize];

	// 解压缩
	uncompress((unsigned char*)pUncompress,
		&outSize,
		(unsigned char*)pScreenData,
		pScreenInfo->m_CompressedSize);

	HDC hDc = ::GetDC(GetSafeHwnd());
	HDC hMemory = ::CreateCompatibleDC(hDc);

	// 创建兼容位图
	HBITMAP hBitmap = ::CreateCompatibleBitmap(hDc,
		pScreenInfo->m_nWndWidth,
		pScreenInfo->m_nWndHeight);

	::SelectObject(hMemory, hBitmap);

	// 设置位图的像素颜色
	::SetBitmapBits(hBitmap, outSize, pUncompress);

	// 创建矩形区域
	CRect rc;
	GetClientRect(&rc);

	// 等比缩放
	StretchBlt(hDc, 0, 0, rc.Width(), rc.Height(), hMemory, 0, 0,
		pScreenInfo->m_nWndWidth, pScreenInfo->m_nWndHeight, SRCCOPY);

	// 删除数据
	delete[] pUncompress;
	delete[] pScreenData;
	delete pScreenInfo;

	// 清理资源
	::DeleteObject(hBitmap);
	::DeleteDC(hMemory);
	::ReleaseDC(GetSafeHwnd(), hDc);

	// 获取下一个屏幕数据
	PACKETHEAD ph;
	ph.m_nCmd = PK_CLIENT_SCREEN_DATA;
	ph.m_nLen = 0;

	if (!m_pMessages->SendData((char*)&ph, sizeof(ph)))
	{
		MessageBox("发送数据失败", "提示", MB_OK);
		return 0;
	}

	return 0;
}


// 鼠标左键双击
void CScreenDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnLButtonDblClk(nFlags, point);

	CString str;
	str.Format(TEXT("%d-%d"), point.x, point.y);
	::SetWindowText(m_hWnd, str);

	PACKETHEAD Mouseph;
	Mouseph.m_nCmd = PK_MOUSE_BUTTON_DBLCLK;
	Mouseph.m_nLen = str.GetLength() + 1;

	// 发送数据长度
	if (!m_pMessages->SendData((char*)&Mouseph, sizeof(Mouseph)))
	{
		AfxMessageBox("发送数据长度失败");
		return;
	}

	// 发送数据信息
	if (!m_pMessages->SendData(str.GetBuffer(), Mouseph.m_nLen))
	{
		AfxMessageBox("发送数据失败");
		return;
	}
}

// 左键按下
void CScreenDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnLButtonDown(nFlags, point);
	CString str;
	str.Format(TEXT("%d-%d"), point.x, point.y);
	::SetWindowText(m_hWnd, str);

	PACKETHEAD Mouseph;
	Mouseph.m_nCmd = PK_MOUSE_BUTTON_DOWN;
	Mouseph.m_nLen = str.GetLength() + 1;

	// 发送数据长度
	if (!m_pMessages->SendData((char*)&Mouseph, sizeof(Mouseph)))
	{
		AfxMessageBox("发送数据长度失败");
		return;
	}

	// 发送数据信息
	if (!m_pMessages->SendData(str.GetBuffer(), Mouseph.m_nLen))
	{
		AfxMessageBox("发送数据失败");
		return;
	}
}


BOOL CScreenDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		UCHAR vKey = (UCHAR)pMsg->wParam;
		DWORD iShift = GetKeyState(VK_SHIFT);
		DWORD iCapital = GetKeyState(VK_CAPITAL);
		BOOL bShift = (iShift & g_KeyMask) == g_KeyMask;
		BOOL bCapital = (iCapital & 1) == 1;
		char szBuff[2] = { 0 }; // 存储按键内容
		PACKETHEAD keyph;
		// 顶部数字键
		if (vKey >= '0' && vKey <= '9')
		{
			szBuff[0] = vKey;
			keyph.m_nCmd = PK_KEYBOARD_INPUT;
			keyph.m_nLen = strlen(szBuff) + 1;

			// 发送数据长度
			if (!m_pMessages->SendData((char*)&keyph, sizeof(keyph)))
			{
				AfxMessageBox("发送按键键盘长度失败");
				return FALSE;
			}

			// 发送数据信息
			if (!m_pMessages->SendData(szBuff, keyph.m_nLen))
			{
				AfxMessageBox("发送按键数据失败");
				return FALSE;
			}
			return TRUE;
		}
		// 字母键
		if (vKey >= 'A' && vKey <= 'Z')
		{
			if (bShift || bCapital)
			{
				// 大写按键
				szBuff[0] = vKey;
				keyph.m_nCmd = PK_KEYBOARD_INPUT;
				keyph.m_nLen = strlen(szBuff) + 1;

				// 发送数据长度
				if (!m_pMessages->SendData((char*)&keyph, sizeof(keyph)))
				{
					AfxMessageBox("发送按键键盘长度失败");
					return FALSE;
				}

				// 发送数据信息
				if (!m_pMessages->SendData(szBuff, keyph.m_nLen))
				{
					AfxMessageBox("发送按键数据失败");
					return FALSE;
				}
			}
			else
			{
				// 小写字母
				szBuff[0] = vKey + 32;
				keyph.m_nCmd = PK_KEYBOARD_INPUT;
				keyph.m_nLen = strlen(szBuff) + 1;

				// 发送数据长度
				if (!m_pMessages->SendData((char*)&keyph, sizeof(keyph)))
				{
					AfxMessageBox("发送按键键盘长度失败");
					return FALSE;
				}

				// 发送数据信息
				if (!m_pMessages->SendData(szBuff, keyph.m_nLen))
				{
					AfxMessageBox("发送按键数据失败");
					return FALSE;
				}
			}
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
