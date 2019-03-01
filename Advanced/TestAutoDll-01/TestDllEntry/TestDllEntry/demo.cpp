#include <stdio.h>
#include <Windows.h>
#include <process.h>

FARPROC pfn;
HMODULE hModule;
unsigned __stdcall threadfunc(void * pThis)
{
	
	
	FreeLibrary(hModule);
	return 0;
}

int main(void)
{
	unsigned uiThread1ID;
	hModule = LoadLibrary("AutoDll.dll");
	if (NULL == hModule)
	{
		printf("load library failed\n");
		getchar();
		return 1;
	}
	pfn = GetProcAddress(hModule, "?hello@@YAXXZ");
	pfn();
	_beginthreadex(NULL, 0, threadfunc, NULL, 0, &uiThread1ID);
	//FreeLibrary(hModule);
	//_endthread();
	getchar();
	return 0;
}