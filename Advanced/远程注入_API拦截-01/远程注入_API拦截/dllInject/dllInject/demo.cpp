#include <Windows.h>
#include <stdio.h>
#include <TlHelp32.h>

DWORD FindTarget(LPCTSTR lpszProcess)
{
	DWORD dwRet = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnapshot, &pe32);
	do
	{
		if (lstrcmpi(pe32.szExeFile, lpszProcess) == 0)
		{
			dwRet = pe32.th32ProcessID;
			break;
		}
	} while (Process32Next(hSnapshot, &pe32));
	CloseHandle(hSnapshot);
	return dwRet;
}

int main(void)
{
	DWORD dwProcessID = 0;
	printf("请输入目标进程的ID：");
	scanf_s("%d", &dwProcessID);

	//PROCESS_CREATE_THREAD表示我可以通过返回的进程句柄在该进程中创建新的线程，也就是调用CreateRemoteThread的权限；
	//PROCESS_VM_OPERATION则表示在该进程中分配/释放内存的权限，也就是调用VirtualAllocEx/VirtualFreeEx的权限；
	HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, dwProcessID);

	// 向目标进程地址空间写入DLL名称
	DWORD dwSize, dwWritten;
	char lpszDll[] = "G:\\c_demo\\远程注入_API拦截\\dll_b\\Debug\\dll_b.dll";
	dwSize = lstrlenA(lpszDll) + 1;
	LPVOID lpBuf = VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);
	if (NULL == lpBuf)
	{
		CloseHandle(hProcess);
		// 失败处理
	}
	if (WriteProcessMemory(hProcess, lpBuf, (LPVOID)lpszDll, dwSize, &dwWritten))
	{
		// 要写入字节数与实际写入字节数不相等，仍属失败
		if (dwWritten != dwSize)
		{
			VirtualFreeEx(hProcess, lpBuf, dwSize, MEM_DECOMMIT);
			CloseHandle(hProcess);
			// 失败处理
		}
	}
	else
	{
		CloseHandle(hProcess);
		// 失败处理
	}
	// 使目标进程调用LoadLibrary，加载DLL
	DWORD dwID;
	LPVOID pFunc = LoadLibraryA;
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
		(LPTHREAD_START_ROUTINE)pFunc, lpBuf, 0, &dwID);

	// 等待LoadLibrary加载完毕
	WaitForSingleObject(hThread, INFINITE);
	// 释放目标进程中申请的空间
	VirtualFreeEx(hProcess, lpBuf, dwSize, MEM_DECOMMIT);
	CloseHandle(hThread);
	CloseHandle(hProcess);
	printf("dll_b注入完毕！\n");
	getchar();
	return 0;
}