/**
ʾ�����Խ�����
���⣺Ԭ����

1.���ڱ���ɫ����  Color Types: COLOR_WINDOW
API˵����

HGDIOBJ GetStockObject(
int fnObject
);

HBRUSH CreateSolidBrush(
COLORREF crColor
);

HBRUSH CreateHatchBrush(
int      fnStyle,
COLORREF clrref
);
fnStyle: HS_BDIAGONAL HS_CROSS HS_DIAGCROSS
HS_FDIAGONAL HS_HORIZONTAL HS_VERTICAL




ʾ�����Խ�����
���⣺Ԭ����

2.���ڷ������  CreateWindow  CreateWindowEx
WS_BORDER��windowsխ�߱�
WS_CAPTION��windows������������WS_BORDER
WS_CHILDWINDOW��WS_CHILD��������һ���Ӵ��ڣ�������WS_POPUP����
WS_CLIPCHILDREN����Ϊ�����ڣ�ˢ��ʱ�ü��Ӵ����������ˢ��Ч��
WS_CLIPSIBLINGS����Ϊ�Ӵ��ڣ�ˢ��ʱ�ü������Ӵ��ڵ��ص��������ˢ��Ч��
WS_DISABLED���رմ�����Ӧ���û����κβ�������Ч
WS_DLGFRAME��������һ���߿���ʽ��ͨ����Ի���һ��ʹ�á�������ʽ�Ĵ��ڲ����б�������
WS_GROUP�����������ʱʹ�á�
��ʾ WS_HSCROLL�����ں���ˮƽ��������
��ʾ WS_VSCROLL�����ں��д�ֱ��������
��ʾ WS_MAXIMIZE�����ڳ�ʼ״̬��󻯡�
WS_MINIMIZE�����ڳ�ʼ״̬��С����
WS_ICONIC��ͬWS_MINIMIZE
WS_MAXIMIZEBOX�����ں�����󻯰�ť
WS_MINIMIZEBOX�����ں�����С����ť
WS_OVERLAPPED���ص�ģʽ�����б����ͱ߿�
WS_TILED��ͬWS_OVERLAPPED
��ʾ WS_OVERLAPPEDWINDOW��(WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)
WS_TILEDWINDOW��ͬWS_OVERLAPPEDWINDOW
WS_POPUP����������ģʽ��������WS_CHILD����
��ʾ WS_POPUPWINDOW��WS_POPUP | WS_BORDER | WS_SYSMENU
WS_SIZEBOX�����ھ��п������߿�
WS_THICKFRAME��ͬWS_SIZEBOX
��ʾ WS_SYSMENU����������ϵͳ�˵������رհ�ť��
WS_TABSTOP����Ϊ�����еĿؼ���������WS_TABSTOP��־�Ŀؼ�����ͨ��TAB��������ת��
WS_VISIBLE�������Ƿ�ɼ��� ��ShowWindow����





ʾ�����Խ�����
���⣺Ԭ����

3.������Ϊ����

SW_HIDE
SW_SHOW
SW_MAXIMIZE
SW_MINIMIZE
SW_FORCEMINIMIZE
SW_SHOWDEFAULT
SW_SHOWMAXIMIZED
SW_SHOWMINIMIZED
SW_SHOWMINNOACTIVE
SW_SHOWNA
SW_SHOWNOACTIVATE
SW_SHOWNORMAL
SW_RESTORE


ʾ�����Խ�����
���⣺Ԭ����

4.����ͼ������
HICON LoadIcon(
HINSTANCE hInstance,
LPCTSTR   lpIconName ==> MAKEINTRESOURCE
);

HANDLE LoadImage(
HINSTANCE hinst,
LPCTSTR   lpszName,  ==> MAKEINTRESOURCE
UINT      uType,
int       cxDesired, ==> GetSystemMetrics
int       cyDesired, ==> GetSystemMetrics
UINT      fuLoad
);


int GetSystemMetrics(  //��λ����
int nIndex
);



ʾ�����Խ�����
���⣺Ԭ����

ͼ���ļ�icon����

��������ICON�ļ�    https://www.ico.la/

��������ICON�ļ�    ������ܣ�anytoicon

����icon��Դ

ʹ��ϵͳͼ��

ʹ���ⲿͼ��




ʾ�����Խ�����
���⣺Ԭ����

���ͼ������
�����ⲿcur�ļ�


HCURSOR LoadCursor(
HINSTANCE hInstance,
LPCTSTR   lpCursorName
);

HANDLE LoadImage(
HINSTANCE hinst,
LPCTSTR   lpszName,  ==> MAKEINTRESOURCE
UINT      uType,
int       cxDesired, ==> GetSystemMetrics
int       cyDesired, ==> GetSystemMetrics
UINT      fuLoad
);


int GetSystemMetrics(  //��λ����
int nIndex
);




ʾ�����Խ�����
���⣺Ԭ����

��Ϣ����

WM_CREATE�����ڴ�����Ϣ

WM_MOVE�������ƶ���Ϣ

WM_SIZE�����ڴ�С�仯��Ϣ

WM_ACTIVATE�����ڻ��Ϣ

WM_LBUTTONDOWN������������
x = LOWORD(lParam); //��ȡ���xλ�ã���Դ���
y = HIWORD(lParam); //��ȡ���yλ�ã���Դ���
GetCursorPos(&pt);  //��ȡ���λ�ã������Ļ
GetSystemMetrics (SM_MOUSEPRESENT);  ȷ������Ƿ����
GetSystemMetrics (SM_CMOUSEBUTTONS); ȷ����갴������

WM_KEYDOWN���а�������















ʾ�����Խ�����
���⣺Ԭ����



���˫���¼�
WM_LBUTTONDBLCLK WM_RBUTTONDBLCLK WM_MBUTTONDBLCLK
wcx.style = CS_DBLCLKS
����ǰ��WM_LBUTTONDOWN��WM_LBUTTONUP��WM_LBUTTONDOWN��WM_LBUTTONUP
���ú�WM_LBUTTONDOWN��WM_LBUTTONUP��WM_LBUTTONDBLCLK��WM_LBUTTONUP









ʾ�����Խ�����
���⣺Ԭ����

ALT������Ϣ
WM_SYSKEYDOWN

��ALT������Ϣ
WM_KEYDOWN

















ʾ����д��Ԭ����  QQ:2064438762
*/
#include <Windows.h>


LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int x = 0;
	int y = 0;
	POINT pt;
	wchar_t pos1[20];
	wchar_t pos2[20];

	switch (uMsg)
	{
	case WM_CREATE:
		x = GetSystemMetrics(SM_MOUSEPRESENT);
		y = GetSystemMetrics(SM_CMOUSEBUTTONS);
		//MessageBox(hWnd, L"WM_CREATE", L"Message", MB_OK);
		break;
	case WM_MOVE:
		//MessageBox(hWnd, L"WM_MOVE", L"Message", MB_OK);
		break;

	case WM_LBUTTONDBLCLK:
		MessageBox(hWnd, L"WM_LBUTTONDBLCLK", L"Message", MB_OK);
		x = LOWORD(lParam); //��ȡ���xλ��
		y = HIWORD(lParam); //��ȡ���yλ��
		GetCursorPos(&pt);
		wsprintf(pos1, L"%d, %d", x, y);
		MessageBox(hWnd, pos1, L"Message", MB_OK);
		wsprintf(pos2, L"%d, %d", pt.x, pt.y);
		MessageBox(hWnd, pos2, L"Message", MB_OK);
		break;

	case WM_LBUTTONDOWN:
		//x = LOWORD(lParam); //��ȡ���xλ��
		//y = HIWORD(lParam); //��ȡ���yλ��
		//GetCursorPos(&pt);
		//wsprintf(pos1, L"%d, %d", x, y);
		//MessageBox(hWnd, pos1, L"Message", MB_OK);
		//wsprintf(pos2, L"%d, %d", pt.x, pt.y);
		//MessageBox(hWnd, pos2, L"Message", MB_OK);
		break;

	case WM_SYSKEYDOWN:
		//MessageBox(hWnd, L"WM_SYSKEYDOWN", L"Message", MB_OK);
		break;

	case WM_SYSKEYUP:
		MessageBox(hWnd, L"WM_SYSKEYUP", L"Message", MB_OK);
		break;

	case WM_KEYDOWN:
		//MessageBox(hWnd, L"WM_KEYDOWN", L"Message", MB_OK);
		break;

	case WM_KEYUP:
		MessageBox(hWnd, L"WM_KEYUP", L"Message", MB_OK);
		break;
	
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HBRUSH hRedBrush = CreateSolidBrush(RGB(0x66, 0, 0x66)); //refcolor  RRGGBB
	HBRUSH hHatchBrush = CreateHatchBrush(HS_CROSS, RGB(0x66, 0, 0x66));
	WNDCLASSEXW wcx;
	wcx.cbSize = sizeof(WNDCLASSEXW);
	//wcx.style = 0;
	//wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.style = CS_DBLCLKS;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = NULL;// hIcon;
	wcx.hCursor = NULL;
					   
	wcx.hbrBackground = hHatchBrush;
	wcx.lpszMenuName = NULL;
	wcx.hIconSm = NULL;
	wcx.lpszClassName = L"MainWindowClass";
	wcx.lpfnWndProc = WinProc;


	//1.ע�ᴰ��
	ATOM win = RegisterClassEx(&wcx);

	//2.��������
	HWND hWnd = CreateWindow(
		L"MainWindowClass",
		L"Ԭ����Ĵ���",
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		800,
		600,
		(HWND)NULL,
		(HMENU)NULL,
		hInstance,
		(LPVOID)NULL
	);

	if (!hWnd)
	{
		MessageBox(NULL, L"��������ʧ��", L"��ʾ", MB_OK);
		return 0;
	}

	//3. ��ʾ����
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);



	//4. ��Ϣ��ȡ���ַ�
	MSG msg;
	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		//TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}