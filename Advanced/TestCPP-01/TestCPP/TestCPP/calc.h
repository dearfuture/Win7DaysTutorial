/*
编码：袁春旭
讲解：袁春旭
主题：带参数的导出函数
 */
#pragma once

extern "C"
{
	__declspec(dllexport) int add1(int a, int b);

	__declspec(dllexport) double add2(double a, double b);

	__declspec(dllexport) int add3(char *str1, int size, char *str2);
}



