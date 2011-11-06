#include "StdAfx.h"
#include <assert.h>
#include "../common/Common.h"
#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#include <boost/pool/object_pool.hpp>

#include "UDTManager.h"

using namespace UDT_Wrapper;

SocketManager::SocketManager( void )
{
	mCommonHandle = NULL ;
	mStop = false ;

	WSADATA data;
	WSAStartup( 0x0202, &data );
}




void SocketManager::SetSocketHandler( SOCKETHANDLE sock, ISocketEvent* handler )
{
	mSockMap[sock]->mEventHandler = handler;
}

SOCKETHANDLE SocketManager::CreateClientSocket( const stAddr& server, ISocketEvent* handler )
{
	struct addrinfo hints, *local = NULL; 

	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = _SOCKTYPE;

	SOCKADDR_IN sa = {0};
	sa.sin_addr.S_un.S_addr = server.mIP.mIP.mIPLong;
	sa.sin_family = 2;
	sa.sin_port = htons( server.mPort );
	
	char service[32];
	sprintf_s(service, "%d", server.mPort );
	if (0 != getaddrinfo( NULL, service , &hints, &local ) )
	{
		printf( "%s", "illegal port number or port is busy.\n" );
		return UDT::INVALID_SOCK;
	}

	int a = UDT::ERROR;
	SOCKETHANDLE sock = UDT::socket( local->ai_family, local->ai_socktype, local->ai_protocol);
	int ret = UDT::connect( sock, ( struct sockaddr* )&sa , sizeof(sa) );
	freeaddrinfo( local );

	if( ret == a )
	{
		return UDT::INVALID_SOCK; 
	}

	SetBlock( false, sock);
	RegisterSocket( sock, _SOCKET_TYPE_CLIENT , handler );

	return sock;
}

SOCKETHANDLE SocketManager::RegisterSocket( SOCKETHANDLE socket, SOCKET_TYPE type, ISocketEvent* handler )
{
	SocketInfo *pinfo = mSockInfoMgr.construct();
	memset( pinfo, 0, sizeof(SocketInfo));
	assert( mSockMap.find( socket) == mSockMap.end() );
	mSockMap[socket] = pinfo;
	pinfo->mEventHandler = handler;
	pinfo->mSock = socket;
	pinfo->mSocketType = type;
	mAllSet.insert( socket );
	return socket;
}

SOCKETHANDLE SocketManager::CreateServerSocket( const stAddr& server, ISocketEvent* handler )
{
	struct addrinfo hints, *local = NULL; 

	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = _SOCKTYPE;

	SOCKADDR_IN sa = {0};
	sa.sin_addr.S_un.S_addr = server.mIP.mIP.mIPLong;
	sa.sin_family = 2;
	sa.sin_port = htons( server.mPort );
	char service[32];
	sprintf_s(service, "%d", server.mPort );
	if (0 != getaddrinfo( NULL, service , &hints, &local ) )
	{
		printf( "%s", "illegal port number or port is busy.\n" );
		return UDT::INVALID_SOCK;
	}
	
	SOCKETHANDLE sock = UDT::socket( local->ai_family, local->ai_socktype, local->ai_protocol);
	freeaddrinfo( local );
	int ret = UDT::bind( sock , (sockaddr*)&sa, sizeof(sa));
	if( ret == UDT::ERROR )
	{
		return UDT::INVALID_SOCK;
	}
	ret = UDT::listen( sock, 5 );
	if( ret == UDT::ERROR )
	{
		return UDT::INVALID_SOCK;
	}

	RegisterSocket( sock, _SOCKET_TYPE_SERVER, handler );
	return sock;
}

bool SocketManager::SetBlock( bool bblock, SOCKETHANDLE socket )
{
	int value = bblock?1:0;
	UDT::setsockopt( socket, 0, UDT_SNDSYN,(void*)&value, sizeof(value) );
	return UDT::setsockopt( socket, 0, UDT_RCVSYN,(void*)(&value), sizeof(value) ) == 0;
}

int SocketManager::Send( SOCKETHANDLE sock, const void* buffer, DWORD length )
{
	SocketInfo* pinfo = mSockMap[ sock ];
	return UDT::sendmsg( sock, (const char*)buffer, length, -1, true );
}

