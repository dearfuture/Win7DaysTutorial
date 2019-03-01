/*
  线程同步：slim 读写锁

  InitializeSRWLock
  AcquireSRWLockExclusive
  ReleaseSRWLockExclusive
  AcquireSRWLockShared
  ReleaseSRWLockShared

  示例讲解：袁春旭










 */


#include <Windows.h>
#include <process.h>
#include <stdio.h.>
#include <stdlib.h>

DWORD WINAPI ThreadWrite(LPVOID *lparam);
DWORD WINAPI ThreadWrite2(LPVOID *lparam);
DWORD WINAPI ThreadRead(LPVOID *lparam);

DWORD g_iCount1 = 0;
DWORD g_iCount2 = 0;
BOOL g_bFlag = TRUE;

SRWLOCK g_sl;

int main(void)
{
	InitializeSRWLock(&g_sl);

	HANDLE hThread[3];
	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)ThreadWrite, NULL, 0, NULL);
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)ThreadRead, NULL, 0, NULL);
	hThread[2] = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)ThreadWrite2, NULL, 0, NULL);
	Sleep(1000);
	g_bFlag = FALSE;
	WaitForMultipleObjects(3, hThread, TRUE, INFINITE);
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);
	CloseHandle(hThread[2]);

	printf("\n--g_iCount1 = %d\n", g_iCount1);
	printf("\n--g_iCount2 = %d\n", g_iCount2);

	system("pause");
	return 0;
}

DWORD WINAPI ThreadWrite(LPVOID *lparam)
{
	while (g_bFlag)
	{
		AcquireSRWLockExclusive(&g_sl);
		g_iCount1++;
		g_iCount2++;
		ReleaseSRWLockExclusive(&g_sl);
	}
	return 0;
}

DWORD WINAPI ThreadWrite2(LPVOID *lparam)
{
	while (g_bFlag)
	{
		AcquireSRWLockExclusive(&g_sl);
		g_iCount1++;
		g_iCount2++;
		ReleaseSRWLockExclusive(&g_sl);
	}
	return 0;
}

DWORD WINAPI ThreadRead(LPVOID *lparam)
{
	while (g_bFlag)
	{
		AcquireSRWLockShared(&g_sl);
		printf("\ng_iCount1 = %d\n", g_iCount1);
		printf("\ng_iCount2 = %d\n", g_iCount2);
		ReleaseSRWLockShared(&g_sl);
	}
	return 0;
}