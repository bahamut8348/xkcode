// xkvpn.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "../testvpn/Utils.h"
#include "xkvpn.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

BOOL STD EnumAdapter( BYTE * buffer, DWORD* buffer_size )
{
	std::vector<Utils::TapInfo> infos = Utils::GetAllTap( );
	if( infos.size( ) == 0 )
	{
		if( buffer_size > 0 )
		{
			*buffer_size = 1;
			buffer[0] = 0;
			return 1;
		}
		else
		{
			*buffer_size = 1;
			return 0;
		}
	}
	size_t total = infos.size( ) *  (infos[0].mGuid.size( )*2 + 2) + 2;
	if( *buffer_size >= total )
	{
		*buffer_size = total ;
		LPWSTR p = ( LPWSTR )buffer;
		for( DWORD a = 0; a < infos.size( ); a++ )
		{
			lstrcpy(p , infos[a].mGuid.c_str() );
			p += infos[a].mGuid.size( );
			p+=1;
		}
		p[1]=0;
		return 1;
	}
	else
	{
		*buffer_size = total ;
		return 0;
	}
	return 1;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

