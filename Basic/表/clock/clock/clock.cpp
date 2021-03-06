/*
案例名称：钟表
编码讲解：袁春旭

直接前置课程：七日成蝶-Windows API编程入门-界面与消息篇

静态阶段

难点说明：
1.找到中心点  GetClientRect
2.找到12个时间刻度 三角函数

需要确定的值
1.表盘窗口大小和颜色     300*300，黑色
2.表盘半径             130  
3.刻度的形状、大小和颜色  圆形，5，白色
4.时针长短、粗细及颜色    40 7  绿色
4.分针长短、粗细及颜色    90 5  黄色
4.秒针长短、粗细及颜色    120 3  红色

动态阶段

难点说明：
1.周期性触发三个指针走动  WM_TIMER
2.每个指针的绘制位置     擦除旧位置，绘制新位置

获取本地时间
SYSTEMTIME
GetLocalTime

设置窗口位置
SetWindowPos

按ESC关闭钟表
 */







//clock.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "clock.h"
#include <math.h>

#define MAX_LOADSTRING 100

#define CLOCK_SIZE 300
#define CLOCK_RADIUS 130
#define CLOCK_SCALE 5
#define CLOCK_HOUR_HAND_LEN 40
#define CLOCK_HOUR_HAND_WID 7
#define CLOCK_MINUTE_HAND_LEN 90
#define CLOCK_MINUTE_HAND_WID 5
#define CLOCK_SECOND_HAND_LEN 120
#define CLOCK_SECOND_HAND_WID 3

#define CLOCK_BORDER 20

#define PI       3.14159265358979323846
#define IDT_TIMER1  1

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

POINT GetCenterPoint(HWND hWnd);
void DrawClockFace(HDC hdc, HWND hWnd);
void DrawSecondHand(HWND hWnd, WORD wSecond);
void DrawMinuteHand(HWND hWnd, WORD wMinute);
void DrawHourHand(HWND hWnd, WORD wHour, WORD wMinute);

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
    LoadStringW(hInstance, IDC_CLOCK, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLOCK));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLOCK));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = NULL;   //MAKEINTRESOURCEW(IDC_CLOCK);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, /*WS_OVERLAPPEDWINDOW*/ WS_POPUP | WS_SIZEBOX,
      CW_USEDEFAULT, 0, CLOCK_SIZE, CLOCK_SIZE, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   SetWindowPos(hWnd, HWND_TOPMOST, 100, 100, CLOCK_SIZE, CLOCK_SIZE, SWP_SHOWWINDOW);

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
		if (char(wParam) == 27)
		{
			KillTimer(hWnd, IDT_TIMER1);
			PostQuitMessage(0);
		}
		break;

	case WM_NCHITTEST:
		UINT nHitTest;
		nHitTest = DefWindowProc(hWnd, message, wParam, lParam);
		if (nHitTest == HTCLIENT)
		{
			nHitTest = HTCAPTION;
		}
		return nHitTest;
		break;

	case WM_CREATE:
		if (SetTimer(hWnd, IDT_TIMER1, 1000, NULL) == 0)
		{
			MessageBox(hWnd, L"定时器设置失败", L"消息", MB_OK);
		}
		break;

	case WM_TIMER:
		{
			SYSTEMTIME time;
			GetLocalTime(&time);
			time.wHour;
			time.wMinute;
			time.wSecond;

			WORD hour = time.wHour % 12;
			DrawSecondHand(hWnd, time.wSecond);
			DrawMinuteHand(hWnd, time.wMinute);
			DrawHourHand(hWnd, hour, time.wMinute);
		}
		
		break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
			DrawClockFace(hdc, hWnd);

			/*
			//分
			HPEN hMinutePen = CreatePen(PS_SOLID, CLOCK_MINUTE_HAND_WID, RGB(255, 255, 0));
			SelectObject(hdc, hMinutePen);
			MoveToEx(hdc, centerPoint.x, centerPoint.y, NULL);
			LineTo(hdc, centerPoint.x + CLOCK_MINUTE_HAND_LEN, centerPoint.y);

			//时
			HPEN hHourPen = CreatePen(PS_SOLID, CLOCK_HOUR_HAND_WID, RGB(0, 255, 0));
			SelectObject(hdc, hHourPen);
			MoveToEx(hdc, centerPoint.x, centerPoint.y, NULL);
			LineTo(hdc, centerPoint.x + CLOCK_HOUR_HAND_LEN, centerPoint.y);
			*/
            EndPaint(hWnd, &ps);
        }
        break;

	case WM_CLOSE:
		KillTimer(hWnd, IDT_TIMER1);
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


POINT GetCenterPoint(HWND hWnd)
{
	POINT centerPoint;
	RECT rect;
	GetClientRect(hWnd, &rect);
	centerPoint.x = rect.right / 2;
	centerPoint.y = rect.bottom / 2;

	return centerPoint;
}


void DrawClockFace(HDC hdc, HWND hWnd)
{
	POINT centerPoint = GetCenterPoint(hWnd);


	SelectObject(hdc, GetStockObject(WHITE_BRUSH));

	Ellipse(hdc, centerPoint.x - CLOCK_SCALE, centerPoint.y - CLOCK_SCALE, centerPoint.x + CLOCK_SCALE, centerPoint.y + CLOCK_SCALE);

	DWORD dwR = centerPoint.x - CLOCK_BORDER;
	double dwHourUnitDegree = 2 * PI / 12;
	POINT point;
	
	
	for (int i = 0; i < 12; i++)
	{
		//point.x = dwR * cos(dwHourUnitDegree * i) + centerPoint.x;
		//point.y = dwR * sin(dwHourUnitDegree * i) + centerPoint.y;
		point.x = dwR * sin(dwHourUnitDegree * i) + centerPoint.x;
		point.y = -(dwR * cos(dwHourUnitDegree * i)) + centerPoint.y;
		Ellipse(hdc, point.x - CLOCK_SCALE, point.y - CLOCK_SCALE, point.x + CLOCK_SCALE, point.y + CLOCK_SCALE);
	}
	
}


