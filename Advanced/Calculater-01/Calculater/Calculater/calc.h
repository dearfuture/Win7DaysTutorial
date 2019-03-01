/*
编码：袁春旭
讲解：袁春旭
主题：调用带参数的导出函数
 */

#pragma once

__declspec(dllexport) int __stdcall add1(int a, int b);

__declspec(dllexport) double __stdcall add2(double a, double b);

__declspec(dllexport) int __stdcall add3(char *str1, int size, char *str2);