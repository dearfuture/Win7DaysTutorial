/*
 编写：袁春旭
 讲解：袁春旭
 内容：线程池同步
 API：InterlockedAdd
 */

#include <stdio.h>
#include <Windows.h>

LONG g_count = 0;

void WINAPI ThreadFunc(PTP_CALLBACK_INSTANCE pInstance, void *p, PTP_WORK pWork)
{
	for (int i = 0; i < 200; i++)
	{
		//g_count = g_count + 1;
		InterlockedAdd(&g_count, 1);
	}
}

int main(void)
{
	PTP_WORK pwk = CreateThreadpoolWork((PTP_WORK_CALLBACK)ThreadFunc, NULL, NULL);
	
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