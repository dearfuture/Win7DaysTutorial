/*
讲解：袁春旭
编码：袁春旭
内容：直接挂钩到应用程序
API：SetWindowsHookEx  UnhookWindowsHookEx

HHOOK WINAPI SetWindowsHookEx(
	int       idHook,
	HOOKPROC  lpfn,
	HINSTANCE hMod,
	DWORD     dwThreadId
);

BOOL WINAPI UnhookWindowsHookEx(
	HHOOK hhk
);


触发：通过按键触发挂钩和解挂钩
七种事件消息：
WH_CALLWNDPROC <--> CallWndProc
WH_CBT <--> CBTProc
WH_DEBUG <--> DebugProc
WH_GETMESSAGE <--> GetMsgProc
WH_KEYBOARD <--> KeyboardProc
WH_MOUSE <--> MouseProc
WH_MSGFILTER <--> MessageProc
*/

//HookAPP.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "HookAPP.h"

typedef struct _MYHOOKDATA
{
	int nType;
	HOOKPROC hkprc;
	HHOOK hhook;
}MYHOOKDATA;

MYHOOKDATA myhookdata[7];

LRESULT WINAPI CallWndProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI CBTProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI DebugProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI MessageProc(int nCode, WPARAM wParam, LPARAM lParam);

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_HOOKAPP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HOOKAPP));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HOOKAPP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_HOOKAPP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		OutputDebugString(L"hookapp WM_CREATE");
		myhookdata[0].nType = WH_CALLWNDPROC;
		myhookdata[0].hkprc = CallWndProc;
		myhookdata[1].nType = WH_CBT;
		myhookdata[1].hkprc = CBTProc;
		myhookdata[2].nType = WH_DEBUG;
		myhookdata[2].hkprc = DebugProc;
		myhookdata[3].nType = WH_GETMESSAGE;
		myhookdata[3].hkprc = GetMsgProc;
		myhookdata[4].nType = WH_KEYBOARD;
		myhookdata[4].hkprc = KeyboardProc;
		myhookdata[5].nType = WH_MOUSE;
		myhookdata[5].hkprc = MouseProc;
		myhookdata[6].nType = WH_MSGFILTER;
		myhookdata[6].hkprc = MessageProc;
		break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

	case WM_CHAR:
		switch (wParam)
		{
		case 'a':
			OutputDebugString(L"hookapp HOOK WH_CALLWNDPROC");
			myhookdata[0].hhook = SetWindowsHookEx(
				myhookdata[0].nType,
				myhookdata[0].hkprc,
				(HINSTANCE)NULL,
				GetCurrentThreadId());
			break;
		case 'b':
			OutputDebugString(L"hookapp UNHOOK WH_CALLWNDPROC");
			UnhookWindowsHookEx(myhookdata[0].hhook);
			break;
		case 'c':
			OutputDebugString(L"hookapp HOOK WH_CBT");
			myhookdata[1].hhook = SetWindowsHookEx(
				myhookdata[1].nType,
				myhookdata[1].hkprc,
				(HINSTANCE)NULL,
				GetCurrentThreadId());
			break;
		case 'd':
			OutputDebugString(L"hookapp UNHOOK WH_CBT");
			UnhookWindowsHookEx(myhookdata[1].hhook);
			break;
		case 'e':
			OutputDebugString(L"hookapp HOOK WH_DEBUG");
			myhookdata[2].hhook = SetWindowsHookEx(
				myhookdata[2].nType,
				myhookdata[2].hkprc,
				(HINSTANCE)NULL,
				GetCurrentThreadId());
			break;
		case 'f':
			OutputDebugString(L"hookapp UNHOOK WH_DEBUG");
			UnhookWindowsHookEx(myhookdata[2].hhook);
			break;
		case 'g':
			OutputDebugString(L"hookapp HOOK WH_GETMESSAGE");
			myhookdata[3].hhook = SetWindowsHookEx(
				myhookdata[3].nType,
				myhookdata[3].hkprc,
				(HINSTANCE)NULL,
				GetCurrentThreadId());
			break;
		case 'h':
			OutputDebugString(L"hookapp UNHOOK WH_GETMESSAGE");
			UnhookWindowsHookEx(myhookdata[3].hhook);
			break;
		case 'i':
			OutputDebugString(L"hookapp HOOK WH_KEYBOARD");
			myhookdata[4].hhook = SetWindowsHookEx(
				myhookdata[4].nType,
				myhookdata[4].hkprc,
				(HINSTANCE)NULL,
				GetCurrentThreadId());
			break;
		case 'j':
			OutputDebugString(L"hookapp UNHOOK WH_KEYBOARD");
			UnhookWindowsHookEx(myhookdata[4].hhook);
			break;
		case 'k':
			OutputDebugString(L"hookapp HOOK WH_MOUSE");
			myhookdata[5].hhook = SetWindowsHookEx(
				myhookdata[5].nType,
				myhookdata[5].hkprc,
				(HINSTANCE)NULL,
				GetCurrentThreadId());
			break;
		case 'l':
			OutputDebugString(L"hookapp UNHOOK WH_MOUSE");
			UnhookWindowsHookEx(myhookdata[5].hhook);
			break;
		case 'm':
			OutputDebugString(L"hookapp HOOK WH_MSGFILTER");
			myhookdata[6].hhook = SetWindowsHookEx(
				myhookdata[6].nType,
				myhookdata[6].hkprc,
				(HINSTANCE)NULL,
				GetCurrentThreadId());
			break;
		case 'n':
			OutputDebugString(L"hookapp UNHOOK WH_MSGFILTER");
			UnhookWindowsHookEx(myhookdata[6].hhook);
			break;
		default:
			break;
		}
		break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


