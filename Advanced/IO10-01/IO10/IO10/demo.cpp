/*
��д��Ԭ����
���⣺Ԭ����
���ݣ��첽��д�ļ�
API��CreateIoCompletionPort,GetQueuedCompletionStatus,PostQueuedCompletionStatus
�첽�жϣ�ERROR_IO_PENDING  GetLastError

�첽IO���֪ͨ�ķ�����
�����豸�ں˶�������һ���̷߳���IO������һ���̶߳Խ�����д���
����һ���豸ͬʱ�������IO�����ʱ�򣬴˷�����Ч��

�����¼��ں˶������ַ�������������һ���豸ͬʱ�������IO����
������һ���̷߳���IO������һ���̶߳Խ�����д���

ʹ�ÿ�����IO�����ַ�������������һ���豸�������IO����
����IO������̱߳���Խ�����д���

ʹ��IO��ɶ˿ڣ����ַ�������������һ���豸ͬʱ�������IO����
������һ���̷߳���IO������һ���̶߳Խ�����д����Ƽ�ʹ�ã������Ժ�����Զ��ܺá�
IO��ɽӿڵĳ��Ծ������̳߳����ʹ��
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
			printf("��Ҫ�˳���");
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