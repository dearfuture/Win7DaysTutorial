/*
���⣺Ԭ����
���ݣ��̳߳�
����������ں˶��󴥷�ʱ����һ������
API:
CreateThreadpoolWait
SetThreadpoolWait
//��������hObject:Ҫ�ȴ����ں˶���
//��������pftTimeout:
	0:����ִ�У�����
	NULL:����
	�������ȵ�ĳʱ���
	�������ٵȶ��
	��λ100���룬�����ϵ1��=1000����=1000*1000΢��=1000*1000*1000����
CloseThreadpoolWait
*/

#include <stdio.h>
#include <Windows.h>

VOID NTAPI WaitCallback(
	PTP_CALLBACK_INSTANCE Instance,
	PVOID                 Context,
	PTP_WAIT              Wait,
	TP_WAIT_RESULT        WaitResult
)
{
	printf("hello yuanchunxu");
}

int main(void)
{
	HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	PTP_WAIT pwait = CreateThreadpoolWait(WaitCallback, NULL, NULL);
	ULARGE_INTEGER ulRelativeStartTime;
	ulRelativeStartTime.QuadPart = (LONGLONG)-(50000000);
	FILETIME ftRelativeStartTime;
	ftRelativeStartTime.dwHighDateTime = ulRelativeStartTime.HighPart;
	ftRelativeStartTime.dwLowDateTime = ulRelativeStartTime.LowPart;
	SetThreadpoolWait(pwait, hEvent, &ftRelativeStartTime);
	getchar();
	SetEvent(hEvent);

	getchar();
	CloseThreadpoolWait(pwait);
	return 0;
}