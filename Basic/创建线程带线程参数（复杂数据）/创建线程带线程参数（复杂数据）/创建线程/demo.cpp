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
	2.�����̣߳��в�  ���� �ַ��� �ṹ�� ����(���ö����Զ������) ���ճɵ�-C++��Ʒ�γ�ȫ����Ƶ�γ�
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
#include <string>
#include <iostream>
using namespace std;

DWORD WINAPI ThreadPro(LPVOID lpParam);
DWORD WINAPI ThreadProex(LPVOID lpParam);

class Coor
{
public:
	Coor(int iX, int iY)
	{
		m_iX = iX;
		m_iY = iY;
	}
	
	void printCoor()
	{
		cout << m_iX << "," << m_iY << endl;
	}
private:
	int m_iX;
	int m_iY;
};

int main(int argc, char *argv[])
{
	DWORD dwThreadId = 0;
	HANDLE hThread;
	int x = 20;
	int y = 40;
	//char str1[200] = "yuanchunxu";
	//char str2[200] = "2064438762";
	Coor coo1(4, 5) ;
	Coor coo2(20, 80);
	string str1 = "yuanchunxu";
	string str2 = "2064438762";
	string *pStr3 = new string("yuanchunxuAAA");
	string *pStr4 = new string("2064438762BBB");
	hThread = CreateThread(NULL, NULL, ThreadPro, &coo1, 0, &dwThreadId);
	CloseHandle(hThread);

	HANDLE hThreadex;
	unsigned int iThreadIdex;
	hThreadex = (HANDLE)_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)ThreadProex, &coo2, 0, &iThreadIdex);
	CloseHandle(hThreadex);
	/*for (int i = 0; i < 200; i++)
	{
		printf("world\n");
	}*/
	system("pause");
	return 0;
}

DWORD WINAPI ThreadPro(LPVOID lpParam)
{
	Coor *p = (Coor *)lpParam;
	p->printCoor();
	return 0;
}

DWORD WINAPI ThreadProex(LPVOID lpParam)
{
	Coor *p = (Coor *)lpParam;
	p->printCoor();
	return 0;
}