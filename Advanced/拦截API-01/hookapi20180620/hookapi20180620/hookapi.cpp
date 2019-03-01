#include "hookapi.h"
#include <Windows.h>

#include <windows.h>
#include <ImageHlp.h>
#pragma comment(lib, "ImageHlp")

#include <tlhelp32.h>

///////////////////////////////////////////////////////////////////////////////


// ��ָ����ַ�õ������õ�ַ��ģ����
static HMODULE WINAPI ModuleFromAddress(PVOID pv)
{
	MEMORY_BASIC_INFORMATION mbi;
	return((VirtualQuery(pv, &mbi, sizeof(mbi)) != 0)
		? (HMODULE)mbi.AllocationBase : NULL);
}


///////////////////////////////////////////////////////////////////////////////

// �쳣���˺���
LONG WINAPI InvalidReadExceptionFilter(PEXCEPTION_POINTERS pep)
{
	LONG lDisposition = EXCEPTION_EXECUTE_HANDLER;
	return lDisposition;
}

///////////////////////////////////////////////////////////////////////////////

CApiHook* CApiHook::sm_pHead = NULL;    // CApiHook��������ͷ���

BOOL CApiHook::ExcludeAPIHookMod = TRUE;    // �Ƿ��ų�HOOK��ģ��ĺ���

PROC CApiHook::sm_pFunFreeLibrary = NULL;    // FreeLibrary���ڴ��еĵ�ַ


											 ///////////////////////////////////////////////////////////////////////////////

											 // ���캯��
CApiHook::CApiHook()
{
	if (sm_pFunFreeLibrary == NULL)
	{
		sm_pFunFreeLibrary = GetProcAddressRaw(GetModuleHandleW(L"Kernel32.dll"), "FreeLibrary");
	}

	// ǰ��һ�����
	m_pNext = sm_pHead;
	sm_pHead = this;
	Init();        // ��ʼ��
}


///////////////////////////////////////////////////////////////////////////////

// ���캯��
CApiHook::CApiHook(PSTR pszCalleeModName, PSTR pszFuncName, PROC pfnHook)
{
	if (sm_pFunFreeLibrary == NULL)
	{
		sm_pFunFreeLibrary = GetProcAddressRaw(GetModuleHandleW(L"Kernel32.dll"), "FreeLibrary");
	}
	// ǰ��һ�����
	m_pNext = sm_pHead;
	sm_pHead = this;
	HookApi(pszCalleeModName, pszFuncName, pfnHook);    // �ҹ�ĳ��API����
}


///////////////////////////////////////////////////////////////////////////////


CApiHook::~CApiHook()
{
	// ��������ɾ�����ڵ�
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
	UnHook();        // �����ҹ�
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
	// ���ﲻ��ģ���������������滻������ַ����ΪNULL����ֹ�ظ�����
	m_hMod = NULL;
	m_pfnOrig = NULL;
}

///////////////////////////////////////////////////////////////////////////////

// ����������Ϊinline���� ���� �õ�������ʵ�ʵ�ַ
FARPROC WINAPI CApiHook::GetProcAddressRaw(HMODULE hmod, PCSTR pszProcName)
{
	return(::GetProcAddress(hmod, pszProcName));    // �õ���������ʵ��ַ
}


///////////////////////////////////////////////////////////////////////////////

