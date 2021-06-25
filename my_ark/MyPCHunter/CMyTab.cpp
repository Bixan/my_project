// CMyTab.cpp: 实现文件
//

#include "pch.h"
#include "MyPCHunter.h"
#include "CMyTab.h"


// CMyTab

IMPLEMENT_DYNAMIC(CMyTab, CTabCtrl)

CMyTab::CMyTab()
{
	m_dwTabNum = 6;
}

CMyTab::~CMyTab()
{
}


BEGIN_MESSAGE_MAP(CMyTab, CTabCtrl)
END_MESSAGE_MAP()



// CMyTab 消息处理程序


