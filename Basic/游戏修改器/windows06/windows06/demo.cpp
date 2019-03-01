/*
教学示例：游戏修改器
功能说明：
	1. 列出现有进程。
	2. 指定进程并修改该进程目标内存。
		1)打开进程，获取内核对象句柄。
		2)查找首轮目标。
		3)列出查找结果。
		4)结果大于1时，查找二轮目标。
		5)修改二轮目标中所有内存的值。
	3. 杀死指定进程。
	4. 退出。

编写者:袁春旭
*/

#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <TlHelp32.h>

const DWORD KONEK = 1024;
const DWORD KPAGE = 4 * KONEK;
const DWORD KONEG = KONEK * KONEK * KONEK;

void ShowMenu();
void ShowProcessList();
void EditProcessData();
void KillProcess();

void FirstRound(HANDLE hProcess, DWORD dwValue, DWORD *pAddrList, DWORD *pAddrListCounter, const DWORD addrListMax);
BOOL CompareOnePage(HANDLE hProcess, DWORD dwBaseAddr, DWORD dwValue, DWORD *pAddrList, DWORD *pAddrListCounter, const DWORD addrListMax);
void ShowAddrList(DWORD *pDwAddrList, DWORD dwAddrCount);
void SecondRound(HANDLE hProcess, DWORD dwValue, DWORD *pAddrList, DWORD dwAddrListCounter, DWORD *pTargetList, DWORD *pTargetCounter);


int main(void)
{
	int select = 0;
	while (true)
	{
		//显示菜单
		ShowMenu();

		while (!scanf_s("%d", &select))
		{
			rewind(stdin);
			printf("please input your choice...");
		}

		switch (select)
		{
		case 1:
			ShowProcessList();
			break;

		case 2:
			EditProcessData();
			break;

		case 3:
			KillProcess();
			break;

		case 4:
			printf("Thank your using ...\n");
			system("pause");
			return 0;

		default:
			printf("Please select again 1-4...\n");
		}
	}
	
	return 0;
}

void ShowMenu()
{
	printf("\n--------------------------\n");
	printf("Menu:\n");
	printf("\t1.Show system process list.\n");
	printf("\t2.Input process id which you want to edit.\n");
	printf("\t3.Input process id which you want to kill.\n");
	printf("\t4.Exit\n");
	printf("--------------------------\n");
}

void ShowProcessList()
{
	PROCESSENTRY32 pc;
	pc.dwSize = sizeof(pc);
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	BOOL bMore = Process32First(hProcessSnap, &pc);
	while(bMore)
	{
		printf("\n-------------------\n");
		printf("%d\n", pc.th32ProcessID);
		wprintf(L"%s\n", pc.szExeFile);
		bMore = Process32Next(hProcessSnap, &pc);
	}
	CloseHandle(hProcessSnap);
}

void EditProcessData()
{
	DWORD dwId = 0;
	DWORD dwSearchValue = 0;
	DWORD dwAddrList[4 * KONEK] = { 0 };
	DWORD dwAddrCount = 0;
	BOOL bRet = FALSE;
	printf("please input process id which you want to edit:");
	while (!scanf_s("%u", &dwId))
	{
		rewind(stdin);
		printf("please input again:");
	}
	//printf("\nprocess id = %u\n", dwId);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwId);
	if (NULL == hProcess)
	{
		printf("Open process failed...\n");
		return;
	}
	else
	{
		printf("Open process success...\n");
		system("pause");
		printf("Please input the value which you want search first round:");
		scanf_s("%u", &dwSearchValue);
		
		FirstRound(hProcess, dwSearchValue, dwAddrList, &dwAddrCount, 4 * KONEK);

		ShowAddrList(dwAddrList, dwAddrCount);

		if (dwAddrCount == 0)
		{
			return;
		}
		else if (dwAddrCount == 1)
		{
			DWORD value;
			printf("input the value which you want to set");
			scanf_s("%u", &value);

			bRet = WriteProcessMemory(hProcess, (LPVOID)dwAddrList[0], (LPCVOID)&value, sizeof(DWORD), NULL);
			if (bRet)
			{
				printf("Edit Success!!!\n");
			}
			else
			{
				printf("Edit failed!!!\n");
			}
		}
		else
		{
			DWORD dwSecondRoundSearchValue = 0;
			DWORD dwTargetList[KONEK] = { 0 };
			DWORD dwTargetCounter = 0;
			printf("input the value which you need second round find:");
			scanf_s("%u", &dwSecondRoundSearchValue);
			//找到的值的个数超过1个
			SecondRound(hProcess, dwSecondRoundSearchValue, dwAddrList, dwAddrCount, dwTargetList, &dwTargetCounter);
			ShowAddrList(dwTargetList, dwTargetCounter);
			DWORD value;
			printf("input the value which you want to set");
			scanf_s("%u", &value);

			for (DWORD i = 0; i < dwTargetCounter; i++)
			{
				bRet = WriteProcessMemory(hProcess, (LPVOID)dwTargetList[i], &value, sizeof(value), NULL);
				if (bRet)
				{
					printf("Edit Success!!!\n");
				}
				else
				{
					printf("Edit failed!!!\n");
				}
			}
			
		}
	}

	CloseHandle(hProcess);
}

