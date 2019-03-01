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


// �쳣���˺���
LONG WINAPI InvalidReadExceptionFilter(PEXCEPTION_POINTERS pep)
{
	LONG lDisposition = EXCEPTION_EXECUTE_HANDLER;
	return lDisposition;
}

int ReplaceIATEntryInOneMod(PCSTR pszCalleeModName,
	PROC pfnCurrent, PROC pfnNew, HMODULE hmodCaller)
{
	ULONG ulSize;

	// һ���쳣���ܱ���������Ϊ����explorer.exe�����Ľ����ܹ����ٵؼ��غ�ж��ģ��
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

	// �ڵ����������в��ҵ�����Ϣ
	for (; pImportDesc->Name; ++pImportDesc)
	{
		// �õ�ģ����pszModName��ģ�����ַ + ����ƫ�ƣ�
		PSTR pszModName = (PSTR)((PBYTE)hmodCaller + pImportDesc->Name);

		printf("\n%s\n", pszModName);

		// �ж�ģ�����Ƿ�Ϊָ��ģ��
		if (lstrcmpiA(pszModName, pszCalleeModName) == 0)
		{
			// ȡ�õ����ߵ��뺯����ַ����ģ���еĺ�����ַ��ģ���ַ + ƫ�ƣ�
			PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)
				((PBYTE)hmodCaller + pImportDesc->FirstThunk);

			// ���µĺ�����ַ�滻��ǰ������ַ
			for (; pThunk->u1.Function; pThunk++)
			{

				// ȡ�ú�����ַ
				PROC* ppfn = (PROC*)&pThunk->u1.Function;

				// �Ƿ�����Ҫ�滻�ĺ������ȽϺ�����ַ��
				BOOL bFound = (*ppfn == pfnCurrent);
				if (bFound)
				{
					// �޸ĵ�ַ
					if (!WriteProcessMemory(GetCurrentProcess(), ppfn, &pfnNew,
						sizeof(pfnNew), NULL) && (ERROR_NOACCESS == GetLastError()))
					{
						// ���ʧ�ܣ������ҳ��������
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

	//��forѭ����ʼ��û�н��뵽return 0�ķ�֧������ζ��û���ҵ���Ӧ��ģ��
	printf("\ncann't find target module\n");
	return 1;
}
