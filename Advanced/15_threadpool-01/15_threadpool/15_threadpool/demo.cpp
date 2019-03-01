/*
��д��Ԭ����
���⣺Ԭ����
���ݣ��̳߳�-��ǰ����
API��DisassociateCurrentThreadFromCallback
*/

#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>


void NTAPI TaskHandler(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_WORK Work)
{
	//xxxxxxxxxxxxx
	DisassociateCurrentThreadFromCallback(Instance);
	Sleep(2000);
	printf("�������߳�!\n");
}


int main(void)
{
	PTP_WORK workItem = CreateThreadpoolWork(TaskHandler, NULL, NULL);

	SubmitThreadpoolWork(workItem);
	
	WaitForThreadpoolWorkCallbacks(workItem, false);
	

	printf("���߳��������\n");
	getchar();
	CloseThreadpoolWork(workItem);

	system("pause");
	return 0;
}