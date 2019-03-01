/*
编码：袁春旭
讲解：袁春旭
主题：带参数的导出函数
*/

#include <string.h>
#include "calc.h"

int add1(int a, int b)
{
	return a + b;
}

double add2(double a, double b)
{
	return a + b;
}

int add3(char *str1, int size, char *str2)
{
	int total = strlen(str1) + strlen(str2);
	if (total >= size)
	{
		return total;
	}
	strcat_s(str1, size, str2);
	return total;
}