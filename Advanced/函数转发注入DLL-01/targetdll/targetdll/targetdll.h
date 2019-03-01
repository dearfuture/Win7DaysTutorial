/*
编码：袁春旭
讲解：袁春旭
内容：函数转发注入DLL
示例：无参函数转发，有参函数转发
关系结构：应用程序--->替代dll--->原dll
当前：原DLL
*/

#pragma once

__declspec(dllexport) void __stdcall hello();

__declspec(dllexport) int __stdcall add1(int a, int b);