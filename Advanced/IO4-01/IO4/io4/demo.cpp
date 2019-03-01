/*
编写：袁春旭
讲解：袁春旭
内容：设置文件指针和文件尾
API：CreateFile  WriteFile  FlushFileBuffers    ReadFile
SetFilePointerEx  GetFileSize  GetFileSizeEx  SetEndOfFile
*/

#include <Windows.h>
#include <stdio.h>

int main(void)
{

	HANDLE hFile = CreateFile(L"1.txt", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, NULL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		printf("CreateFile failed!!!\n");
		getchar();
		return 1;
	}
	LARGE_INTEGER liMove;
	liMove.QuadPart = 2;
	SetFilePointerEx(hFile, liMove, NULL, FILE_BEGIN);

	SetEndOfFile(hFile);

	CloseHandle(hFile);


	getchar();
	return 0;
}