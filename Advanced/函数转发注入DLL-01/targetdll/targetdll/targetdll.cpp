/*
���룺Ԭ����
���⣺Ԭ����
���ݣ�����ת��ע��DLL
ʾ�����޲κ���ת�����вκ���ת��
��ϵ�ṹ��Ӧ�ó���--->���dll--->ԭdll
 */

#include <stdio.h>
#include "targetdll.h"

void __stdcall hello()
{
	printf("\n---------------------\n");
	printf("--------------hello-------------");
	printf("\n---------------------\n");
}

int __stdcall add1(int a, int b)
{
	printf("\n---------------------\n");
	printf("--------add-----%d", a + b);
	printf("\n---------------------\n");
	return a + b;
}