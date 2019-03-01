/*
编写：袁春旭
讲解：袁春旭
内容：异步读写文件
API：CreateFile  WriteFile   ReadFile
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

char enContent_r[255] = { 0 };
char enContent_w[255] = "yuanchunxu";

VOID ReadFunc(
	DWORD dwErrorCode,
	DWORD dwNumberOfBytesTransfered,
	LPOVERLAPPED lpOverlapped
	)
{
	printf("%s", enContent_r);
}

VOID WriteFunc(
	DWORD dwErrorCode,
	DWORD dwNumberOfBytesTransfered,
	LPOVERLAPPED lpOverlapped
)
{

}

int main(void)
{

	//------------------------------------------------------

	
	HANDLE hFile2 = CreateFile(L"1.txt", GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
	if (INVALID_HANDLE_VALUE == hFile2)
	{
		printf("CreateFile failed!!!\n");
		getchar();
		return 1;
	}
	OVERLAPPED ol2 = { 0 };   //OVERLAPPED变量必须全部置为零，否则异步读取失败
	ol2.Offset = 0;
	OVERLAPPED ol1 = { 0 };
	//ol1.hEvent = CreateEvent(NULL, true, false, NULL);
	//ol2.hEvent = CreateEvent(NULL, true, false, NULL);
	//BOOL ret = ReadFile(hFile2, enContent_r, 255, NULL, &ol2);
	BOOL ret = ReadFileEx(hFile2, enContent_r, 255, &ol2, (LPOVERLAPPED_COMPLETION_ROUTINE)ReadFunc);
	//WriteFile(hFile2, enContent_w, 255, NULL, &ol1);
	WriteFileEx(hFile2, enContent_w, 10, &ol2, (LPOVERLAPPED_COMPLETION_ROUTINE)WriteFunc);
	SleepEx(1000, true);
	WaitForSingleObjectEx(hFile2, INFINITE, true);
	//SignalObjectAndWait()
	//GetQueuedCompletionStatusEx
	//MsgWaitForMultipleObjectsEx(,,,,MWMO_ALERTABLE)

	if (!ret)
	{
		DWORD err = GetLastError();
		if (ERROR_IO_PENDING == err)
		{
			/*HANDLE h[2];
			h[0] = ol1.hEvent;
			h[1] = ol2.hEvent;
			DWORD objnum = WaitForMultipleObjects(2, h, false, INFINITE);
			switch (objnum)
			{
			case WAIT_OBJECT_0:
				break;
			case WAIT_OBJECT_0 + 1:
				break;
			}

			printf("read from file, content is : %s", enContent_r);*/
		}
		else
		{
			printf("failed !!!\n");
		}

	}
	CloseHandle(ol1.hEvent);
	CloseHandle(ol2.hEvent);
	CloseHandle(hFile2);



	getchar();
	return 0;
}