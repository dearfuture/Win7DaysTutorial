/*
±àÂë£ºÔ¬´ºĞñ
½²½â£ºÔ¬´ºĞñ
Ö÷Ìâ£º±àÒëC++ÓïÑÔµÄDLL
*/

#include <string.h>
#include "calc.h"

int add1(int a, int b)
{
	return a + b;
}

double add2(double a, double b)
{
	return a + b;
}

int add3(char *str1, int size, char *str2)
{
	int total = strlen(str1) + strlen(str2);
	if (total >= size)
	{
		return total;
	}
	strcat_s(str1, size, str2);
	return total;
}