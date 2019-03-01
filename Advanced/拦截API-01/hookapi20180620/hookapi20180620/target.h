#pragma once

extern "C"
{
	__declspec(dllexport) void __stdcall testme();
	__declspec(dllexport) int __stdcall testadd(int a, int b);
	__declspec(dllexport) void __stdcall testhello();
}