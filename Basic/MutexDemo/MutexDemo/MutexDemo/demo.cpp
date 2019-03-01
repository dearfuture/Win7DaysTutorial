/*
  �߳�ͬ���� mutex   ������
  ������ʹ�����º���

  CreateMutex
  CreateMutexEx
  OpenMutex
  ReleaseMutex

  WaitForSingleObject   //�ȴ�����״̬





  ʾ�����⣺Ԭ����




















  ʾ����д��Ԭ����   QQ��2064438762
*/

#include <Windows.h>
#include <process.h>
#include <stdio.h.>
#include <stdlib.h>

DWORD WINAPI BaoShu1(LPVOID *lparam);
DWORD WINAPI BaoShu2(LPVOID *lparam);
DWORD WINAPI BaoShu3(LPVOID *lparam);

HANDLE g_hMutex;

int main(void)
{
	g_hMutex = CreateMutex(NULL, FALSE, NULL);

	HANDLE hThread[3];
	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)BaoShu1, NULL, 0, NULL);
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)BaoShu2, NULL, 0, NULL);
	hThread[2] = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)BaoShu3, NULL, 0, NULL);
	
	//getchar();
	//ReleaseMutex(g_hMutex);

	WaitForMultipleObjects(3, hThread, TRUE, INFINITE);

	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);
	CloseHandle(hThread[2]);



	system("pause");
	return 0;
}

DWORD WINAPI BaoShu1(LPVOID *lparam)
{
	WaitForSingleObject(g_hMutex, INFINITE);
	for (int i = 0; i < 10; i++)
	{
		printf("1\n");
	}

	return 0;
}

DWORD WINAPI BaoShu2(LPVOID *lparam)
{
	WaitForSingleObject(g_hMutex, INFINITE);
	for (int i = 0; i < 10; i++)
	{
		printf("2\n");
	}

	return 0;
}

DWORD WINAPI BaoShu3(LPVOID *lparam)
{
	WaitForSingleObject(g_hMutex, INFINITE);
	for (int i = 0; i < 10; i++)
	{
		printf("3\n");
	}

	return 0;
}