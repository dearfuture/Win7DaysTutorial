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

	//------------------------------------------------------

	char enContent_r[255] = { 0 };
	char enContent_w[255] = "yuanchunxu";
	HANDLE hFile2 = CreateFile(L"1.txt", GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
	if (INVALID_HANDLE_VALUE == hFile2)
	{
		printf("CreateFile failed!!!\n");
		getchar();
		return 1;
	}
	OVERLAPPED ol2 = { 0 };   //OVERLAPPED��������ȫ����Ϊ�㣬�����첽��ȡʧ��
	ol2.Offset = 2;
	OVERLAPPED ol1 = { 0 };
	ol1.hEvent = CreateEvent(NULL, true, false, NULL);
	ol2.hEvent = CreateEvent(NULL, true, false, NULL);
	BOOL ret = ReadFile(hFile2, enContent_r, 255, NULL, &ol2);
	WriteFile(hFile2, enContent_w, 255, NULL, &ol1);
	if (!ret)
	{
		DWORD err = GetLastError();
		if (ERROR_IO_PENDING == err)
		{
			HANDLE h[2];
			h[0] = ol1.hEvent;
			h[1] = ol2.hEvent;
			DWORD objnum = WaitForMultipleObjects(2, h, false, INFINITE);
			switch (objnum)
			{
			case WAIT_OBJECT_0:
				break;
			case WAIT_OBJECT_0+1:
				break;
			}
			
			printf("read from file, content is : %s", enContent_r);
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