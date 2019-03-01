/*
�߳�ͬ������ʵս--�ؼ�����չ
�ؼ��� | �ٽ���  +  ��������

CRITICAL_SECTION
CONDITION_VARIABLE

������������ʹ�ó��ϣ�
������ + ������  ����������������Ʒ�������ѣ�

�õ���API������
InitializeCriticalSection
InitializeCriticalSectionAndSpinCount
InitializeCriticalSectionEx
EnterCriticalSection
TryEnterCriticalSection
LeaveCriticalSection
DeleteCriticalSection

WakeConditionVariable
SleepConditionVariableCS

BOOL SleepConditionVariableCS(
	PCONDITION_VARIABLE ConditionVariable,
	PCRITICAL_SECTION   CriticalSection,
	DWORD               dwMilliseconds
);

VOID WakeConditionVariable(
	PCONDITION_VARIABLE ConditionVariable
);


ʾ�����⣺Ԭ����













ʾ����д��Ԭ����   QQ��2064438762
*/


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
CONDITION_VARIABLE g_cond;   //��������

CRITICAL_SECTION g_cs;
vector<int> vct;

int main(void)
{
	InitializeCriticalSectionAndSpinCount(&g_cs, 10);
	HANDLE hThread[2];
	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)ReadProc, NULL, 0, NULL);
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)WriteProc, NULL, 0, NULL);
	
	Sleep(2000);
	g_bFlag = FALSE;
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	DeleteCriticalSection(&g_cs);
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
		EnterCriticalSection(&g_cs);
		vct.push_back(g_iCount);
		g_iCount++;
		cout << "write finished" << endl;
		LeaveCriticalSection(&g_cs);
		
		WakeConditionVariable(&g_cond);

		//Sleep(10);
	}
	return 0;
}


DWORD WINAPI ReadProc(LPVOID *lparam)
{
	while (g_bFlag)
	{
		EnterCriticalSection(&g_cs);
		
		while (vct.empty())
		{
			//goto end;
			SleepConditionVariableCS(&g_cond, &g_cs, INFINITE);
		}
		cout << vct.back() << endl;
		vct.pop_back();
		cout << "read finished" << endl;
	//end:
		LeaveCriticalSection(&g_cs);
		
		//Sleep(10);
	}

	return 0;
}