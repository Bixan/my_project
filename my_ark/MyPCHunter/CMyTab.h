#pragma once


// CMyTab

class CMyTab : public CTabCtrl
{
	DECLARE_DYNAMIC(CMyTab)

public:
	CMyTab();
	virtual ~CMyTab();
public:
	DWORD m_dwTabNum; // 存储Tab项的数量
	CDialogEx* m_DialogObj[6]; // 存储对话框对象
protected:
	DECLARE_MESSAGE_MAP()
};


