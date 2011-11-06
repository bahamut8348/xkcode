// hookdll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <stdio.h>
#include "DetoursWapper.h"
#include "network.h"
static LONG dwSlept = 0;
static VOID (WINAPI * TrueSleep)(DWORD dwMilliseconds) = Sleep;


extern "C" __declspec( dllexport ) void Entry() {}


VOID WINAPI TimedSleep(DWORD dwMilliseconds)
{
	DWORD dwBeg = GetTickCount();
	 printf("dll: Sleep Start.\n");
	TrueSleep(dwMilliseconds);
	 printf("dll: Sleep End.\n");
	DWORD dwEnd = GetTickCount();
	InterlockedExchangeAdd(&dwSlept, dwEnd - dwBeg);
}



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  dwReason,
                       LPVOID lpReserved
					 )
{

	printf("hook.dll: DllMain %d.\n", dwReason);
	if (dwReason == DLL_PROCESS_ATTACH) {
		printf("hook.dll: Starting.\n");

		Detours::Instance()->Attach( TrueSleep, TimedSleep );
		DetoursNetwork();
	}
	else if (dwReason == DLL_PROCESS_DETACH) 
	{
		Detours::Instance()->Release();
	}
	return TRUE;

}



