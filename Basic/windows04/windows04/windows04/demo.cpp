//进程查看器
//write by yuanchunxu
#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <TlHelp32.h>

int main()
{
	PROCESSENTRY32 pc;
	pc.dwSize = sizeof(pc);
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hProcessSnap)
	{
		printf("get snapshop failed\n");
		system("pause");
		return 0;
	}
	BOOL bMore = Process32First(hProcessSnap, &pc);
	while (bMore)
	{
		printf("\n---------------------------\n");
		printf("id:%d\n", pc.th32ProcessID);
		wprintf(L"name:%s\n", pc.szExeFile);
		bMore = Process32Next(hProcessSnap, &pc);
	}
	CloseHandle(hProcessSnap);
	system("pause");
	return 0;
}