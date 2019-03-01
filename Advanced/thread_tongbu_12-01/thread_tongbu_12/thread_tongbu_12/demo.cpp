/*
编写：袁春旭
讲解：袁春旭
内容：线程池同步-互斥量
API：ReleaseMutexWhenCallbackReturns
*/


#include <stdio.h>
#include <Windows.h>

LONG g_count = 0;

void WINAPI ThreadFunc(PTP_CALLBACK_INSTANCE pInstance, void *p, PTP_WORK pWork)
{
	WaitForSingleObject(*(HANDLE*)p, INFINITE);
	for (int i = 0; i < 200; i++)
	{
		g_count = g_count + 1;
	}
	ReleaseMutexWhenCallbackReturns(pInstance, *(HANDLE*)p);
}

int main(void)
{
	HANDLE hMutex = CreateMutex(NULL, FALSE, NULL);

	PTP_WORK pwk = CreateThreadpoolWork((PTP_WORK_CALLBACK)ThreadFunc, &hMutex, NULL);

	for (int i = 0; i < 100000; i++)
	{
		SubmitThreadpoolWork(pwk);
	}


	WaitForThreadpoolWorkCallbacks(pwk, FALSE);

	CloseThreadpoolWork(pwk);

	CloseHandle(hMutex);

	printf("g_count = %d\n", g_count);

	getchar();
	return 0;
}