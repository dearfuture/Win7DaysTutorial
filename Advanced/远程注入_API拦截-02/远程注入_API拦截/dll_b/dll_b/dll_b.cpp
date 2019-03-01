/*
编码：袁春旭
讲解：袁春旭
主题：远程注入+拦截API
说明：
MyAPP：窗口程序，被用户远程注入，目标实验品
dll_a.dll：MyAPP加载的DLL，用于弹窗
dll_b.dll（当前）：用于拦截后替换dll_a.dll中的弹窗函数
dllInject：用于远程注入MyAPP并通过dll_b.dll替换dll_a.dll中的弹窗函数
*/
// dll_b.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "dll_b.h"

int __stdcall OutputMsg_B()
{
	::MessageBox(NULL, L"DLL_B", L"DLL_B", MB_OK);
	return 0;
}