/*
编写：袁春旭
讲解：袁春旭
内容：线程池-自定义线程池
API：
CreateThreadpool 创建线程池
CloseThreadpool  关闭线程池
SetThreadpoolThreadMinimum 设置线程池最小资源数
SetThreadpoolThreadMaximum 设置线程池最大资源数

InitializeThreadpoolEnvironment 初始化线程池环境变量
DestroyThreadpoolEnvironment    销毁线程池环境变量
SetThreadpoolCallbackPool       设置线程池回调的线程池

CreateThreadpoolCleanupGroup        创建清理组
CloseThreadpoolCleanupGroupMembers  关闭清理组
SetThreadpoolCallbackCleanupGroup   为线程池设定清理组

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
	printf("主线程\n");
	CloseThreadpoolWork(pwork);

	CloseThreadpoolCleanupGroupMembers(cleanupGroup, false, NULL);

	DestroyThreadpoolEnvironment(&cbe);

	CloseThreadpool(mypool);


	getchar();
	return 0;
}