#ifndef FUNCTION_H  
#define FUNCTION_H  

#include <iostream>
#include <string>
#include <Windows.h>
#include <sql.h> 
#include <sqlext.h> 
#include <sqltypes.h>
using namespace std;

// 释放资源
void FreeHandle();

// 将 string 转换为 wstring
wstring StringToWString(const string& str);

// 测试返回值 ret
void DeBug(SQLRETURN ret);

// 连接数据库
void Connect();

// 插入操作
void InsertOp(int choiceTable);

// 删除操作
void DeleteOp(int choiceTable);

// 查询操作
void QueryOp(int choiceTable);

// 修改操作
void ModifyOp(int choiceTable);

// 显示菜单
void DisplayMenu();

#endif