#include "StdAfx.h"




#include <set>

#include "VPNClient.h"
#include "vpnio.h"

namespace pyvpn
{
	namespace bp = boost::python;
	namespace io = boost::asio;
	
	Client::Client( const bp::object & o, const bp::str & s , const bp::tuple & server )
		:mSock( vpnio::instance( ), io::ip::udp::v4( ) )
		, mEvent( o ) 
		, mPacketSequence( 0 )
		, mCommandSequence( 0 )
		, mCmdCheck( vpnio::instance( ) )
	{
		mSock.bind( io::ip::udp::endpoint( io::ip::udp::v4(),  0 ) );
		
		std::string serveraddr = bp::extract<std::string>( server[0] );
		int serverport = bp::extract<int>( server[1] );
			
		io::ip::address_v4 addr = io::ip::address_v4::from_string( serveraddr );
		mServerAddr.address( addr );
		mServerAddr.port(  serverport );

		const char *key_ptr; int key_len = bp::len(s);
		key_ptr = bp::extract<const char *>(s );

		memcpy_s( &mKey, sizeof( mKey ), key_ptr, key_len );

	}

	Client::~Client( void )
	{

	}

	void Client::Login( const std::wstring & localip )
	{
		USES_CONVERSION;
		vpn_packet::Login l;
		io::ip::address_v4 addr;
		addr.from_string(  W2A( localip.c_str( ) ) );
		l.mLocalAddr = addr.to_ulong( );

		SendCmd( vpn_packet::_CMD_LOGIN , l );
	}

	void Client::HandleSendToResult( const boost::system::error_code & e, std::size_t len, std::string * send_s )
	{
		delete send_s;
		if( len == 0 )
		{
			printf( e.message( ).c_str( ) );
		}
	}

	void InitClient()
	{
		bp::class_<Client, boost::noncopyable>("Client", bp::init< const bp::object &, const bp::str &,const bp::tuple &>() )
			.def("login", &Client::Login );
			;
	}
}


