/*
  �߳�ͬ����SignalObjectAndWait

  ʾ�����⣺Ԭ����

  ��һ�����������������ź������¼�

  �ڶ������������������ź������¼�����ʱ�������̡��̡߳���ҵ��



















  ʾ����д��Ԭ����   QQ��2064438762
 */



#include <Windows.h>
#include <process.h>
#include <stdio.h.>
#include <stdlib.h>

DWORD WINAPI sayhello(LPVOID *lparam);

HANDLE g_hEvent;

int main(void)
{
	g_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);


	HANDLE hThread;
	hThread = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)sayhello, NULL, 0, NULL);
	//SetEvent(g_hEvent);
	//xxx  xxx   xxx
	//WaitForSingleObject(hThread, INFINITE);
	SignalObjectAndWait(g_hEvent, hThread, INFINITE, TRUE);
	for (int i = 0; i < 100; i++)
	{
		printf("world\n");
	}

	CloseHandle(hThread);



	system("pause");
	return 0;
}

DWORD WINAPI sayhello(LPVOID *lparam)
{
	WaitForSingleObject(g_hEvent, INFINITE);
	for (int i = 0; i < 100; i++)
	{
		printf("hello\n");
	}

	return 0;
}