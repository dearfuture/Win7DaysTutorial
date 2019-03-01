#include "hookapi.h"
#include <Windows.h>

#include <windows.h>
#include <ImageHlp.h>
#pragma comment(lib, "ImageHlp")

#include <tlhelp32.h>

///////////////////////////////////////////////////////////////////////////////


// 从指定地址得到包含该地址的模块句柄
static HMODULE WINAPI ModuleFromAddress(PVOID pv)
{
	MEMORY_BASIC_INFORMATION mbi;
	return((VirtualQuery(pv, &mbi, sizeof(mbi)) != 0)
		? (HMODULE)mbi.AllocationBase : NULL);
}


///////////////////////////////////////////////////////////////////////////////

// 异常过滤函数
LONG WINAPI InvalidReadExceptionFilter(PEXCEPTION_POINTERS pep)
{
	LONG lDisposition = EXCEPTION_EXECUTE_HANDLER;
	return lDisposition;
}

///////////////////////////////////////////////////////////////////////////////

CApiHook* CApiHook::sm_pHead = NULL;    // CApiHook对象链表头结点

BOOL CApiHook::ExcludeAPIHookMod = TRUE;    // 是否排除HOOK本模块的函数

PROC CApiHook::sm_pFunFreeLibrary = NULL;    // FreeLibrary的内存中的地址


											 ///////////////////////////////////////////////////////////////////////////////

											 // 构造函数
CApiHook::CApiHook()
{
	if (sm_pFunFreeLibrary == NULL)
	{
		sm_pFunFreeLibrary = GetProcAddressRaw(GetModuleHandleW(L"Kernel32.dll"), "FreeLibrary");
	}

	// 前插一个结点
	m_pNext = sm_pHead;
	sm_pHead = this;
	Init();        // 初始化
}


///////////////////////////////////////////////////////////////////////////////

// 构造函数
CApiHook::CApiHook(PSTR pszCalleeModName, PSTR pszFuncName, PROC pfnHook)
{
	if (sm_pFunFreeLibrary == NULL)
	{
		sm_pFunFreeLibrary = GetProcAddressRaw(GetModuleHandleW(L"Kernel32.dll"), "FreeLibrary");
	}
	// 前插一个结点
	m_pNext = sm_pHead;
	sm_pHead = this;
	HookApi(pszCalleeModName, pszFuncName, pfnHook);    // 挂钩某个API函数
}


///////////////////////////////////////////////////////////////////////////////


CApiHook::~CApiHook()
{
	// 从链表中删除本节点
	CApiHook * p = sm_pHead;
	if (p == this)
	{
		sm_pHead = p->m_pNext;
	}
	else
	{
		BOOL bFound = FALSE;

		for (; p->m_pNext != NULL; p = p->m_pNext)
		{
			if (p->m_pNext == this)
			{
				p->m_pNext = p->m_pNext->m_pNext;
				break;
			}
		}
	}
	UnHook();        // 撤销挂钩
}


///////////////////////////////////////////////////////////////////////////////

