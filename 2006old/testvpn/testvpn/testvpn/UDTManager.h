#pragma once

typedef int SOCKETHANDLE;

namespace UDT_Wrapper
{



class ISocketEvent
{
public :
	virtual void OnDataRecv( SOCKETHANDLE sock,  LPCSTR data, DWORD length ) = 0;
	virtual void OnConnect(SOCKETHANDLE sock,  bool bSuccess ) = 0;
	virtual void OnDisconnect( SOCKETHANDLE sock ) = 0;
	virtual void OnAccept(SOCKETHANDLE sock,  SOCKETHANDLE  newsock ) = 0;
};

class SocketManager : public SingletonT< SocketManager >
{

	enum SOCKET_TYPE
	{
		_SOCKET_TYPE_CLIENT,
		_SOCKET_TYPE_SERVER,
		_SOCKET_TYPE_NEW,
	};

	enum SOCKTYPE
	{
		_SOCKTYPE = SOCK_DGRAM,
	};
	
	struct SocketInfo
	{
		SocketInfo(){}
		ISocketEvent*	mEventHandler;
		SOCKETHANDLE		mSock;
		SOCKET_TYPE		mSocketType;
		DWORD			mUserData;
		DWORD			mUserData2;
	};
	
	boost::object_pool<SocketInfo> mSockInfoMgr;
	boost::object_pool<stBuffer> mBufferMgr;
	UDT::UDSET mAllSet;

	ISocketEvent* mCommonHandle;
	bool mStop;
	typedef stdext::hash_map< SOCKETHANDLE, SocketInfo* > SocketMap;

	SocketMap mSockMap;
public:
	SocketManager(void);
	~SocketManager(void);

	SOCKETHANDLE CreateClientSocket( const stAddr& server, ISocketEvent* handler );
	SOCKETHANDLE CreateServerSocket( const stAddr& server, ISocketEvent* handler );
	SOCKETHANDLE RegisterSocket( SOCKETHANDLE socket, SOCKET_TYPE type, ISocketEvent* handler);
	void SetSocketUserData( SOCKETHANDLE socket , DWORD userdata );
	void SetSocketUserData2( SOCKETHANDLE socket , DWORD userdata );

	DWORD GetSocketUserData( SOCKETHANDLE socket ) const;
	DWORD GetSocketUserData2( SOCKETHANDLE socket ) const;

	const SocketInfo* GetSocketInfo( SOCKETHANDLE socket ) const;
	SocketInfo* GetSocketInfo( SOCKETHANDLE socket );
	
	bool UnregisterSocket( SOCKETHANDLE socket );
	bool SetBlock( bool bblock, SOCKETHANDLE socket );

	bool GetSocketName( SOCKETHANDLE sock, stAddr & addr );
	bool GetSocketPeerName( SOCKETHANDLE sock, stAddr & addr );
	
	int Send( SOCKETHANDLE sock, const void* buffer, DWORD length );

	void SetSocketHandler( SOCKETHANDLE sock, ISocketEvent* handler );
	void CloseSocket( SOCKETHANDLE sock );

	void Stop(){ mStop = true; }

	int RunOnce(DWORD timeout = 10000);
	int Run();
};
}