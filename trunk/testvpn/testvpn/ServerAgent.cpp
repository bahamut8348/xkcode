#include "StdAfx.h"
#include "ServerAgent.h"

ServerAgent::ServerAgent( void )
{
}

ServerAgent::~ServerAgent( void )
{
}

void ServerAgent::SetServer( const TCHAR* server )
{
	mServer = server;
}

void ServerAgent::PreLogin( )
{
	
}

BOOL ServerAgent::Init( const TCHAR* server )
{
	SetServer( server );

	mSock = UDT::socket( AF_INET, SOCK_DGRAM, 0 );
	struct addrinfo hints,  *peer;

	char temp[32];
	sprintf_s(temp, "%d", _PreServer_Port );
	USES_CONVERSION;
	if ( 0 != getaddrinfo( W2A( mServer.c_str() ), temp, &hints, &peer ) )
	{
		wprintf(L"incorrect server/peer address. %s:%d\n", server, _PreServer_Port );
		assert( 0 );
		return 0;
	}

	return TRUE;
}



