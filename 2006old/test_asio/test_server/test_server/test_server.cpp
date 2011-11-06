// test_server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>


using namespace boost;


class Socket : public asio::ip::tcp::socket, public boost::enable_shared_from_this<Socket>

{
public : Socket( asio::io_service & io ):asio::ip::tcp::socket( io ){}
		 ~Socket()
		 {
				printf("--- socket release\n" );
		 }
}
;

typedef shared_ptr<Socket> SocketPtr;

class Server
{

public:
	int port;
	
	asio::ip::tcp::acceptor _acceptor;
	
	boost::array<char, 1024000> recv_buffer_;
	
	void start_accept( )
	{
		SocketPtr sock(new Socket(_acceptor.io_service()));
		_acceptor.async_accept( *sock ,  boost::bind(&Server::handle_accept, this, sock, boost::asio::placeholders::error));
	}

	void start_recv( SocketPtr & sock )
	{
		asio::async_read( *sock,  asio::buffer(recv_buffer_), boost::bind(&Server::handle_data, this,sock, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred ));
	}

	void handle_data( SocketPtr sockptr, const boost::system::error_code & err, size_t trans_length )
	{
		// printf("+++ data from %s,length: %d\n", sockptr->remote_endpoint().address().to_string().c_str(), trans_length );
		if (trans_length == 0)
		{
			printf("--- Disconnect %s\n", sockptr->remote_endpoint().address().to_string().c_str() );
		}
		else
		{
			start_recv( sockptr );
		}
	}

	void handle_accept( SocketPtr sockptr, const boost::system::error_code & err)
	{
		start_recv( sockptr );
		start_accept( );
		printf("+++ %s\n", sockptr->remote_endpoint().address().to_string().c_str() );
	}


	Server( int _port, asio::io_service & io ):
	  port( _port ),
	  _acceptor( io, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), _port ))
	  {
			
	  }
};

int _tmain(int argc, _TCHAR* argv[])
{

	int port = _wtoi( argv[1] );
	asio::io_service io;
	Server server( port, io );
	server.start_accept();

	try{
		io.run();
	}
	catch( ...)
	{
		printf("error" );
		
	}

	return 0;
}

