#include <sys/epoll.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <fcntl.h>
#include <list>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <ext/hash_set>
#include <ext/hash_map>

#include "ae.h"
#include "buffer.h"
#include "log.h"
#include "appinfo.h"
#include "timer.h"
#include "netmanager.h"

const size_t  g_maxbufflen = 8192;

void FileProcReadCallBack( struct aeEventLoop* loop, int fd, void *data, int mask ){
	typedef int (NetManager::*HANDLE)(int);
	SockHandle *handle = ( SockHandle * )(data);
	logdebug("FileReadCallBack %d", fd );
	(NetManager::Instance()->*(handle->handle))(fd);
//	delete handle;
}

void FileProcConnectCallBack( struct aeEventLoop* loop, int fd, void *data, int mask ){
	
	logdebug( "FileProcConnectCallBack %d, %d", fd, mask );
	aeDeleteFileEvent( loop, fd, AE_WRITABLE );
	aeDeleteFileEvent( loop, fd, AE_READABLE );
	NetManager::Instance()->HandleClientConnect( fd );
	
	aeCreateFileEvent( NetManager::Instance()->mEventLoop, fd, AE_READABLE, FileProcReadCallBack, new SockHandle( fd, &NetManager::HandleClientSock ) );
}

void FileProcConnectFailedCallBack( struct aeEventLoop* loop, int fd, void *data, int mask ){

	logdebug( "FileConnectFailedCallBack %d %d", fd, mask );
	aeDeleteFileEvent( loop, fd, AE_READABLE );
	aeDeleteFileEvent( loop, fd, AE_WRITABLE );
	NetManager::Instance()->HandleClientConnectFailed( fd );
}

NetManager::NetManager()
{
	mEventLoop = aeCreateEventLoop();
	if (mEpoll == -1 )
	{
		logerr("NetManager epoll_create() error");
	}
	mToStop = 0;	
	mDataSend = 0;
	mLastTime = 0;
	mLastSend = 0;
	mRecvBufferLen = 102400;
	mRecvBuffer = MemBuffer::Instance()->Get( mRecvBufferLen );
}

int NetManager::Send( int sock, const char * buffer, size_t len )
{
	logdebug(">>>Send sock %d, len %d", sock, len );
//	logdump( "Send", buffer, len );
	int r = 0;	
	int count = 0;	
	while( 1 ){
		count ++;
		int sendlen = len;
		int offset = 0;
		while( 1 ){
			r = send( sock, buffer+offset, sendlen, MSG_NOSIGNAL );
			if( r == sendlen || r <0 || r == 0 )
				break;
			sendlen = len - r;
			offset = r;
		}
		if( r > 0 ){
			AddSendRate( r );
			break;
		}
		if( errno != EAGAIN ){
			break;
		}
		if( count > 6 ){
			logerr("[%d] send again %d\n", sock, count );
			return r;
		}
	}
	return r;
}

void NetManager::AddSendRate( int dlen ){
	unsigned long long now = Timer::GetTime();
	if( now - mLastTime >= 1000 ){
		AppInfo::Instance()->AddRate( mDataSend );
		mLastTime = now;
		mDataSend = 0;
	}else{
		mDataSend += dlen;	
	}
}

NetManager::~NetManager()
{
	// close all client socket
	for( ConnMap::iterator it = mConnmap.begin(); it != mConnmap.end(); it ++ ){
		loginfo("Disconnect sock: %d", it->first );
		shutdown( it->first, SHUT_RDWR );
		close( it->first );
	}

	// close all server socket
	for( IntSet::iterator it = mServerSet.begin(); it != mServerSet.end(); it ++ )
	{
		shutdown( *it, SHUT_RDWR );
		loginfo( "close socket %d", *it );
		int r=	close(*it);
		if( r != 0 ) logerr( "close error" );
		
	}
	aeDeleteEventLoop( mEventLoop );
	loginfo( "close all socket" );
	MemBuffer::Instance()->Release( mRecvBuffer );
	logdebug("NetManager dispose");
}

