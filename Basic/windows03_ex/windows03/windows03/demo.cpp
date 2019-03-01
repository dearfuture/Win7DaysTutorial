#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>

//创建新的控制台
int main(void)
{
	int ret = 0;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = TRUE;
	ZeroMemory(&pi, sizeof(pi));
	//WCHAR commandline[] = L"cmd";
	//ret = CreateProcess(NULL, commandline, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	WCHAR commandline1[] = L"notepad.exe";
	WCHAR commandline2[] = L"calc";
	//ret = CreateProcess(NULL, commandline, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi);
	ret = CreateProcess(commandline1, commandline2, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi);
	if (ret)
	{
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		printf("threadid = %d\n", pi.dwThreadId);
		printf("processid = %d\n", pi.dwProcessId);
	}
	else
	{
		printf("启动失败");
	}
	system("pause");
	return 0;
}