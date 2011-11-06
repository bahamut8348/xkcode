#include "StdAfx.h"

#include "UDPSocket.h"
#include "Net.h"


Net* Net::sInstance = NULL;

Net::Net(  )
{
	sInstance = this ;
	mUDPSock.Start();
}

Net::~Net( void )
{
	
}

Net* Net::Instance( )
{
	assert( sInstance );
	return sInstance;
}

BOOL Net::SendUDP( const stAddr& addr, LPBYTE buffer, DWORD length )
{
	return mUDPSock.SendTo( addr, buffer, length );
}