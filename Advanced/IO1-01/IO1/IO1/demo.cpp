#include <Windows.h>
#include <stdio.h>

int main(void)
{
	char enContent[] = "yuanchunxu";
	char cnContent[] = "Ô¬´ºÐñ";

	HANDLE hFile = CreateFile(L"1.txt", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		printf("CreateFile failed!!!\n");
		getchar();
		return 1;
	}

	WriteFile(hFile, enContent, strlen(enContent), NULL, NULL);
	WriteFile(hFile, cnContent, strlen(cnContent), NULL, NULL);
	FlushFileBuffers(hFile);
	CancelSynchronousIo(hFile);
	CloseHandle(hFile);


	//------------------------------------------------------

	char enContent_r[255] = {0};

	HANDLE hFile2 = CreateFile(L"1.txt", GENERIC_READ, 0, NULL, OPEN_ALWAYS, NULL, NULL);
	if (INVALID_HANDLE_VALUE == hFile2)
	{
		printf("CreateFile failed!!!\n");
		getchar();
		return 1;
	}

	ReadFile(hFile2, enContent_r, 255, NULL, NULL);

	CloseHandle(hFile2);

	printf("read from file, content is : %s", enContent_r);
	getchar();
	return 0;
}