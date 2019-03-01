#pragma once

#include "Person.h"

class Chinese : public Person
{
public:
	Chinese();
	virtual ~Chinese();
	virtual void say();
	virtual void eat();
};

__declspec(dllexport) Person* GetPersonInstance();

__declspec(dllexport) void ReleaseInstance(Person *p);