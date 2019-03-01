/*
讲解：袁春旭
内容：线程池
情况二：每隔一段时间调用一个函数
API:
CreateThreadpoolTimer
SetThreadpoolTimer
//参数二：pftDueTime:第一次调用回调函数的时间，-1立即执行，负数为相对时间，
//单位100纳秒，换算关系1秒=1000毫秒=1000*1000微秒=1000*1000*1000纳秒
//参数三：msPeriod:该参数为0表示只触发一次，非零值表示两次调用回调函数的时间间隔
//参数四：msWindowLength:该参数为参数三的周期性提供一个随机延迟时间值
//参数四作用1：避免多计时器同频回调冲突。
//参数四作用2：避免多计时器相近时段上下文切换。7+2  8+2  在范围9重叠，可以省去放入线程池及再从线程池取出的时间。
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