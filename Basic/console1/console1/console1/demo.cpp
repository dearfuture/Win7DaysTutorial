#include <Windows.h>
#include <stdio.h>
int main(void)
{
	HWND hWnd = FindWindow(NULL, L"无标题 - 记事本");
	if (hWnd)
	{
		SendMessage(hWnd, WM_CLOSE, 0, 0);
		printf("ok");
		getchar();
		return 0;
	}

	printf("failed");
	getchar();
	return 0;
}