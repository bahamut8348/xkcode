#include <windows.h>
class base
{
	public:
	virtual int foo() = 0;	
	virtual int get(int c) =0;
};

class A: public base
{
	public:
	int foo()
	{
		return 5;	
	}
	
	int get(int c)
	{
		return c;
		
	}
};


extern "C" __declspec( dllexport )  A * CreateA()
{
	return new A;
}