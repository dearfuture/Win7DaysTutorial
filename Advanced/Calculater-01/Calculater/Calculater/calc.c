#include "calc.h"
#include <string.h>

int __stdcall add1(int a, int b)
{
	return a + b;
}

double __stdcall add2(double a, double b)
{
	return a + b;
}

int __stdcall add3(char *str1, int size, char *str2)
{
	int total = strlen(str1) + strlen(str2);
	if (total >= size)
	{
		return total;
	}
	strcat_s(str1, size, str2);
	return total;
}
