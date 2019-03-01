#include <Windows.h>
#include "resource.h"

//ATOM                MyRegisterClass(HINSTANCE hInstance);
//BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDD_DIALOG1));

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, About);
	

	
	return 0;
}


INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		
		if (LOWORD(wParam) == IDOK)
		{
			WCHAR username[32];
			WCHAR password[32];
			GetDlgItemText(hDlg, IDC_EDIT1, username, 32);
			GetDlgItemText(hDlg, IDC_EDIT2, password, 32);

			if (!wcscmp(username, L"topsec") && !wcscmp(password, L"topsec"))
			{
				MessageBox(hDlg, L"success", L"message", MB_OK);
			}
			else
			{
				MessageBox(hDlg, L"failed", L"message", MB_OK);
			}
			//EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		
		break;
	}
	return (INT_PTR)FALSE;
}