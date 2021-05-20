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
	cout << "ID��" << m_nID << " "
		<< "������" << m_szName << " "
		<< "������ݣ�" << m_nYear << " "
		<< "�����·�" << m_nMonth << " "
		<< "�Ա�(1/�У�2/Ů��3/������4/δ֪)��" << m_nSex << endl;
}
