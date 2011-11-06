#pragma once



namespace vpn_server
{
	
	struct client
	{
		WORD mKey[32];
		std::wstring mLocalAddr;
	};
	
	
	
	
	namespace io = boost::asio;
	class server_imp
	{
	public:
		server_imp(void);
		~server_imp(void);

		io::io_service mService;

		io::ip::udp::socket mSock;

		void start( );
		void run( );

		void onLogin( const vpn_packet::Login & login, WORD* key );

		char mReadBuffer[2048];


		void handleDataRecv( const boost::system::error_code & e, std::size_t len );

	};

}

