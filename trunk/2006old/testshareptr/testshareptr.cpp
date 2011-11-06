// testshareptr.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"



template <typename T>
class SharePtr
{
public:
	SharePtr():m_Ptr(0),m_Count(0)
	{

	}
	SharePtr(SharePtr & sp):m_Ptr(sp.m_Ptr), m_Count(sp.m_Count)
	{
		m_Ptr = sp.m_Ptr;
		sp.Incf();
		
	}

public :
	SharePtr( T * ptr): m_Ptr(ptr), m_Count(1)
	{

	}
	~SharePtr()
	{
		Decf();
	}
	T *m_Ptr;

	_void Incf()
	{
		++m_Count;
	}

	_void Decf()
	{
		m_Count--;
		if(m_Count ==0)
		{
			delete m_Ptr;
		}
	}

	operator T *const & () const
	{
		return m_Ptr;
	}




public:	
	SharePtr & operator = (  SharePtr & sp )
	{
		m_Count = sp.m_Count;
		m_Ptr = sp.m_Ptr;
		sp.Incf();
		return *this;
	}
public:

	int m_Count;
};


void test(  )
{

}

int main(int argc, _char* argv[])
{
	
	SharePtr<char> sp2;

	{
		SharePtr<char> sp(new char[100]);
		sp2 = sp;
	}

	SharePtr<char> sp3(sp2);

	//Array<SharePtr<char>> m_test;
	//m_test.Append(sp);
	//m_test.Append(sp2);
	//m_test.Append(sp3);

	Console::Print(L"sp3: %d, sp2: %d", sp3.m_Count, sp2.m_Count);
	
	return 0;
}

