/*----------------------------------------*/
/*
 线程编码实战
 
 需要用到的函数：
	CreateThread   
	_beginthreadex  
	SetThreadPriority
	ExitThread
	TerminateThread
	ExitProcess
	_endthreadex
	1.创建线程，无参  CreateThread  _beginthreadex
	2.创建线程，有参  整型 字符串 结构体 对象(内置对象，自定义对象) 七日成蝶-C++精品课程全集视频课程
	3.给线程函数传参
	4.设置优先级
	5.创建多个不同优先级线程
	6.在线程中添加对象，看线程退出时是否执行了析构函数
	7.ExitThread，_endthreadex线程，看线程退出时是否执行了析构函数
	8.TerminateThread线程，看线程退出时是否执行了析构函数
	9.ExitProcess进程，看线程退出时是否执行了析构函数






	作者：袁春旭（QQ：2064438762）
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
		cout << m_iX << "------" << m_iY << endl;
	}

	~Coor()
	{
		cout << m_iX << "||||||" << m_iY << endl;
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
	
	hThread = CreateThread(NULL, NULL, ThreadPro, NULL, 0, &dwThreadId);
	CloseHandle(hThread);

	HANDLE hThreadex;
	unsigned int iThreadIdex;
	hThreadex = (HANDLE)_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)ThreadProex, NULL, 0, &iThreadIdex);
	CloseHandle(hThreadex);
	
	system("pause");
	return 0;
}

DWORD WINAPI ThreadPro(LPVOID lpParam)
{
	Coor coo(1,2);
	return 0;
}

DWORD WINAPI ThreadProex(LPVOID lpParam)
{
	Coor coo(3, 4);
	return 0;
}