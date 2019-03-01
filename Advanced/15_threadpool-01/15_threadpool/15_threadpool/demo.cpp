/*
编写：袁春旭
讲解：袁春旭
内容：线程池-提前返回
API：DisassociateCurrentThreadFromCallback
*/

#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>


void NTAPI TaskHandler(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_WORK Work)
{
	//xxxxxxxxxxxxx
	DisassociateCurrentThreadFromCallback(Instance);
	Sleep(2000);
	printf("我是子线程!\n");
}


int main(void)
{
	PTP_WORK workItem = CreateThreadpoolWork(TaskHandler, NULL, NULL);

	SubmitThreadpoolWork(workItem);
	
	WaitForThreadpoolWorkCallbacks(workItem, false);
	

	printf("主线程运行完毕\n");
	getchar();
	CloseThreadpoolWork(workItem);

	system("pause");
	return 0;
}