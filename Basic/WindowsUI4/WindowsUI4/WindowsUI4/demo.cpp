//unicode
//windows
#include <Windows.h>
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	::MessageBox(NULL, L"hello world", L"first window", MB_OK);
	return 0;
}