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














 ʾ����д��Ԭ����  QQ:2064438762
 */
#include <Windows.h>
#include "resource.h"

LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int width = GetSystemMetrics(SM_CXSMICON);
	int height = GetSystemMetrics(SM_CYSMICON);
	//HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	HICON hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, width, height, LR_DEFAULTCOLOR);
	HBRUSH hRedBrush = CreateSolidBrush(RGB(0x66, 0, 0x66)); //refcolor  RRGGBB
	HBRUSH hHatchBrush = CreateHatchBrush(HS_CROSS, RGB(0x66, 0, 0x66));
	WNDCLASSEXW wcx;
	wcx.cbSize = sizeof(WNDCLASSEXW);
	//wcx.style = 0;
	wcx.style = CS_HREDRAW;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = hIcon;
	wcx.hCursor = NULL;
	//wcx.hbrBackground = (HBRUSH)COLOR_ACTIVEBORDER; //NULL; ALT+F4
	//wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_PEN);
	//wcx.hbrBackground = hRedBrush;
	wcx.hbrBackground = hHatchBrush;
	wcx.lpszMenuName = NULL;
	wcx.hIconSm = hIcon;
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