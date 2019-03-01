/*
程序编写：袁春旭
讲解：袁春旭
内容：
模拟代填RTX腾讯通-寻找窗口

*/
#include <Windows.h>

int main(void)
{
	char className[] = "#32770";
	char username[] = "yuanchunxu";
	char password[] = "qirichengdie";
	//HWND hWnd = FindWindow(NULL, "财务系统登录");
	//HWND hWnd = FindWindow(className, NULL);
	HWND hWnd = FindWindow(className, "RTX 试用版");
	if (hWnd == NULL)
	{
		MessageBox(NULL, "寻找窗口失败", "消息", MB_OK);
		return 0;
	}

	HWND hWndPanel = FindWindowEx(hWnd, NULL, className, "RTX_LOGIN_DLG");
	if (hWndPanel == NULL)
	{
		MessageBox(NULL, "寻找面板失败", "消息", MB_OK);
		return 0;
	}

	
	HWND hWndEdit = FindWindowEx(hWndPanel, NULL, "Edit", NULL);
	if (hWndEdit == NULL)
	{
		MessageBox(NULL, "寻找文本框失败", "消息", MB_OK);
		return 0;
	}
	
	HWND hWndPass = FindWindowEx(hWndPanel, hWndEdit, "Edit", NULL);
	if (hWndPass == NULL)
	{
		MessageBox(NULL, "寻找密码框失败", "消息", MB_OK);
		return 0;
	}
	
	HWND hWndButton = FindWindowEx(hWndPanel, NULL, "Button", "登录");
	if (hWndButton == NULL)
	{
		MessageBox(NULL, "寻找按钮失败", "消息", MB_OK);
		return 0;
	}
	
	SendMessage(hWndEdit, WM_SETTEXT, sizeof(username) / sizeof(char), (LPARAM)username);
	SendMessage(hWndPass, WM_SETTEXT, sizeof(password) / sizeof(char), (LPARAM)password);
	SendMessage(hWndButton, BM_CLICK, NULL, NULL);
	
	return 0;
}