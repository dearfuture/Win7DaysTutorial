/**
示例：自建窗口
讲解：袁春旭

1.窗口背景色设置  Color Types: COLOR_WINDOW
API说明：

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




示例：自建窗口
讲解：袁春旭

2.窗口风格设置  CreateWindow  CreateWindowEx
WS_BORDER：windows窄线边
WS_CAPTION：windows标题条，包含WS_BORDER
WS_CHILDWINDOW、WS_CHILD：窗口是一个子窗口，不能与WS_POPUP共用
WS_CLIPCHILDREN：作为父窗口，刷新时裁剪子窗口区域，提高刷新效率
WS_CLIPSIBLINGS：作为子窗口，刷新时裁剪其它子窗口的重叠区域，提高刷新效率
WS_DISABLED：关闭窗口响应，用户的任何操作均无效
WS_DLGFRAME：窗口有一个边框样式，通常与对话框一起使用。这种样式的窗口不能有标题栏。
WS_GROUP：窗口组件化时使用。
演示 WS_HSCROLL：窗口含有水平滚动条。
演示 WS_VSCROLL：窗口含有垂直滚动条。
演示 WS_MAXIMIZE：窗口初始状态最大化。
WS_MINIMIZE：窗口初始状态最小化。
WS_ICONIC：同WS_MINIMIZE
WS_MAXIMIZEBOX：窗口含有最大化按钮
WS_MINIMIZEBOX：窗口含有最小化按钮
WS_OVERLAPPED：重叠模式，含有标题框和边框
WS_TILED：同WS_OVERLAPPED
演示 WS_OVERLAPPEDWINDOW：(WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)
WS_TILEDWINDOW：同WS_OVERLAPPEDWINDOW
WS_POPUP：弹出窗口模式，不能与WS_CHILD共用
演示 WS_POPUPWINDOW：WS_POPUP | WS_BORDER | WS_SYSMENU
WS_SIZEBOX：窗口具有可伸缩边框。
WS_THICKFRAME：同WS_SIZEBOX
演示 WS_SYSMENU：标题栏带系统菜单，即关闭按钮。
WS_TABSTOP：作为窗口中的控件，设置了WS_TABSTOP标志的控件可以通过TAB键焦点轮转。
WS_VISIBLE：窗口是否可见。 由ShowWindow设置





示例：自建窗口
讲解：袁春旭

3.窗口行为设置

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


示例：自建窗口
讲解：袁春旭

4.窗口图标设置
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


int GetSystemMetrics(  //单位像素
int nIndex
);



示例：自建窗口
讲解：袁春旭

图标文件icon制作

在线制作ICON文件    https://www.ico.la/

离线制作ICON文件    软件介绍：anytoicon

导入icon资源

使用系统图标

使用外部图标




示例：自建窗口
讲解：袁春旭

鼠标图像设置
导入外部cur文件


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


int GetSystemMetrics(  //单位像素
int nIndex
);




示例：自建窗口
讲解：袁春旭

消息处理

WM_CREATE：窗口创建消息

WM_MOVE：窗口移动消息

WM_SIZE：窗口大小变化消息

WM_ACTIVATE：窗口活动消息

WM_LBUTTONDOWN：鼠标左键按下
x = LOWORD(lParam); //获取鼠标x位置，相对窗口
y = HIWORD(lParam); //获取鼠标y位置，相对窗口
GetCursorPos(&pt);  //获取鼠标位置，相对屏幕
GetSystemMetrics (SM_MOUSEPRESENT);  确定鼠标是否存在
GetSystemMetrics (SM_CMOUSEBUTTONS); 确定鼠标按键数量

WM_KEYDOWN：有按键按下















示例：自建窗口
讲解：袁春旭



鼠标双击事件
WM_LBUTTONDBLCLK WM_RBUTTONDBLCLK WM_MBUTTONDBLCLK
wcx.style = CS_DBLCLKS
设置前：WM_LBUTTONDOWN，WM_LBUTTONUP，WM_LBUTTONDOWN，WM_LBUTTONUP
设置后：WM_LBUTTONDOWN，WM_LBUTTONUP，WM_LBUTTONDBLCLK，WM_LBUTTONUP









示例：自建窗口
讲解：袁春旭

ALT按键消息
WM_SYSKEYDOWN

非ALT按键消息
WM_KEYDOWN

















示例编写：袁春旭  QQ:2064438762
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
		x = LOWORD(lParam); //获取鼠标x位置
		y = HIWORD(lParam); //获取鼠标y位置
		GetCursorPos(&pt);
		wsprintf(pos1, L"%d, %d", x, y);
		MessageBox(hWnd, pos1, L"Message", MB_OK);
		wsprintf(pos2, L"%d, %d", pt.x, pt.y);
		MessageBox(hWnd, pos2, L"Message", MB_OK);
		break;

	case WM_LBUTTONDOWN:
		//x = LOWORD(lParam); //获取鼠标x位置
		//y = HIWORD(lParam); //获取鼠标y位置
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


	//1.注册窗口
	ATOM win = RegisterClassEx(&wcx);

	//2.创建窗口
	HWND hWnd = CreateWindow(
		L"MainWindowClass",
		L"袁春旭的窗口",
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
		MessageBox(NULL, L"创建窗口失败", L"提示", MB_OK);
		return 0;
	}

	//3. 显示窗口
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);



	//4. 消息获取，分发
	MSG msg;
	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		//TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}