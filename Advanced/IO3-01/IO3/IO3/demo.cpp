/*
编写：袁春旭
讲解：袁春旭
内容：获取文件大小
API：CreateFile  WriteFile  FlushFileBuffers    ReadFile
SetFilePointerEx  GetFileSize  GetFileSizeEx
*/

#include <Windows.h>
#include <stdio.h>



int main(void)
{
	HANDLE hFile = CreateFile(L"1.txt", 0, 0, NULL, OPEN_EXISTING, NULL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		printf("CreateFile failed!!!\n");
		getchar();
		return 1;
	}

	ULARGE_INTEGER ulFileSize;
	ulFileSize.LowPart = GetFileSize(hFile, &ulFileSize.HighPart);
	printf("file1 size : %u\n", ulFileSize.QuadPart);

	LARGE_INTEGER lFileSize;
	BOOL ret = GetFileSizeEx(hFile, &lFileSize);
	if (ret)
	{
		printf("file2 size : %u\n", lFileSize.QuadPart);
	}

	CloseHandle(hFile);
	getchar();
	return 0;
}