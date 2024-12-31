// 使用 Unicode 版本的函数实现数据库连接，确保所有字符串都是宽字符类型，并使用相应的 Unicode 版本的 ODBC 函数。
#include "function.h"
using namespace std;

int main()
{
	Connect();
	DisplayMenu();
	system("pause");
	return 0;
}