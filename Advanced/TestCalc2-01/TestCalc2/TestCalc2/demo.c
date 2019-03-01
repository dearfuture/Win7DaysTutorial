/*
编码：袁春旭
讲解：袁春旭
主题：动态调用dll中的带参函数
 */

#include <stdio.h>
#include <Windows.h>

typedef int (*Add1)(int a, int b);
typedef double(*AAA2)(double a, double b);
typedef int(*STRCAT)(char *str1, int size, char *str2);

int main(void)
{
	char str1[255] = "hello ";
	char str2[] = "world";
	char path[255] = { 0 };
	HMODULE hModule = GetModuleHandle("Calculater.dll");
	DWORD len = 0;
	if (NULL == hModule)
	{
		hModule = LoadLibrary("Calculater.dll");
		if (NULL == hModule)
		{
			printf("load library failed...\n");
			getchar();
			return 1;
		}

		len = GetModuleFileName(hModule, path, 255);
		printf("%s\n", path);
	}

	Add1 add1 = (Add1)GetProcAddress(hModule, "add1");
	printf("%d\n",add1(19, 20));


	AAA2 add2 = (AAA2)GetProcAddress(hModule, "add2");
	printf("%lf\n", add2(1.9, 2.2));


	STRCAT add3 = (STRCAT)GetProcAddress(hModule, "add3");
	add3(str1, 255, str2);
	printf("%s\n", str1);


	FreeLibrary(hModule);
	getchar();
	return 0;
}