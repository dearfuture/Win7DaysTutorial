/*
讲解：袁春旭
内容：线程池
情况三：在内核对象触发时调用一个函数
API:
CreateThreadpoolWait
SetThreadpoolWait
//参数二：hObject:要等待的内核对象
//参数三：pftTimeout:
	0:立即执行，不等
	NULL:死等
	正数：等到某时间点
	负数：再等多久
	单位100纳秒，换算关系1秒=1000毫秒=1000*1000微秒=1000*1000*1000纳秒
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