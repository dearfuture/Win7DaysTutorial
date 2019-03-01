/*
程序编写：袁春旭
内容讲解：袁春旭
主题：使用dll，方法一
*/

#include <stdio.h>
#include "hello.h"

#pragma comment(lib, "HelloWorld.lib")

int main(void)
{
	SayHello();
	getchar();
	return 0;
}