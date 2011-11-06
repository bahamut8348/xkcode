#pragma once


namespace pyvpn
{
	
	namespace bp = boost::python;
	namespace asio = boost::asio;
	struct Client
	{
	public:
		Client( const bp::object & o, const bp::str & s, const bp::tuple & server  );
		
		asio::ip::udp::socket mSock;
		asio::ip::udp::endpoint mServerAddr;

		bp::object mEvent;

		vpn_packet::UserKey mKey;

		stdext::hash_map<DWORD, DWORD> mCmdQ;

		asio::deadline_timer mCmdCheck;	

		~Client(void);

		DWORD mPacketSequence;
		DWORD mCommandSequence;

		template< typename CMD >
		void SendCmd( WORD cmd_no, CMD  cmd, DWORD cmd_seq = 0 )
		{
			if( cmd_seq == 0 )
			{
				cmd_seq = ++mCommandSequence;
				mCmdQ.insert( std::pair<DWORD, DWORD>( cmd_seq, 0 ) );
			}
			else
			{
				stdext::hash_map<DWORD, DWORD>::iterator iter = mCmdQ.find( cmd_seq );
				if(  iter == mCmdQ.end( ) )
				{
					return ;
				}
				else
				{
					iter->second ++;
				}
			}
			
			mPacketSequence ++;
			vpn_packet::Header   h;
			h.mCommand = cmd_no;
			h.mCommandSeq = cmd_seq;
			h.mFlag = 1;
			h.mSequence = mPacketSequence;
			h.mUserKey = mKey;

			std::ostringstream archive_stream;
			boost::archive::text_oarchive archive(archive_stream);
			archive << h << cmd;
			
			std::string * s =new std::string( archive_stream.str( ) );

			mSock.async_send_to( io::buffer(*s) , mServerAddr , boost::bind( &Client::HandleSendToResult, this, io::placeholders::error, io::placeholders::bytes_transferred, s) );

			mCmdCheck.expires_from_now( boost::posix_time::microseconds( 500 ) );
			mCmdCheck.async_wait(  boost::bind( &Client::SendCmd<CMD>, this, cmd_no, cmd, cmd_seq )  );

									
		}
		
		void HandleSendToResult( const boost::system::error_code & e, std::size_t len, std::string * send_s );

		
		void Login( const std::wstring & localip );

	};

	void InitClient( );

}


