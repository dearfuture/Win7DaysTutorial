//TestHookMsg.cpp: 定义应用程序的入口点。
//
/*
编码：袁春旭
讲解：袁春旭
内容：挂钩指定线程
*/

#include "stdafx.h"
#include "TestHookMsg.h"
#include <tlhelp32.h>   //快照API头文件

#define MAX_LOADSTRING 100

// 全局变量: 
HMODULE g_hModule = NULL;
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

typedef void (WINAPI *SUHF)(int id);
typedef void (WINAPI *SHV)(HHOOK hookval);
SUHF shf = NULL;
SUHF uhf = NULL;
SHV shv = NULL;
HOOKPROC pfn = NULL;
HHOOK g_hHook;
DWORD g_ulThreadId = 0;

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
DWORD GetMainThreadIdFromName(WCHAR* szName);

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
    LoadStringW(hInstance, IDC_TESTHOOKMSG, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTHOOKMSG));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTHOOKMSG));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TESTHOOKMSG);
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


// 由进程名获取主线程ID(需要头文件tlhelp32.h)
// 失败返回0
// 功能：通过名称获取主线程ID
DWORD GetMainThreadIdFromName(WCHAR* szName)
{
	DWORD idThread = 0;         // 主线程ID
	DWORD idProcess = 0;        // 进程ID

	PROCESSENTRY32 pe;      // 进程信息
	pe.dwSize = sizeof(PROCESSENTRY32);
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // 获取系统进程列表
	if (Process32First(hSnapshot, &pe))      // 返回系统中第一个进程的信息
	{
		do
		{
			if (0 == _wcsicmp(pe.szExeFile, szName)) // 不区分大小写比较
			{
				idProcess = pe.th32ProcessID;
				break;
			}
		} while (Process32Next(hSnapshot, &pe));      // 下一个进程
	}
	CloseHandle(hSnapshot); // 删除快照
	if (idProcess == 0)
	{
		return 0;
	}

	// 获取进程的主线程ID
	THREADENTRY32 te;       // 线程信息
	te.dwSize = sizeof(THREADENTRY32);
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0); // 系统所有线程快照
	if (Thread32First(hSnapshot, &te))       // 第一个线程
	{
		do
		{
			if (idProcess == te.th32OwnerProcessID)      // 认为找到的第一个该进程的线程为主线程
			{
				idThread = te.th32ThreadID;
				break;
			}
		} while (Thread32Next(hSnapshot, &te));           // 下一个线程
	}
	CloseHandle(hSnapshot); // 删除快照
	return idThread;
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
WCHAR buf2[30] = { 0 };
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            //分析菜单选择: 
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
			shf(0);
			break;
		case 'b':
			OutputDebugString(L"hookapp UNHOOK WH_CALLWNDPROC");
			uhf(0);
			break;
		case 'c':
			OutputDebugString(L"hookapp HOOK WH_CBT");
			shf(1);
			break;
		case 'd':
			OutputDebugString(L"hookapp UNHOOK WH_CBT");
			uhf(1);
			break;
		case 'e':
			OutputDebugString(L"hookapp HOOK WH_DEBUG");
			shf(2);
			break;
		case 'f':
			OutputDebugString(L"hookapp UNHOOK WH_DEBUG");
			uhf(2);
			break;
		case 'g':
			OutputDebugString(L"hookapp HOOK WH_GETMESSAGE");
			shf(3);
			break;
		case 'h':
			OutputDebugString(L"hookapp UNHOOK WH_GETMESSAGE");
			uhf(3);
			break;
		case 'i':
			OutputDebugString(L"hookapp HOOK WH_KEYBOARD");
			shf(4);
			break;
		case 'j':
			OutputDebugString(L"hookapp UNHOOK WH_KEYBOARD");
			uhf(4);
			break;
		case 'k':
			OutputDebugString(L"hookapp HOOK WH_MOUSE");
			shf(5);
			break;
		case 'l':
			OutputDebugString(L"hookapp UNHOOK WH_MOUSE");
			uhf(5);
			break;
		case 'm':
			OutputDebugString(L"hookapp HOOK WH_MSGFILTER");
			shf(6);
			break;
		case 'n':
			OutputDebugString(L"hookapp UNHOOK WH_MSGFILTER");
			uhf(6);
			break;

		case 'x':  //加载dll
			OutputDebugString(L"hookapp load library");
			g_hModule = LoadLibrary(L"HookMsg.dll");
			shf = (SUHF)GetProcAddress(g_hModule, "SetHookById");
			uhf = (SUHF)GetProcAddress(g_hModule, "UnHookById");
			break;
		case 'y':  //释放dll
			UnhookWindowsHookEx(g_hHook);
			OutputDebugString(L"hookapp free library");
			FreeLibrary(g_hModule);
			break;

		case 'p':
			OutputDebugString(L"hookapp load library");
			g_hModule = LoadLibrary(L"HookMsg.dll");
			pfn = (HOOKPROC)GetProcAddress(g_hModule, "MouseProcEx");
			shv = (SHV)GetProcAddress(g_hModule, "SetHookVal");
			g_ulThreadId = GetMainThreadIdFromName(L"notepad.exe");
			g_hHook = SetWindowsHookEx(
				WH_MOUSE,
				pfn,
				(HINSTANCE)g_hModule,
				//GetCurrentThreadId()
				//0
				g_ulThreadId
			);
			wsprintf(buf2, L"hookapp TEST g_hHook = %x", g_hHook);
			OutputDebugString(buf2);
			shv(g_hHook);

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

//“关于”框的消息处理程序。
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