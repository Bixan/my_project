// Server.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "../common/CUMTSocket.h"
#include <mysql.h>
#include <string>
using namespace std;
#pragma comment(lib, "libmysql.lib")

#define INCREASE	"increase"	// 增加
#define DELETE	"delete"	// 删除
#define REVISE	"revise"	// 修改
#define QUERY	"query"	// 查询
#define EXIT		"exit"		// 退出

//#define STU_TABLE		"T_STUDENT"	// 学生表
//#define CLASS_TABLE		"T_CLASS"	// 班级表
//#define COURSE_TABLE	"T_COURSE"	// 课程表
//#define SELECT_TABLE		"T_SELECT"	// 选课表

using namespace std;
CUMTSocket g_socket;
MYSQL g_mysql;

// 初始化socket
void InitSocket()
{
	g_socket.CreateSocket();
	// 绑定IP 端口
	g_socket.Accept("127.0.0.1", 0x6868);
}

// 初始化数据库
int InitMySQL()
{
	// 初始化数据库
	if (mysql_init(&g_mysql) == NULL)
	{
		cout << "[MySQL]: " << mysql_error(&g_mysql) << endl;
		return -1;
	}

	// 设置字符集
	if (!mysql_set_character_set(&g_mysql, "gbk"))
	{
		cout << "[MySQL]: New client character set:"
			<< mysql_character_set_name(&g_mysql)
			<< endl;
	}

	// 连接数据库
	if (mysql_real_connect(&g_mysql,
		"localhost",
		"root",
		"Li991230.",
		NULL,
		3306,
		NULL, 0) == NULL)
	{
		cout << "[MySQL]: " << mysql_error(&g_mysql) << endl;
		return -1;
	}

	// 创建表
	char szSQLTable[0x100] = "CREATE DATABASE IF NOT EXISTS STU_TABLE";
	if (mysql_real_query(&g_mysql, szSQLTable, strlen(szSQLTable) + 1) != 0)
	{
		cout << "[MySQL]: " << mysql_error(&g_mysql) << endl;
		return -1;
	}

	// 使用创建的学生表
	char szUSETable[0x100] = "USE STU_TABLE";
	if (mysql_real_query(&g_mysql, szUSETable, strlen(szUSETable) + 1) != 0)
	{
		cout << "[MySQL]: " << mysql_error(&g_mysql) << endl;
		return -1;
	}

	// 创建班级表
	char szSQLClassTable[0x100] = "CREATE TABLE IF NOT EXISTS T_CLASS ("
		"CLASS_ID varchar(255),"
		"CLASS_NAME varchar(255) NOT NULL,"
		"PRIMARY KEY(CLASS_ID));";

	if (mysql_real_query(&g_mysql, szSQLClassTable, strlen(szSQLClassTable) + 1) != 0)
	{
		cout << "[MySQL]: " << mysql_error(&g_mysql) << endl;

		return -1;
	}

	// 创建学生表
	char szSQLStuTable[0x100] = "CREATE TABLE IF NOT EXISTS T_STUDENT ("
		"STU_ID varchar(255),"
		"STU_NAME varchar(255) NOT NULL,"
		"CLASS_ID varchar(255) NOT NULL,"
		"PRIMARY KEY(STU_ID),"
		"CONSTRAINT FK1 FOREIGN KEY(CLASS_ID) REFERENCES T_CLASS (CLASS_ID));";
	if (mysql_real_query(&g_mysql, szSQLStuTable, strlen(szSQLStuTable) + 1) != 0)
	{
		cout << "[MySQL]: " << mysql_error(&g_mysql) << endl;
		return -1;
	}

	// 创建课程表
	char szSQLCourseTable[0x100] = "CREATE TABLE IF NOT EXISTS T_COURSE ("
		"COURSE_ID varchar(255),"
		"COURSE_NAME varchar(255) NOT NULL,"
		"PRIMARY KEY(COURSE_ID));";
	if (mysql_real_query(&g_mysql, szSQLCourseTable, strlen(szSQLCourseTable) + 1) != 0)
	{
		cout << "[MySQL]: " << mysql_error(&g_mysql) << endl;
		return -1;
	}

	// 创建选课表
	char szSQLSelectTable[0x500] = "CREATE TABLE IF NOT EXISTS T_SELECT ("
		"STU_ID varchar(255),"
		"COURSE_ID varchar(255),"
		"PRIMARY KEY(STU_ID, COURSE_ID),"
		"CONSTRAINT FK2 FOREIGN KEY ( STU_ID ) REFERENCES T_STUDENT ( STU_ID ),"
		"CONSTRAINT FK3 FOREIGN KEY ( COURSE_ID ) REFERENCES T_COURSE ( COURSE_ID ));";
	if (mysql_real_query(&g_mysql, szSQLSelectTable, strlen(szSQLSelectTable) + 1) != 0)
	{
		cout << "[MySQL]: " << mysql_error(&g_mysql) << endl;
		return -1;
	}

	// 创建用户表 -- 保存数据库用户名及密码
#if 0
	char szSQLUserTable[0x100] = "CREATE TABLE IF NOT EXISTS T_USER("
		"user_name varchar(255),"
		"password varchar(255) NOT NULL,"
		"PRIMARY KEY(user_name))";
	if (mysql_real_query(&g_mysql, szSQLUserTable, strlen(szSQLUserTable) + 1) != 0)
	{
		cout << "[MySQL]: " << mysql_error(&g_mysql) << endl;
		return -1;
	}
#endif // 0

	return 0;
}