void CApiHook::Init()
{
	m_hMod = NULL;
	m_pszCalleeModName = NULL;
	m_pszFuncName = NULL;
	m_pfnOrig = NULL;
	m_pfnHook = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void CApiHook::Uninit()
{
	// 这里不把模块名、函数名和替换函数地址设置为NULL，防止重复加载
	m_hMod = NULL;
	m_pfnOrig = NULL;
}

///////////////////////////////////////////////////////////////////////////////

// 本函数不能为inline类型 —— 得到函数的实际地址
FARPROC WINAPI CApiHook::GetProcAddressRaw(HMODULE hmod, PCSTR pszProcName)
{
	return(::GetProcAddress(hmod, pszProcName));    // 得到函数的真实地址
}


///////////////////////////////////////////////////////////////////////////////

// 在某个模块中HOOK某个API函数 —— 重要
BOOL WINAPI CApiHook::HookApi(PSTR pszCalleeModName, PSTR pszFuncName, PROC pfnHook)
{
	if (m_pszCalleeModName != NULL)
		UnHook();

	// 得到HOOK信息
	m_hMod = GetModuleHandleA(pszCalleeModName);    // 得到模块句柄
	m_pszCalleeModName = pszCalleeModName;
	m_pszFuncName = pszFuncName;
	m_pfnHook = pfnHook;    // 替换函数地址
	if (m_hMod)
		m_pfnOrig = GetProcAddressRaw(m_hMod, m_pszFuncName);    // 原函数地址

																 // 如果原函数地址为NULL，可能是因为模块并没有加载（比如延迟加载DLL就会出现该问题）
	if (m_pfnOrig == NULL)
	{
#ifdef _DEBUG
		wchar_t szPathname[MAX_PATH];
		GetModuleFileNameW(NULL, szPathname, _countof(szPathname));
		wchar_t sz[1024];
		wsprintfW(sz, L"[%4u - %s] impossible to find %S\r\n",
			GetCurrentProcessId(), szPathname, pszFuncName);
		OutputDebugString(sz);
#endif
		return FALSE;
	}

#ifdef _DEBUG
	// This section was used for debugging sessions when Explorer died as 
	// a folder content was requested
	// 
	//static BOOL s_bFirstTime = TRUE;
	//if (s_bFirstTime)
	//{
	//   s_bFirstTime = FALSE;

	//   wchar_t szPathname[MAX_PATH];
	//   GetModuleFileNameW(NULL, szPathname, _countof(szPathname));
	//   wchar_t* pszExeFile = wcsrchr(szPathname, L'\\') + 1;
	//   OutputDebugStringW(L"Injected in ");
	//   OutputDebugStringW(pszExeFile);
	//   if (_wcsicmp(pszExeFile, L"Explorer.EXE") == 0)
	//   {
	//      DebugBreak();
	//   }
	//   OutputDebugStringW(L"\n   --> ");
	//   wsprintfW(szPathname, L"%S", pszFuncName);
	//   OutputDebugStringW(szPathname);
	//   OutputDebugStringW(L"\n");
	//}
#endif

	// HOOK某个函数
	ReplaceIATEntryInAllMods(m_pszCalleeModName, m_pfnOrig, m_pfnHook);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////

// 撤销挂钩
void WINAPI CApiHook::UnHook()
{
	if (m_pfnHook == NULL)
		return;

	// 撤销挂钩 —— 将函数地址复原
	ReplaceIATEntryInAllMods(m_pszCalleeModName, m_pfnHook, m_pfnOrig);

	Uninit();
}


///////////////////////////////////////////////////////////////////////////////

// 将本进程中所有模块的IAT中的指定函数地址pfnCurrent替换为pfnNew
BOOL CApiHook::ReplaceIATEntryInAllMods(PCSTR pszCalleeModName,
	PROC pfnCurrent, PROC pfnNew)
{
	if (pszCalleeModName == NULL || pfnCurrent == NULL || pfnNew == NULL)
		return FALSE;

	// 得到当前模块（即包含这段代码的模块）
	HMODULE hmodThisMod = ExcludeAPIHookMod
		? ModuleFromAddress(ReplaceIATEntryInAllMods) : NULL;

	// 得到本进程中的模块列表
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return FALSE;


	MODULEENTRY32 me = { sizeof(me) };

	for (BOOL bOk = Module32First(hSnapshot, &me); bOk; bOk = Module32Next(hSnapshot, &me))
	{
		// 不替换当前模块的函数
		if (me.hModule != hmodThisMod)
		{
			ReplaceIATEntryInOneMod(pszCalleeModName, pfnCurrent, pfnNew, me.hModule);
		}
	}

	CloseHandle(hSnapshot);

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////


// 替换模块导入表中的函数地址
void WINAPI CApiHook::ReplaceIATEntryInOneMod(PCSTR pszCalleeModName,
	PROC pfnCurrent, PROC pfnNew, HMODULE hmodCaller)
{
	ULONG ulSize;

	// 一个异常可能被触发，因为诸如explorer.exe这样的进程能够快速地加载和卸载模块
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc = NULL;
	__try
	{
		pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToDataEx(
			hmodCaller, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &ulSize, NULL);
	}
	__except (InvalidReadExceptionFilter(GetExceptionInformation()))
	{

	}

	if (pImportDesc == NULL)
		return;

	// 在导入描述符中查找导入信息
	for (; pImportDesc->Name; ++pImportDesc)
	{
		// 得到模块名pszModName（模块基地址 + 名称偏移）
		PSTR pszModName = (PSTR)((PBYTE)hmodCaller + pImportDesc->Name);

		// 判断模块名是否为指定模块
		if (lstrcmpiA(pszModName, pszCalleeModName) == 0)
		{
			// 取得调用者导入函数地址表在模块中的函数地址（模块基址 + 偏移）
			PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)
				((PBYTE)hmodCaller + pImportDesc->FirstThunk);

			// 用新的函数地址替换当前函数地址
			for (; pThunk->u1.Function; pThunk++)
			{

				// 取得函数地址
				PROC* ppfn = (PROC*)&pThunk->u1.Function;

				// 是否是需要替换的函数（比较函数地址）
				BOOL bFound = (*ppfn == pfnCurrent);
				if (bFound)
				{
					// 修改地址
					if (!WriteProcessMemory(GetCurrentProcess(), ppfn, &pfnNew,
						sizeof(pfnNew), NULL) && (ERROR_NOACCESS == GetLastError()))
					{
						// 如果失败，则更改页保护属性
						DWORD dwOldProtect;
						if (VirtualProtect(ppfn, sizeof(pfnNew), PAGE_WRITECOPY,
							&dwOldProtect))
						{

							WriteProcessMemory(GetCurrentProcess(), ppfn, &pfnNew,
								sizeof(pfnNew), NULL);
							VirtualProtect(ppfn, sizeof(pfnNew), dwOldProtect,
								&dwOldProtect);
						}
					}
					return;
				}
			}
		}
	}
}


///////////////////////////////////////////////////////////////////////////////

// 替换模块导出表中函数的地址
void WINAPI CApiHook::ReplaceEATEntryInOneMod(HMODULE hmod, PCSTR pszFunctionName, PROC pfnNew)
{
	// 得到模块导出节地址
	ULONG ulSize;

	// 得到导出节目录地址
	PIMAGE_EXPORT_DIRECTORY pExportDir = NULL;
	__try
	{
		pExportDir = (PIMAGE_EXPORT_DIRECTORY)ImageDirectoryEntryToData(
			hmod, TRUE, IMAGE_DIRECTORY_ENTRY_EXPORT, &ulSize);
	}
	__except (InvalidReadExceptionFilter(GetExceptionInformation()))
	{

	}

	if (pExportDir == NULL)
		return;

	PDWORD pdwNamesRvas = (PDWORD)((PBYTE)hmod + pExportDir->AddressOfNames);
	PWORD pdwNameOrdinals = (PWORD)
		((PBYTE)hmod + pExportDir->AddressOfNameOrdinals);
	PDWORD pdwFunctionAddresses = (PDWORD)
		((PBYTE)hmod + pExportDir->AddressOfFunctions);

	// 遍历模块函数名表
	for (DWORD n = 0; n < pExportDir->NumberOfNames; n++)
	{
		// 取得函数名
		PSTR pszFuncName = (PSTR)((PBYTE)hmod + pdwNamesRvas[n]);

		// 如果不是指定的函数，则继续遍历
		if (lstrcmpiA(pszFuncName, pszFunctionName) != 0)
			continue;

		// 得到这个指定函数的序号
		WORD ordinal = pdwNameOrdinals[n];

		// 得到函数地址
		PROC * ppfn = (PROC*)&pdwFunctionAddresses[ordinal];

		// 用RVA（相对虚拟地址）更换成新的地址
		pfnNew = (PROC)((PBYTE)pfnNew - (PBYTE)hmod);

		// 用新的函数地址替换当前函数地址
		if (!WriteProcessMemory(GetCurrentProcess(), ppfn, &pfnNew, sizeof(pfnNew), NULL)
			&& (ERROR_NOACCESS == GetLastError()))
		{
			DWORD dwOldProtect;
			if (VirtualProtect(ppfn, sizeof(pfnNew), PAGE_WRITECOPY,
				&dwOldProtect))
			{

				WriteProcessMemory(GetCurrentProcess(), ppfn, &pfnNew,
					sizeof(pfnNew), NULL);
				VirtualProtect(ppfn, sizeof(pfnNew), dwOldProtect, &dwOldProtect);
			}
		}
		break;
	}
}


///////////////////////////////////////////////////////////////////////////////

// 替换LoadLibaray*函数和GetProcAddress函数

CApiHook CApiHook::sm_LoadLibraryA("Kernel32.dll", "LoadLibraryA",
(PROC)CApiHook::LoadLibraryA_Hook);

CApiHook CApiHook::sm_LoadLibraryW("Kernel32.dll", "LoadLibraryW",
(PROC)CApiHook::LoadLibraryW_Hook);

CApiHook CApiHook::sm_LoadLibraryExA("Kernel32.dll", "LoadLibraryExA",
(PROC)CApiHook::LoadLibraryExA_Hook);

CApiHook CApiHook::sm_LoadLibraryExW("Kernel32.dll", "LoadLibraryExW",
(PROC)CApiHook::LoadLibraryExW_Hook);

CApiHook CApiHook::sm_GetProcAddress("Kernel32.dll", "GetProcAddress",
(PROC)CApiHook::GetProcAddress_Hook);

CApiHook CApiHook::sm_FreeLibrary("Kernel32.dll", "FreeLibrary",
(PROC)CApiHook::FreeLibrary_Hook);


///////////////////////////////////////////////////////////////////////////////

// 防止运行时加载模块（如线程调用LoadLibarayW等函数）
void CApiHook::FixupNewlyLoadedModule(PCSTR pszModPath, HMODULE hmod, DWORD dwFlags)
{
	// 如果一个新的模块被加载，重新挂钩函数
	if ((hmod != NULL) &&
		(hmod != ModuleFromAddress(FixupNewlyLoadedModule)) &&
		((dwFlags & LOAD_LIBRARY_AS_DATAFILE) == 0) &&
		((dwFlags & LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE) == 0) &&
		((dwFlags & LOAD_LIBRARY_AS_IMAGE_RESOURCE) == 0)
		)
	{
		// 遍历CApiHook对象链，挂钩个对象要求的API函数
		for (CApiHook* p = sm_pHead; p != NULL; p = p->m_pNext)
		{
			// 如果某个对象的原函数地址为NULL，可能是延迟加载等原因导致原来DLL不在内存中而引起的
			// 因此在这里根据模块名获得函数现在的地址
			if (p->m_pfnOrig == NULL)
			{
				if (lstrcmpiA(p->m_pszCalleeModName, pszModPath) == 0)
				{
					p->m_hMod = hmod;
					p->m_pfnOrig = p->GetProcAddressRaw(hmod, p->m_pszFuncName);
				}
			}
			if (p->m_pfnOrig != NULL)
			{
				// 重新挂钩每个函数
				ReplaceIATEntryInAllMods(p->m_pszCalleeModName, p->m_pfnOrig, p->m_pfnHook);
			}
			else
			{
#ifdef _DEBUG
				// 可能是延迟加载导致p->m_pfnOrig为空
				wchar_t szPathname[MAX_PATH];
				GetModuleFileNameW(NULL, szPathname, _countof(szPathname));
				wchar_t sz[1024];
				wsprintfW(sz, L"[%4u - %s] impossible to find %S\r\n",
					GetCurrentProcessId(), szPathname, p->m_pszCalleeModName);
				OutputDebugString(sz);
#endif
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

// 防止运行时卸载模块（如线程调用FreeLibrary等函数）
BOOL WINAPI CApiHook::FixupNewlyUnLoadModule(HMODULE hmod)
{
	if (hmod == NULL || hmod == ModuleFromAddress(FixupNewlyUnLoadModule))
		return FALSE;

	BOOL bFree = TRUE;
	wchar_t szModPath[MAX_PATH];
	szModPath[0] = L'\0';

	if (!GetModuleFileNameW(hmod, szModPath, MAX_PATH))
		return FALSE;

	if (!::FreeLibrary(hmod))
		return FALSE;

	bFree = TRUE;

	if (GetModuleFileNameW(hmod, szModPath, MAX_PATH))
	{
		// 如果还能够得到地址，则说明该模块在进程地址空间中并没有真正卸载
		if (GetModuleHandleW(szModPath))
			bFree = FALSE;
	}

	// 如果卸载了，则撤销某些挂钩
	if (bFree)
	{
		for (CApiHook * p = sm_pHead; p; p = p->m_pNext)
		{
			if (p->m_pfnOrig && p->m_hMod == hmod)
				p->UnHook();
		}
	}
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////

// 挂钩LoadLibarayA的替换函数
HMODULE WINAPI CApiHook::LoadLibraryA_Hook(PCSTR pszModulePath)
{
	HMODULE hmod = ::LoadLibraryA(pszModulePath);
	FixupNewlyLoadedModule(pszModulePath, hmod, 0);
	return hmod;
}


///////////////////////////////////////////////////////////////////////////////

// 挂钩LoadLibraryW的替换函数
HMODULE WINAPI CApiHook::LoadLibraryW_Hook(PCWSTR pszModulePath)
{
	HMODULE hmod = ::LoadLibraryW(pszModulePath);
	char szModPath[MAX_PATH];
	WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pszModulePath, -1,
		szModPath, MAX_PATH * sizeof(char), NULL, NULL);
	FixupNewlyLoadedModule(szModPath, hmod, 0);
	return hmod;
}


///////////////////////////////////////////////////////////////////////////////

// 挂钩LoadLibraryExA的替换函数
HMODULE WINAPI CApiHook::LoadLibraryExA_Hook(PCSTR pszModulePath,
	HANDLE hFile, DWORD dwFlags)
{
	HMODULE hmod = ::LoadLibraryExA(pszModulePath, hFile, dwFlags);
	FixupNewlyLoadedModule(pszModulePath, hmod, dwFlags);
	return hmod;
}


///////////////////////////////////////////////////////////////////////////////

// 挂钩LoadLibraryExW的替换函数
HMODULE WINAPI CApiHook::LoadLibraryExW_Hook(PCWSTR pszModulePath,
	HANDLE hFile, DWORD dwFlags)
{

	HMODULE hmod = ::LoadLibraryExW(pszModulePath, hFile, dwFlags);
	char szModPath[MAX_PATH];
	WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pszModulePath, -1,
		szModPath, MAX_PATH * sizeof(char), NULL, NULL);
	FixupNewlyLoadedModule(szModPath, hmod, dwFlags);
	return hmod;
}


///////////////////////////////////////////////////////////////////////////////

// 挂钩GetProcAddress的替换函数
FARPROC WINAPI CApiHook::GetProcAddress_Hook(HMODULE hmod, PCSTR pszProcName)
{
	// 取得函数的实际地址
	FARPROC pfn = GetProcAddressRaw(hmod, pszProcName);

	CApiHook * p = sm_pHead;
	for (; (pfn != NULL) && (p != NULL); p = p->m_pNext)
	{
		// 查看是否是想挂钩的函数，如果是，则返回新的替换函数
		if (pfn == p->m_pfnOrig)
		{
			// 返回我们的替换函数地址
			pfn = p->m_pfnHook;
			break;
		}
	}
	return pfn;    // 返回替换函数地址
}

///////////////////////////////////////////////////////////////////////////////

// 拦截FreeLibrary的替换函数
// 注意函数采用内联汇编进行编写，需要使用__declspec(naked)标记函数，让编译器不产生框架代码，自己维护堆栈平衡
__declspec(naked) BOOL WINAPI CApiHook::FreeLibrary_Hook(HMODULE hLibModule)
{
	__asm
	{
		push ebp;
		mov ebp, esp;
		pushad;
		push dword ptr[FreeLibrary_Hook];
		call dword ptr[ModuleFromAddress];
		cmp[ebp + 8], eax;
		jne NotMe;
		mov eax, sm_pHead;
	Next:
		cmp eax, NULL;
		je    CallTrue;
		mov edx, [eax].m_pNext;
		push edx;
		push eax; ; 这里相当于压入this指针
			call UnHook; ; 相当于调用[eax].UnHook
			pop eax;
		jmp    Next;
	CallTrue:
		popad;
		pop ebp;
		jmp dword ptr[sm_pFunFreeLibrary]; ; 如果是卸载本模块，则可能是其他程序调用，需要跳转到真正的FreeLibaray函数的地址

			; 不是卸载的本模块
			NotMe :
		push[ebp + 8];
		call FixupNewlyUnLoadModule; ; 不是本模块卸载，则调用修正的FreeLibrary函数

			popad;
		pop ebp;
		mov eax, TRUE;
		ret 4;
	}
}