/*
�����д��Ԭ����
���⣺Ԭ����
���ݣ�
ģ�����RTX��Ѷͨ-Ѱ�Ҵ���

*/
#include <Windows.h>

int main(void)
{
	char className[] = "#32770";
	char username[] = "yuanchunxu";
	char password[] = "qirichengdie";
	//HWND hWnd = FindWindow(NULL, "����ϵͳ��¼");
	//HWND hWnd = FindWindow(className, NULL);
	HWND hWnd = FindWindow(className, "RTX ���ð�");
	if (hWnd == NULL)
	{
		MessageBox(NULL, "Ѱ�Ҵ���ʧ��", "��Ϣ", MB_OK);
		return 0;
	}

	HWND hWndPanel = FindWindowEx(hWnd, NULL, className, "RTX_LOGIN_DLG");
	if (hWndPanel == NULL)
	{
		MessageBox(NULL, "Ѱ�����ʧ��", "��Ϣ", MB_OK);
		return 0;
	}

	
	HWND hWndEdit = FindWindowEx(hWndPanel, NULL, "Edit", NULL);
	if (hWndEdit == NULL)
	{
		MessageBox(NULL, "Ѱ���ı���ʧ��", "��Ϣ", MB_OK);
		return 0;
	}
	
	HWND hWndPass = FindWindowEx(hWndPanel, hWndEdit, "Edit", NULL);
	if (hWndPass == NULL)
	{
		MessageBox(NULL, "Ѱ�������ʧ��", "��Ϣ", MB_OK);
		return 0;
	}
	
	HWND hWndButton = FindWindowEx(hWndPanel, NULL, "Button", "��¼");
	if (hWndButton == NULL)
	{
		MessageBox(NULL, "Ѱ�Ұ�ťʧ��", "��Ϣ", MB_OK);
		return 0;
	}
	
	SendMessage(hWndEdit, WM_SETTEXT, sizeof(username) / sizeof(char), (LPARAM)username);
	SendMessage(hWndPass, WM_SETTEXT, sizeof(password) / sizeof(char), (LPARAM)password);
	SendMessage(hWndButton, BM_CLICK, NULL, NULL);
	
	return 0;
}