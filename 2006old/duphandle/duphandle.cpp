// duphandle.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <ace\INET_Addr.h>
#include <ace\Sock_Connector.h>
#include <ace\SOCK_Stream.h>
#include <ace\SPIPE.h>
#include <ace\SPIPE_Acceptor.h>
#include <ace\SPIPE_Connector.h>
#include <ace\Pipe.h>
#include <ace\UPIPE_Acceptor.h>
#include <ace\OS.h>

#include <atlbase.h>

struct Packet_Head
{
	DWORD mVersion;
	DWORD mCommand;
	Packet_Head()
	{
		mVersion = 1;
	}
};

template< typename T >
struct Packet_T : public Packet_Head
{
	Packet_T()
	{
		mCommand = T::_COMMAND_ID;
	}
};

struct Packet_RequestSocketHandle : public Packet_T<Packet_RequestSocketHandle>
{
	enum{
		_COMMAND_ID = 0x01,

	};
	DWORD mSubProcessID;
};

struct Packet_ResponseSocketHandle : public Packet_T< Packet_ResponseSocketHandle >
{
	
	enum
	{
		_COMMAND_ID = 0x02,
	};
	ACE_HANDLE mFileHandle;
};


int ACE_MAIN(int argc, _TCHAR* argv[])
{
	
	ACE_DEBUG( (LM_INFO,L"Start\n")  );

	ACE_OS::socket_init();
	BOOL	bMainProcess = FALSE;

	// get a exist pipe
	ACE_SPIPE_Connector pipe_connector;
	ACE_SPIPE_Addr pipe_addr(L"mypipe");
	ACE_SPIPE_Stream pipe_stream;
	ACE_SPIPE_Acceptor pipe_acceptor;


	ACE_SOCK_CONNECTOR sock_connector;
	ACE_SOCK_STREAM sock_stream;
	ACE_INET_Addr	server_addr;

	if( pipe_connector.connect(pipe_stream, pipe_addr) == -1 ) 
	{
		// failed , new acceptor
		bMainProcess = TRUE;
		pipe_acceptor.open( pipe_addr );

	}
	else{
		// processor already exist
		Packet_RequestSocketHandle packet;
		packet.mSubProcessID = ACE_OS::getpid();
		pipe_stream.send(&packet, sizeof(packet));
		
		Packet_ResponseSocketHandle res;
		pipe_stream.recv(&res, sizeof res);
		sock_stream.set_handle( res.mFileHandle );
	}

	if ( bMainProcess )
	{
		server_addr.set( 9000, "localhost" );
		if( sock_connector.connect(sock_stream, server_addr) == -1 )
		{
			ACE_DEBUG( (LM_DEBUG, L"Error: %m\n"));
			return 1;
		}
		
	}
	{
		// send 
		ACE_TCHAR *str = L"I'm the first\n";
		if ( sock_stream.send( str, ACE_OS::strlen( str ) * 2 ) == -1 )
		{
			ACE_DEBUG( (LM_DEBUG, "send Error: Handle: %d, %m\n", sock_stream.get_handle()));
			return 1;
		}

	}
	
	
	
	
	

	while( 1 )
	{
		ACE_Time_Value tv(0,10);
		ACE_OS::sleep(tv);

		if(bMainProcess)
		{
			ACE_SPIPE_Stream pipe_stream;
			if( pipe_acceptor.accept(pipe_stream, 0,&tv) == 0)
			{
				// ok
				Packet_RequestSocketHandle packet;
				pipe_stream.recv( &packet, sizeof(packet) );
				Packet_ResponseSocketHandle res;
				ACE_HANDLE proc_handle = ::OpenProcess( PROCESS_ALL_ACCESS , TRUE, packet.mSubProcessID );
				ACE_HANDLE new_handle;
				::DuplicateHandle( ::GetCurrentProcess(), sock_stream.get_handle(),proc_handle, &new_handle, 0, FALSE, DUPLICATE_SAME_ACCESS  );
				//ACE_OS::dup( sock_stream.get_handle(),  new_handle);
				res.mFileHandle = new_handle;
				
				ACE_OS::close( proc_handle );
				
				pipe_stream.send( &res, sizeof res );

			}
		}


		{
			USES_CONVERSION;		
			// socket receive
			CHAR msg[128] = {0};
			tv.set(0, 10);
			
			if(  sock_stream.recv( msg, 128, &tv) > 0 )
			{
				ACE_DEBUG( (LM_INFO, L"Recv: %s",  A2W(msg) ));
			}
			else
			{
				
			}
			
		}
		
		
	}

	ACE_DEBUG( ( LM_INFO, L"End\n" ));
	
	return 0;
}

