/*
���룺Ԭ����
���⣺Ԭ����
���ݣ�windowsע���API
����ע���regedit
Ŀ���ֵ��
HKEY_LOCAL_MACHINE\Software\Microsoft\Windows NT\CurrentVersion\Windows\
AppInit_DLLs     LoadAppInit_DLLs
API��RegOpenKeyEx RegSetValueEx RegQueryValueEx RegCloseKey
ע������
HKEY_CLASSES_ROOT
HKEY_LOCAL_MACHINE
HKEY_CURRENT_USER
HKEY_USERS
HKEY_CURRENT_CONFIG
Ȩ�ޣ�KEY_READ KEY_WRITE KEY_EXECUTE KEY_ALL_ACCESS
�漰���������ͣ�REG_SZ REG_DWORD

 */
#include <windows.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
	HKEY key;
	DWORD LoadAppInitVal = 1;
	WCHAR AppInitVal[] = L"test11.dll";
	WCHAR subKey[] = L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows";
	LSTATUS ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, subKey, 0, KEY_ALL_ACCESS, &key);
	if (ret != ERROR_SUCCESS)
	{
		printf("open key failed....\n");
		getchar();
		return 0;
	}

	ret = RegSetValueEx(key, L"AppInit_DLLs", 0, REG_SZ, (const BYTE*)AppInitVal, (wcslen(AppInitVal) + 1) * sizeof(WCHAR));
	if (ret != ERROR_SUCCESS)
	{
		printf("set key failed....\n");
		getchar();
		return 0;
	}

	ret = RegSetValueEx(key, L"LoadAppInit_DLLs", 0, REG_DWORD, (const BYTE*)&LoadAppInitVal, sizeof(DWORD));
	if (ret != ERROR_SUCCESS)
	{
		printf("set key failed....\n");
		getchar();
		return 0;
	}

	RegCloseKey(key);

	getchar();
	return 0;
}