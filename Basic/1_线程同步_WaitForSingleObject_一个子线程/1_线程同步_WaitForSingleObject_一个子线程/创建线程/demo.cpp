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


DWORD WINAPI ThreadPro(LPVOID lpParam);

int main(int argc, char *argv[])
{
	DWORD dwThreadId = 0;

	HANDLE hThreadex;
	unsigned int iThreadIdex;
	hThreadex = (HANDLE)_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)ThreadPro, NULL, 0, &iThreadIdex);
	DWORD errNo = WaitForSingleObject(hThreadex, 10000);
	CloseHandle(hThreadex);
	
	for (int i = 0; i < 200; i++)
	{
		printf("world\n");
	}
	printf("--%x--", errNo);
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