int SocketManager::RunOnce( DWORD timeout)
{
	SOCKETHANDLE newclient;
	UDT::UDSET rs = mAllSet;
	UDT::UDSET ws;
	UDT::UDSET errs = mAllSet;
	
	UDT::UDSET *prs = &rs;
	

	sockaddr_storage clientaddr;
	int addrlen = sizeof(clientaddr);

	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = timeout;

	int number = UDT::select( 0, &rs, &ws, &errs, &tv );
	if( number == UDT::ERROR )
	{
		return UDT::ERROR;
	}
	if( number > 0 )
	{
		for( UDT::UDSET::iterator Iter = rs.begin(); Iter != rs.end(); Iter++ )
		{
			SOCKETHANDLE& sock = *Iter;
			SocketInfo  *pinfo = mSockMap[sock];
			if( pinfo->mSocketType == _SOCKET_TYPE_SERVER )
			{
				if (UDT::INVALID_SOCK == (newclient = UDT::accept(sock, (sockaddr*)&clientaddr, &addrlen)))
				{
					printf("accept: %s", UDT::getlasterror( ).getErrorMessage( ) );
					assert( 0 );
				}
				else
				{
					SetBlock( false, newclient);
					RegisterSocket( newclient, _SOCKET_TYPE_NEW,NULL );
					pinfo->mEventHandler->OnAccept( sock, newclient );
				}
			}
			else if( pinfo->mSocketType == _SOCKET_TYPE_CLIENT  || pinfo->mSocketType == _SOCKET_TYPE_NEW )
			{
				stBuffer* buffer = mBufferMgr.construct();
				buffer->Create();
				int ret = UDT::recvmsg( sock,( char* ) buffer->GetBuffer(), stBuffer::_MaxSize );
				if( ret > 0 )
				{
					pinfo->mEventHandler->OnDataRecv( sock, (LPCSTR)buffer->GetBuffer(), ret );	
				}
				else
				{
					pinfo->mEventHandler->OnDisconnect( sock );
					CloseSocket( sock );
					if( errs.find(sock) != errs.end() )
					{
						errs.erase( sock );
					}
				}
				mBufferMgr.destroy( buffer );
			}
			else
			{
				assert(0);
			}
		}

		for( UDT::UDSET::iterator Iter = errs.begin(); Iter != errs.end(); Iter++ )
		{
			UDTSOCKET& sock = *Iter;
			SocketInfo  *pinfo = GetSocketInfo( sock );
			if( pinfo )
			{
				if( pinfo->mEventHandler )
					pinfo->mEventHandler->OnDisconnect( sock );
			}
			
			CloseSocket( sock );
		}
	}
	return 1;
}

int SocketManager::Run()
{
	while ( mStop == false )
	{
		if( UDT::ERROR == RunOnce( 200 ) ) return UDT::ERROR ;
	}
	return 1;
}

bool SocketManager::UnregisterSocket( UDTSOCKET socket )
{
	SocketInfo* pinfo = GetSocketInfo( socket );
	assert(pinfo);
	if( pinfo )
	{
		mSockMap.erase( socket );
		mSockInfoMgr.destroy( pinfo );
		mAllSet.erase( socket );
	}
	
	return true;
}

DWORD SocketManager::GetSocketUserData( UDTSOCKET socket ) const
{
	const SocketInfo* pinfo = GetSocketInfo( socket );
	return pinfo->mUserData;
}

DWORD SocketManager::GetSocketUserData2( UDTSOCKET socket ) const
{
	const SocketInfo* pinfo = GetSocketInfo( socket );
	return pinfo->mUserData2;
}

void SocketManager::SetSocketUserData( UDTSOCKET socket , DWORD userdata )
{
	SocketInfo* pinfo = GetSocketInfo( socket );
	pinfo->mUserData = userdata ;
}

void SocketManager::SetSocketUserData2( UDTSOCKET socket , DWORD userdata )
{
	SocketInfo* pinfo = mSockMap[ socket ];
	pinfo->mUserData2 = userdata ;
}

const SocketManager::SocketInfo* SocketManager::GetSocketInfo( UDTSOCKET socket ) const
{
	SocketMap::const_iterator iter = mSockMap.find(socket);
	if( iter != mSockMap.end() )
	{
		SocketInfo *pinfo = iter->second;
		return pinfo;
	}
	return 0;
}

SocketManager::SocketInfo* SocketManager::GetSocketInfo( UDTSOCKET socket )
{
	SocketMap::iterator iter = mSockMap.find(socket);
	if( iter != mSockMap.end() )
	{
		SocketInfo *pinfo = iter->second;
		return pinfo;
	}
	return 0;
}

SocketManager::~SocketManager( void )
{
	
}

bool SocketManager::GetSocketName( UDTSOCKET sock, stAddr & addr )
{
	SOCKADDR_IN sa;
	int len = sizeof(sa);
	if( UDT::ERROR == UDT::getsockname( sock,(sockaddr*)&sa, &len ) )
		return false;
	addr.mIP.mIP.mIPLong = sa.sin_addr.S_un.S_addr;
	addr.mPort = ntohs(sa.sin_port);
	return true;
}

bool SocketManager::GetSocketPeerName(UDTSOCKET sock, stAddr & addr )
{
	SOCKADDR_IN sa;
	int len = sizeof(sa);
	if( UDT::ERROR == UDT::getpeername( sock, (sockaddr*)&sa, &len ) )
		return false;
	addr.mIP.mIP.mIPLong = sa.sin_addr.S_un.S_addr;
	addr.mPort = ntohs( sa.sin_port );
	return true;
}

void SocketManager::CloseSocket( UDTSOCKET sock )
{
	UnregisterSocket( sock );
	UDT::close( sock );
}