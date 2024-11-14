// 使用 Unicode 版本的函数实现数据库连接，确保所有字符串都是宽字符类型，并使用相应的 Unicode 版本的 ODBC 函数。
#include <iostream>
#include <string>
#include <Windows.h>
#include <sql.h> 
#include <sqlext.h> 
#include <sqltypes.h>
using namespace std;

//定义全局变量
SQLRETURN ret = NULL;	//返回信息
SQLHENV henv = NULL;	//环境句柄
SQLHDBC hdbc = NULL;	//连接句柄
SQLHSTMT hstmt = NULL;	//语句句柄

// 释放资源
void FreeHandle()
{
	SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
	SQLFreeHandle(SQL_HANDLE_ENV, henv);
}

// 将 string 转换为 wstring
wstring StringToWString(const string& str)
{
	wstring result;
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	if (len < 0)return result;
	wchar_t* buffer = new wchar_t[len + 1];
	if (buffer == NULL)return result;
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';
	result.append(buffer);
	delete[] buffer;
	return result;
	// 原方案
	// return wstring(str.begin(), str.end());
}

// 测试返回值ret
void DeBug(SQLRETURN ret)
{	
	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
	{
		cout << "SQL_SUCCESS" << endl;
	}
	else if (ret == SQL_ERROR)
	{
		cout << "SQL_ERROR" << endl;
	}
	else if (ret == SQL_INVALID_HANDLE)
	{
		cout << "SQL_INVALID_HANDLE" << endl;
	}
	else if (ret == SQL_NEED_DATA)
	{
		cout << "SQL_NEED_DATA" << endl;
	}
	else if (ret == SQL_NO_DATA_FOUND)
	{
		cout << "SQL_NO_DATA_FOUND" << endl;
	}
	else
	{
		cout << "NO_INFO" << endl;
	}
}

void Connect() // 数据库连接函数
{
	// 申请环境
	ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) 
	{
		cout << "申请环境失败！" << endl;
		return;
	}

	// 设置环境
	ret = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) 
	{
		cout << "设置环境失败！" << endl;
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
		return;
	}

	// 申请数据库连接
	ret = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) 
	{
		cout << "申请数据库连接失败！" << endl;
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
		return;
	}

	// 连接数据库
	wstring wserver = StringToWString("SQL");
	wstring wuser = StringToWString("sa");
	wstring wpassword = StringToWString("123456");

	ret = SQLConnectW(hdbc, (SQLWCHAR*)wserver.c_str(), SQL_NTS, (SQLWCHAR*)wuser.c_str(), SQL_NTS, (SQLWCHAR*)wpassword.c_str(), SQL_NTS);
	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) 
	{
		cout << "数据库连接成功!" << endl;
	}
	else 
	{
		cout << "数据库连接失败！" << endl;
		SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
		return;
	}
}

void InsertOp(int choiceTable) 
{
	ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt); // 申请句柄  
	string str1 = "use student"; // 选择数据库student
	wstring wstr1 = StringToWString(str1);
	ret = SQLExecDirectW(hstmt, (SQLWCHAR*)wstr1.c_str(), SQL_NTS);
    DeBug(ret);

	switch (choiceTable) {
	case 1: // 学生表  Student
	{
		// 测试用例
		// wstring sql = L"insert into Student values('201', 'Alice', 20, '计算机系')";
		// ret = SQLExecDirectW(hstmt, (SQLWCHAR*)sql.c_str(), SQL_NTS);
		cout << "请依次输入学号、姓名、年龄、所在系" << endl;
		string Sno, Sname, Sage, Sdept;
		cin >> Sno >> Sname >> Sage >> Sdept;
		string sql = "insert into Student values('" + Sno + "','" + Sname + "'," + Sage + ",'" + Sdept + "')";		
		wstring wsql = StringToWString(sql);
		wcout << wsql << endl;

		ret = SQLExecDirectW(hstmt, (SQLWCHAR*)wsql.c_str(), SQL_NTS);
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
			cout << "学生信息插入成功！" << endl;
		}
		else {
			cout << "学生信息插入失败！" << endl;
		}
		break;
	}
	case 2: // 课程表  Course
	{
		cout << "请依次输入课程号、课程名、先行课" << endl;
		string Cno, Cname, Cbefore;
		cin >> Cno >> Cname >> Cbefore;
		string sql = "insert into Course values('" + Cno + "','" + Cname + "','" + Cbefore + "')";
		wstring wsql = StringToWString(sql);

		ret = SQLExecDirectW(hstmt, (SQLWCHAR*)wsql.c_str(), SQL_NTS);
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
			cout << "课程信息插入成功！" << endl;
		}
		else {
			cout << "课程信息插入失败！" << endl;
		}
		break;
	}
	case 3: // 成绩表  sc
	{
		cout << "请依次输入学号、课程号、成绩" << endl;
		string Sno, Cno, Grade;
		cin >> Sno >> Cno >> Grade;
		string sql = "insert into SC values ('" + Sno + "','" + Cno + "'," + Grade + ")";
		wstring wsql = StringToWString(sql);

		ret = SQLExecDirectW(hstmt, (SQLWCHAR*)wsql.c_str(), SQL_NTS);
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
			cout << "成绩信息插入成功！" << endl;
		}
		else {
			cout << "成绩信息插入失败！" << endl;
		}
		break;
	}
	default:
		cout << "无效的表选择！" << endl;
		break;
	}

	FreeHandle(); // 释放资源  
}

void DeleteOp(int choiceTable)
{

}

void QueryOp(int choiceTable)
{

}

void ModifyOp(int choiceTable)
{

}

void DisplayMenu()
{
	while (true)
	{
		int choiceTable, choiceOp;
		cout << "=====学习信息数据库管理系统=====" << endl;
		cout << "选择要操作的数据表" << endl;
		cout << "0.退出系统" << endl;
		cout << "1.Student表" << endl;
		cout << "2.Course表" << endl;
		cout << "3.sc表" << endl;
		cout << "请输入你的选择：";
		cin >> choiceTable;
		if (choiceTable == 0)	break;
		//system("cls");
		cout << "选择要进行的操作" << endl;
		cout << "0.返回" << endl;
		cout << "1.添加信息" << endl;
		cout << "2.删除信息" << endl;
		cout << "3.查询信息" << endl;
		cout << "4.修改信息" << endl;
		cout << "请输入你的选择：";
		cin >> choiceOp;
		//system("cls");

		switch (choiceOp) {
		case 1:	InsertOp(choiceTable); break;
		case 2: DeleteOp(choiceTable); break;
		case 3: QueryOp(choiceTable);  break;
		case 4: ModifyOp(choiceTable); break;
		default: break;
		}
	}
}

int main()
{
	Connect();
	DisplayMenu();
	system("pause");
	return 0;
}