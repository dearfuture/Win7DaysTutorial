/*
���룺Ԭ����
���⣺Ԭ����
���⣺Զ��ע��+����API
˵����
MyAPP�����ڳ��򣬱��û�Զ��ע�룬Ŀ��ʵ��Ʒ
dll_a.dll��MyAPP���ص�DLL�����ڵ���
dll_b.dll����ǰ�����������غ��滻dll_a.dll�еĵ�������
dllInject������Զ��ע��MyAPP��ͨ��dll_b.dll�滻dll_a.dll�еĵ�������
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