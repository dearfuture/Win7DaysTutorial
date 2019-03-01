#pragma once
#include <Windows.h>

extern "C"
{
	__declspec(dllexport) void __stdcall SetHookById(int id);

	__declspec(dllexport) void __stdcall UnHookById(int id);

	__declspec(dllexport) LRESULT WINAPI MouseProcEx(int nCode, WPARAM wParam, LPARAM lParam);

	__declspec(dllexport) void __stdcall SetHookVal(HHOOK hookval);
}