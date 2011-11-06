#include "StdAfx.h"
#include "Utils.h"
#include "IOCP.h"
#include "Device.h"

#include "UDPSocket.h"
#include "Net.h"

Device::Device(void)
{
	mOperNumber = 0;
}

Device::~Device(void)
{
	CloseHandle( mHandle );
	mReadBuf.Delete( );
}

BOOL Device::Create( LPCTSTR guid )
{
	
	TCHAR path[1024];
	swprintf_s( path, 1024, L"\\\\.\\Global\\%s.tap", guid );
	mHandle = CreateFile ( path, GENERIC_READ | GENERIC_WRITE,
		0, /* was: FILE_SHARE_READ */
		0, OPEN_EXISTING,FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_OVERLAPPED, 0 );
	
	if( mHandle != INVALID_HANDLE_VALUE )
	{
		std::vector<Utils::PanelReg> tapinfolist = Utils::GetPanelReg();
		for( DWORD a=0; a< tapinfolist.size(); a++ )
		{
			if( tapinfolist[a].mGuid == guid )
			{
				mName = tapinfolist[a].mName;
				break;
			}
		}

		mReadBuf.Create( );
	}

	return mHandle != INVALID_HANDLE_VALUE ;
	
}

BOOL Device::GetVersion( ULONG & major, ULONG& manor, ULONG& debug )
{
	stCommandVersion ver;
	BOOL ret = DoCommand( ver );
	major = ver.mMinMajor;
	manor = ver.mMInManor;
	debug = ver.mbDebug;
	return ret;
}

BOOL Device::SetIPAddr( LPCTSTR localip, LPCTSTR localNetmask, LPCTSTR remoteNetmask )
{
	mLocalIP.fromString( localip );
	stCommandDHCP dhcp;
	dhcp.mAdaptNetmask.fromString( localNetmask );
	dhcp.mLocal = localip;
	if( lstrlen(remoteNetmask) == 0 )
	{
		dhcp.mRemoteNetmask.mIP.mIPLong = dhcp.mLocal.mIP.mIPLong & dhcp.mAdaptNetmask.mIP.mIPLong;
	}
	else
	{
		dhcp.mRemoteNetmask = remoteNetmask;
	}
	dhcp.mDHCPLeaseTime.mTime = 3651000;
	return DoCommand( dhcp );
}

BOOL Device::SetConnect( BOOL bConnect )
{
	stCommandStatus status;
	status.mbConnect = bConnect;
	return DoCommand( status );
}

HANDLE Device::GetHandle() 
{
	return mHandle;
}

void Device::OnDataRead( stBuffer* buf )
{
	mOperNumber --;
	StartRecv();
	
	stIP from, to;
	BOOL bret = ParseBuffer( buf->GetBuffer(), from, to );
	if( bret == FALSE )
	{
		
	}
	else
	{
#ifdef LOG
		wprintf( L"Data From: %s, To: %s\r\n", from.toString().c_str(), to.toString().c_str() );
#endif
		if( to.toString() == L"10.8.1.6" )
		{
			stAddr addr;
			addr.mIP.fromString(L"192.168.1.65");
			addr.mPort = _Listen_Port;
			Net::Instance()->SendUDP( addr, buf->GetBuffer(), buf->GetLength() );
		}
		else if( to.toString() == L"10.8.1.5" )
		{
			stAddr addr;
			addr.mIP.fromString(L"192.168.1.128");
			addr.mPort = _Listen_Port;
			Net::Instance()->SendUDP( addr, buf->GetBuffer(), buf->GetLength() );
		}
	}

	BufferMgr::Instance()->ReleaseObject( buf );
	
}

void Device::OnDataWrite( stBuffer* buf )
{
	BufferMgr::Instance()->ReleaseObject( buf );
}

VOID Device::StartRecv()
{
	mOperNumber++;
	stBuffer *buffer = BufferMgr::Instance()->CreateObject();
	buffer->Create();
	IOCP::Instance()->ReadFile( this, buffer );
}