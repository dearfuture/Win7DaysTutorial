// dll_a.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"


int __stdcall OutputMsg()
{
	::MessageBox(NULL, L"DLL_A", L"DLL_A", MB_OK);
	return 0;
}