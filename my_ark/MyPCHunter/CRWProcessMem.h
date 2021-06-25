#pragma once


// CRWProcessMem 对话框

class CRWProcessMem : public CDialogEx
{
	DECLARE_DYNAMIC(CRWProcessMem)

public:
	CRWProcessMem(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CRWProcessMem();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CRWProcessMem };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit m_PID; // 目标进程PID
	CEdit m_RWAddr; // 操作进程中的目标地址
	CEdit m_ByteSize; // 读、写字节数
	CString m_csRWBuff; // 保存读取写入的内存地址
	DWORD m_dwByteSize; // 读取或者修改的字节数
	DWORD m_dwPID; // 保存当前选择进程的PID
public:
	afx_msg void OnBnClickedRead();
	afx_msg void OnBnClickedWrite();
	char* CStringToChar(CString cs);
	CString CharToCString(char* pBuff);
};
