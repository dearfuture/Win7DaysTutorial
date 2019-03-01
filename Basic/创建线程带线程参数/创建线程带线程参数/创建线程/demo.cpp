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
	2.�����̣߳��в�  ���� �ַ��� �ṹ�� ���� ���ճɵ�-C++��Ʒ�γ�ȫ����Ƶ�γ�
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
#include <process.h>

DWORD WINAPI ThreadPro(LPVOID lpParam);
DWORD WINAPI ThreadProex(LPVOID lpParam);

typedef struct _coor
{
	int x;
	int y;
}Coor;

int main(int argc, char *argv[])
{
	DWORD dwThreadId = 0;
	HANDLE hThread;
	int x = 20;
	int y = 40;
	char str1[200] = "yuanchunxu";
	char str2[200] = "2064438762";
	Coor coo1 = {4,5};
	Coor coo2 = {20, 80};
	hThread = CreateThread(NULL, NULL, ThreadPro, &coo1, 0, &dwThreadId);
	CloseHandle(hThread);

	HANDLE hThreadex;
	unsigned int iThreadIdex;
	hThreadex = (HANDLE)_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)ThreadProex, &coo2, 0, &iThreadIdex);
	CloseHandle(hThreadex);
	for (int i = 0; i < 200; i++)
	{
		printf("world\n");
	}
	system("pause");
	return 0;
}

DWORD WINAPI ThreadPro(LPVOID lpParam)
{
	Coor *p = (Coor *)lpParam;
	for (int i = 0; i < 200; i++)
	{
		printf("hello %d,%d\n", p->x, p->y);
	}
	return 0;
}

DWORD WINAPI ThreadProex(LPVOID lpParam)
{
	Coor *p = (Coor *)lpParam;
	for (int i = 0; i < 200; i++)
	{
		printf("_beginthreadex %d,%d\n", p->x, p->y);
	}
	return 0;
}