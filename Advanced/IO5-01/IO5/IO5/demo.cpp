/*
��д��Ԭ����
���⣺Ԭ����
���ݣ��첽��д�ļ�
API��CreateFile  WriteFile   ReadFile  
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
	//����������
	BOOL ret = WriteFile(hFile, enContent, strlen(enContent), &writtenCount, &ol1);
	if (false == ret)
	{
		DWORD err = GetLastError();
		if (ERROR_IO_PENDING == err)
		{
			printf("�����첽����\n");
			WaitForSingleObject(hFile, INFINITE);
			printf("�첽д�����");
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
			printf("�����첽����\n");
			WaitForSingleObject(hFile2, INFINITE);
			printf("�첽��ȡ���");
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