void DrawSecondHand(HWND hWnd, WORD wSecond)
{
	double secondDegree = 2 * PI / 60;
	POINT centerPoint = GetCenterPoint(hWnd);
	HDC hdc = GetDC(hWnd);
	DWORD dwR = centerPoint.x - CLOCK_BORDER - 10;


	HPEN hSecondErasePen = CreatePen(PS_SOLID, CLOCK_SECOND_HAND_WID, RGB(0, 0, 0));
	SelectObject(hdc, hSecondErasePen);
	MoveToEx(hdc, centerPoint.x, centerPoint.y, NULL);
	POINT endPoint;
	WORD oldSecond = 0;
	if (wSecond == 0)
	{
		oldSecond = 59;
	}
	else
	{
		oldSecond = wSecond - 1;
	}
	endPoint.x = dwR * sin(secondDegree * oldSecond) + centerPoint.x;
	endPoint.y = -(dwR * cos(secondDegree * oldSecond)) + centerPoint.y;
	LineTo(hdc, endPoint.x, endPoint.y);


	HPEN hSecondPen = CreatePen(PS_SOLID, CLOCK_SECOND_HAND_WID, RGB(255, 0, 0));
	SelectObject(hdc, hSecondPen);
	MoveToEx(hdc, centerPoint.x, centerPoint.y, NULL);
	//POINT endPoint;
	endPoint.x = dwR * sin(secondDegree * wSecond) + centerPoint.x;
	endPoint.y = -(dwR * cos(secondDegree * wSecond)) + centerPoint.y;
	LineTo(hdc, endPoint.x, endPoint.y);
}

void DrawMinuteHand(HWND hWnd, WORD wMinute)
{
	static WORD minute = wMinute;
	double secondDegree = 2 * PI / 60;
	POINT centerPoint = GetCenterPoint(hWnd);
	HDC hdc = GetDC(hWnd);
	DWORD dwR = centerPoint.x - CLOCK_BORDER - 30;
	POINT endPoint;
	if (minute != wMinute)
	{
		minute = wMinute;
		HPEN hMinuteErasePen = CreatePen(PS_SOLID, CLOCK_MINUTE_HAND_WID, RGB(0, 0, 0));
		SelectObject(hdc, hMinuteErasePen);
		MoveToEx(hdc, centerPoint.x, centerPoint.y, NULL);
		
		WORD oldMinute = 0;
		if (wMinute == 0)
		{
			oldMinute = 59;
		}
		else
		{
			oldMinute = wMinute - 1;
		}
		endPoint.x = dwR * sin(secondDegree * oldMinute) + centerPoint.x;
		endPoint.y = -(dwR * cos(secondDegree * oldMinute)) + centerPoint.y;
		LineTo(hdc, endPoint.x, endPoint.y);
	}
	


	HPEN hMinutePen = CreatePen(PS_SOLID, CLOCK_MINUTE_HAND_WID, RGB(255, 255, 0));
	SelectObject(hdc, hMinutePen);
	MoveToEx(hdc, centerPoint.x, centerPoint.y, NULL);
	//POINT endPoint;
	endPoint.x = dwR * sin(secondDegree * wMinute) + centerPoint.x;
	endPoint.y = -(dwR * cos(secondDegree * wMinute)) + centerPoint.y;
	LineTo(hdc, endPoint.x, endPoint.y);
}

void DrawHourHand(HWND hWnd, WORD wHour, WORD wMinute)
{
	double secondDegree = 2 * PI / 60;
	double hourToMinuteDegreeUnit = 2 * PI / 12 / 60;
	double hourDegreeUnit = 2 * PI / 12;

	POINT centerPoint = GetCenterPoint(hWnd);
	HDC hdc = GetDC(hWnd);
	DWORD dwR = centerPoint.x - CLOCK_BORDER - 50;


	HPEN hHourErasePen = CreatePen(PS_SOLID, CLOCK_HOUR_HAND_WID, RGB(0, 0, 0));
	SelectObject(hdc, hHourErasePen);
	MoveToEx(hdc, centerPoint.x, centerPoint.y, NULL);
	POINT endPoint;
	
	WORD oldMinute = 0;
	if (wMinute == 0)
	{
		oldMinute = 59;
	}
	else
	{
		oldMinute = wMinute - 1;
	}

	endPoint.x = dwR * sin(hourDegreeUnit * wHour + hourToMinuteDegreeUnit * oldMinute) + centerPoint.x;
	endPoint.y = -(dwR * cos(hourDegreeUnit * wHour + hourToMinuteDegreeUnit * oldMinute)) + centerPoint.y;
	LineTo(hdc, endPoint.x, endPoint.y);


	HPEN hHourPen = CreatePen(PS_SOLID, CLOCK_HOUR_HAND_WID, RGB(0, 255, 0));
	SelectObject(hdc, hHourPen);
	MoveToEx(hdc, centerPoint.x, centerPoint.y, NULL);
	//POINT endPoint;
	endPoint.x = dwR * sin(hourDegreeUnit * wHour + hourToMinuteDegreeUnit * wMinute) + centerPoint.x;
	endPoint.y = -(dwR * cos(hourDegreeUnit * wHour + hourToMinuteDegreeUnit * wMinute)) + centerPoint.y;
	LineTo(hdc, endPoint.x, endPoint.y);
}