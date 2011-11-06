#include "StdAfx.h"

#include "vpnio.h"
#include "VPN.h"

namespace pyvpn
{
	inline void set_winerr( int err = 0 )
	{
		PyErr_SetFromWindowsErr( GetLastError( ) );
	}


	VPN::VPN( const std::wstring & guid, const bp::object & o , bp::object  key )
		: mService( vpnio::instance() )
		, mHandle( mService )

	{
		TCHAR path[1024];
		swprintf_s( path, 1024, L"\\\\.\\Global\\%s.tap", guid.c_str( ) );
		mWinHandle = CreateFile ( path, GENERIC_READ | GENERIC_WRITE, 0, /* was: FILE_SHARE_READ */ 0, OPEN_EXISTING,FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_OVERLAPPED, 0 );
		if( mWinHandle == INVALID_HANDLE_VALUE )
		{
			set_winerr();
			bp::throw_error_already_set( );
		}
		mDataCallback = o;
		mDataCallbackKey = key;

		mHandle.assign( mWinHandle );


	}

	VPN::~VPN( void )
	{
		if( mWinHandle != INVALID_HANDLE_VALUE )
		{
			if( ! CloseHandle( mWinHandle ) )
			{
				set_winerr();
				bp::throw_error_already_set();
			}
		}
	}

	void VPN::SetConnect( bool bconnect )
	{
		DWORD 	c = bconnect;
		if( ! DoCommand( TAP_IOCTL_SET_MEDIA_STATUS, c ) )
		{
			set_winerr( );
			bp::throw_error_already_set( );
		}
	}

	void VPN::SetIP( const std::wstring & localip, const std::wstring & localnetmask /*= "255.255.255.0"*/ )
	{
		struct ConfigDHCP
		{
			DWORD mLocal;
			DWORD mAdaptNetmask;
			DWORD mRemoteNetmask;
			DWORD mDHCPLeaseTime;
		};

		USES_CONVERSION;

		ConfigDHCP dhcp;
		dhcp.mLocal = htonl( io::ip::address_v4::from_string(  W2A( localip.c_str( ) ) ).to_ulong( ) );
		dhcp.mAdaptNetmask = htonl( io::ip::address_v4::from_string(  W2A( localnetmask.c_str( ) ) ).to_ulong( ) );
		dhcp.mRemoteNetmask = dhcp.mLocal & dhcp.mAdaptNetmask;
		dhcp.mDHCPLeaseTime = 3651000;

		if( ! DoCommand( TAP_IOCTL_CONFIG_DHCP_MASQ, dhcp ) )
		{
			set_winerr( );
			bp::throw_error_already_set( );
		}

	}

	boost::python::tuple VPN::GetVersion()
	{
		struct CommandVersion
		{
			enum
			{
				_CMD = TAP_IOCTL_GET_VERSION,
			};
			ULONG mMinMajor;
			ULONG mMInManor;
			ULONG mbDebug;
		};

		CommandVersion ver;

		if( ! DoCommand( CommandVersion::_CMD, ver ) )
		{
			set_winerr( );
			bp::throw_error_already_set( );
		}

		return bp::make_tuple( ver.mMinMajor, ver.mMInManor, (bool)ver.mbDebug );

	}

	void VPN::Read()
	{
		const io::mutable_buffer read_buf = io::buffer( mReadBuffer );
		mHandle.async_read_some( io::buffer( read_buf ) , boost::bind( &VPN::HandleRead, this, io::placeholders::error, io::placeholders::bytes_transferred ) );
	}

	void VPN::HandleRead( const  boost::system::error_code & err, std::size_t translen )
	{
		bp::call<void>( mDataCallback.ptr( ), bp::str( mReadBuffer, translen ), mDataCallbackKey );
	}

	void VPN::HandleWrite( const  boost::system::error_code & err, std::size_t translen )
	{
		if( translen <= 0 )
		{
			PyErr_SetFromWindowsErr( err.value( ) );
			bp::throw_error_already_set( );
		}
	}

	void VPN::Write( const bp::str & s )
	{
		char * s_ptr = 0;
		int s_len = bp::len( s );
		s_ptr = bp::extract<char *>( s );
		mHandle.async_write_some( io::buffer( mWriteBuffer, s_len  ) , boost::bind( &VPN::HandleRead, this, io::placeholders::error, io::placeholders::bytes_transferred ) );
	}

	ULONG VPN::GetMTU()
	{
		ULONG mtu;
		if( ! DoCommand( TAP_IOCTL_GET_MTU, mtu ) )
		{
			set_winerr( );
			bp::throw_error_already_set( );
		}
		return mtu;
	}
	void InitVPN()
	{
		bp::class_< VPN, boost::noncopyable >( "VPN", bp::init<const std::wstring &, const bp::object &,bp::optional< bp::object > >( ) )
			.def("connect", &VPN::SetConnect  )
			.def("setip", &VPN::SetIP )
			.def("read", &VPN::Read )
			.def("write", &VPN::Write )
			.add_property( "mtu", &VPN::GetMTU )
			;
	}

}

