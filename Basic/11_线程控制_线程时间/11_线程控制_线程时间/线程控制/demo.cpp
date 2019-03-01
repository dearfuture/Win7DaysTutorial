/*----------------------------------------*/
/*
线程编码实战

需要用到的函数：
SuspendThread
ResumeThread
Sleep
SwitchToThread
GetTickCount64
GetThreadTimes
GetProcessTimes
GetThreadContext
SetThreadContext
1.创建线程，挂起线程，恢复线程   _beginthreadex
2.创建线程，线程睡眠  Sleep   SwitchToThread  SetThreadPriority
3.创建多线程，线程转换
4.创建线程，给线程记时  GetTickCount64  GetThreadTimes FileTimeToSystemTime
5.创建进程，给进程记时  GetProcessTimes
6.取出和设置上下文     GetThreadContext  SetThreadContext







作者：袁春旭（QQ：2064438762）
*/
/*----------------------------------------*/

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>


DWORD WINAPI ThreadPro1(LPVOID)
{
	//ULONGLONG ullStart = 0;
	//ULONGLONG ullEnd = 0;
	//ullStart = GetTickCount64();
	FILETIME ftCreateTime, ftExitTime, ftKernelTimeStart, ftKernelTimeEnd, ftUserTimeStart, ftUserTimeEnd;
	GetThreadTimes(GetCurrentThread(), &ftCreateTime, &ftExitTime, &ftKernelTimeStart, &ftUserTimeStart);
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			//Sleep(10);
			printf("hello world\n");
		}
	}
	GetThreadTimes(GetCurrentThread(), &ftCreateTime, &ftExitTime, &ftKernelTimeEnd, &ftUserTimeEnd);
	SYSTEMTIME st1,st2;
	FILETIME ftCounter1;
	ftCounter1.dwHighDateTime = ftKernelTimeEnd.dwHighDateTime - ftKernelTimeStart.dwHighDateTime;
	ftCounter1.dwLowDateTime = ftKernelTimeEnd.dwLowDateTime - ftKernelTimeStart.dwLowDateTime;
	FileTimeToSystemTime(&ftCounter1, &st1);
	printf("%d.%d\n", st1.wSecond,st1.wMilliseconds);

	FILETIME ftCounter2;
	ftCounter2.dwHighDateTime = ftUserTimeEnd.dwHighDateTime - ftUserTimeStart.dwHighDateTime;
	ftCounter2.dwLowDateTime = ftUserTimeEnd.dwLowDateTime - ftUserTimeStart.dwLowDateTime;

	FileTimeToSystemTime(&ftCounter2, &st2);
	printf("%d.%d\n", st2.wSecond,st2.wMilliseconds);
	//ullEnd = GetTickCount64();
	//printf("用了%ul毫秒\n", ullEnd - ullStart);
	return 0;
}

int main(int argc, char *argv[])
{
	HANDLE hThread1;
	unsigned int iThreadIdex1;
	hThread1 = (HANDLE)_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)ThreadPro1, NULL, 0, &iThreadIdex1);
	CloseHandle(hThread1);

	system("pause");
	return 0;
}