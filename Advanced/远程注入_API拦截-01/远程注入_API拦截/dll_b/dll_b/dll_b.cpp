// dll_b.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "dll_b.h"

int __stdcall OutputMsg_B()
{
	::MessageBox(NULL, L"DLL_B", L"DLL_B", MB_OK);
	return 0;
}