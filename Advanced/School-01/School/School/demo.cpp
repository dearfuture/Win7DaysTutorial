/*
编码：袁春旭
讲解：袁春旭
内容：工业类库的玩法
 */
#include <stdio.h>
#include <Windows.h>
#include "Person.h"

typedef Person* (*GetPersonInstance1)();
typedef void (*ReleaseInstance1)(Person *p);

int main(void)
{
	HMODULE hModule = LoadLibrary("CompanyA.dll");
	if (NULL == hModule)
	{
		printf("加载失败\n");
		return 0;
	}

	GetPersonInstance1 pfn = (GetPersonInstance1)GetProcAddress(hModule, "?GetPersonInstance@@YAPAVPerson@@XZ");
	Person *p = pfn();
	p->say();
	p->eat();

	ReleaseInstance1 pfn2 = (ReleaseInstance1)GetProcAddress(hModule, "?ReleaseInstance@@YAXPAVPerson@@@Z");
	pfn2(p);

	FreeLibrary(hModule);

	getchar();
	return 0;
}