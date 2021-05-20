#pragma once


// CMyEdit

class CMyEdit : public CEdit
{
	DECLARE_DYNAMIC(CMyEdit)

public:
	CMyEdit();
	virtual ~CMyEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	// 处理按键
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};

