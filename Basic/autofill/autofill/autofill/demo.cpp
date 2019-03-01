#include <windows.h> 
BOOL CALLBACK EnumChildProc(HWND hWnd, LPARAM lParam);
int main(void)
{
	//wchar_t className[] = L"notepad";
	wchar_t className[] = L"#32770";
	//wchar_t className[] = L"Intermediate D3D Window";
	//wchar_t className[] = L"WindowsForms10.Window.8.app.0.141b42a_r9_ad1";
	//wchar_t editName[] = L"WindowsForms10.EDIT.app.0.141b42a_r9_ad1";
	//wchar_t javaName[] = L"SunAwtFrame";
	wchar_t temp1[256] = L"topsec";
	wchar_t temp2[256] = L"topsec";


	//STARTUPINFO si = { sizeof(si) };
	//PROCESS_INFORMATION pi;
	//si.dwFlags = STARTF_USESHOWWINDOW;//指定wShowWindow成员有效
	//si.wShowWindow = TRUE;//此成员设为TRUE的话则显示新建进程的主窗口
	//BOOL bRet = CreateProcess(
	//	NULL,//不在此指定可执行文件的文件名
	//	szCommandLine,//命令行参数
	//	NULL,//默认进程安全性
	//	NULL,//默认进程安全性
	//	FALSE,//指定当前进程内句柄不可以被子进程继承
	//	0,//为新进程创建一个新的控制台窗口
	//	NULL,//使用本进程的环境变量
	//	NULL,//使用本进程的驱动器和目录
	//	&si,
	//	&pi);

	//HWND hWnd = NULL;
	HWND hWnd = ::FindWindow(NULL, L"Dialog");
	//HWND hWnd = ::FindWindow(className, NULL);
	//HWND hWnd = ::FindWindowEx(NULL, NULL, className, NULL);
	//HWND hWnd = ::FindWindowEx(NULL, NULL, javaName, NULL);

	


	//000802E0
	if (hWnd)
	{
		//::SendMessage(hWnd, WM_CLOSE, NULL, NULL);
		//HWND hWndPanel = ::FindWindowEx(hWnd, 0, NULL, L"Chrome Legacy Window");
		
		HWND hWndEdit1 = ::FindWindowEx(hWnd, 0, L"Edit", NULL);
		if (hWndEdit1)
		{
			::SendMessage(hWndEdit1, WM_SETTEXT, sizeof(temp1) / sizeof(char), (LPARAM)temp1);
		}

		HWND hWndEdit2 = ::FindWindowEx(hWnd, hWndEdit1, L"Edit", NULL);
		if (hWndEdit2)
		{
			::SendMessage(hWndEdit2, WM_SETTEXT, sizeof(temp2) / sizeof(char), (LPARAM)temp2);
		}

		//HWND hWndButton = ::FindWindowEx(hWnd, NULL, NULL, L"OK");
		HWND hWndButton = ::FindWindowEx(hWnd, NULL, L"Button", L"OK");
		if (hWndButton)
		{
			::SendMessage(hWndButton, BM_CLICK, NULL, NULL);
		}
		
			/*HWND hWndEdit2 = ::FindWindowEx(hWndPanel, hWndEdit, L"Edit", NULL);
			if (hWndEdit2)
			{
				::SendMessage(hWndEdit2, WM_SETTEXT, sizeof(temp2) / sizeof(char), (LPARAM)temp2);
			}*/
			//HWND hWndButton = ::FindWindowEx(hWndPanel, 0, NULL, L"登录");
			//if (hWndButton)
			//点击按钮
			//	::SendMessage(hWndButton, BM_CLICK, NULL, NULL);
		


		//::SendMessage(hWndEdit, WM_SETTEXT, sizeof(temp2) / sizeof(char), (LPARAM)temp2);
		//	::SetWindowText(hWndEdit, L"yuanchunxu");
		//::EnumChildWindows(hWnd, EnumChildProc, 0);
		
	}
	//MessageBox(NULL, L"fail!", L"fail", MB_OK);
	return 0;
}

BOOL CALLBACK EnumChildProc(HWND hWnd, LPARAM lParam)
{
	wchar_t temp1[256];
	wchar_t temp2[256] = L"ABC";

	::GetClassName(hWnd, temp1, 255);


	if (!::wcscmp(temp1, L"Edit"))
	{
		//::SendMessage(hWnd, WM_GETTEXT, sizeof(temp2) / sizeof(char), (LPARAM)temp2);
		::SendMessage(hWnd, WM_SETTEXT, sizeof(temp2) / sizeof(char), (LPARAM)temp2);
		//EDIT的句柄，消息，接收缓冲区大小，接收缓冲区指针 
		//::MessageBox(NULL, temp2, L"get", MB_OK);
		//return 0;
	}
	::wsprintf(temp2, L"classname: %s", temp1);
	MessageBox(NULL, temp2, L"cwnd", MB_OK);
	return true;
}