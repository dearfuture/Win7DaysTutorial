#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>

//�����µĿ���̨
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
	WCHAR commandline[] = L"cmd";
	ret = CreateProcess(NULL, commandline, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	if (ret)
	{
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		printf("threadid = %d\n", pi.dwThreadId);
		printf("processid = %d\n", pi.dwProcessId);
	}
	else
	{
		printf("����ʧ��");
	}
	system("pause");
	return 0;
}