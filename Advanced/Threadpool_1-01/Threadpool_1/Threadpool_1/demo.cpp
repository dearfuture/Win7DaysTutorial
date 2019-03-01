
//讲解：袁春旭
//主题：线程池
//API：TrySubmitThreadpoolCallback 缺点：每次提交创建新的工作项
//     CreateThreadpoolWork
//	   SubmitThreadpoolWork
//     WaitForThreadpoolWorkCallbacks
//     CloseThreadpoolWork
//情形一：以异步方式调用

#include <stdio.h>
#include <Windows.h>

VOID NTAPI SimpleCallback(
	PTP_CALLBACK_INSTANCE Instance,
	PVOID                 Context
	);

VOID NTAPI WorkCallback(
	PTP_CALLBACK_INSTANCE Instance,
	PVOID                 Context,
	PTP_WORK              Work
	);

int main(void)
{
	char str[] = "yuanchunxu";
	/*if (TrySubmitThreadpoolCallback(SimpleCallback, str, NULL))
	{
		printf("执行成功!\n");
	}*/
	PTP_WORK workitem = CreateThreadpoolWork(WorkCallback, str, NULL);
	SubmitThreadpoolWork(workitem);
	SubmitThreadpoolWork(workitem);
	SubmitThreadpoolWork(workitem);
	SubmitThreadpoolWork(workitem);

	CloseThreadpoolWork(workitem);

	getchar();
	return 0;
}

VOID NTAPI SimpleCallback(
	PTP_CALLBACK_INSTANCE Instance,
	PVOID                 Context
)
{
	printf("hello %s\n", (char *)Context);
}

VOID NTAPI WorkCallback(
	PTP_CALLBACK_INSTANCE Instance,
	PVOID                 Context,
	PTP_WORK              Work
)
{
	printf("hello %s\n", (char *)Context);
}