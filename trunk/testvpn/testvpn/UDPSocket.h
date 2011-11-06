#pragma once

class UDPSocket : public IOCPHandler
{
public :
	UDPSocket(  );
	int SendTo( const stAddr& addr, LPBYTE buffer, DWORD length );
	void Start( );

	HANDLE GetHandle()
	{
		return (HANDLE)mSock;
	}

	virtual void OnDataRead( stBuffer* buf, const stAddr& from );
	virtual void OnDataWrite( stBuffer* buf, const stAddr& from );
	
private:
	SOCKET mSock;
};
