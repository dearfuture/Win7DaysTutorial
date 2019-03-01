/*
编码：袁春旭
讲解：袁春旭
内容：C++调用dll中的导出类
 */

#include <stdio.h>
#include "Student.h"
#pragma comment(lib, "Student.lib")

int main(void)
{
	Student stu;
	stu.SayHello();

	getchar();
	return 0;
}