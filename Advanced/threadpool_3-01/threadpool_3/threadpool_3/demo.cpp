/*
���⣺Ԭ����
���ݣ��̳߳�
�������ÿ��һ��ʱ�����һ������
API:
CreateThreadpoolTimer
SetThreadpoolTimer
//��������pftDueTime:��һ�ε��ûص�������ʱ�䣬-1����ִ�У�����Ϊ���ʱ�䣬
//��λ100���룬�����ϵ1��=1000����=1000*1000΢��=1000*1000*1000����
//��������msPeriod:�ò���Ϊ0��ʾֻ����һ�Σ�����ֵ��ʾ���ε��ûص�������ʱ����
//�����ģ�msWindowLength:�ò���Ϊ���������������ṩһ������ӳ�ʱ��ֵ
//����������1��������ʱ��ͬƵ�ص���ͻ��
//����������2��������ʱ�����ʱ���������л���7+2  8+2  �ڷ�Χ9�ص�������ʡȥ�����̳߳ؼ��ٴ��̳߳�ȡ����ʱ�䡣
CloseThreadpoolTimer
*/

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

int g_nSecLeft = 10;
#define ID_MSGBOX_STATIC_TEXT 0x0000ffff

VOID NTAPI MsgBoxCallback(
	PTP_CALLBACK_INSTANCE Instance,
	PVOID                 Context,
	PTP_TIMER             Timer
	)
{
	HWND hwnd = FindWindow(NULL, "tip_yuanchunxu");
	if (NULL == hwnd)
	{
		return;
	}
	char str[20] = {0};
	g_nSecLeft--;
	if (g_nSecLeft <= 0)
	{
		EndDialog(hwnd, IDOK);
	}
	_itoa_s(g_nSecLeft, str, 20, 10);
	//printf("hello\n");
	SetDlgItemText(hwnd, ID_MSGBOX_STATIC_TEXT, str);
}

int main(void)
{
	PTP_TIMER lpTimer = CreateThreadpoolTimer(MsgBoxCallback, NULL, NULL);

	ULARGE_INTEGER ulRelativeStartTime1;
	ulRelativeStartTime1.QuadPart = (LONGLONG)-(10000000);
	FILETIME ftRelativeStartTime1;
	ftRelativeStartTime1.dwHighDateTime = ulRelativeStartTime1.HighPart;
	ftRelativeStartTime1.dwLowDateTime = ulRelativeStartTime1.LowPart;

	/*
	ULARGE_INTEGER ulRelativeStartTime2;
	ulRelativeStartTime2.QuadPart = -1;
	FILETIME ftRelativeStartTime2;
	ftRelativeStartTime2.dwHighDateTime = ulRelativeStartTime2.HighPart;
	ftRelativeStartTime2.dwLowDateTime = ulRelativeStartTime2.LowPart;
	*/

	SetThreadpoolTimer(lpTimer, &ftRelativeStartTime1, 1000, 0);
	MessageBox(NULL, "1000", "tip_yuanchunxu", MB_OK);
	CloseThreadpoolTimer(lpTimer);

	MessageBox(NULL, g_nSecLeft <= 1 ? "Timeout" : "user responded", "tip", MB_OK);
	

	getchar();
	return 0;
}