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

int main(void)
{
	/*
	char enContent[] = "yuanchunxu";
	DWORD writtenCount = 0;
	HANDLE hFile = CreateFile(L"1.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN | FILE_FLAG_OVERLAPPED, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		printf("CreateFile failed !!!\n");
		getchar();
		return 1;
	}
	printf("CreateFile ok\n");

	OVERLAPPED ol1 = { 0 };
	//。。。。。
	BOOL ret = WriteFile(hFile, enContent, strlen(enContent), &writtenCount, &ol1);
	if (false == ret)
	{
		DWORD err = GetLastError();
		if (ERROR_IO_PENDING == err)
		{
			printf("正在异步操作\n");
			WaitForSingleObject(hFile, INFINITE);
			printf("异步写入完毕");
		}
		else
		{
			printf("WriteFile failed !!!\n");
			getchar();
			return 1;
		}
		
	}
	//FlushFileBuffers(hFile);
	//printf("WriteFile ok\n");
	CloseHandle(hFile);
	*/
	//-----------------------------------------
	
	char enContent_r[255] = { 0 };
	HANDLE hFile2 = CreateFile(L"1.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if (INVALID_HANDLE_VALUE == hFile2)
	{
		printf("CreateFile failed !!!\n");
		getchar();
		return 1;
	}
	DWORD readCount = 0;
	OVERLAPPED ol2 = { 0 };
	BOOL ret2 = ReadFile(hFile2, enContent_r, 255, &readCount, &ol2);
	if (false == ret2)
	{
		DWORD err = GetLastError();
		if (ERROR_IO_PENDING == err)
		{
			printf("正在异步操作\n");
			WaitForSingleObject(hFile2, INFINITE);
			printf("异步读取完毕");
			printf("%s", enContent_r);
		}
		else
		{
			printf("ReadFile failed !!!\n");
			getchar();
			return 1;
		}
		
	}

	CloseHandle(hFile2);
	//getchar();
	
	
	getchar();
	return 0;
}