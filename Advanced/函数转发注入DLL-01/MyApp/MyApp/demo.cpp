/*
���룺Ԭ����
���⣺Ԭ����
���ݣ�����ת��ע��DLL
ʾ�����޲κ���ת�����вκ���ת��
��ϵ�ṹ��Ӧ�ó���--->���dll--->ԭdll
��ǰ��DLL�ĵ��ó���
*/

#include <stdio.h>
#include <Windows.h>

typedef int (FAR WINAPI *ADD1)(int a, int b);

int main(void)
{
	HMODULE hModule = LoadLibrary(L"targetdll.dll");
	if (NULL == hModule)
	{
		printf("����ʧ��\n");
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