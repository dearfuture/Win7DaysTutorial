/*
编码：袁春旭
讲解：袁春旭
内容：函数转发注入DLL
示例：无参函数转发，有参函数转发
关系结构：应用程序--->替代dll--->原dll
当前：替代DLL
关键指令：#pragma comment(linker, "/export:导出函数名称=被转发的dll的名称.被转发的函数名称")
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