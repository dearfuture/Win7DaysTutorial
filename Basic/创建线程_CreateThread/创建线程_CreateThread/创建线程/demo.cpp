/*----------------------------------------*/
/*
 �̱߳���ʵս
 
 ��Ҫ�õ��ĺ�����
	CreateThread   
	_beginthreadex  
	SetThreadPriority
	ExitThread
	TerminateThread
	ExitProcess
	_endthreadex
	1.�����̣߳��޲�  CreateThread  _beginthreadex
	2.�����̣߳��в�
	3.���̺߳�������
	4.�������ȼ�
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

DWORD WINAPI ThreadPro(LPVOID lpParam);

int main(int argc, char *argv[])
{
	DWORD dwThreadId = 0;
	HANDLE hThread;
	hThread = CreateThread(NULL, NULL, ThreadPro, NULL, 0, &dwThreadId);
	CloseHandle(hThread);

	for (int i = 0; i < 20; i++)
	{
		printf("world\n");
	}
	system("pause");
	return 0;
}

DWORD WINAPI ThreadPro(LPVOID lpParam)
{
	for (int i = 0; i < 20; i++)
	{
		printf("hello\n");
	}
	return 0;
}