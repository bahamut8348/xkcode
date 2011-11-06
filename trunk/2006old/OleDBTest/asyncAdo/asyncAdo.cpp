// asyncAdo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TestAdo.h"

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale( LC_ALL, "Chinese" );
	CoInitialize(NULL);
	asyncAdo::CTestAdo ado;
	ado.Start();
	Sleep(10000);
	_tprintf(L"DONE\n");
	::CoUninitialize();
	return 0;
}

