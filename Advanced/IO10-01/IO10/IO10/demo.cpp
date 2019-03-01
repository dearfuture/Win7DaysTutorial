/*
编写：袁春旭
讲解：袁春旭
内容：异步读写文件
API：CreateIoCompletionPort,GetQueuedCompletionStatus,PostQueuedCompletionStatus
异步判断：ERROR_IO_PENDING  GetLastError

异步IO完成通知的方法：
触发设备内核对象：允许一个线程发出IO请求，另一个线程对结果进行处理。
当向一个设备同时发出多个IO请求的时候，此方法无效。

触发事件内核对象：这种方法允许我们向一个设备同时发出多个IO请求，
它允许一个线程发出IO请求，另一个线程对结果进行处理。

使用可提醒IO：这种方法允许我们向一个设备发出多个IO请求。
发出IO请求的线程必须对结果进行处理。

使用IO完成端口：这种方法允许我们向一个设备同时发出多个IO请求。
它允许一个线程发出IO请求，另一个线程对结果进行处理。推荐使用，伸缩性和灵活性都很好。
IO完成接口的初衷就是与线程池配合使用
*/

#include <Windows.h>
#include <stdio.h>
#include <process.h>

HANDLE hCicp = NULL;

unsigned int ThreadFunc(void *arg)
{
	getchar();
	OVERLAPPED ol = { 0 };
	ULONG_PTR key = 10;
	PostQueuedCompletionStatus(hCicp, 4, key, &ol);
	return 0;
}

int main(void)
{
	char username[255] = { 0 };
	HANDLE hFile = CreateFile(L"1.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		printf("Create File failed...\n");
		getchar();
		return 1;
	}
	hCicp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (NULL == hCicp)
	{
		printf("Create Io Completion Port failed ...\n");

	}
	ULONG_PTR CK_READ = 0 ;
	CreateIoCompletionPort(hFile, hCicp, CK_READ, 0);
	unsigned int uiThreadID = 0;
	HANDLE hth = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)ThreadFunc, NULL, 0, &uiThreadID);

	OVERLAPPED ol = {0};
	BOOL ret = ReadFile(hFile, username, 255, NULL, &ol);
	DWORD transferedByte = 0;
	void *lpContext = NULL;
	OVERLAPPED *pOl = NULL;


	while (GetQueuedCompletionStatus(hCicp, &transferedByte, (LPDWORD)&lpContext, &pOl, INFINITE))
	{
		if (lpContext != NULL && 10 == (unsigned long)lpContext)
		{
			printf("我要退出了");
			break;
		}
		printf("%s", username);
	}
	CloseHandle(hth);
	CloseHandle(hFile);
	CloseHandle(hCicp);

	getchar();
	return 0;
}