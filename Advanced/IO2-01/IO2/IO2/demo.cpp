/*
±àÐ´£ºÔ¬´ºÐñ
½²½â£ºÔ¬´ºÐñ
ÄÚÈÝ£º±àÂë¶ÔIOµÄÓ°Ïì  unicode
API£ºCreateFile  WriteFile  FlushFileBuffers    ReadFile
SetFilePointerEx
*/
#include <Windows.h>
#include <stdio.h>
#include <locale.h>

int main(void)
{
	wchar_t enContent[] = L"Ô¬´ºÐñ";
	DWORD writtenCount = 0;
	HANDLE hFile = CreateFile(L"1.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		printf("CreateFile failed !!!\n");
		getchar();
		return 1;
	}
	printf("CreateFile ok\n");
	wchar_t head = 0xfeff;
	BOOL ret = WriteFile(hFile, &head, sizeof(wchar_t), NULL, NULL);
	if (false == ret)
	{
		printf("WriteFile failed !!!\n");
		getchar();
		return 1;
	}
	//¡£¡£¡£¡£¡£
	ret = WriteFile(hFile, enContent, wcslen(enContent)*2, &writtenCount, NULL);
	if (false == ret)
	{
		printf("WriteFile failed !!!\n");
		getchar();
		return 1;
	}
	FlushFileBuffers(hFile);
	printf("WriteFile ok\n");
	CloseHandle(hFile);

	//-----------------------------------------
	
	wchar_t enContent_r[255] = { 0 };
	HANDLE hFile2 = CreateFile(L"1.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, NULL, NULL);
	if (INVALID_HANDLE_VALUE == hFile2)
	{
		printf("CreateFile failed !!!\n");
		getchar();
		return 1;
	}
	LARGE_INTEGER liMove;
	liMove.QuadPart = 2;
	
	SetFilePointerEx(hFile2, liMove, NULL, FILE_BEGIN);

	DWORD readCount = 0;
	BOOL ret2 = ReadFile(hFile2, enContent_r, 255, &readCount, NULL);
	if (false == ret2)
	{
		printf("ReadFile failed !!!\n");
		getchar();
		return 1;
	}

	CloseHandle(hFile2);

	_wsetlocale(LC_ALL, L"chs");
	//setlocale(LC_ALL, "chs");
	wprintf(L"%s", enContent_r);
	
	getchar();
	return 0;
}