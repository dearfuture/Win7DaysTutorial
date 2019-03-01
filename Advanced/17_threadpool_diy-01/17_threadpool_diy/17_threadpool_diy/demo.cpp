/*
��д��Ԭ����
���⣺Ԭ����
���ݣ��̳߳�-�Զ����̳߳�
API��
CreateThreadpool �����̳߳�
CloseThreadpool  �ر��̳߳�
SetThreadpoolThreadMinimum �����̳߳���С��Դ��
SetThreadpoolThreadMaximum �����̳߳������Դ��

InitializeThreadpoolEnvironment ��ʼ���̳߳ػ�������
DestroyThreadpoolEnvironment    �����̳߳ػ�������
SetThreadpoolCallbackPool       �����̳߳ػص����̳߳�

CreateThreadpoolCleanupGroup        ����������
CloseThreadpoolCleanupGroupMembers  �ر�������
SetThreadpoolCallbackCleanupGroup   Ϊ�̳߳��趨������

*/

#include <stdio.h>
#include <Windows.h>

void NTAPI TaskHandler(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_WORK Work)
{

	Sleep(2000);
	printf("hello world!\n");
}

int main(void)
{
	PTP_POOL mypool = CreateThreadpool(NULL);

	SetThreadpoolThreadMinimum(mypool, 1);
	SetThreadpoolThreadMaximum(mypool, 10);

	TP_CALLBACK_ENVIRON cbe;

	InitializeThreadpoolEnvironment(&cbe);

	SetThreadpoolCallbackPool(&cbe, mypool);

	PTP_CLEANUP_GROUP cleanupGroup = CreateThreadpoolCleanupGroup();

	SetThreadpoolCallbackCleanupGroup(&cbe, cleanupGroup, NULL);

	PTP_WORK pwork = CreateThreadpoolWork((PTP_WORK_CALLBACK)TaskHandler, NULL, &cbe);

	SubmitThreadpoolWork(pwork);

	WaitForThreadpoolWorkCallbacks(pwork, false);
	printf("���߳�\n");
	CloseThreadpoolWork(pwork);

	CloseThreadpoolCleanupGroupMembers(cleanupGroup, false, NULL);

	DestroyThreadpoolEnvironment(&cbe);

	CloseThreadpool(mypool);


	getchar();
	return 0;
}