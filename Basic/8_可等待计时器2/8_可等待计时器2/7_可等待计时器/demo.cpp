/*
线程同步：可等待计时器-WaitableTimer
   
   方式一：从指定时间点开始触发计时器
   方式二：从启动开始计时，多长时间后首次触发

   CreateWaitableTimer
   OpenWaitableTimer
   SetWaitableTimer
   CancelWaitableTimer

   WaitForSingleObject   //等待触发状态

   示例讲解：袁春旭



















   示例编写：袁春旭   QQ：2064438762
*/


#include <Windows.h>
#include <process.h>
#include <stdio.h.>
#include <stdlib.h>

DWORD WINAPI BaoShu1(LPVOID *lparam);
DWORD WINAPI BaoShu2(LPVOID *lparam);
DWORD WINAPI BaoShu3(LPVOID *lparam);

HANDLE g_hTimer;

int main(void)
{
	//1秒 = 1000毫秒 = 1000 * 1000微秒 = 1000 * 1000 * 1000纳秒
	//注意，必须为有符号数，否则无法形成负数
	const int KOneSecond = 10 * 1000 * 1000;   

	LARGE_INTEGER li;
	//li.LowPart = ftUTC.dwLowDateTime;
	//li.HighPart = ftUTC.dwHighDateTime;
	li.QuadPart = -(5 * KOneSecond);
	g_hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
	SetWaitableTimer(g_hTimer, &li, 3000, NULL, NULL, FALSE);

	HANDLE hThread[3];
	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)BaoShu1, NULL, 0, NULL);
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)BaoShu2, NULL, 0, NULL);
	hThread[2] = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)BaoShu3, NULL, 0, NULL);
	
	WaitForMultipleObjects(3, hThread, TRUE, INFINITE);
	CancelWaitableTimer(g_hTimer);

	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);
	CloseHandle(hThread[2]);

	system("pause");
	return 0;
}

DWORD WINAPI BaoShu1(LPVOID *lparam)
{
	WaitForSingleObject(g_hTimer, INFINITE);
	for (int i = 0; i < 10; i++)
	{
		printf("1\n");
	}

	return 0;
}

DWORD WINAPI BaoShu2(LPVOID *lparam)
{
	WaitForSingleObject(g_hTimer, INFINITE);
	for (int i = 0; i < 10; i++)
	{
		printf("2\n");
	}

	return 0;
}

DWORD WINAPI BaoShu3(LPVOID *lparam)
{
	WaitForSingleObject(g_hTimer, INFINITE);
	for (int i = 0; i < 10; i++)
	{
		printf("3\n");
	}

	return 0;
}