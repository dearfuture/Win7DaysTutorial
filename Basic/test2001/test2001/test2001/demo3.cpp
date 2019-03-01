#include <Windows.h>
#include <stdlib.h>
#include <iostream>
#include <string>
using namespace std;

#ifndef SM_SERVERR2
# define SM_SERVERR2 89
#endif
#ifndef VER_SUITE_STORAGE_SERVER
# define VER_SUITE_STORAGE_SERVER 0x00002000
#endif

#ifndef VER_SUITE_COMPUTE_SERVER
# define VER_SUITE_COMPUTE_SERVER 0x00004000
#endif

string GetSystemName()
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	OSVERSIONINFOEX os;
	os.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if (GetVersionEx((OSVERSIONINFO *)&os))
	{
		string vname;
		switch (os.dwMajorVersion) {
		case 4:
			switch (os.dwMinorVersion) {
			case 0:
				if (os.dwPlatformId == VER_PLATFORM_WIN32_NT)vname = "Windows NT 4.0";
				else if (os.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)vname = "Windows 95";
				break;
			case 10:
				vname = "Windows 98";
				break;
			case 90:
				vname = "Windows Me";
				break;
			}
			break;
		case 5:
			switch (os.dwMinorVersion) {
			case 0:
				vname = "Windows 2000";
				break;
			case 1:
				vname = "Windows XP";
				break;
			case 2:
				if (os.wProductType == VER_NT_WORKSTATION && \
					info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
					vname = "Windows XP Professional x64 Edition";
				else if (GetSystemMetrics(SM_SERVERR2) == 0)
					vname = "Windows Server 2003";
				else if (GetSystemMetrics(SM_SERVERR2) != 0)
					vname = "Windows Server 2003 R2";
				break;
			}
			break;
		case 6:
			switch (os.dwMinorVersion) {
			case 0:
				if (os.wProductType == VER_NT_WORKSTATION)
					vname = "Windows Vista";
				else
					vname = "Windows Server 2008";
				break;
			case 1:
				if (os.wProductType == VER_NT_WORKSTATION)
					vname = "Windows 7";
				else
					vname = "Windows Server 2008 R2";
				break;
			case 2:
				if (os.wProductType == VER_NT_WORKSTATION)
					vname = "Windows 8";
				else
					vname = "Windows Server 2012";
				break;
			case 3:
				if (os.wProductType == VER_NT_WORKSTATION)
					vname = "Windows 8.1";
				else
					vname = "Windows Server 2012 R2";
				break;
			}
			break;
		case 10:
		default:
			vname = "unknow os";
		}
		return vname;
	}
	else
		return "GetVersionEx failed";
}

int main(void)
{
	cout << GetSystemName() << endl;
	system("pause");
	return 0;
}