LRESULT WINAPI CallWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	switch (nCode)
	{
	case HC_ACTION:
		OutputDebugString(L"hookapp CallWndProc HC_ACTION");
		break;
	}

	return CallNextHookEx(myhookdata[0].hhook, nCode, wParam, lParam);
}

LRESULT WINAPI CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	switch (nCode)
	{
	case HCBT_ACTIVATE:
		OutputDebugString(L"hookapp CBTProc HCBT_ACTIVATE");
		break;

	case HCBT_CLICKSKIPPED:
		OutputDebugString(L"hookapp CBTProc HCBT_CLICKSKIPPED");
		break;

	case HCBT_MINMAX:
		OutputDebugString(L"hookapp CBTProc HCBT_MINMAX");
		break;

	default:
		OutputDebugString(L"hookapp CBTProc default");
		break;
	}

	return CallNextHookEx(myhookdata[1].hhook, nCode, wParam, lParam);
}

LRESULT WINAPI DebugProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	switch (nCode)
	{
	case HC_ACTION:
		OutputDebugString(L"hookapp DebugProc HC_ACTION");
		break;
	}

	return CallNextHookEx(myhookdata[2].hhook, nCode, wParam, lParam);
}

LRESULT WINAPI GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	switch (nCode)
	{
	case HC_ACTION:
		OutputDebugString(L"hookapp GetMsgProc HC_ACTION");
		break;
	}

	return CallNextHookEx(myhookdata[3].hhook, nCode, wParam, lParam);
}

LRESULT WINAPI KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode < 0)
		return CallNextHookEx(myhookdata[4].hhook, nCode, wParam, lParam);

	switch (nCode)
	{
	case HC_ACTION:
		OutputDebugString(L"hookapp KeyboardProc HC_ACTION");
		if (lParam & 0x20000000)  //按下ALT键
		{
			OutputDebugString(L"hookapp KeyboardProc HC_ACTION [ALT] KEYDOWN");
		}
		if (lParam & 0x80000000)  //释放按下的键
		{
			OutputDebugString(L"hookapp KeyboardProc HC_ACTION KEYUP");
		}
		break;
	}

	return CallNextHookEx(myhookdata[4].hhook, nCode, wParam, lParam);
}

LRESULT WINAPI MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode < 0)
		return CallNextHookEx(myhookdata[5].hhook, nCode, wParam, lParam);
	
	switch (wParam)
	{
	case WM_LBUTTONDOWN:
		OutputDebugString(L"hookapp MouseProc WM_LBUTTONDOWN");
		PMOUSEHOOKSTRUCT pInfo = (PMOUSEHOOKSTRUCT)lParam;
		WCHAR buf[30] = {0};
		wsprintf(buf, L"hookapp (%d, %d)", pInfo->pt.x, pInfo->pt.y);
		OutputDebugString(buf);
		break;
	}

	return CallNextHookEx(myhookdata[5].hhook, nCode, wParam, lParam);
}

LRESULT WINAPI MessageProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	switch (nCode)
	{
	case MSGF_DIALOGBOX:
		OutputDebugString(L"hookapp MessageProc MSGF_DIALOGBOX");
		break;

	case MSGF_MENU:
		OutputDebugString(L"hookapp MessageProc MSGF_MENU");
		break;

	default:
		OutputDebugString(L"hookapp MessageProc default");
		break;
	}

	return CallNextHookEx(myhookdata[6].hhook, nCode, wParam, lParam);
}