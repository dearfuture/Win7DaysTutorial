#pragma once

extern "C"
{
	__declspec(dllexport) void __stdcall SetHookById(int id);

	__declspec(dllexport) void __stdcall UnHookById(int id);

	__declspec(dllexport) unsigned long __stdcall GetThreadIdByName(wchar_t* szName);
}