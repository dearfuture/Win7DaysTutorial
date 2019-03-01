/*
讲解：袁春旭
编码：袁春旭
内容：挂钩指定线程
 */
#include "HookMsg.h"
#include <Windows.h>
#include <Tlhelp32.h>

typedef struct _MYHOOKDATA
{
	int nType;
	HOOKPROC hkprc;
	HHOOK hhook;
}MYHOOKDATA;

/*
1、语法写错，注意空格
2、共享节的名字过长，长度不能超过32个字符
3、共享节中的数据都要初始化
 */
#pragma data_seg("shared")
MYHOOKDATA myhookdata[7] = {0};
HHOOK g_hHook = 0;
#pragma data_seg()

#pragma comment(linker, "/section:shared,rws")

LRESULT WINAPI CallWndProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI CBTProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI DebugProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI MessageProc(int nCode, WPARAM wParam, LPARAM lParam);


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD ul_reason_for_call,
	LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		OutputDebugString(L"hookapp DLL_PROCESS_ATTACH");
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
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

void __stdcall SetHookById(int id)
{
	OutputDebugString(L"hookapp SetHookById start");
	myhookdata[id].hhook = SetWindowsHookEx(
		myhookdata[id].nType,
		myhookdata[id].hkprc,
		(HINSTANCE)NULL,
		//GetCurrentThreadId()
		0
	);
	OutputDebugString(L"hookapp SetHookById end");
}

void __stdcall UnHookById(int id)
{
	UnhookWindowsHookEx(myhookdata[id].hhook);
}


LRESULT WINAPI MouseProcEx(int nCode, WPARAM wParam, LPARAM lParam)
{
	WCHAR buf2[30] = { 0 };
	
	if (nCode < 0)
		return CallNextHookEx(g_hHook, nCode, wParam, lParam);

	switch (wParam)
	{
	case WM_LBUTTONDOWN:
		OutputDebugString(L"hookapp MouseProc WM_LBUTTONDOWN");
		PMOUSEHOOKSTRUCT pInfo = (PMOUSEHOOKSTRUCT)lParam;
		WCHAR buf[30] = { 0 };
		wsprintf(buf, L"hookapp (%d, %d)", pInfo->pt.x, pInfo->pt.y);
		OutputDebugString(buf);
		wsprintf(buf2, L"hookapp g_hHook = %x", g_hHook);
		OutputDebugString(buf2);
		break;
	}

	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

void __stdcall SetHookVal(HHOOK hookval)
{
	g_hHook = hookval;
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
	if (nCode < 0)
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
	if (nCode < 0)
		return CallNextHookEx(myhookdata[5].hhook, nCode, wParam, lParam);

	switch (wParam)
	{
	case WM_LBUTTONDOWN:
		OutputDebugString(L"hookapp MouseProc WM_LBUTTONDOWN");
		PMOUSEHOOKSTRUCT pInfo = (PMOUSEHOOKSTRUCT)lParam;
		WCHAR buf[30] = { 0 };
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