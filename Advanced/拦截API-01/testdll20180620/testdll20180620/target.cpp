#include "target.h"
#include <stdio.h>

void __stdcall testme()
{
	printf("\ntestme\n");
}

int __stdcall testadd(int a, int b)
{
	return a + b;
}

void __stdcall testhello()
{
	printf("\ntesthello\n");
}