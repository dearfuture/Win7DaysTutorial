/*----------------------------------------*/
/*
 �̱߳���ʵս
 
 ��Ҫ�õ��ĺ�����
	CreateThread 
	_beginthreadex
	SetThreadPriority
	ResumeThread
	ExitThread
	TerminateThread
	ExitProcess
	_endthreadex
	1.�����̣߳��޲�  CreateThread  _beginthreadex
	2.�����̣߳��в�
	3.���̺߳�������
	4.�������ȼ�  SetThreadPriority  ResumeThread
	5.���������ͬ���ȼ��߳�
	6.���߳�����Ӷ��󣬿��߳��˳�ʱ�Ƿ�ִ������������
	7.ExitThread��_endthreadex�̣߳����߳��˳�ʱ�Ƿ�ִ������������
	8.TerminateThread�̣߳����߳��˳�ʱ�Ƿ�ִ������������
	9.ExitProcess���̣����߳��˳�ʱ�Ƿ�ִ������������





	���ߣ�Ԭ����QQ��2064438762��
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