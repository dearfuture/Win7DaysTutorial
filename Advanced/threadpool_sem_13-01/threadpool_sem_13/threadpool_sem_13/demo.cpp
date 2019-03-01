/*
编写：袁春旭
讲解：袁春旭
内容：线程池同步-信号量
API：ReleaseSemaphoreWhenCallbackReturns
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
	ReleaseSemaphoreWhenCallbackReturns(pInstance, *(HANDLE*)p, 1);
}

int main(void)
{
	HANDLE hSemaphore = CreateSemaphore(NULL, 0, 100, NULL);  //初始100个资源，给定1个资源

	ReleaseSemaphore(hSemaphore, 10, NULL);

	PTP_WORK pwk = CreateThreadpoolWork((PTP_WORK_CALLBACK)ThreadFunc, &hSemaphore, NULL);

	for (int i = 0; i < 100000; i++)
	{
		SubmitThreadpoolWork(pwk);
	}


	WaitForThreadpoolWorkCallbacks(pwk, FALSE);

	CloseThreadpoolWork(pwk);

	CloseHandle(hSemaphore);

	printf("g_count = %d\n", g_count);

	getchar();
	return 0;
}