int setnonblocking( int sockfd )
{
	int val = 5;
	if( -1 == setsockopt( sockfd, SOL_TCP, TCP_DEFER_ACCEPT, &val, sizeof(val) ) )
	{
		logerr("setsocket TCP_DEFER_ACCEPT");
		return 0;
	}

	if( fcntl( sockfd, F_SETFL, fcntl(sockfd, F_GETFD,0)|O_NONBLOCK) == -1 )
	{
		logerr("setnonblocking");
		return 0;
	}
	return 1;
}

int NetManager::Stop()
{
	mToStop = 1;	
	return 1;
}

int NetManager::CreateServer( int port  ){
	int fd = CreateServerSocket( port, 0 );
	// add to epoll
//	epoll_event ev = {0};
//	ev.events= EPOLLIN | EPOLLET;
//	ev.data.ptr = new SockHandle( fd, &NetManager::HandleServerSock );
//	if( epoll_ctl( mEpoll, EPOLL_CTL_ADD, fd, &ev ) < 0 )
//	{
//		logerr("CreateServer epoll_ctl() error");	
//		return 0;
//	}



	aeCreateFileEvent( mEventLoop, fd, AE_READABLE, FileProcReadCallBack, new SockHandle( fd, &NetManager::HandleServerSock ) );
	// save to set	
	mServerSet.insert( fd );
	return fd;
}

int NetManager::CreateConnection( const char * host, int port )
{
	int sock = socket( AF_INET, SOCK_STREAM, 0 );
	if( sock == -1 )
	{
		logerr( "CreateConnection socket Error " );
		return 0;
	}
	
	setnonblocking( sock );

	struct sockaddr_in sa;
	bzero( &sa, sizeof( sa ) );
	sa.sin_family = AF_INET;
	sa.sin_port = htons( port );
	sa.sin_addr.s_addr = inet_addr( host );
	
	if( 0 != connect( sock, ( struct sockaddr *)&sa, sizeof( sa ) ) ){
//		logerr( "CreateConnection connect error" );
//		return 0;
	}
	mConnmap[sock] = Connection();
	mConnmap[sock].fd = sock;
	SockHandle * handle = new SockHandle( sock, &NetManager::HandleServerSock );
	aeCreateFileEvent( mEventLoop, sock, AE_READABLE, FileProcConnectFailedCallBack, handle );
	aeCreateFileEvent( mEventLoop, sock, AE_WRITABLE, FileProcConnectCallBack, handle );
	return sock;
}

int NetManager::CreateServerSocket( int port , int server )
{
	int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if( listen_sock == -1 )
	{
		logerr("CreateServer socket() error" );
		return 0;
	}
	
	int on = 1;
	if( setsockopt( listen_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) ) < 0 ){
		logerr("Setsockopt SO_REUSEADDR error" );
		return 0;
	}

	setnonblocking( listen_sock );

	sockaddr_in listen_sa;

	bzero( &listen_sa, sizeof( listen_sa ) );
	listen_sa.sin_family = AF_INET;
	listen_sa.sin_port = htons ( port );
	listen_sa.sin_addr.s_addr = server;
	if( bind(  listen_sock, (sockaddr*)&listen_sa, sizeof(listen_sa) ) < 0 )
	{
		logerr("CreateServer bind() error ");
		return 0;
	}
	
	if( listen( listen_sock, 5 ) == -1 )
	{
		logerr("CreateServer listen() error");
		return 0;
	}
	return listen_sock;
}

