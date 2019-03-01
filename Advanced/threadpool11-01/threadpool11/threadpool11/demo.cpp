/*
编写：袁春旭
讲解：袁春旭
内容：线程池同步-临界区
API：LeaveCriticalSectionWhenCallbackReturns
*/

#include <stdio.h>
#include <Windows.h>

LONG g_count = 0;

void WINAPI ThreadFunc(PTP_CALLBACK_INSTANCE pInstance, void *p, PTP_WORK pWork)
{
	EnterCriticalSection((CRITICAL_SECTION*)p);
	for (int i = 0; i < 200; i++)
	{
		g_count = g_count + 1;
	}
	//LeaveCriticalSection((CRITICAL_SECTION*)p);
	LeaveCriticalSectionWhenCallbackReturns(pInstance, (CRITICAL_SECTION*)p);
}

int main(void)
{
	CRITICAL_SECTION cs;
	InitializeCriticalSection(&cs);

	PTP_WORK pwk = CreateThreadpoolWork((PTP_WORK_CALLBACK)ThreadFunc, &cs, NULL);

	for (int i = 0; i < 100000; i++)
	{
		SubmitThreadpoolWork(pwk);
	}
	
	WaitForThreadpoolWorkCallbacks(pwk, FALSE);

	DeleteCriticalSection(&cs);

	CloseThreadpoolWork(pwk);

	printf("g_count = %d\n", g_count);

	getchar();
	return 0;
}