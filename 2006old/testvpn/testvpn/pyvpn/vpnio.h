#pragma once

class vpnio
{
public:
	vpnio(void);
	~vpnio(void);
	
	static boost::asio::io_service & instance( )
	{
		static boost::asio::io_service i;
		return i;
	}

	static void run( )
	{
		instance().run_one( );
	}

};