int NetManager::CreateManServer( int port ){
	int fd = CreateServerSocket( port, inet_addr("127.0.0.1") );
	// add to epoll
	epoll_event ev = {0};
	ev.events= EPOLLIN | EPOLLET;
	ev.data.ptr = new SockHandle( fd, &NetManager::HandleManServerSock );
	if( epoll_ctl( mEpoll, EPOLL_CTL_ADD, fd, &ev ) < 0 )
	{
		logerr("CreateServer epoll_ctl() error");	
		return 0;
	}
	// save to set	
	mServerSet.insert( fd );
	return 1;
}
int NetManager::HandleManClientSock( int sock ){
	int r = recv( sock, mRecvBuffer, mRecvBufferLen, 0 );
	if( r <= 0 ){
		mManServerCallback_OnDisconnect( sock );
		DisconnectClient( sock );
	}

	mManServerCallback_OnData( sock, mRecvBuffer, r );
}

int NetManager::HandleManServerSock( int sock ){
	sockaddr_in sa;
	socklen_t len = sizeof( sa );
	int newfd = 0;
	if( (newfd = accept( sock,(sockaddr *)&sa, &len ) ) == -1 )
	{
		if( errno == EAGAIN ){

		}else{
			logerr("HandleServerSock accept() error");
		}
		return 0;
	}
	setnonblocking( newfd );

	int buffsize = 2<<18;
	if( setsockopt( newfd, SOL_SOCKET, SO_SNDBUF, &buffsize, sizeof( buffsize ) ) < 0 ){
		logerr("[%d] setopterror SO_SNDBUF", sock );
	}

	buffsize = 2<<12;
	if( setsockopt( newfd, SOL_SOCKET, SO_RCVBUF, &buffsize, sizeof( buffsize ) ) < 0 ){
		logerr("[%d] setopterror SO_SNDBUF", sock );
	}

//	epoll_event ev = {0};
//	ev.data.ptr = new SockHandle( newfd, &NetManager::HandleManClientSock );
//	ev.events = EPOLLERR| EPOLLIN | EPOLLET;
//	if( epoll_ctl( mEpoll, EPOLL_CTL_ADD, newfd, &ev ) < 0 )
//	{
//		logerr("HandleServerSock epoll_ctl() error");
//		return 0;
//	}
	aeCreateFileEvent( mEventLoop, newfd, AE_READABLE, FileProcReadCallBack, new SockHandle( newfd, &NetManager::HandleManClientSock ) );
	
	logdebug("[+] new client: %d", newfd );

	mConnmap[newfd] = Connection( );
	mConnmap[newfd].fd = newfd;
	
	mManServerCallback_OnConnect( newfd );		
	return 1;
}

int NetManager::HandleClientConnectFailed( int sock ){
	mClientCallback_OnConnectFailed( sock );
	mConnmap.erase( sock );
	return 1;
}

int NetManager::HandleClientConnect( int sock ){
//	mConnmap[sock] = Connection();
//	mConnmap[sock].fd = sock;
	aeCreateFileEvent( mEventLoop, sock, AE_READABLE, FileProcReadCallBack, new SockHandle( sock, &NetManager::HandleClientSock ) );
	mClientCallback_OnConnect( sock );
	return 1;
}

int NetManager::HandleServerSock( int sock )
{
	sockaddr_in sa;
	socklen_t len = sizeof( sa );
	int newfd = 0;
	if( (newfd = accept( sock,(sockaddr *)&sa, &len ) ) == -1 )
	{
		if( errno == EAGAIN ){

		}else{
			logerr("HandleServerSock accept() error");
		}
		return 0;
	}
	setnonblocking( newfd );

	int buffsize = 2<<18;
	if( setsockopt( newfd, SOL_SOCKET, SO_SNDBUF, &buffsize, sizeof( buffsize ) ) < 0 ){
		logerr("[%d] setopterror SO_SNDBUF", sock );
	}

	buffsize = 2<<12;
	if( setsockopt( newfd, SOL_SOCKET, SO_RCVBUF, &buffsize, sizeof( buffsize ) ) < 0 ){
		logerr("[%d] setopterror SO_SNDBUF", sock );
	}
/*
	epoll_event ev = {0};
	ev.data.ptr = new SockHandle( newfd, &NetManager::HandleClientSock );
	ev.events = EPOLLERR| EPOLLIN | EPOLLET;
	if( epoll_ctl( mEpoll, EPOLL_CTL_ADD, newfd, &ev ) < 0 )
	{
		logerr("HandleServerSock epoll_ctl() error");
		return 0;
	}
*/
	//	logdebug("[+] new client: %d", newfd );

	aeCreateFileEvent( mEventLoop, newfd, AE_READABLE, FileProcReadCallBack, new SockHandle( newfd, &NetManager::HandleClientSock ) );

	mConnmap[newfd] = Connection( );
	mConnmap[newfd].fd = newfd;
	mConnmap[newfd].server = sock;
	
	mServerCallback_OnConnect( newfd );		
	return HandleServerSock( sock );
}

