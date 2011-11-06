#include <iostream>
using namespace std;
class Base
{
protected:
	int a;

void seta(int b)
	{
	a=b;
	}

public:
	void show()
	{
		cout<<"a:"<<a<<endl;
	}
};

class A : public Base
{
public:
	void a_seta(int b)
	{
		seta(b);
	}
};

class B : public Base
{
public:
	void b_seta(int b)
	{
		seta(b);
	}
};


class C: public A, public B
{
	
};

int main()
{
	C c;
	c.a_seta(2);
	c.b_seta(3);
	c.A::show();
	c.B::show();
	return 1;
}

