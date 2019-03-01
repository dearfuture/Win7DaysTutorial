/*----------------------------------------*/
/*
 线程编码实战
 
 需要用到的函数：
	CreateThread 
	_beginthreadex
	SetThreadPriority
	ResumeThread
	ExitThread
	TerminateThread
	ExitProcess
	_endthreadex
	1.创建线程，无参  CreateThread  _beginthreadex
	2.创建线程，有参
	3.给线程函数传参
	4.设置优先级  SetThreadPriority  ResumeThread
	5.创建多个不同优先级线程
	6.在线程中添加对象，看线程退出时是否执行了析构函数
	7.ExitThread，_endthreadex线程，看线程退出时是否执行了析构函数
	8.TerminateThread线程，看线程退出时是否执行了析构函数
	9.ExitProcess进程，看线程退出时是否执行了析构函数





	作者：袁春旭（QQ：2064438762）
 */
/*----------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <process.h>

DWORD WINAPI ThreadPro(LPVOID lpParam);
DWORD WINAPI ThreadProex(LPVOID lpParam);

int main(int argc, char *argv[])
{
	DWORD dwThreadId = 0;
	HANDLE hThread;
	hThread = CreateThread(NULL, NULL, ThreadPro, NULL, CREATE_SUSPENDED, &dwThreadId);
	SetThreadPriority(hThread, THREAD_PRIORITY_LOWEST);
	ResumeThread(hThread);
	CloseHandle(hThread);

	HANDLE hThreadex;
	unsigned int iThreadIdex;
	hThreadex = (HANDLE)_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)ThreadProex, NULL, CREATE_SUSPENDED, &iThreadIdex);
	SetThreadPriority(hThreadex, THREAD_PRIORITY_IDLE);
	ResumeThread(hThreadex);
	CloseHandle(hThreadex);

	
	/*
	for (int i = 0; i < 200; i++)
	{
		printf("world\n");
	}
	*/
	system("pause");
	return 0;
}

DWORD WINAPI ThreadPro(LPVOID lpParam)
{
	for (int i = 0; i < 200; i++)
	{
		printf("hello\n");
	}
	return 0;
}

DWORD WINAPI ThreadProex(LPVOID lpParam)
{
	for (int i = 0; i < 200; i++)
	{
		printf("_beginthreadex\n");
	}
	return 0;
}