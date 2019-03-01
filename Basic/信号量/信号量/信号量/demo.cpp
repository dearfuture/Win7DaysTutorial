/*
线程同步： Semaphore   信号量
互斥量使用以下函数

CreateSemaphore
CreateSemaphoreEx
OpenSemaphore
ReleaseSemaphore

WaitForSingleObject   //等待信号量资源





示例讲解：袁春旭




















示例编写：袁春旭   QQ：2064438762
*/
#include <Windows.h>
#include <process.h>
#include <stdio.h.>
#include <stdlib.h>

DWORD WINAPI ThreadProc1(LPVOID lpParameter);
DWORD WINAPI ThreadProc2(LPVOID lpParameter);
DWORD WINAPI ThreadProc3(LPVOID lpParameter);
DWORD WINAPI ThreadProc4(LPVOID lpParameter);
DWORD WINAPI ThreadProc5(LPVOID lpParameter);

HANDLE hSemaphore;

int main(void)
{
	hSemaphore = CreateSemaphore(NULL, 2, 2, NULL);
	HANDLE hThread[5];

	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)ThreadProc1, NULL, 0, NULL);
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)ThreadProc2, NULL, 0, NULL);
	hThread[2] = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)ThreadProc3, NULL, 0, NULL);
	hThread[3] = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)ThreadProc4, NULL, 0, NULL);
	hThread[4] = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)ThreadProc5, NULL, 0, NULL);

	WaitForMultipleObjects(5, hThread, TRUE, INFINITE);

	for(int i = 0; i < 5; i++)
	{
		CloseHandle(hThread[i]);
	}

	CloseHandle(hSemaphore);

	system("pause");
	return 0;
}

DWORD WINAPI ThreadProc1(LPVOID lpParameter)
{
	WaitForSingleObject(hSemaphore, INFINITE);
	printf("\nproc1 start...\n");
	for(int i = 0; i < 3; i++)
	{
		printf("proc1 running...\n");
	}
	printf("\nproc1 end...\n");
	ReleaseSemaphore(hSemaphore, 1, NULL);
	return 0;
}

DWORD WINAPI ThreadProc2(LPVOID lpParameter)
{
	WaitForSingleObject(hSemaphore, INFINITE);
	printf("\nproc2 start...\n");
	for(int i = 0; i < 3; i++)
	{
		printf("proc2 running...\n");
	}
	printf("\nproc2 end...\n");
	ReleaseSemaphore(hSemaphore, 1, NULL);
	return 0;
}

DWORD WINAPI ThreadProc3(LPVOID lpParameter)
{
	WaitForSingleObject(hSemaphore, INFINITE);
	printf("\nproc3 start...\n");
	for(int i = 0; i < 3; i++)
	{
		printf("proc3 running...\n");
	}
	printf("\nproc3 end...\n");
	ReleaseSemaphore(hSemaphore, 1, NULL);
	return 0;
}

DWORD WINAPI ThreadProc4(LPVOID lpParameter)
{
	WaitForSingleObject(hSemaphore, INFINITE);
	printf("\nproc4 start...\n");
	for(int i = 0; i < 3; i++)
	{
		printf("proc4 running...\n");
	}
	printf("\nproc4 end...\n");
	ReleaseSemaphore(hSemaphore, 1, NULL);
	return 0;
}

DWORD WINAPI ThreadProc5(LPVOID lpParameter)
{
	WaitForSingleObject(hSemaphore, INFINITE);
	printf("\nproc5 start...\n");
	for(int i = 0; i < 3; i++)
	{
		printf("proc5 running...\n");
	}
	printf("\nproc5 end...\n");
	ReleaseSemaphore(hSemaphore, 1, NULL);
	return 0;
}