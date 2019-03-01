#include <iostream>
#include "Chinese.h"
using namespace std;

Person::~Person()
{
	cout << "~Person" << endl;
}

Chinese::Chinese()
{
	cout << "Chinese" << endl;
}

Chinese::~Chinese()
{
	cout << "~Chinese" << endl;
}

void Chinese::say()
{
	cout << "��ͨ��" << endl;
}

void Chinese::eat()
{
	cout << "����" << endl;
}

Person* GetPersonInstance()
{
	return new Chinese();
}

void ReleaseInstance(Person *p)
{
	delete p;
}