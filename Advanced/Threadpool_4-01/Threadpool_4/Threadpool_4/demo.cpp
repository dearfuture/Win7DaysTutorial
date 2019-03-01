/*
���⣺Ԭ����
���ݣ��̳߳�
����ģ����첽IO�������ʱ����һ������
API:
CreateThreadpoolIO
//����һ��hDevice:IO�豸���
//��������pfnIoCallback:�ص�����
StartThreadpoolIo
CancelThreadpoolIo
CloseThreadpoolIo
*/

#include <stdio.h>
#include <Windows.h>

VOID WINAPI Win32IoCallback(
	PTP_CALLBACK_INSTANCE Instance,
	PVOID                 Context,
	PVOID                 Overlapped,
	ULONG                 IoResult,
	ULONG_PTR             NumberOfBytesTransferred,
	PTP_IO                Io
)
{
	printf("%d\n", NumberOfBytesTransferred);
	printf("hello yuanchunxu\n");
}

int main(void)
{
	HANDLE hFile = CreateFile("test.txt", GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		printf("create failed...\n");
		getchar();
		return 0;
	}
	PTP_IO ptpIo = CreateThreadpoolIo(hFile, Win32IoCallback, NULL, NULL);
	StartThreadpoolIo(ptpIo);

	char str[] = "yuanchunxu";
	DWORD count = 0;
	OVERLAPPED ol = { 0 };
	WriteFile(hFile, (LPCVOID)str, strlen(str), &count, &ol);
	CloseHandle(hFile);
	getchar();
	CloseThreadpoolIo(ptpIo);

	getchar();
	return 0;
}