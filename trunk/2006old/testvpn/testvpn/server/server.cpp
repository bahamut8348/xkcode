// server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "pycfg.h"
#include "packet.h"

#include "server_imp.h"



int _tmain(int argc, _TCHAR* argv[ ] )
{
	int port = pycfg::CFG::instance( ).getServerPort();
	namespace io = boost::asio;
	
	using namespace vpn_server;

	server_imp ser;
	ser.start( );
	ser.run( );


	
	
	
	return 0;
}

