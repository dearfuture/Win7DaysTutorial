/*
编写：袁春旭
讲解：袁春旭
内容：线程池-资源判断
API：CallbackMayRunLong
*/

#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>

unsigned long g_count = 0;
void NTAPI TaskHandler(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_WORK Work)
{
	if (CallbackMayRunLong(Instance))
	{
		printf("%d 还有剩余资源\n", InterlockedIncrement(&g_count));
	}
	else
	{
		printf("资源用光了\n");
	}
	
	Sleep(5000);
	printf("我是子线程!\n");
}


int main(void)
{
	PTP_WORK workItem = CreateThreadpoolWork(TaskHandler, NULL, NULL);

	for (int i = 0; i < 1000; i++)
	{
		SubmitThreadpoolWork(workItem);
	}
	

	WaitForThreadpoolWorkCallbacks(workItem, false);


	printf("主线程运行完毕\n");
	getchar();
	CloseThreadpoolWork(workItem);

	system("pause");
	return 0;
}