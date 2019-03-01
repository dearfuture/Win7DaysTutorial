/*
���룺Ԭ����
���⣺Ԭ����
���ݣ���ҵ�����淨
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
		printf("����ʧ��\n");
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