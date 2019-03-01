/*
�����д��Ԭ����
��ؿγ̣����ճɵ�-C++����ƪ̰֮������Ƶ�γ�
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
bool foodFlag;//ʳ������Ƿ��غ�����
RECT rect;


bool init(HINSTANCE hINSTANCE, INT nCmdShow);
LRESULT CALLBACK important(HWND, UINT, WPARAM, LPARAM);
void start(HWND hwnd);

int WINAPI WinMain(HINSTANCE hINSTANCE, HINSTANCE hPrevInst, LPSTR lpszCmdLine, INT nCmdShow)
{

	MSG Msg;
	init(hINSTANCE, nCmdShow);
	//��Ϣѭ��
	while (GetMessage(&Msg, NULL, 0, 0)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

//--------------
LRESULT CALLBACK important(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;  //�豸�����ľ��
	HBRUSH hBrush;//��ˢ���
	PAINTSTRUCT PrStr;
	switch (message) {
	case WM_CREATE:
		GetClientRect(hwnd, &rect);
		start(hwnd);
		break;

	case WM_KEYDOWN://���������Ϣ
		switch (wParam)
		{
		case VK_LEFT:
			if (direction < 3)//��ǰ����Ϊ��ֱ����
				direction = 3;
			break;
		case VK_RIGHT:
			if (direction < 3)//��ǰ����Ϊˮƽ����
				direction = 4;
			break;
		case VK_UP:
			if (direction > 2)//��ǰ����Ϊˮƽ����
				direction = 1;
			break;
		case VK_DOWN:
			if (direction > 2)
				direction = 2;
			break;
		}
		return 0;

	case WM_TIMER:  //��ʱ�ػ�
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
		//ײǽ��
		if (snakes[0].x < 0 || snakes[0].x > lineX - STEP || snakes[0].y < 0 || snakes[0].y > rect.bottom - STEP / 2) {
			KillTimer(hwnd, 1);
			int re = MessageBox(hwnd, TEXT("ײǽ��!�Ƿ������"), TEXT("Never Give Up!"), MB_YESNO);
			if (re == IDYES) {
				start(hwnd);
			}
			else {
				SendMessage(hwnd, WM_DESTROY, NULL, NULL);
			}
			return 0;
		}
		//�Ե���ʳ��
		if (snakes[0].x == food.x && snakes[0].y == food.y) {
			snakes[length].x = snakes[length - 1].x;
			snakes[length].y = snakes[length - 1].y;
			length++;
			score += 100;
			int tempLevel = level;
			level = score / 1000 + 1;
			if (level != tempLevel) {
				KillTimer(hwnd, 1);
				SetTimer(hwnd, 1, speed[level - 1], NULL);//����ʱ��
			}
			if (level > 5) {
				KillTimer(hwnd, 1);
				MessageBox(hwnd, TEXT("��ϲ��ͨ����!"), TEXT("GOOD!"), MB_OK);
			}
			if (length == SNAKE_LEN) {
				KillTimer(hwnd, 1);
				MessageBox(hwnd, TEXT("��ϲ��ͨ����!"), TEXT("GOOD!"), MB_OK);
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
		//�Ե��Լ���
		for (i = 1; i < length; ++i) {
			if (snakes[i].x == snakes[0].x && snakes[i].y == snakes[0].y) {
				KillTimer(hwnd, 1);
				int re = MessageBox(hwnd, TEXT("ײ���Լ���!�Ƿ������"), TEXT("Never Give Up!"), MB_YESNO);
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
		SelectObject(hDC, hBrush);  //��ӽ��豸��
		for (i = 0; i < length; ++i)
			Rectangle(hDC, snakes[i].x, snakes[i].y, snakes[i].x + STEP, snakes[i].y + STEP);
		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		SelectObject(hDC, hBrush);  //��ӽ��豸��
		Rectangle(hDC, food.x, food.y, food.x + STEP, food.y + STEP);
		MoveToEx(hDC, lineX, 0, NULL);
		LineTo(hDC, lineX, lineY);
		SetTextColor(hDC, RGB(255, 0, 0));
		_itow(score, cScore, 10);
		_itow(level, cLevel, 10);
		TextOut(hDC, scoreX, scoreY / 2, L"������", 6);
		TextOut(hDC, scoreX + 3 * STEP, scoreY / 2, cScore, wcslen(cScore));
		TextOut(hDC, scoreX, scoreY / 2 + 3 * STEP, L"�ȼ���", 6);
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
	SetTimer(hwnd, 1, speed[level - 1], NULL);//����ʱ��
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
	wchar_t lpszClassName[] = L"����";
	wchar_t lpszTitle[] = L"̰����";

	//������Ķ���
	wndclass.style = 0;
	wndclass.lpfnWndProc = important;//��Ϣ������
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hINSTANCE;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = lpszClassName;

	//������ע��
	if (!RegisterClass(&wndclass)) {
		MessageBeep(0);
		return FALSE;
	}

	//��������
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