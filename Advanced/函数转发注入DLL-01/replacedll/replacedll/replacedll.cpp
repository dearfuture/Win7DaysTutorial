/*
���룺Ԭ����
���⣺Ԭ����
���ݣ�����ת��ע��DLL
ʾ�����޲κ���ת�����вκ���ת��
��ϵ�ṹ��Ӧ�ó���--->���dll--->ԭdll
��ǰ�����DLL
�ؼ�ָ�#pragma comment(linker, "/export:������������=��ת����dll������.��ת���ĺ�������")
*/
#include <Windows.h>
#include <stdio.h>
#include "replacedll.h"

#pragma comment(linker, "/export:hello=targetdll1.hello")

#pragma comment(linker, "/export:add2=targetdll1.add1")

void __stdcall test()
{
	printf("\n---------------------\n");
	printf("--------------test-------------");
	printf("\n---------------------\n");
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		OutputDebugString("load replacedll");
		break;
	case DLL_THREAD_ATTACH:break;
	case DLL_THREAD_DETACH:break;
	case DLL_PROCESS_DETACH:
		OutputDebugString("unload replacedll");
		break;
	}
	return TRUE;
}