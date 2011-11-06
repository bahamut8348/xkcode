// testoutput.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>


int test()
{

	::FreeConsole ();

	DWORD ret = ::AttachConsole( 3948 );
	int err = GetLastError();
	return 1;
}

int _tmain(int argc, _TCHAR* argv[])
{
	
	test();
	
	int a = 0;
	while(1)
	{
		_flushall();
		printf("test\n");
		Sleep(500);
	}
	
	_flushall();
	Sleep(10000);
	return 0;
}

