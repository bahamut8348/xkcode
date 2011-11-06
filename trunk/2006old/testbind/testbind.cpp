// testbind.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>

using namespace std;

struct Person
{
	Person(const string& name)
		: name_(name)
	{}

	string Name()
	{
		return name_;
	}

	string name_;
};

template <typename R, typename T>
class simple_binder
{
public:
	explicit simple_binder(R (T::*pfn)())
		: pfn_(pfn)
	{}

	R operator()(T& t)
	{
		return (t.*pfn_)();
	}
private:
	R (T::*pfn_)();
};

class placeholder{};
placeholder _1;

template <typename R, typename T>
simple_binder<R, T>
simple_bind( R (T::*pfn)(), placeholder&)
{
	return simple_binder<R, T>(pfn);
}

int main()
{
	Person person("Ralph");
	cout << simple_bind(&Person::Name, _1)(person)<< endl;
}

