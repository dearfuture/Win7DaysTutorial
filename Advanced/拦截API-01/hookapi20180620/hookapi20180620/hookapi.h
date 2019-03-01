#pragma once

#include <Windows.h>

/*
BOOL WINAPI DllMain(HINSTANCE hInstDll, DWORD fdwReason, PVOID fImpLoad)
{
	static HANDLE hThread;
	static DWORD dwThreadId;
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		g_hInstDll = hInstDll;
		hThread = BEGINTHREADEX(NULL, 0, ThreadProc, (PVOID)hInstDll, 0, &dwThreadId);
		if (hThread == NULL)
		{
			return FALSE;
		}
		break;

	case DLL_PROCESS_DETACH:
		SendMessage(g_hWnd, WM_CLOSE, 0, 0);
		WaitForSingleObject(hThread, INFINITE);    // ���������ѭ��

		CloseHandle(hThread);    // ִ�в�������
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;

	default:
		break;
	}
	return TRUE;
}
*/

class CApiHook
{
public:

	CApiHook();

	// ���캯�� ���� ������ģ����HOOK����pszFuncName�������滻ΪpfnHook
	CApiHook(PSTR pszCalleeModName, PSTR pszFuncName, PROC pfnHook);

	// ����
	~CApiHook();

	// ����ԭ���ĺ�����ַ
	operator PROC()
	{
		return m_pfnOrig;    // ����ԭ������ַ
	}

	// �Ƿ�ҹ���ģ���еĺ���
	static BOOL ExcludeAPIHookMod;


public:
	// GetProcAddressRaw����ʵ�ʵ�GetProcAddress����
	static FARPROC WINAPI GetProcAddressRaw(HMODULE hmod, PCSTR pszProcName);

	BOOL WINAPI HookApi(PSTR pszCalleeModName, PSTR pszFuncName, PROC pfnHook);    // HOOKĳ������
	void WINAPI UnHook();        // �����ҹ�

protected:
	void WINAPI Init();
	void WINAPI Uninit();

private:
	static PVOID sm_pvMaxAppAddr;
	static PROC  sm_pFunFreeLibrary;    // FreeLibrary��ʵ�ʵ�ַ

	static CApiHook * sm_pHead;        // ��������ͷ�ڵ�
	CApiHook * m_pNext;                // ��һ������ڵ�

	PCSTR m_pszCalleeModName;        // ҪHOOK�������ڵ�ģ�����ƣ�ANSI��
	PCSTR m_pszFuncName;            // ҪHOOK�ĺ����ĺ�������ANSI��
	PROC m_pfnOrig;                    // ����ԭ��ַ
	PROC m_pfnHook;                    // HOOK�滻�����ĵ�ַ
	HMODULE m_hMod;                    // ģ����

private:
	// ������ģ��ĵ�������ҹ�һ������
	static BOOL WINAPI ReplaceIATEntryInAllMods(PCSTR pszCalleeModName,
		PROC pfnOrig, PROC pfnHook);

	// ��һ��ģ��ĵ�������ҹ�һ������
	static void WINAPI ReplaceIATEntryInOneMod(PCSTR pszCalleeModName,
		PROC pfnOrig, PROC pfnHook, HMODULE hmodCaller);

	// ��ĳ��ģ��ĵ��������ҹ�һ������
	static void WINAPI ReplaceEATEntryInOneMod(HMODULE hmod, PCSTR pszFunctionName, PROC pfnNew);

private:
	// ��һ��DLL���¼��ص�ʱ��ʹ�øú���HOOKһ������
	static void WINAPI FixupNewlyLoadedModule(PCSTR pszModName, HMODULE hmod, DWORD dwFlags);

	// ��һ��DLL����FreeLibaray�Ⱥ���ж��ĳ��ģ��ʱ�����ô˺���
	static BOOL WINAPI FixupNewlyUnLoadModule(HMODULE hmod);

	// һЩ�滻����
	static HMODULE WINAPI LoadLibraryA_Hook(PCSTR pszModulePath);
	static HMODULE WINAPI LoadLibraryW_Hook(PCWSTR pszModulePath);
	static HMODULE WINAPI LoadLibraryExA_Hook(PCSTR pszModulePath,
		HANDLE hFile, DWORD dwFlags);
	static HMODULE WINAPI LoadLibraryExW_Hook(PCWSTR pszModulePath,
		HANDLE hFile, DWORD dwFlags);
	static FARPROC WINAPI GetProcAddress_Hook(HMODULE hmod, PCSTR pszProcName);
	static BOOL WINAPI FreeLibrary_Hook(HMODULE hLibModule);

private:
	// �ҹ�ĳЩ���⺯����CApiHook����
	static CApiHook sm_LoadLibraryA;    // �ҹ�LoadLibryraA������CApiHook����
	static CApiHook sm_LoadLibraryW;    // �ҹ�LoadLibryraW������CApiHook����
	static CApiHook sm_LoadLibraryExA;    // �ҹ�LoadLibryraExA������CApiHook����
	static CApiHook sm_LoadLibraryExW;    // �ҹ�LoadLibryraExW������CApiHook����
	static CApiHook sm_GetProcAddress;    // �ҹ�GetProcAddress������CApiHook����
	static CApiHook sm_FreeLibrary;        // �ҹ�FreeLibrary������CApiHook����
};
