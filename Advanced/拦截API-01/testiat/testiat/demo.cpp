#include <stdio.h>
#include <Windows.h>
#include "target.h"

#pragma comment(lib, "testdll20180620.lib")

int main(void)
{
	testme();
	testadd(3, 5);
	testhello();

	HMODULE hModule = LoadLibrary(L"hookapi20180620.dll");
	FARPROC pfn = GetProcAddress(hModule, "hookapitest");
	int ret = pfn();
	if (ret == 0)
	{
		testme();
	}
	printf("ret = %d\n", ret);
	FreeLibrary(hModule);
	getchar();
	return 0;
}