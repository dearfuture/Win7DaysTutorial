/*
编码：袁春旭
讲解：袁春旭
主题：C++项目调用C语言编写的dll
*/

#include <stdio.h>
#include "calc.h"

#pragma comment(lib, "Calculater.lib")

int main(void)
{
	int a1 = 10;
	int b1 = 25;
	int c1 = 0;
	c1 = add1(a1, b1);
	printf("%d\n", c1);

	double a2 = 1.2;
	double b2 = 2.5;
	double c2 = 0;
	c2 = add2(a2, b2);
	printf("%lf\n", c2);

	char str1[255] = "hello ";
	char str2[] = "world";
	int len = 0;
	len = add3(str1, 255, str2);
	printf("%s\n", str1);
	

	getchar();
	return 0;
}