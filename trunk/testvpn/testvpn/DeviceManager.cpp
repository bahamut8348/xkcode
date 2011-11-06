#include "StdAfx.h"
#include "IOCP.h"
#include "Device.h"
#include "DeviceManager.h"


DeviceManager* DeviceManager::s_instance = NULL;

DeviceManager::DeviceManager( void )
{
}

DeviceManager::~DeviceManager( void )
{
}

DeviceManager* DeviceManager::Instance( )
{
	if( s_instance == NULL )
	{
		s_instance = new DeviceManager;
	}
	return s_instance;
}

BOOL DeviceManager::AddDevice( Device* dev )
{
	mDevs.push_back( dev );
	IOCP::Instance()->AddHandler( dev );
	return TRUE;
}

Device* DeviceManager::CreateDevice( LPCTSTR guid )
{
	Device* dev = new Device;
	if( TRUE == dev->Create( guid ) )
	{
		AddDevice(dev);
		return dev;
	}
	delete dev;
	return NULL;
}

BOOL DeviceManager::Write( const stIP& ip, LPBYTE buffer, DWORD length )
{
	Device* dev = GetDeviceFromIP( ip );
	DWORD write_length = 0;
	stBuffer* pbuf = BufferMgr::Instance()->CreateObject();
	pbuf->Create( );
	pbuf->Set( buffer, length );
	BOOL bret = IOCP::Instance()->WriteFile( dev, pbuf );
	assert( bret );
	return bret;
}

Device* DeviceManager::GetDeviceFromIP( const stIP& ip )
{
	return mDevs[0];
}