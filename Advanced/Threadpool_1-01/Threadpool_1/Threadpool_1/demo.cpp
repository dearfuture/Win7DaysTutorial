
//���⣺Ԭ����
//���⣺�̳߳�
//API��TrySubmitThreadpoolCallback ȱ�㣺ÿ���ύ�����µĹ�����
//     CreateThreadpoolWork
//	   SubmitThreadpoolWork
//     WaitForThreadpoolWorkCallbacks
//     CloseThreadpoolWork
//����һ�����첽��ʽ����

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
		printf("ִ�гɹ�!\n");
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