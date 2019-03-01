/*
��д��Ԭ����
���⣺Ԭ����
���ݣ��̳߳�-��Դ�ж�
API��CallbackMayRunLong
*/

#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>

unsigned long g_count = 0;
void NTAPI TaskHandler(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_WORK Work)
{
	if (CallbackMayRunLong(Instance))
	{
		printf("%d ����ʣ����Դ\n", InterlockedIncrement(&g_count));
	}
	else
	{
		printf("��Դ�ù���\n");
	}
	
	Sleep(5000);
	printf("�������߳�!\n");
}


int main(void)
{
	PTP_WORK workItem = CreateThreadpoolWork(TaskHandler, NULL, NULL);

	for (int i = 0; i < 1000; i++)
	{
		SubmitThreadpoolWork(workItem);
	}
	

	WaitForThreadpoolWorkCallbacks(workItem, false);


	printf("���߳��������\n");
	getchar();
	CloseThreadpoolWork(workItem);

	system("pause");
	return 0;
}