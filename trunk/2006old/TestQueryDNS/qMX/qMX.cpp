// qMX.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <vector>
#include <string>
#include <windows.h>
#include <WinDNS.h>


extern "C"
{
	__declspec(dllexport)  BOOL QueryMX( LPCSTR host, LPTSTR ipaddr )
	{
		DNS_RECORD* ppQueryResultsSet, *pResultSet = NULL;
		DNS_STATUS statusDNS = ::DnsQuery( host, DNS_TYPE_MX, 
			DNS_QUERY_STANDARD, NULL, 
			&ppQueryResultsSet, NULL );
		if( statusDNS == ERROR_SUCCESS )
		{
			std::vector<std::string> vec;
			pResultSet = ppQueryResultsSet;
			do 
			{
				ppQueryResultsSet->Data.MX.pNameExchange;
				if( pResultSet->wType == DNS_TYPE_MX) vec.push_back( pResultSet->Data.MX.pNameExchange );
				pResultSet = pResultSet->pNext;
			} while ( pResultSet );

			UINT number = 0;
			::rand_s( &number );
			if( number == UINT_MAX )
				number--;
			int idx = (unsigned int) ((double)number / (double) UINT_MAX * (double)(vec.size()) );

			strcpy( ipaddr, vec[idx].c_str());
			return vec.size();
		}
	return 0;
	}
};

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}


