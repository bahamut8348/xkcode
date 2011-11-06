// python_mssql.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "sqlconvert.h"
#include "mssql.h"


#ifdef _MANAGED
#pragma managed(push, off)
#endif


BOOST_PYTHON_MODULE(mssql)
{

	Py_Initialize( );
	initConvert();
	init_mssql();

}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{	
	if(ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		CoInitialize(NULL);

		PyDateTime_IMPORT;
	}
	


	
	return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

