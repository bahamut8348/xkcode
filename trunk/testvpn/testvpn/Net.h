#pragma once

class Net
{
public:
	static Net* sInstance;
	static Net* Instance( );

public:

	BOOL SendUDP( const stAddr& addr, LPBYTE buffer, DWORD length );

	UDPSocket mUDPSock;

	Net(  );
	~Net( void );
};
