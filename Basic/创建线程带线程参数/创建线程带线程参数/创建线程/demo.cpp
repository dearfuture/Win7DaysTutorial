/*----------------------------------------*/
/*
 线程编码实战
 
 需要用到的函数：
	CreateThread   
	_beginthreadex  
	SetThreadPriority
	ExitThread
	TerminateThread
	ExitProcess
	_endthreadex
	1.创建线程，无参  CreateThread  _beginthreadex
	2.创建线程，有参  整型 字符串 结构体 对象 七日成蝶-C++精品课程全集视频课程
	3.给线程函数传参
	4.设置优先级
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

typedef struct _coor
{
	int x;
	int y;
}Coor;

int main(int argc, char *argv[])
{
	DWORD dwThreadId = 0;
	HANDLE hThread;
	int x = 20;
	int y = 40;
	char str1[200] = "yuanchunxu";
	char str2[200] = "2064438762";
	Coor coo1 = {4,5};
	Coor coo2 = {20, 80};
	hThread = CreateThread(NULL, NULL, ThreadPro, &coo1, 0, &dwThreadId);
	CloseHandle(hThread);

	HANDLE hThreadex;
	unsigned int iThreadIdex;
	hThreadex = (HANDLE)_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)ThreadProex, &coo2, 0, &iThreadIdex);
	CloseHandle(hThreadex);
	for (int i = 0; i < 200; i++)
	{
		printf("world\n");
	}
	system("pause");
	return 0;
}

DWORD WINAPI ThreadPro(LPVOID lpParam)
{
	Coor *p = (Coor *)lpParam;
	for (int i = 0; i < 200; i++)
	{
		printf("hello %d,%d\n", p->x, p->y);
	}
	return 0;
}

DWORD WINAPI ThreadProex(LPVOID lpParam)
{
	Coor *p = (Coor *)lpParam;
	for (int i = 0; i < 200; i++)
	{
		printf("_beginthreadex %d,%d\n", p->x, p->y);
	}
	return 0;
}