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
	printf("������Ŀ����̵�ID��");
	scanf_s("%d", &dwProcessID);

	//PROCESS_CREATE_THREAD��ʾ�ҿ���ͨ�����صĽ��̾���ڸý����д����µ��̣߳�Ҳ���ǵ���CreateRemoteThread��Ȩ�ޣ�
	//PROCESS_VM_OPERATION���ʾ�ڸý����з���/�ͷ��ڴ��Ȩ�ޣ�Ҳ���ǵ���VirtualAllocEx/VirtualFreeEx��Ȩ�ޣ�
	HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, dwProcessID);

	// ��Ŀ����̵�ַ�ռ�д��DLL����
	DWORD dwSize, dwWritten;
	char lpszDll[] = "G:\\c_demo\\Զ��ע��_API����\\dll_b\\Debug\\dll_b.dll";
	dwSize = lstrlenA(lpszDll) + 1;
	LPVOID lpBuf = VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);
	if (NULL == lpBuf)
	{
		CloseHandle(hProcess);
		// ʧ�ܴ���
	}
	if (WriteProcessMemory(hProcess, lpBuf, (LPVOID)lpszDll, dwSize, &dwWritten))
	{
		// Ҫд���ֽ�����ʵ��д���ֽ�������ȣ�����ʧ��
		if (dwWritten != dwSize)
		{
			VirtualFreeEx(hProcess, lpBuf, dwSize, MEM_DECOMMIT);
			CloseHandle(hProcess);
			// ʧ�ܴ���
		}
	}
	else
	{
		CloseHandle(hProcess);
		// ʧ�ܴ���
	}
	// ʹĿ����̵���LoadLibrary������DLL
	DWORD dwID;
	LPVOID pFunc = LoadLibraryA;
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
		(LPTHREAD_START_ROUTINE)pFunc, lpBuf, 0, &dwID);

	// �ȴ�LoadLibrary�������
	WaitForSingleObject(hThread, INFINITE);
	// �ͷ�Ŀ�����������Ŀռ�
	VirtualFreeEx(hProcess, lpBuf, dwSize, MEM_DECOMMIT);
	CloseHandle(hThread);
	CloseHandle(hProcess);
	printf("dll_bע����ϣ�\n");
	getchar();
	return 0;
}