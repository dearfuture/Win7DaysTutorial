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










示例编写：袁春旭  QQ:2064438762
*/
#include <Windows.h>

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
	//HICON hIcon = LoadIcon(NULL, IDI_WARNING);
	//HICON hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, width, height, LR_DEFAULTCOLOR);
	HICON hIcon = (HICON)LoadImage(hInstance, L"test.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	
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