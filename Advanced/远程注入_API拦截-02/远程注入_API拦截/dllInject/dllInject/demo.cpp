/*
���룺Ԭ����
���⣺Ԭ����
���⣺Զ��ע��+����API
˵����
MyAPP�����ڳ��򣬱��û�Զ��ע�룬Ŀ��ʵ��Ʒ
dll_a.dll��MyAPP���ص�DLL�����ڵ���
dll_b.dll���������غ��滻dll_a.dll�еĵ�������
dllInject����ǰ��������Զ��ע��MyAPP��ͨ��dll_b.dll�滻dll_a.dll�еĵ�������
*/
#include <Windows.h>
#include <stdio.h>


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
	char lpszDll[] = "dll_b.dll";
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