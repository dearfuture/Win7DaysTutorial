//�첽IO���֪ͨ�ķ���
//�����豸�ں˶���

#include <Windows.h>
#include <stdio.h>

int main(void)
{

	//------------------------------------------------------
	
	char enContent_r[255] = { 0 };

	HANDLE hFile2 = CreateFile(L"1.txt", GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
	if (INVALID_HANDLE_VALUE == hFile2)
	{
		printf("CreateFile failed!!!\n");
		getchar();
		return 1;
	}
	OVERLAPPED ol2 = {0};   //OVERLAPPED��������ȫ����Ϊ�㣬�����첽��ȡʧ��
	ol2.Offset = 2;
	BOOL ret = ReadFile(hFile2, enContent_r, 255, NULL, &ol2);

	if (!ret)
	{
		DWORD err = GetLastError();
		if (ERROR_IO_PENDING == err)
		{
			WaitForSingleObject(hFile2, INFINITE);
			printf("read from file, content is : %s", enContent_r);
		}
		else
		{
			printf("failed !!!\n");
		}
		
	}

	CloseHandle(hFile2);

	
	
	getchar();
	return 0;
}