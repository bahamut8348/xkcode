#include "StdAfx.h"

#include "UDPSocket.h"
#include "Device.h"
#include "DeviceManager.h"

UDPSocket::UDPSocket(  )
{
	mSock = WSASocket( AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, NULL, WSA_FLAG_OVERLAPPED );
	SOCKADDR_IN sa= {0};
	sa.sin_port = htons( _Listen_Port );
	sa.sin_family = 2;
	::bind( mSock, (sockaddr*)&sa, sizeof(sa) );
	IOCP::Instance()->AddHandler( this );
}

void UDPSocket::Start( )
{
	stBuffer* buf = BufferMgr::Instance()->CreateObject();
	buf->Create();
	IOCP::Instance()->RecvFrom( this, buf );
}

int UDPSocket::SendTo( const stAddr& addr, LPBYTE buffer, DWORD length )
{
	stBuffer* buf = BufferMgr::Instance()->CreateObject();
	buf->Create();
	buf->Set( buffer, length );
	return IOCP::Instance()->SendTo( this, buf, addr );
}

void UDPSocket::OnDataRead( stBuffer* buf, const stAddr& from )
{
	Start();
	if( buf->GetLength() )
	{
		DeviceManager::Instance()->Write( 0, buf->GetBuffer(), buf->GetLength() );
	}
	BufferMgr::Instance()->ReleaseObject( buf );
}

void UDPSocket::OnDataWrite( stBuffer* buf, const stAddr& from )
{
	BufferMgr::Instance()->ReleaseObject( buf );
}