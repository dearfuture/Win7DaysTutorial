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
		WaitForSingleObject(hThread, INFINITE);    // 这里存在死循环

		CloseHandle(hThread);    // 执行不到这里
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

	// 构造函数 —— 在所有模块中HOOK函数pszFuncName，将其替换为pfnHook
	CApiHook(PSTR pszCalleeModName, PSTR pszFuncName, PROC pfnHook);

	// 析构
	~CApiHook();

	// 返回原来的函数地址
	operator PROC()
	{
		return m_pfnOrig;    // 返回原函数地址
	}

	// 是否挂钩本模块中的函数
	static BOOL ExcludeAPIHookMod;


public:
	// GetProcAddressRaw调用实际的GetProcAddress函数
	static FARPROC WINAPI GetProcAddressRaw(HMODULE hmod, PCSTR pszProcName);

	BOOL WINAPI HookApi(PSTR pszCalleeModName, PSTR pszFuncName, PROC pfnHook);    // HOOK某个函数
	void WINAPI UnHook();        // 撤销挂钩

protected:
	void WINAPI Init();
	void WINAPI Uninit();

private:
	static PVOID sm_pvMaxAppAddr;
	static PROC  sm_pFunFreeLibrary;    // FreeLibrary的实际地址

	static CApiHook * sm_pHead;        // 对象链表头节点
	CApiHook * m_pNext;                // 下一个对象节点

	PCSTR m_pszCalleeModName;        // 要HOOK函数所在的模块名称（ANSI）
	PCSTR m_pszFuncName;            // 要HOOK的函数的函数名（ANSI）
	PROC m_pfnOrig;                    // 函数原地址
	PROC m_pfnHook;                    // HOOK替换函数的地址
	HMODULE m_hMod;                    // 模块句柄

private:
	// 在所有模块的导入节区挂钩一个函数
	static BOOL WINAPI ReplaceIATEntryInAllMods(PCSTR pszCalleeModName,
		PROC pfnOrig, PROC pfnHook);

	// 在一个模块的导入节区挂钩一个函数
	static void WINAPI ReplaceIATEntryInOneMod(PCSTR pszCalleeModName,
		PROC pfnOrig, PROC pfnHook, HMODULE hmodCaller);

	// 在某个模块的导出节区挂钩一个函数
	static void WINAPI ReplaceEATEntryInOneMod(HMODULE hmod, PCSTR pszFunctionName, PROC pfnNew);

private:
	// 当一个DLL被新加载的时候使用该函数HOOK一个函数
	static void WINAPI FixupNewlyLoadedModule(PCSTR pszModName, HMODULE hmod, DWORD dwFlags);

	// 当一个DLL调用FreeLibaray等函数卸载某个模块时，调用此函数
	static BOOL WINAPI FixupNewlyUnLoadModule(HMODULE hmod);

	// 一些替换函数
	static HMODULE WINAPI LoadLibraryA_Hook(PCSTR pszModulePath);
	static HMODULE WINAPI LoadLibraryW_Hook(PCWSTR pszModulePath);
	static HMODULE WINAPI LoadLibraryExA_Hook(PCSTR pszModulePath,
		HANDLE hFile, DWORD dwFlags);
	static HMODULE WINAPI LoadLibraryExW_Hook(PCWSTR pszModulePath,
		HANDLE hFile, DWORD dwFlags);
	static FARPROC WINAPI GetProcAddress_Hook(HMODULE hmod, PCSTR pszProcName);
	static BOOL WINAPI FreeLibrary_Hook(HMODULE hLibModule);

private:
	// 挂钩某些特殊函数的CApiHook对象
	static CApiHook sm_LoadLibraryA;    // 挂钩LoadLibryraA函数的CApiHook对象
	static CApiHook sm_LoadLibraryW;    // 挂钩LoadLibryraW函数的CApiHook对象
	static CApiHook sm_LoadLibraryExA;    // 挂钩LoadLibryraExA函数的CApiHook对象
	static CApiHook sm_LoadLibraryExW;    // 挂钩LoadLibryraExW函数的CApiHook对象
	static CApiHook sm_GetProcAddress;    // 挂钩GetProcAddress函数的CApiHook对象
	static CApiHook sm_FreeLibrary;        // 挂钩FreeLibrary函数的CApiHook对象
};
