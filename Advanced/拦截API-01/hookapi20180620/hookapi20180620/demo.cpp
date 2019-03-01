#include <Windows.h>
#include <stdio.h>
#include <ImageHlp.h>
//#include "target.h"
#pragma comment(lib, "ImageHlp")

//#pragma comment(lib, "testdll20180620.lib")

LONG WINAPI InvalidReadExceptionFilter(PEXCEPTION_POINTERS pep);
int ReplaceIATEntryInOneMod(PCSTR pszCalleeModName,
	PROC pfnCurrent, PROC pfnNew, HMODULE hmodCaller);

int mytestme()
{
	printf("\nmytestme\n");
	return 0;
}

int __stdcall hookapitest()
{
	//testme();
	PROC pfnNew = (PROC)mytestme;

	//HMODULE hModule1 = LoadLibrary(L"testdll20180620.dll");
	//HMODULE hModule2 = LoadLibrary(L"testiat.exe");
	PROC pfnOrig = GetProcAddress(GetModuleHandle(L"testdll20180620"), "testme");
	//PROC pfnOrig = GetProcAddress(hModule1, "testme");
	//PROC pfnOrig = GetProcAddress(GetModuleHandle(L"Kernel32"), "ExitProcess");
	//HMODULE hmodCaller = LoadLibrary(L"testiat.exe");
	HMODULE hmodCaller = GetModuleHandle(L"testiat.exe");
	
	return ReplaceIATEntryInOneMod(
		"testdll20180620.dll",
		pfnOrig,
		pfnNew,
		hmodCaller
	);
}


// 异常过滤函数
LONG WINAPI InvalidReadExceptionFilter(PEXCEPTION_POINTERS pep)
{
	LONG lDisposition = EXCEPTION_EXECUTE_HANDLER;
	return lDisposition;
}

int ReplaceIATEntryInOneMod(PCSTR pszCalleeModName,
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
	__except (InvalidReadExceptionFilter(GetExceptionInformation()))
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
