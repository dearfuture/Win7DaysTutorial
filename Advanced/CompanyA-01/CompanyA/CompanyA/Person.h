#pragma once

class Person
{
public:
	virtual void say() = 0;
	virtual void eat() = 0;
	virtual ~Person() = 0;
};