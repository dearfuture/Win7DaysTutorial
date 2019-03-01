/*
编写：袁春旭
讲解：袁春旭
内容：线程池同步-事件内核对象
API：SetEventWhenCallbackReturns
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
	SetEventWhenCallbackReturns(pInstance, *(HANDLE*)p);
}

int main(void)
{
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	SetEvent(hEvent);

	PTP_WORK pwk = CreateThreadpoolWork((PTP_WORK_CALLBACK)ThreadFunc, &hEvent, NULL);

	for (int i = 0; i < 100000; i++)
	{
		SubmitThreadpoolWork(pwk);
	}


	WaitForThreadpoolWorkCallbacks(pwk, FALSE);

	CloseThreadpoolWork(pwk);

	printf("g_count = %d\n", g_count);

	getchar();
	return 0;
}