void KillProcess()
{
	BOOL bRet = FALSE;
	DWORD dwId = 0;
	printf("Please input process id while you want to kill...\n");
	while (!scanf_s("%u", &dwId))
	{
		rewind(stdin);
		printf("please input process id while you want to kill");
	}
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwId);

	if (hProcess != NULL)
	{
		bRet = TerminateProcess(hProcess, 0);
	}
	CloseHandle(hProcess);

	if (bRet)
	{
		printf("kill process success\n");
	}
	else
	{
		printf("kill process failed\n");
	}

}


void FirstRound(HANDLE hProcess, DWORD dwValue, DWORD *pAddrList, DWORD *pAddrListCounter, const DWORD addrListMax)
{
	DWORD dwBaseAddr = 64 * KONEK;

	DWORD dwPageCount = (2 * KONEG - 64 * KONEK * 2) / KPAGE;
	printf("%u pages\n", dwPageCount);

	printf("Start searching ...\n");
	DWORD dwBeginAddr = dwBaseAddr;
	for (; dwBaseAddr < 2 * KONEG - 64 * KONEK; dwBaseAddr += KPAGE)
	{
		if (!CompareOnePage(hProcess, dwBaseAddr, dwValue, pAddrList, pAddrListCounter, addrListMax))
		{
			return;
		}

		//计算进度百分比
		DWORD page = (dwBaseAddr - dwBeginAddr) / KPAGE + 1;
		printf("current is %u page\n", page);
		double temp = ((double)page / dwPageCount) * 100;
		printf("-----%%%f------\n", temp);

	}
	printf("\nSearch finished...\n");
	system("pause");
}

BOOL CompareOnePage(HANDLE hProcess, DWORD dwBaseAddr, DWORD dwValue, DWORD *pAddrList, DWORD *pAddrListCounter, const DWORD addrListMax)
{
	BYTE byPage[KPAGE] = {0};
	if (!ReadProcessMemory(hProcess, (LPCVOID)dwBaseAddr, (LPVOID)byPage, KPAGE, NULL))
	{
		printf("Read Memory error!!!\n");
		return TRUE;
	}

	DWORD *pdwPointer = NULL;
	pdwPointer = (DWORD *)byPage;

	for (DWORD i = 0; i < KONEK; i++)
	{
		if (*pAddrListCounter >= addrListMax)
		{
			printf("Too many data, can not save...\n");
			return FALSE;
		}

		if (pdwPointer[i] == dwValue)
		{
			pAddrList[*pAddrListCounter] = dwBaseAddr + i * sizeof(DWORD);
			(*pAddrListCounter)++;
		}
	}
	return TRUE;
}

void ShowAddrList(DWORD *pDwAddrList, DWORD dwAddrCount)
{
	printf("\n----------Address list begin----------\n");
	for (DWORD i = 0; i < dwAddrCount; i++)
	{
		printf("%X\n", pDwAddrList[i]);
	}
	printf("\n----------Address list end------------\n");
}

void SecondRound(HANDLE hProcess, DWORD dwValue, DWORD *pAddrList, DWORD dwAddrListCounter, DWORD *pTargetList, DWORD *pTargetCounter)
{
	DWORD dwTemp = 0;
	for (DWORD i = 0; i < dwAddrListCounter; i++)
	{
		if (ReadProcessMemory(hProcess, (LPCVOID)pAddrList[i], &dwTemp, sizeof(dwTemp), NULL))
		{
			if (dwTemp == dwValue)
			{
				pTargetList[*pTargetCounter] = pAddrList[i];
				(*pTargetCounter)++;

			}
		}
	}
}