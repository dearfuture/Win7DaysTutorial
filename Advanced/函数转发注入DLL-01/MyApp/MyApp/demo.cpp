/*
编码：袁春旭
讲解：袁春旭
内容：函数转发注入DLL
示例：无参函数转发，有参函数转发
关系结构：应用程序--->替代dll--->原dll
当前：DLL的调用程序
*/

#include <stdio.h>
#include <Windows.h>

typedef int (FAR WINAPI *ADD1)(int a, int b);

int main(void)
{
	HMODULE hModule = LoadLibrary(L"targetdll.dll");
	if (NULL == hModule)
	{
		printf("加载失败\n");
		getchar();
		return 0;
	}

	FARPROC pfn = GetProcAddress(hModule, "hello");
	pfn();

	ADD1 pfn1 = (ADD1)GetProcAddress(hModule, "add2");
	pfn1(3, 5);

	FARPROC pfn2 = GetProcAddress(hModule, "test");
	pfn2();

	FreeLibrary(hModule);
	getchar();
	return 0;
}