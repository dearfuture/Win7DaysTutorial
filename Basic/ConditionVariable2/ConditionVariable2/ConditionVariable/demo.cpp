#include <Windows.h>
#include <process.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
using namespace std;

DWORD WINAPI ReadProc(LPVOID *lparam);
DWORD WINAPI WriteProc(LPVOID *lparam);

DWORD g_iCount = 0;


BOOL g_bFlag = TRUE;

CONDITION_VARIABLE g_cond;

SRWLOCK g_sl;

vector<int> vct;

int main(void)
{
	InitializeSRWLock(&g_sl);

	HANDLE hThread[2];
	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)ReadProc, NULL, 0, NULL);
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)WriteProc, NULL, 0, NULL);
	
	Sleep(2000);
	g_bFlag = FALSE;
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);

	cout << "g_iCount = " << g_iCount << endl;
	

	system("pause");
	return 0;
}

DWORD WINAPI WriteProc(LPVOID *lparam)
{
	while (g_bFlag)
	{
		AcquireSRWLockExclusive(&g_sl);
		vct.push_back(g_iCount++);
		cout << "write finished" << endl;
		ReleaseSRWLockExclusive(&g_sl);
		
		WakeConditionVariable(&g_cond);

		//Sleep(100);
	}
	return 0;
}


DWORD WINAPI ReadProc(LPVOID *lparam)
{
	while (g_bFlag)
	{
		AcquireSRWLockShared(&g_sl);
		
		while (vct.empty())
		{
			SleepConditionVariableSRW(&g_cond, &g_sl, INFINITE, CONDITION_VARIABLE_LOCKMODE_SHARED);
		}
		cout << vct.back() << endl;
		vct.pop_back();
		cout << "read finished" << endl;
		ReleaseSRWLockShared(&g_sl);
		
		//Sleep(100);

	}


	return 0;
}