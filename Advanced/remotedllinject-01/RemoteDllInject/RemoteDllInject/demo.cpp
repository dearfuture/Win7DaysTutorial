/*
讲解：袁春旭
编码：袁春旭
主题：远程注入
API:
VirtualAllocEx
VirtualFreeEx
WriteProcessMemory
CreateRemoteThread
原理说明：给指定进程新建一个线程，同时使该线程加载指定的DLL
三个工程：
A：DLL
B：当前
C：目标进程
 */

#include <Windows.h>
#include <stdio.h>

int main(void)
{
	DWORD dwProcessId = 0;
	scanf_s("%d", &dwProcessId);
	HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, dwProcessId);
	if (NULL == hProcess)
	{
		printf("OpenProcess failed...\n");
		getchar();
		return 0;
	}

	char dllName[] = "D:\\mydll.dll";
	DWORD dllsize = strlen(dllName) + 1;
	DWORD dwWritten = 0;
	LPVOID lpBuf = VirtualAllocEx(hProcess, NULL, dllsize, MEM_COMMIT, PAGE_READWRITE);
	if (NULL == lpBuf)
	{
		CloseHandle(hProcess);
		printf("VirtualAllocEx failed...\n");
		getchar();
		return 0;

	}
	if (WriteProcessMemory(hProcess, lpBuf, (LPCVOID)dllName, dllsize, &dwWritten))
	{
		if (dwWritten != dllsize)
		{
			VirtualFreeEx(hProcess, lpBuf, dllsize, MEM_DECOMMIT);
			CloseHandle(hProcess);
			printf("WriteProcessMemory failed...\n");
			getchar();
			return 0;
		}
	}
	else
	{
		printf("WriteProcessMemory failed...\n");
		CloseHandle(hProcess);
		getchar();
		return 0;
	}
	DWORD dwID;
	LPVOID pFunc = LoadLibraryA;
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFunc, lpBuf, 0, &dwID);
	if (hThread == NULL)
	{
		printf("CreateRemoteThread failed...\n");
		VirtualFreeEx(hProcess, lpBuf, dllsize, MEM_DECOMMIT);
		CloseHandle(hProcess);
		getchar();
		return 0;
	}
	WaitForSingleObject(hThread, INFINITE);
	VirtualFreeEx(hProcess, lpBuf, dllsize, MEM_DECOMMIT);
	CloseHandle(hThread);
	CloseHandle(hProcess);
	

	getchar();
	return 0;
}