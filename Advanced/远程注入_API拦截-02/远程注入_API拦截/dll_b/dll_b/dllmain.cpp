/*
编码：袁春旭
讲解：袁春旭
主题：远程注入+拦截API
说明：
MyAPP：窗口程序，被用户远程注入，目标实验品
dll_a.dll：MyAPP加载的DLL，用于弹窗
dll_b.dll（当前）：用于拦截后替换dll_a.dll中的弹窗函数
dllInject：用于远程注入MyAPP并通过dll_b.dll替换dll_a.dll中的弹窗函数
*/
// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <stdio.h>
#include "dll_b.h"
#include <ImageHlp.h>
#pragma comment(lib, "ImageHlp")

int ReplaceIATEntry(PCSTR pszCalleeModName,
	PROC pfnCurrent, PROC pfnNew, HMODULE hmodCaller);

int hookapitest()
{
	PROC pfnNew = (PROC)OutputMsg_B;  //替换源，用OutputMsg_B替换掉进程空间中已经存在的函数
	PROC pfnOld = GetProcAddress(GetModuleHandle(L"dll_a"), "OutputMsg"); //替换目标OutputMsg
	if (NULL == pfnOld)
	{
		printf("get dll_a OutputMsg failed");
		getchar();
		return 1;
	}
	HMODULE hmodCaller = GetModuleHandle(L"MyAPP.exe");
	if (NULL == hmodCaller)
	{
		printf("get dll_a GetModuleHandle MyAPP.exe failed");
		getchar();
		return 1;
	}
	return ReplaceIATEntry(
		"dll_a.dll",
		pfnOld,
		pfnNew,
		hmodCaller
	);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		::MessageBox(NULL, L"DLL_PROCESS_ATTACH DLL_B", L"DLL_PROCESS_ATTACH DLL_B", MB_OK);
		hookapitest();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		::MessageBox(NULL, L"DLL_PROCESS_DETACH DLL_B", L"DLL_PROCESS_DETACH DLL_B", MB_OK);
		break;
	}
	return TRUE;
}

int ReplaceIATEntry(PCSTR pszCalleeModName,
	PROC pfnCurrent, PROC pfnNew, HMODULE hmodCaller)
{
	ULONG ulSize;

	// 一个异常可能被触发，因为诸如explorer.exe这样的进程能够快速地加载和卸载模块
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc = NULL;
	__try
	{
		pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToDataEx(
			hmodCaller, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &ulSize, NULL);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		printf("exception");
		return 1;
	}

	if (pImportDesc == NULL)
		return 2;

	// 在导入描述符中查找导入信息
	for (; pImportDesc->Name; ++pImportDesc)
	{
		// 得到模块名pszModName（模块基地址 + 名称偏移）
		PSTR pszModName = (PSTR)((PBYTE)hmodCaller + pImportDesc->Name);

		printf("\n%s\n", pszModName);

		// 判断模块名是否为指定模块
		if (lstrcmpiA(pszModName, pszCalleeModName) == 0)
		{
			// 取得调用者导入函数地址表在模块中的函数地址（模块基址 + 偏移）
			PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)
				((PBYTE)hmodCaller + pImportDesc->FirstThunk);

			// 用新的函数地址替换当前函数地址
			for (; pThunk->u1.Function; pThunk++)
			{

				// 取得函数地址
				PROC* ppfn = (PROC*)&pThunk->u1.Function;

				// 是否是需要替换的函数（比较函数地址）
				BOOL bFound = (*ppfn == pfnCurrent);
				if (bFound)
				{
					// 修改地址
					if (!WriteProcessMemory(GetCurrentProcess(), ppfn, &pfnNew,
						sizeof(pfnNew), NULL) && (ERROR_NOACCESS == GetLastError()))
					{
						// 如果失败，则更改页保护属性
						DWORD dwOldProtect;
						if (VirtualProtect(ppfn, sizeof(pfnNew), PAGE_WRITECOPY,
							&dwOldProtect))
						{

							WriteProcessMemory(GetCurrentProcess(), ppfn, &pfnNew,
								sizeof(pfnNew), NULL);
							VirtualProtect(ppfn, sizeof(pfnNew), dwOldProtect,
								&dwOldProtect);
							return 0;
						}

					}
					else
					{
						return 0;
					}

				}
			}

		}

	}

	//在for循环中始终没有进入到return 0的分支，就意味着没有找到对应的模块
	printf("\ncann't find target module\n");
	return 1;
}