int NetManager::GetSockServer( int sock )
{
	if( mConnmap.find( sock ) == mConnmap.end() ){
		logerr("GetSockServer error");
		return 0;
	}
	return mConnmap[sock].server;
}

inline static uint32_t tobig( uint32_t d )
{
	return ntohl( d );
}

int NetManager::HandleClientSock( int sock )
{
	
	if (mConnmap.find(sock) == mConnmap.end() ) return 0;
	
	Connection & cinfo = mConnmap[sock];

	int recvlen = recv( sock, mRecvBuffer, mRecvBufferLen, 0 );
	int totalrecvlen = recvlen ;
	if( totalrecvlen == mRecvBufferLen )
	{
		loginfo("[%d] Got a big buffer, length: %d ", recvlen );
	}
	if( recvlen  <= 0 )
	{
		if ( errno != EAGAIN && errno != EINTR )
		{
			logerr("[%d] Handle Client Socket recv <= 0 ", sock );
			mServerCallback_OnDisconnect( sock );
			DisconnectClient( sock );
			return 1;
		}
		return 1;
	}
	char *recvbuffer = mRecvBuffer;
	
	// 还没得到长度
	logdebug(">>>sock: %d, info.buffer:%d, info.wantlen: %d, info.recvlen: %d, recvlen: %d", sock, cinfo.buffer, cinfo.wantlen, cinfo.recvlen, recvlen );
	if( cinfo.buffer !=0  && cinfo.wantlen ==0 )
	{
		size_t cpylen = 4 - cinfo.recvlen ;
		if( recvlen >= cpylen ) // 长度够4个字节了
		{
			memcpy( cinfo.buffer + cinfo.recvlen , recvbuffer, cpylen );
			recvbuffer += cpylen;
			recvlen -= cpylen;
			assert( recvlen >= 0 );
			
			uint32_t *datalen = ( uint32_t *) cinfo.buffer ;
			cinfo.wantlen = tobig( *datalen );
			if( cinfo.wantlen > g_maxbufflen )
			{
				logerr("sock: %d, Data too long %d, should < %d, disconnect client" , sock, cinfo.wantlen, g_maxbufflen );
				mServerCallback_OnDisconnect( sock );
				DisconnectClient( sock );
				return 1;
			}
			else
			{
				// 申请足够的内存
//				logdebug("(187)new mem %d", cinfo.wantlen );
				MemBuffer::Instance()->Release( cinfo.buffer );
				cinfo.buffer = MemBuffer::Instance()->Get( cinfo.wantlen );
				cinfo.recvlen = 0;
			}

		}
		else // 长度不够，再复制到 buffer 里面
		{
//			logdebug("长度不够copy memcpy %d", recvlen );
			memcpy( cinfo.buffer + cinfo.recvlen, recvbuffer, recvlen );
			recvbuffer += recvlen;
			cinfo.recvlen += recvlen;
			recvlen -= recvlen;
		}
	}

	// 得到长度了
//	logdebug("recvlen %d, cinfo.wantlen: %d", recvlen,cinfo.wantlen );
	if( cinfo.buffer != 0 && cinfo.wantlen >0  ) 
	{
		size_t needlen = cinfo.wantlen - cinfo.recvlen ;
		if( needlen <= recvlen ) // 需要的长度小于剩余的长度
		{
			memcpy( cinfo.buffer + cinfo.recvlen , recvbuffer , needlen );
			cinfo.recvlen += needlen ;
			recvbuffer += needlen;
			recvlen -= needlen;	
			mServerCallback_OnData( sock, cinfo.buffer, cinfo.recvlen  );
			MemBuffer::Instance()->Release( cinfo.buffer );
			cinfo.wantlen = 0;
			cinfo.recvlen = 0;
			cinfo.buffer = 0;
		}
		else
		{
			memcpy( cinfo.buffer + cinfo.recvlen , recvbuffer, recvlen );
			recvlen =0;
			cinfo.recvlen += recvlen ;
		}
	}
	
	if( recvlen == 0 )
	{	
		if( totalrecvlen == mRecvBufferLen )
		{
			return HandleClientSock( sock  );
		}
		return 1;
	}
	// 处理下面的部分
	while(recvlen)
	{
		if( recvlen < 4 )
		{
			assert( cinfo.buffer == 0 );
			cinfo.buffer = MemBuffer::Instance()->Get( g_maxbufflen );
			memcpy( cinfo.buffer, recvbuffer, recvlen );
			cinfo.wantlen = 0;
			cinfo.recvlen = recvlen ;
			recvlen = 0;

		}
		else
		{
			uint32_t *datalen = ( uint32_t *) recvbuffer;
			*datalen = tobig( *datalen );
			if( *datalen + 4 <= recvlen ) // 总的长度大于需要的长度
			{
				mServerCallback_OnData ( sock, recvbuffer+4 , *datalen );	
				recvbuffer += (*datalen + 4);
				recvlen -= (*datalen + 4);
			}
			else
			{
				if( *datalen > g_maxbufflen ) 
				{
					logerr("Data too long  %d, should < %d ,disconnect fd: %d",  *datalen, g_maxbufflen ,sock );
					DisconnectClient( sock );
					return 1;
				}
		//		logdebug("new mem %d, cpylen: %d", *datalen, recvlen - 4 );
				if( cinfo.buffer != NULL )
				{
					logerr( "cinfo.buffer must == NULL, cinfo.recvlen: %d, cinfo.wantlen: %d ", cinfo.recvlen, cinfo.wantlen );
				}
				assert( cinfo.buffer == 0);
				cinfo.buffer = MemBuffer::Instance()->Get( *datalen );
				memcpy( cinfo.buffer, recvbuffer + 4, recvlen - 4);
				cinfo.wantlen = *datalen ;
				cinfo.recvlen = recvlen - 4;
				recvlen = 0;
			}
		}
	}
	
	if( totalrecvlen == mRecvBufferLen )
	{
		return HandleClientSock( sock  );
	}	
	return 1;
}

int NetManager::Run()
{
	// 
	while( !mToStop )
	{
		mServerCallback_OnIdle();
		aeProcessEvents( mEventLoop, AE_FILE_EVENTS | AE_DONT_WAIT );
//		printf("run..\n");
	}
}

void NetManager::DisconnectClient( int sock )
{
	SockMap::iterator si = mSockmap.find( sock );
	if( si == mSockmap.end() ){
		return;
	}
	SockHandle * psh = si->second;
	delete psh;
	if( mConnmap.find( sock ) != mConnmap.end() ){
		if( mConnmap[sock].buffer )
			MemBuffer::Instance()->Release( mConnmap[sock].buffer);
		mConnmap[sock].buffer = 0;
		mConnmap.erase( sock );
	}

	aeDeleteFileEvent( mEventLoop, sock, AE_READABLE | AE_WRITABLE );

	close(sock);
	loginfo("[%d] DisconnectClient fd: %d", sock, sock );
}

