// xktest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "Packetdef.h"
#include "IEvent.h"
#include <conio.h>
#include "XKQQ.h"



void SendChat( XKQQ * qq, asio::deadline_timer * t )
{
	static dword a = 0;
	char buffer[10];
	sprintf( buffer, "%d ",a++);
	qq->SendChat( 8389527 ,buffer );
	
	boost::asio::strand strand( XKQQ::g_IO );
	t->expires_from_now( boost::posix_time::seconds(3) );
	t->async_wait(strand.wrap(boost::bind(SendChat, qq, t )));
}

class xEvent : public IEvent
{
public:
	XKQQ *mqq;
	asio::deadline_timer * mptimer;
	void OnToken( dword result , const stQQToken & token )
	{
		mqq->SendLogin( "3281044p" );
	}

	void OnLog( const std::string& s)
	{
		printf("%s\n", s.c_str() );
	}

	void OnLoginResult( dword result, const std::string & s)
	{
		if(result != 0)
		{
			printf("Login Error: %d, %s\n", result, s.c_str());
		}
		else
		{
			SendChat( mqq, mptimer );
		}
	}

	void OnFriendMessage( dword sender, const std::string& msg )
	{
		if( msg == "g")
		{
			mqq->SendGetBuddyList( 0 );
		}
		printf("%u: %s\n",sender, msg.c_str() );
	}
};




int _tmain(int argc, _TCHAR* argv[])
{
	
	byte buf[] = "\x00\x80\x03\x97\x01\x12\xf8\x94\xbe\x5b\xc8\x7e\x31\x19\xcc\x6e\xbd\xcb\xe7\xa5\xce\x3f\x59\xb0\x00\x0b\x1a\xd5\x48\x25\x36\xf4\x00\x00\x00\x00\x00\x01\x01\x00\xcd\x00\x01\x32\x32\x32\x32\x32\x32\x32\x32\x32\x32\x32\x32\x32\x32\x32\x20\x00\x0c\x00\x00\xff\x00\x86\x02\xcb\xce\xcc\xe5\x0d";



	stIMPacketRes res;
	res.FromBuffer( buf, sizeof( buf ) -1 );

	dword sender = res.mSender;


	
	XKQQ qq;
	xEvent et;
	et.mqq = &qq;
	qq.mpEvent = &et;
	et.mptimer  = new asio::deadline_timer( XKQQ::g_IO);
	qq.SetServer( "58.251.63.57", 8000 );
	qq.mQQnum = 18020500;
	//qq.SendChat( 8389527 ,"test ");
	qq.SendTokenRequest( );
	
	while (1)
	{
		XKQQ::UpDate();
	}
	
	
	return 0;
}

