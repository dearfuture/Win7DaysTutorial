/*
代码编写：袁春旭
相关课程：七日成蝶-C++升华篇之贪吃蛇视频课程
 */
#include <windows.h>
#include <math.h>
#include <time.h>

#define STEP 20
#define SNAKE_LEN 250

struct Snake {
	int x;
	int y;
}snakes[SNAKE_LEN];
struct Food {
	int x;
	int y;
}food;

int i = 0;
int direction, length, score, level;
int speed[5] = { 200, 100, 50, 25, 12 };
wchar_t cScore[5], cLevel[2];
int lineX = 600, lineY = 463, scoreX = 600 + 2 * STEP, scoreY = lineY / 3 * 2;
bool foodFlag;//食物产生是否重合蛇身
RECT rect;


bool init(HINSTANCE hINSTANCE, INT nCmdShow);
LRESULT CALLBACK important(HWND, UINT, WPARAM, LPARAM);
void start(HWND hwnd);

int WINAPI WinMain(HINSTANCE hINSTANCE, HINSTANCE hPrevInst, LPSTR lpszCmdLine, INT nCmdShow)
{

	MSG Msg;
	init(hINSTANCE, nCmdShow);
	//消息循环
	while (GetMessage(&Msg, NULL, 0, 0)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

//--------------
LRESULT CALLBACK important(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;  //设备上下文句柄
	HBRUSH hBrush;//画刷句柄
	PAINTSTRUCT PrStr;
	switch (message) {
	case WM_CREATE:
		GetClientRect(hwnd, &rect);
		start(hwnd);
		break;

	case WM_KEYDOWN://处理键盘消息
		switch (wParam)
		{
		case VK_LEFT:
			if (direction < 3)//当前方向为垂直方向
				direction = 3;
			break;
		case VK_RIGHT:
			if (direction < 3)//当前方向为水平方向
				direction = 4;
			break;
		case VK_UP:
			if (direction > 2)//当前方向为水平方向
				direction = 1;
			break;
		case VK_DOWN:
			if (direction > 2)
				direction = 2;
			break;
		}
		return 0;

	case WM_TIMER:  //定时重绘
		for (i = length - 1; i > 0; --i) {
			snakes[i].x = snakes[i - 1].x;
			snakes[i].y = snakes[i - 1].y;
		}
		switch (direction)
		{
		case 1:
			snakes[0].y -= STEP;
			break;
		case 2:
			snakes[0].y += STEP;
			break;
		case 3:
			snakes[0].x -= STEP;
			break;
		case 4:
			snakes[0].x += STEP;
			break;
		}
		//撞墙了
		if (snakes[0].x < 0 || snakes[0].x > lineX - STEP || snakes[0].y < 0 || snakes[0].y > rect.bottom - STEP / 2) {
			KillTimer(hwnd, 1);
			int re = MessageBox(hwnd, TEXT("撞墙了!是否继续？"), TEXT("Never Give Up!"), MB_YESNO);
			if (re == IDYES) {
				start(hwnd);
			}
			else {
				SendMessage(hwnd, WM_DESTROY, NULL, NULL);
			}
			return 0;
		}
		//吃到了食物
		if (snakes[0].x == food.x && snakes[0].y == food.y) {
			snakes[length].x = snakes[length - 1].x;
			snakes[length].y = snakes[length - 1].y;
			length++;
			score += 100;
			int tempLevel = level;
			level = score / 1000 + 1;
			if (level != tempLevel) {
				KillTimer(hwnd, 1);
				SetTimer(hwnd, 1, speed[level - 1], NULL);//设置时间
			}
			if (level > 5) {
				KillTimer(hwnd, 1);
				MessageBox(hwnd, TEXT("恭喜你通关了!"), TEXT("GOOD!"), MB_OK);
			}
			if (length == SNAKE_LEN) {
				KillTimer(hwnd, 1);
				MessageBox(hwnd, TEXT("恭喜你通关了!"), TEXT("GOOD!"), MB_OK);
				return 0;
			}
			do {
				foodFlag = false;
				food.x = (rand() % lineX) / STEP*STEP;
				food.y = (rand() % (rect.bottom - STEP)) / STEP*STEP;
				for (i = 0; i < length; ++i) {
					if (snakes[i].x == food.x && snakes[i].y == food.y) {
						foodFlag = true;
					}
				}
			} while (foodFlag);
		}
		//吃到自己了
		for (i = 1; i < length; ++i) {
			if (snakes[i].x == snakes[0].x && snakes[i].y == snakes[0].y) {
				KillTimer(hwnd, 1);
				int re = MessageBox(hwnd, TEXT("撞到自己了!是否继续？"), TEXT("Never Give Up!"), MB_YESNO);
				if (re == IDYES) {
					start(hwnd);
				}
				else {
					SendMessage(hwnd, WM_DESTROY, NULL, NULL);
				}
				return 0;
			}
		}
		InvalidateRect(hwnd, &rect, 1);
		return 0;
	case WM_PAINT:
		hDC = BeginPaint(hwnd, &PrStr);
		hBrush = CreateSolidBrush(RGB(0, 255, 0));
		SelectObject(hDC, hBrush);  //添加进设备表
		for (i = 0; i < length; ++i)
			Rectangle(hDC, snakes[i].x, snakes[i].y, snakes[i].x + STEP, snakes[i].y + STEP);
		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		SelectObject(hDC, hBrush);  //添加进设备表
		Rectangle(hDC, food.x, food.y, food.x + STEP, food.y + STEP);
		MoveToEx(hDC, lineX, 0, NULL);
		LineTo(hDC, lineX, lineY);
		SetTextColor(hDC, RGB(255, 0, 0));
		_itow(score, cScore, 10);
		_itow(level, cLevel, 10);
		TextOut(hDC, scoreX, scoreY / 2, L"分数：", 6);
		TextOut(hDC, scoreX + 3 * STEP, scoreY / 2, cScore, wcslen(cScore));
		TextOut(hDC, scoreX, scoreY / 2 + 3 * STEP, L"等级：", 6);
		TextOut(hDC, scoreX + 3 * STEP, scoreY / 2 + 3 * STEP, cLevel, wcslen(cLevel));
		EndPaint(hwnd, &PrStr);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		KillTimer(hwnd, 1);
		return 0;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}
//--------------

void start(HWND hwnd)
{
	direction = 4, length = 3, score = 0, level = 1;
	SetTimer(hwnd, 1, speed[level - 1], NULL);//设置时间
	snakes[0].x = 400;
	snakes[1].x = snakes[0].x - STEP;
	snakes[2].x = snakes[1].x - STEP;
	snakes[0].y = snakes[1].y = snakes[2].y = 200;
	srand(time(0));
	do {
		foodFlag = false;
		food.x = (rand() % lineX) / STEP*STEP;
		food.y = (rand() % (rect.bottom - STEP)) / STEP*STEP;
		for (i = 0; i < length; ++i) {
			if (snakes[i].x == food.x && snakes[i].y == food.y) {
				foodFlag = true;
			}
		}
	} while (foodFlag);
}

bool init(HINSTANCE hINSTANCE, INT nCmdShow)
{
	HWND hwnd;
	WNDCLASS wndclass;
	wchar_t lpszClassName[] = L"窗口";
	wchar_t lpszTitle[] = L"贪吃蛇";

	//窗口类的定义
	wndclass.style = 0;
	wndclass.lpfnWndProc = important;//消息处理函数
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hINSTANCE;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = lpszClassName;

	//窗口类注册
	if (!RegisterClass(&wndclass)) {
		MessageBeep(0);
		return FALSE;
	}

	//创建窗口
	hwnd = CreateWindow(
		lpszClassName,
		lpszTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		STEP * 40,//300,
		STEP * 25,//700,
		NULL,
		NULL,
		hINSTANCE,
		NULL
	);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	return true;
}