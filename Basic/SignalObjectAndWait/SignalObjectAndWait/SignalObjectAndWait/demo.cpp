/*
  线程同步：SignalObjectAndWait

  示例讲解：袁春旭

  第一个参数：互斥量、信号量、事件

  第二个参数：互斥量、信号量、事件、计时器、进程、线程、作业等



















  示例编写：袁春旭   QQ：2064438762
 */



#include <Windows.h>
#include <process.h>
#include <stdio.h.>
#include <stdlib.h>

DWORD WINAPI sayhello(LPVOID *lparam);

HANDLE g_hEvent;

int main(void)
{
	g_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);


	HANDLE hThread;
	hThread = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)sayhello, NULL, 0, NULL);
	//SetEvent(g_hEvent);
	//xxx  xxx   xxx
	//WaitForSingleObject(hThread, INFINITE);
	SignalObjectAndWait(g_hEvent, hThread, INFINITE, TRUE);
	for (int i = 0; i < 100; i++)
	{
		printf("world\n");
	}

	CloseHandle(hThread);



	system("pause");
	return 0;
}

DWORD WINAPI sayhello(LPVOID *lparam)
{
	WaitForSingleObject(g_hEvent, INFINITE);
	for (int i = 0; i < 100; i++)
	{
		printf("hello\n");
	}

	return 0;
}