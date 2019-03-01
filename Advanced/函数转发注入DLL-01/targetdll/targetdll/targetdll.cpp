/*
编码：袁春旭
讲解：袁春旭
内容：函数转发注入DLL
示例：无参函数转发，有参函数转发
关系结构：应用程序--->替代dll--->原dll
 */

#include <stdio.h>
#include "targetdll.h"

void __stdcall hello()
{
	printf("\n---------------------\n");
	printf("--------------hello-------------");
	printf("\n---------------------\n");
}

int __stdcall add1(int a, int b)
{
	printf("\n---------------------\n");
	printf("--------add-----%d", a + b);
	printf("\n---------------------\n");
	return a + b;
}