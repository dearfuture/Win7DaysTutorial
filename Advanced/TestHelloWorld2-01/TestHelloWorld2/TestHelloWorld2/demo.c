/**
编码：袁春旭
讲解：袁春旭
主题：使用dll，方法二
API：LoadLibrary  FreeLibrary  GetProcAddress
GetModuleHandle  GetModuleFileName 
 */
#include <stdio.h>
#include <Windows.h>

int main(void)
{
	char path[255] = {0};
	HMODULE hModule = GetModuleHandle("HelloWorld.dll");
	DWORD len = 0;
	if (NULL == hModule)
	{
		hModule = LoadLibrary("HelloWorld.dll");
		if (NULL == hModule)
		{
			printf("load library failed...\n");
			getchar();
			return 1;
		}

		len = GetModuleFileName(hModule, path, 255);
		printf("%s\n", path);
	}
	
	FARPROC fn = GetProcAddress(hModule, "SayHello");
	fn();
	FreeLibrary(hModule);
	getchar();
	return 0;
}