// TestQueryDNS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <limits.h>

#include <windows.h>
#include <WinDNS.h>
#include <string>
#include <vector>

BOOL QueryMX( LPCSTR host, LPTSTR ipaddr )
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
		
		u_int number = 0;
		::rand_s( &number );
		if( number == UINT_MAX )
			number--;
		int idx = (unsigned int) ((double)number / (double) UINT_MAX * (double)(vec.size()) );
	
		strcpy( ipaddr, vec[idx].c_str());
		return 1;
	}

	return FALSE;
}


int _tmain(int argc, _TCHAR* argv[])
{
	char buf[32];
	QueryMX( "yahoo.com",buf );printf("%s\n", buf );
	QueryMX( "yahoo.com",buf );printf("%s\n", buf );
	QueryMX( "yahoo.com",buf );printf("%s\n", buf );
	QueryMX( "yahoo.com",buf );printf("%s\n", buf );
	QueryMX( "yahoo.com",buf );printf("%s\n", buf );
	QueryMX( "yahoo.com",buf );printf("%s\n", buf );
	QueryMX( "yahoo.com",buf );printf("%s\n", buf );

	QueryMX( "yahoo.com",buf );printf("%s\n", buf );

	QueryMX( "yahoo.com",buf );printf("%s\n", buf );

	QueryMX( "yahoo.com",buf );printf("%s\n", buf );

	QueryMX( "yahoo.com",buf );printf("%s\n", buf );

	QueryMX( "yahoo.com",buf );printf("%s\n", buf );

	QueryMX( "yahoo.com",buf );printf("%s\n", buf );

	QueryMX( "yahoo.com",buf );printf("%s\n", buf );

	QueryMX( "yahoo.com",buf );printf("%s\n", buf );

	QueryMX( "yahoo.com",buf );printf("%s\n", buf );

	QueryMX( "yahoo.com",buf );printf("%s\n", buf );

	QueryMX( "yahoo.com",buf );printf("%s\n", buf );

	QueryMX( "yahoo.com",buf );printf("%s\n", buf );

	QueryMX( "yahoo.com",buf );printf("%s\n", buf );

	QueryMX( "yahoo.com",buf );printf("%s\n", buf );

	QueryMX( "yahoo.com",buf );printf("%s\n", buf );

	QueryMX( "yahoo.com",buf );printf("%s\n", buf );

	QueryMX( "yahoo.com",buf );printf("%s\n", buf );


	return 0;
}

