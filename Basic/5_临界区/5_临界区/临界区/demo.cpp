/*
  线程同步编码实战
  关键段 | 临界区

  CRITICAL_SECTION

  用到的API函数：
  InitializeCriticalSection
  InitializeCriticalSectionAndSpinCount 
  InitializeCriticalSectionEx
  EnterCriticalSection
  TryEnterCriticalSection
  LeaveCriticalSection
  DeleteCriticalSection

  示例讲解：袁春旭













  示例编写：袁春旭   QQ：2064438762
 */

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <process.h>

DWORD WINAPI ThreadCounter(LPVOID);

int g_iCount1 = 0;
int g_iCount2 = 0;
int g_iFlag = 1;

CRITICAL_SECTION g_cs;

int main(void)
{
	HANDLE hThread[2];
	unsigned int iThreadIdex;
	//InitializeCriticalSection(&g_cs);
	InitializeCriticalSectionAndSpinCount(&g_cs, 10);
	//InitializeCriticalSectionEx(&g_cs, 10, 0);
	hThread[0] = (HANDLE)_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)ThreadCounter, NULL, 0, &iThreadIdex);
	hThread[1] = (HANDLE)_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)ThreadCounter, NULL, 0, &iThreadIdex);

	Sleep(1000);
	g_iFlag = 0;

	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

	printf("g_iCount1 = %d\n", g_iCount1);
	printf("g_iCount2 = %d\n", g_iCount2);

	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);

	DeleteCriticalSection(&g_cs);

	system("pause");
	return 0;
}

DWORD WINAPI ThreadCounter(LPVOID lpParameter)
{
	//EnterCriticalSection(&g_cs);
	while(g_iFlag)
	{
		if (TryEnterCriticalSection(&g_cs))
		{
			g_iCount1++; //InterLockedxxxx
			g_iCount2++;
			LeaveCriticalSection(&g_cs);
		}
		else
		{
			printf("hello");
		}
	}
	//LeaveCriticalSection(&g_cs);
	return 0;
}
