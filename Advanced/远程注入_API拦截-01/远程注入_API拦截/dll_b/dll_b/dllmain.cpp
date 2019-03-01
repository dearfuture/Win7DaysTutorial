// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <stdio.h>
#include "dll_b.h"
#include <ImageHlp.h>
#pragma comment(lib, "ImageHlp")

LONG WINAPI InvalidReadExceptionFilter(PEXCEPTION_POINTERS pep);
int ReplaceIATEntryInOneMod(PCSTR pszCalleeModName,
	PROC pfnCurrent, PROC pfnNew, HMODULE hmodCaller);

int hookapitest()
{
	OutputDebugString(L"hookapitest 1");
	PROC pfnNew = (PROC)OutputMsg_B;  //�滻Դ����OutputMsg_B�滻�����̿ռ����Ѿ����ڵĺ���
	OutputDebugString(L"hookapitest 2");
	PROC pfnOrig = GetProcAddress(GetModuleHandle(L"dll_a"), "OutputMsg"); //�滻Ŀ��OutputMsg
	if (NULL == pfnOrig)
	{
		OutputDebugString(L"hookapitest 3");
		printf("get dll_a OutputMsg failed");
		getchar();
		return 1;
	}
	OutputDebugString(L"hookapitest 3");
	HMODULE hmodCaller = GetModuleHandle(L"MyAPP.exe");
	if (NULL == hmodCaller)
	{
		OutputDebugString(L"hookapitest 4");
		printf("get dll_a GetModuleHandle MyAPP.exe failed");
		getchar();
		return 1;
	}
	OutputDebugString(L"hookapitest 5");
	return ReplaceIATEntryInOneMod(
		"dll_a.dll",
		pfnOrig,
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