bool QueryTablesData()
{
	cout << "[Server]: 查询数据成功" << endl;
	return true;
}


bool CommonTablesData()
{
	char szBytesAllRecv[0x1000];
	int nBytesRecv = 0;		 // 当前接收字节数
	// 接收客服端发送的数据
	g_socket.Recv(szBytesAllRecv, sizeof(szBytesAllRecv), nBytesRecv);
	char szBuff[0x100] = { 0 };

	memcpy(szBuff, szBytesAllRecv, nBytesRecv);
	if (mysql_query(&g_mysql, szBuff) != 0)
	{
		cout << "[MySQL]: " << mysql_error(&g_mysql) << endl;
		return false;
	}
	return true;
}

// 增加表项数据
bool IncreaseTableData()
{
	if (!CommonTablesData())
	{
		return false;
	}
	return true;
}

// 删除表项数据
bool DeleteTableData()
{
	if (!CommonTablesData())
	{
		return false;
	}
	return true;
}

// 修改表项数据
bool ReviseTableData()
{
	if (!CommonTablesData())
	{
		return false;
	}
	return true;
}


// 查询数据表
bool QueryTableData(const char* pszBuff, int nLen)
{
	// 查询结果
	if (mysql_real_query(&g_mysql, pszBuff, nLen) != 0)
	{
		cout << "[MySQL]: " << mysql_error(&g_mysql) << endl;
		return false;
	}

	// 保存结果
	MYSQL_RES* result = mysql_store_result(&g_mysql);

	// 遍历字段
	unsigned int num_fields;
	unsigned int i = 0;
	MYSQL_FIELD* fields;

	int nItem = 0;
	num_fields = mysql_num_fields(result);
	fields = mysql_fetch_fields(result);
	string strTemp;
	for (i = 0; i < num_fields; i++)
	{
		strTemp += fields[i].name;
		strTemp += "-";
	}
	strTemp = strTemp.substr(0, strTemp.length() - 1);

	// 向客户端发送字段
	g_socket.Send(strTemp.data(), strTemp.length());
	Sleep(100);

	// 遍历数据
	string strTemp2;
	MYSQL_ROW row;
	while ((row = mysql_fetch_row(result)))
	{
		unsigned long* lengths;
		lengths = mysql_fetch_lengths(result);
		for (i = 0; i < num_fields; i++)
		{
			if (i == 0)
			{
				strTemp2 += row[i];
				continue;
			}
			strTemp2 += "-";
			strTemp2 += row[i];
		}
		strTemp2 += "\r\n";
		nItem++;
	}
	// 向客户端发送表数据
	g_socket.Send(strTemp2.data(), strTemp2.length());
	Sleep(100);
	// 释放
	mysql_free_result(result);
	return true;
}

int main()
{
	cout << "[Server]: 服务器启动成功" << endl;
	// 连接数据库
	InitMySQL();
	cout << "[Server]: 服务器连接数据库成功" << endl;
	cout << "[Server]: 等待客户端连接" << endl;
	// 初始化socket
	InitSocket();
	cout << "[Server]: 客户端连接成功" << endl;

	int nBytesAllRecv = 0;		// 接收总字节数
	// 接收数据
	while (true)
	{
		char szBytesAllRecv[0x1000] = { 0 };
		int nBytesRecv = 0;		 // 当前接收字节数
		// 接收数据
		g_socket.Recv(szBytesAllRecv, sizeof(szBytesAllRecv), nBytesRecv);
		char szBuff[100] = { 0 };
		memcpy(szBuff, szBytesAllRecv, nBytesRecv);
		// 增加
		if (strcmp(szBuff, INCREASE) == 0)
		{
			// 增加表数据
			if (!IncreaseTableData())
			{
				cout << "[Server]: 添加数据失败" << endl;
				continue;
			}
			cout << "[Server]: 添加数据成功" << endl;
			continue;
		}
		else if (strcmp(szBuff, DELETE) == 0)
		{
			// 删除表数据
			if (!DeleteTableData())
			{
				cout << "[Server]: 删除数据失败" << endl;
				continue;
			}
			cout << "[Server]: 删除数据成功" << endl;
			continue;
		}
		else if (strcmp(szBuff, REVISE) == 0)
		{
			// 删除表数据
			if (!ReviseTableData())
			{
				cout << "[Server]: 修改数据失败" << endl;
				continue;
			}
			cout << "[Server]: 修改数据成功" << endl;
			continue;
		}
		else if (strcmp(szBuff, QUERY) == 0)
		{
			if (QueryTablesData())
			{
				continue;
			}
			// 查询表数据
			char szAllRecvQueryBytes[0x100] = { 0 };
			int nAllRecvQueryBytes = 0;		 // 当前接收字节数
			// 接收查询的数据
			g_socket.Recv(szAllRecvQueryBytes, sizeof(szAllRecvQueryBytes), nAllRecvQueryBytes);
			char szBuffTemp[100] = { 0 };
			memcpy(szBuffTemp, szAllRecvQueryBytes, nAllRecvQueryBytes);
			if (!QueryTableData(szBuffTemp, nAllRecvQueryBytes))
			{
				cout << "[Server]: 查询数据失败" << endl;
				continue;
			}
			cout << "[Server]: 查询数据成功" << endl;
			continue;
		}
		else if (strcmp(szBuff, EXIT) == 0)
		{
			cout << "[Server]: 客户端退出连接" << endl;
			mysql_close(&g_mysql);
			return 0;
		}
	}

	return 0;
}