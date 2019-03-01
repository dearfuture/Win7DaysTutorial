/*
程序讲解：袁春旭

涉及API
SetPriorityClass
SetProcessPriorityBoost

REALTIME_PRIORITY_CLASS
HIGH_PRIORITY_CLASS
ABOVE_NORMAL_PRIORITY_CLASS
NORMAL_PRIORITY_CLASS
BELOW_NORMAL_PRIORITY_CLASS
IDLE_PRIORITY_CLASS
*/


#include <Windows.h>
#include <stdlib.h>



int main(void)
{
	STARTUPINFO si1;
	PROCESS_INFORMATION pi1;

	ZeroMemory(&si1, sizeof(si1));
	si1.cb = sizeof(si1);
	ZeroMemory(&pi1, sizeof(pi1));

	STARTUPINFO si2;
	PROCESS_INFORMATION pi2;

	ZeroMemory(&si2, sizeof(si2));
	si2.cb = sizeof(si2);
	ZeroMemory(&pi2, sizeof(pi2));

	CreateProcess(NULL, "5x_demo1.exe", NULL, NULL, FALSE, 0, NULL, NULL, &si1, &pi1); 
	SetPriorityClass(pi1.hProcess, IDLE_PRIORITY_CLASS);
	//true:关闭动态调整
	SetProcessPriorityBoost(pi1.hProcess, true);
	CreateProcess(NULL, "5x_demo2.exe", NULL, NULL, FALSE, 0, NULL, NULL, &si2, &pi2);
	
	SetPriorityClass(pi2.hProcess, HIGH_PRIORITY_CLASS);
	WaitForSingleObject(pi1.hProcess, INFINITE);
	WaitForSingleObject(pi2.hProcess, INFINITE);

	CloseHandle(pi1.hProcess);
	CloseHandle(pi1.hThread);

	CloseHandle(pi2.hProcess);
	CloseHandle(pi2.hThread);

	system("pause");
	return 0;
}