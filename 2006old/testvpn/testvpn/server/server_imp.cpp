#include "StdAfx.h"

#include "packet.h"
#include "server_imp.h"
#include "pycfg.h"


namespace vpn_server
{
	namespace io = boost::asio;

	server_imp::server_imp( void ):
	mSock( mService )
	{
		int port = pycfg::CFG::instance().getServerPort( );

		mSock.open( io::ip::udp::v4());
		mSock.bind( io::ip::udp::endpoint( io::ip::udp::v4(),  port ) );
	}

	server_imp::~server_imp(void)
	{
	}

	void server_imp::start( )
	{
		
		static io::ip::udp::endpoint endpoint;
		mSock.async_receive_from( io::buffer(mReadBuffer), endpoint, boost::bind(&server_imp::handleDataRecv, this, io::placeholders::error, io::placeholders::bytes_transferred ) );
	}

	void server_imp::handleDataRecv( const boost::system::error_code & e, std::size_t  len )
	{
		std::string archive_data( mReadBuffer, len );
		std::istringstream archive_stream(archive_data, std::ios_base::binary | std::ios_base::in );
		boost::archive::text_iarchive ar( archive_stream  );
		ar.load_binary( mReadBuffer, len );
		vpn_packet::Header header;
		ar >> header;

		WORD* userkey = (WORD*)&header.mUserKey;
	
		switch( header.mCommand )
		{
		case  vpn_packet::_CMD_LOGIN:
			   {
				   vpn_packet::Login login;
				   ar >> login;
				   onLogin( login , userkey );
			   }
		default:
			printf("handleDataRecv, Error CMD: %d\r\n" , header.mCommand );
		}
		
		start( );

		return ;
	}

	void server_imp::run( )
	{
		mService.run( );
	}

	void server_imp::onLogin( const vpn_packet::Login & login, WORD* key )
	{
		printf("login, %s\r\n" , io::ip::address_v4( login.mLocalAddr ).to_string().c_str( ) );
	}
}


