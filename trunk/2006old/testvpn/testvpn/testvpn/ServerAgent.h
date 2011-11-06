#pragma once


class IServerAgentEvent
{
public:

};

class ServerAgent
{
public:
	std::wstring mServer;
	UDTSOCKET mSock;

public:
	BOOL Init( const TCHAR* server );
	void SetServer( const TCHAR* server );
	void PreLogin( );

	template<typename T>
	void SendCommand( const T* , DWORD callback )
	{
		
	}

public:
	ServerAgent( void );
	~ServerAgent( void );
};
