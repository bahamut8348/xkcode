
#ifndef _NETMANAGER_H_
#define _NETMANAGER_H_

typedef void (*SERVER_CB_ONCONNECT) ( int );
typedef void (*CLIENT_CB_ONCONNECT) ( int );
typedef void (*SERVER_CB_ONDATA)( int, char* , int );
typedef void(*SERVER_CB_ONDISCONNECT)( int );
typedef void (*SERVER_CB_ONPACKET)(int , char *);
typedef void (*SERVER_CB_ONIDLE)();

struct SockHandle;

class NetManager
{
	public:
	NetManager();
	~NetManager();
	
	char *mRecvBuffer;
	int mRecvBufferLen;

	int mEpoll; //epoll handle
	struct	aeEventLoop *mEventLoop;
	typedef __gnu_cxx::hash_set<int> IntSet;
	IntSet mServerSet;
	SERVER_CB_ONCONNECT mServerCallback_OnConnect;
	SERVER_CB_ONCONNECT mClientCallback_OnConnect;  // for connect 
	SERVER_CB_ONCONNECT mClientCallback_OnConnectFailed;
	SERVER_CB_ONDATA mServerCallback_OnData;
	SERVER_CB_ONPACKET mServerCallback_OnPacket;
	SERVER_CB_ONDISCONNECT mServerCallback_OnDisconnect;
	SERVER_CB_ONIDLE mServerCallback_OnIdle;

	SERVER_CB_ONDATA mManServerCallback_OnData;
	SERVER_CB_ONCONNECT mManServerCallback_OnConnect;
	SERVER_CB_ONDISCONNECT mManServerCallback_OnDisconnect;
	
	struct Connection
	{
		int fd;
		int server;
		int wantlen;
		int recvlen;
		int bufflen;
		char *buffer;

		int HandleBuffer( const char* buffer, size_t bufferlen );

		Connection( ):fd(0), server(0), wantlen(0), recvlen(0), buffer(0),bufflen(0){}	
	};
	
	typedef __gnu_cxx::hash_map<int, Connection> ConnMap;
	typedef __gnu_cxx::hash_map<int, SockHandle*> SockMap;
	SockMap mSockmap;
	ConnMap mConnmap;
	// return server sock fd 
	// if failed return 0
	int	CreateServer( int port );
	int CreateConnection( const char * host, int port );
	int CreateManServer( int port );
	int CreateServerSocket( int port , int server );
	int Run( );
	int Stop();
	void DisconnectClient( int sock );
	int GetSockServer( int sock );
	int HandleServerSock( int sock );
	int HandleClientSock( int sock );

	int HandleClientConnect( int sock );
	int HandleClientConnectFailed( int sock );

	int HandleManServerSock( int sock );
	int HandleManClientSock( int sock );

	int Send( int sock, const char * buffer, size_t len );

	void AddSendRate( int len );

	static NetManager * Instance() 
	{
		static NetManager o;
		return &o;
	}
	private:
	int mToStop;
	int mServerSock;

	unsigned long long  mLastTime;
	unsigned int mLastSend;
	unsigned int mDataSend;
};

struct SockHandle{
	int fd;
	int (NetManager::*handle)(int) ;
	
	SockHandle( int _fd, int (NetManager::*_handle)( int ) ){
		fd = _fd;
		handle = _handle;

		NetManager::Instance()->mSockmap[fd] = this;
	}

	~SockHandle( ){
		NetManager::Instance()->mSockmap.erase( fd );
	}

};

#endif
