#include "StdAfx.h"
#include "VPNServer.h"

VPNServer::VPNServer(void)
{
}

VPNServer::~VPNServer(void)
{
}

UDTSOCKET VPNServer::CreateServer( u_short port )
{
	UDTSOCKET server;

	addrinfo hints;
	addrinfo* res;

	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	//hints.ai_socktype = SOCK_DGRAM;

	char service[32];

	server = UDT::socket( AF_INET, SOCK_DGRAM, 0 );
	sprintf_s(service, "%d", port );
	if (0 != getaddrinfo( NULL, service , &hints, &res ) )
	{
		printf( "%s", "illegal port number or port is busy.\n" );
		return 0;
	}
	if ( UDT::ERROR == UDT::bind( server, res->ai_addr,(int) res->ai_addrlen ) )
	{
		printf( "bind: %s\n", UDT::getlasterror().getErrorMessage() );
		return 0;
	}

	if ( UDT::ERROR == UDT::listen( server, 10 ) )
	{
		printf( "listen: %s\n", UDT::getlasterror().getErrorMessage() );
		return 0;
	}

	return server;
}

void VPNServer::Init()
{
	UDTSOCKET mPreServer = CreateServer( _PreServer_Port );
	UDTSOCKET mServer = CreateServer( _Server_Port );

	AddSocket( mPreServer );
	AddSocket( mServer );

	return;
}

void VPNServer::Update()
{
	UDTSOCKET newclient;
	UDT::UDSET rs = mAllrs;

	sockaddr_storage clientaddr;
	int addrlen = sizeof(clientaddr);

	timeval tv;
	tv.tv_sec = 10;
	tv.tv_usec = 0;

	int number = UDT::select( 0, &rs, NULL, NULL, &tv );
	if( number > 0 )
	{
		for( UDT::UDSET::iterator Iter = rs.begin(); Iter != rs.end(); Iter++ )
		{
			UDTSOCKET& sock = *Iter;
			if( sock == mPreServer || sock == mServer )
			{
				if (UDT::INVALID_SOCK == (newclient = UDT::accept(sock, (sockaddr*)&clientaddr, &addrlen)))
				{
					printf("accept: %s", UDT::getlasterror().getErrorMessage() );
					return;
				}
				else
				{

				}
			}
		}
	}
}

void VPNServer::AddClient( UDTSOCKET sock )
{
	Client* client =NULL;
	ClientMap::iterator Iter = mClientMap.find( sock );
	if( Iter == mClientMap.end( ) )
	{
		client = new Client;
		mClientMap[ sock ] = client;
	}
}