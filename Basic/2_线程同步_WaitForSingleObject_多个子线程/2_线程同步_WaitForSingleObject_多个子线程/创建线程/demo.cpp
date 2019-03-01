/*----------------------------------------*/
/*
 线程同步编码实战
 
 需要用到的函数：
	WaitForSingleObject
	WaitForMultipleObjects
	1.主线程和子线程执行顺序控制
	2.子线程间执行顺序控制
	

















	作者：袁春旭（QQ：2064438762）
 */
/*----------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <process.h>

int a = 10;
int b = 20;
int c = 0;

HANDLE hThread1;
HANDLE hThread2;

HANDLE hThread[4];

DWORD WINAPI GetNum(LPVOID lpParam);
DWORD WINAPI GetResult(LPVOID lpParam);
DWORD WINAPI AAA(LPVOID lpParam);
DWORD WINAPI BBB(LPVOID lpParam);

int main(int argc, char *argv[])
{
	DWORD dwThreadId = 0;

	
	unsigned int iThreadIdex;
	hThread[0] = (HANDLE)_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)GetResult, NULL, 0, &iThreadIdex);
	hThread[1] = (HANDLE)_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)GetNum, NULL, 0, &iThreadIdex);
	hThread[2] = (HANDLE)_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)AAA, NULL, 0, &iThreadIdex);
	hThread[3] = (HANDLE)_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)BBB, NULL, 0, &iThreadIdex);
	//WaitForSingleObject(hThread[0], INFINITE);
	//WaitForSingleObject(hThread[1], INFINITE);
	DWORD errNo = WaitForMultipleObjects(4, hThread, false, INFINITE);
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);
	CloseHandle(hThread[2]);
	CloseHandle(hThread[3]);
	
	printf("--errNO = %d--\n", errNo);
	system("pause");
	return 0;
}

DWORD WINAPI GetNum(LPVOID lpParam)
{
	a = 100;
	b = 200;
	return 0;
}

DWORD WINAPI GetResult(LPVOID lpParam)
{
	WaitForSingleObject(hThread[1], INFINITE);
	c = a + b;
	return 0;
}

DWORD WINAPI AAA(LPVOID lpParam)
{
	a = 100;
	b = 200;
	return 0;
}

DWORD WINAPI BBB(LPVOID lpParam)
{
	a = 100;
	b = 200;
	return 0;
}