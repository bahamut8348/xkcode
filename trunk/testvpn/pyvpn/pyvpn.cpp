// pyvpn.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <tchar.h>
#include "../testvpn/Utils.h"

#include "vpnio.h"
#include "VPN.h"
#include "VPNClient.h"

namespace bp = boost::python;
namespace io = boost::asio;

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

static DWORD get_version( )
{
	return 1;
}

static bp::list get_taps( )
{
	bp::list l;
	std::vector<Utils::TapInfo> taps =  Utils::GetAllTap();
	for( u_int a = 0; a < taps.size( ); a ++ )
	{
		l.append( taps[a].mGuid );
	}
	
	return l;
}

static void run( )
{
	vpnio::run( );
}

BOOST_PYTHON_MODULE( pyvpn )
{
	bp::def("version", get_version );
	bp::def("taps", get_taps );
	bp::def("run", run );

	pyvpn::InitVPN( );
	pyvpn::InitClient( );
}

