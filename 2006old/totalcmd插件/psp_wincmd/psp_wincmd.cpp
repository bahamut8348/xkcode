// psp_wincmd.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "psp.h"


extern PackInfo g_ArchList[MAX_NUM];;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    
	if( ul_reason_for_call == DLL_PROCESS_ATTACH )
	{
		Memory::MemSet(g_ArchList, 0, sizeof( g_ArchList ) );
		CreateResourceManager( );
	}
	else if( ul_reason_for_call == DLL_PROCESS_DETACH )
	{
		GetResourceManagerSingleton( )->Release( );
	}
	
	return TRUE;
}



