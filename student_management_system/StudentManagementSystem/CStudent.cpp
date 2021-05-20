#include "CStudent.h"

CStudent::CStudent()
{
}

CStudent::CStudent(int nID, int nYear, int nMonth, int nSex, CString szName):
	m_nID(nID),
	m_nYear(nYear),
	m_nMonth(nMonth),
	m_nSex(nSex),
	m_szName(szName)
{}

CStudent::CStudent(const CStudent& stu)
{
	m_nID = stu.m_nID;
	m_nYear = stu.m_nYear;
	m_nMonth = stu.m_nMonth;
	m_nSex = stu.m_nSex;
	m_szName = stu.m_szName;
}

CStudent& CStudent::operator=(const CStudent& stu)
{
	if (this != &stu)
	{
		m_nID = stu.m_nID;
		m_nYear = stu.m_nYear;
		m_nMonth = stu.m_nMonth;
		m_nSex = stu.m_nSex;
		m_szName = stu.m_szName;
	}
	return *this;
}

CStudent::~CStudent()
{}

void CStudent::ShowStuData()
{
	cout << "ID：" << m_nID << " "
		<< "姓名：" << m_szName << " "
		<< "出生年份：" << m_nYear << " "
		<< "出生月份" << m_nMonth << " "
		<< "性别(1/男，2/女，3/其它，4/未知)：" << m_nSex << endl;
}