// ��ĳ��ģ����HOOKĳ��API���� ���� ��Ҫ
BOOL WINAPI CApiHook::HookApi(PSTR pszCalleeModName, PSTR pszFuncName, PROC pfnHook)
{
	if (m_pszCalleeModName != NULL)
		UnHook();

	// �õ�HOOK��Ϣ
	m_hMod = GetModuleHandleA(pszCalleeModName);    // �õ�ģ����
	m_pszCalleeModName = pszCalleeModName;
	m_pszFuncName = pszFuncName;
	m_pfnHook = pfnHook;    // �滻������ַ
	if (m_hMod)
		m_pfnOrig = GetProcAddressRaw(m_hMod, m_pszFuncName);    // ԭ������ַ

																 // ���ԭ������ַΪNULL����������Ϊģ�鲢û�м��أ������ӳټ���DLL�ͻ���ָ����⣩
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

	// HOOKĳ������
	ReplaceIATEntryInAllMods(m_pszCalleeModName, m_pfnOrig, m_pfnHook);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////

// �����ҹ�
void WINAPI CApiHook::UnHook()
{
	if (m_pfnHook == NULL)
		return;

	// �����ҹ� ���� ��������ַ��ԭ
	ReplaceIATEntryInAllMods(m_pszCalleeModName, m_pfnHook, m_pfnOrig);

	Uninit();
}


///////////////////////////////////////////////////////////////////////////////

// ��������������ģ���IAT�е�ָ��������ַpfnCurrent�滻ΪpfnNew
BOOL CApiHook::ReplaceIATEntryInAllMods(PCSTR pszCalleeModName,
	PROC pfnCurrent, PROC pfnNew)
{
	if (pszCalleeModName == NULL || pfnCurrent == NULL || pfnNew == NULL)
		return FALSE;

	// �õ���ǰģ�飨��������δ����ģ�飩
	HMODULE hmodThisMod = ExcludeAPIHookMod
		? ModuleFromAddress(ReplaceIATEntryInAllMods) : NULL;

	// �õ��������е�ģ���б�
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return FALSE;


	MODULEENTRY32 me = { sizeof(me) };

	for (BOOL bOk = Module32First(hSnapshot, &me); bOk; bOk = Module32Next(hSnapshot, &me))
	{
		// ���滻��ǰģ��ĺ���
		if (me.hModule != hmodThisMod)
		{
			ReplaceIATEntryInOneMod(pszCalleeModName, pfnCurrent, pfnNew, me.hModule);
		}
	}

	CloseHandle(hSnapshot);

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////


// �滻ģ�鵼����еĺ�����ַ
void WINAPI CApiHook::ReplaceIATEntryInOneMod(PCSTR pszCalleeModName,
	PROC pfnCurrent, PROC pfnNew, HMODULE hmodCaller)
{
	ULONG ulSize;

	// һ���쳣���ܱ���������Ϊ����explorer.exe�����Ľ����ܹ����ٵؼ��غ�ж��ģ��
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

	// �ڵ����������в��ҵ�����Ϣ
	for (; pImportDesc->Name; ++pImportDesc)
	{
		// �õ�ģ����pszModName��ģ�����ַ + ����ƫ�ƣ�
		PSTR pszModName = (PSTR)((PBYTE)hmodCaller + pImportDesc->Name);

		// �ж�ģ�����Ƿ�Ϊָ��ģ��
		if (lstrcmpiA(pszModName, pszCalleeModName) == 0)
		{
			// ȡ�õ����ߵ��뺯����ַ����ģ���еĺ�����ַ��ģ���ַ + ƫ�ƣ�
			PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)
				((PBYTE)hmodCaller + pImportDesc->FirstThunk);

			// ���µĺ�����ַ�滻��ǰ������ַ
			for (; pThunk->u1.Function; pThunk++)
			{

				// ȡ�ú�����ַ
				PROC* ppfn = (PROC*)&pThunk->u1.Function;

				// �Ƿ�����Ҫ�滻�ĺ������ȽϺ�����ַ��
				BOOL bFound = (*ppfn == pfnCurrent);
				if (bFound)
				{
					// �޸ĵ�ַ
					if (!WriteProcessMemory(GetCurrentProcess(), ppfn, &pfnNew,
						sizeof(pfnNew), NULL) && (ERROR_NOACCESS == GetLastError()))
					{
						// ���ʧ�ܣ������ҳ��������
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

// �滻ģ�鵼�����к����ĵ�ַ
void WINAPI CApiHook::ReplaceEATEntryInOneMod(HMODULE hmod, PCSTR pszFunctionName, PROC pfnNew)
{
	// �õ�ģ�鵼���ڵ�ַ
	ULONG ulSize;

	// �õ�������Ŀ¼��ַ
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

	// ����ģ�麯������
	for (DWORD n = 0; n < pExportDir->NumberOfNames; n++)
	{
		// ȡ�ú�����
		PSTR pszFuncName = (PSTR)((PBYTE)hmod + pdwNamesRvas[n]);

		// �������ָ���ĺ��������������
		if (lstrcmpiA(pszFuncName, pszFunctionName) != 0)
			continue;

		// �õ����ָ�����������
		WORD ordinal = pdwNameOrdinals[n];

		// �õ�������ַ
		PROC * ppfn = (PROC*)&pdwFunctionAddresses[ordinal];

		// ��RVA����������ַ���������µĵ�ַ
		pfnNew = (PROC)((PBYTE)pfnNew - (PBYTE)hmod);

		// ���µĺ�����ַ�滻��ǰ������ַ
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

// �滻LoadLibaray*������GetProcAddress����

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

// ��ֹ����ʱ����ģ�飨���̵߳���LoadLibarayW�Ⱥ�����
void CApiHook::FixupNewlyLoadedModule(PCSTR pszModPath, HMODULE hmod, DWORD dwFlags)
{
	// ���һ���µ�ģ�鱻���أ����¹ҹ�����
	if ((hmod != NULL) &&
		(hmod != ModuleFromAddress(FixupNewlyLoadedModule)) &&
		((dwFlags & LOAD_LIBRARY_AS_DATAFILE) == 0) &&
		((dwFlags & LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE) == 0) &&
		((dwFlags & LOAD_LIBRARY_AS_IMAGE_RESOURCE) == 0)
		)
	{
		// ����CApiHook���������ҹ�������Ҫ���API����
		for (CApiHook* p = sm_pHead; p != NULL; p = p->m_pNext)
		{
			// ���ĳ�������ԭ������ַΪNULL���������ӳټ��ص�ԭ����ԭ��DLL�����ڴ��ж������
			// ������������ģ������ú������ڵĵ�ַ
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
				// ���¹ҹ�ÿ������
				ReplaceIATEntryInAllMods(p->m_pszCalleeModName, p->m_pfnOrig, p->m_pfnHook);
			}
			else
			{
#ifdef _DEBUG
				// �������ӳټ��ص���p->m_pfnOrigΪ��
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

// ��ֹ����ʱж��ģ�飨���̵߳���FreeLibrary�Ⱥ�����
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
		// ������ܹ��õ���ַ����˵����ģ���ڽ��̵�ַ�ռ��в�û������ж��
		if (GetModuleHandleW(szModPath))
			bFree = FALSE;
	}

	// ���ж���ˣ�����ĳЩ�ҹ�
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

// �ҹ�LoadLibarayA���滻����
HMODULE WINAPI CApiHook::LoadLibraryA_Hook(PCSTR pszModulePath)
{
	HMODULE hmod = ::LoadLibraryA(pszModulePath);
	FixupNewlyLoadedModule(pszModulePath, hmod, 0);
	return hmod;
}


///////////////////////////////////////////////////////////////////////////////

// �ҹ�LoadLibraryW���滻����
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

// �ҹ�LoadLibraryExA���滻����
HMODULE WINAPI CApiHook::LoadLibraryExA_Hook(PCSTR pszModulePath,
	HANDLE hFile, DWORD dwFlags)
{
	HMODULE hmod = ::LoadLibraryExA(pszModulePath, hFile, dwFlags);
	FixupNewlyLoadedModule(pszModulePath, hmod, dwFlags);
	return hmod;
}


///////////////////////////////////////////////////////////////////////////////

// �ҹ�LoadLibraryExW���滻����
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

// �ҹ�GetProcAddress���滻����
FARPROC WINAPI CApiHook::GetProcAddress_Hook(HMODULE hmod, PCSTR pszProcName)
{
	// ȡ�ú�����ʵ�ʵ�ַ
	FARPROC pfn = GetProcAddressRaw(hmod, pszProcName);

	CApiHook * p = sm_pHead;
	for (; (pfn != NULL) && (p != NULL); p = p->m_pNext)
	{
		// �鿴�Ƿ�����ҹ��ĺ���������ǣ��򷵻��µ��滻����
		if (pfn == p->m_pfnOrig)
		{
			// �������ǵ��滻������ַ
			pfn = p->m_pfnHook;
			break;
		}
	}
	return pfn;    // �����滻������ַ
}

///////////////////////////////////////////////////////////////////////////////

// ����FreeLibrary���滻����
// ע�⺯���������������б�д����Ҫʹ��__declspec(naked)��Ǻ������ñ�������������ܴ��룬�Լ�ά����ջƽ��
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
		push eax; ; �����൱��ѹ��thisָ��
			call UnHook; ; �൱�ڵ���[eax].UnHook
			pop eax;
		jmp    Next;
	CallTrue:
		popad;
		pop ebp;
		jmp dword ptr[sm_pFunFreeLibrary]; ; �����ж�ر�ģ�飬�����������������ã���Ҫ��ת��������FreeLibaray�����ĵ�ַ

			; ����ж�صı�ģ��
			NotMe :
		push[ebp + 8];
		call FixupNewlyUnLoadModule; ; ���Ǳ�ģ��ж�أ������������FreeLibrary����

			popad;
		pop ebp;
		mov eax, TRUE;
		ret 4;
	}
}