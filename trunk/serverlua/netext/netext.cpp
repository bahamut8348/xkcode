#include <signal.h>
#include <arpa/inet.h>

#include <assert.h>
#include <string.h>

#include <hash_map>
#include <hash_set>
#include <string>
#include <list>

#include "../lua/lua.hpp"
#include "netmanager.h"
#include "timer.h"

int log_level = 3;
int logf_g = 0;

static lua_State* g_lua_state = NULL;

static int l_version( lua_State *L ){
	lua_pushnumber( L, 3 );
	return 1;
}

class StackCheck{
	int mStackLen;
	lua_State* mL;
	char mFun[1024];
	public:
	StackCheck( lua_State * L, const char *n ){
		mL = L;
		strcpy( mFun, n );
		mStackLen = lua_gettop(L);	
	}
	~StackCheck(){
		if( mStackLen != lua_gettop(mL) ){
			printf("lua stack error %s, %d != %d\n", mFun, mStackLen, lua_gettop(mL) );
		}
		assert( mStackLen == lua_gettop(mL) );
	}
};

#define __CHECK_STACK_  StackCheck sc( L, __func__ )

static int l_test( lua_State *L ){
	const char *s = lua_tostring( L, 1 );
	printf("value: %s", s );
	lua_pushvalue( L, 1 );	
	return 1;
}
static int net_run( lua_State *L ){
	NetManager::Instance()->Run();
	return 0;
}

struct CallLaterInfo{
	unsigned long long time;
	unsigned int num;
};

static std::list<CallLaterInfo> g_callinfoList;
static unsigned int g_callinfoNum = 1;

static void on_idle( ){
		
	lua_State* L = g_lua_state;
	__CHECK_STACK_;
	// handle call timer
	unsigned long long now = Timer::GetTime();
	for( auto iter = g_callinfoList.begin(); iter != g_callinfoList.end(); ){
		CallLaterInfo & info = *iter;
		if( int(info.time - now) <= 0  ){
			
		//	printf("TT %lld\n",info.time - now );
			// call functiLn
			lua_getglobal( L, "call_table" );
			lua_rawgeti( L, -1, info.num );
			int n = lua_gettop( L ); // globals[tn]
			lua_rawgeti( L, n, 1 ); // t[1] // paras count
			int pn = lua_tonumber( L,  -1 );
			lua_pop( L, 1 ) ; // t[1]
			for( int i=0; i<pn; i++){
				lua_rawgeti( L, n, i+2);
			}
			int r = lua_pcall( L, pn-1, LUA_MULTRET, 0 );
			if( r != 0 ){
				lua_error( L );
			}
			
			lua_pop( L, 1 ); // pop parameters table

			// remove globals[tn][info.num] )
			lua_pushnil( L );
			lua_rawseti( L, -2, info.num );

			lua_pop( L, 1 );  // call_table
		
			iter = g_callinfoList.erase( iter );
		}
		else{
			iter ++;
		}
	}
}

static int net_socket_getpeername( lua_State* L ){
	lua_getfield( L, 1, "fd" );
	int fd = lua_tonumber( L, -1 );
	lua_pop( L, 1 );

	sockaddr_in sa = {0};
	socklen_t salen = sizeof( sa );
	getpeername( fd, (sockaddr*)&sa, &salen );
	lua_pushstring( L, inet_ntoa( sa.sin_addr ) );
	lua_pushnumber( L, ntohs( sa.sin_port ) );
	return 2;
}

static int net_socket_send( lua_State* L ){
	if( lua_istable( L, 1 ) ){
		return luaL_error( L, "parameter 1 must be table");
	}
	luaL_checkstring( L, 2 );
	lua_getfield( L, 1, "fd" );
	int fd = lua_tonumber( L, -1 );
	lua_pop( L, 1 );
	
	size_t len = 0;
	const char *buff = lua_tolstring( L, 2 , &len );

	int r = NetManager::Instance()->Send( fd, buff, len );
	lua_pushnumber( L, r );
	return 1;
}

static int net_socket_close( lua_State* L ){
	__CHECK_STACK_;
	
	lua_getfield( L, 1, "fd" );
	int fd = lua_tonumber( L, -1 );
	lua_pop( L, 1 );
	
	int server = NetManager::Instance()->GetSockServer( fd );
	
	NetManager::Instance()->DisconnectClient( fd );
	
	if( server ){
		lua_getglobal( L, "server_socks");
		lua_rawgeti( L, -1, server );
		lua_getfield( L, -1, "clients" );
		lua_pushnil( L );
		lua_rawseti( L, -2, fd );
		lua_pop( L, 3 );
	}

	return 0;
}

static void init_sockmetatable( lua_State* L ){
	__CHECK_STACK_;

	// socket method table
	lua_newtable( L );

	lua_pushcfunction( L, net_socket_getpeername );
	lua_setfield( L, -2, "getpeername" );

	lua_pushcfunction( L, net_socket_send );
	lua_setfield( L, -2, "send" );

	lua_pushcfunction( L, net_socket_close );
	lua_setfield( L, -2, "close" );

	lua_setglobal( L, "socket" );

	// socket metatable
	lua_newtable( L );
	lua_getglobal( L, "socket");
	lua_setfield( L, -2, "__index" );

	lua_setglobal( L, "socket_meta" );
}

static void on_client_connect( int fd ){
	printf( "# on_client_connect_success %d\n", fd );
}

static void on_client_connect_failed( int fd ){
	printf("# on_client_connect_failed %d\n", fd );
}

static void on_newsock( int fd ){
	lua_State *L = g_lua_state;
	__CHECK_STACK_ ;
	int server = NetManager::Instance()->GetSockServer( fd );
	printf("on_new sock server %d\n", server );	
	lua_newtable( L );  // socket
	lua_getglobal( L, "socket_meta" );
	lua_setmetatable( L, -2 );

	lua_pushnumber( L, fd );
	lua_setfield( L, -2, "fd" );

	if( server ){
		lua_getglobal( L, "server_socks" );
		lua_rawgeti( L, -1, server );
		
		lua_getfield( L, -1, "clients" );
		lua_pushvalue( L, -4 );
		lua_rawseti( L, -2, fd );
		lua_pop( L, 1 );  // pop clients

		lua_getfield( L, -1, "proto" );
		lua_getfield( L, -1, "connectionMade" );
		lua_pushvalue( L, -3 );
		lua_pushvalue( L, -6 );
		if( 0!= lua_pcall( L, 2, 0, 0 )){  // connectionMade( proto, sock )
			lua_error( L );	
			lua_pop( L, 3 );
		}

		lua_pop( L, 3 );
	}

	lua_pop( L, 1 ); // pop socket
}
static void on_data( int fd, char *buff, int len ){
	lua_State *L = g_lua_state;	
	__CHECK_STACK_ ;
	int server = NetManager::Instance()->GetSockServer( fd );
	if( server ){
		lua_getglobal( L, "server_socks" );
		lua_rawgeti( L, -1, server );

		lua_getfield( L, -1, "proto" );
		lua_getfield( L, -1, "dataReceived");

		lua_getfield( L, -3, "clients" );
		lua_rawgeti( L, -1, fd );

		lua_pushvalue( L, -3 );
		lua_pushvalue( L, -5 );
		lua_pushvalue( L, -3 );
		lua_pushlstring( L, buff, len );

		if( 0!= lua_pcall( L, 3, 0, 0 ) ){
			lua_error( L );
			lua_pop( L, 3 );
		}
		lua_pop( L, 6 );
	}
}
static void on_disconnect( int fd ){
	lua_State *L = g_lua_state;	
	__CHECK_STACK_;
	int server = NetManager::Instance()->GetSockServer( fd );
	printf("on_disconnect %d\n", fd );
	if( server ){
		lua_getglobal( L, "server_socks" );
		lua_rawgeti( L, -1, server );
		
		lua_getfield( L, -1, "proto" );
		lua_getfield( L, -1, "connectionLost" );

		lua_getfield( L, -3, "clients" );
		lua_rawgeti( L, -1, fd );

		// remove clients[fd]
		lua_pushnil( L );
		lua_rawseti( L, -3, fd);

		lua_pushvalue( L, -3 );  // connectionLost
		lua_pushvalue( L, -5 );
		lua_pushvalue( L, -3 );
		if( 0!= lua_pcall( L, 2, 0, 0 ) ){
			lua_error( L );
			lua_pop( L, 3 );
		}
		lua_pop( L, 6 );
	}
	else{
		printf("# client  disconnect\n");
	}
}

static int net_callLater( lua_State *L ){
	const char *tn = "call_table";
	
	__CHECK_STACK_;

// callLater( 1500, foo, 1,2,3,4 )	
	int n = lua_gettop( L );
	if( n < 2 ){
		return	luaL_error( L, "need at least 2 parameters" );
	}	
	lua_Integer t = lua_tonumber( L, 1 );
	t =Timer::GetTime() + t;
	
	CallLaterInfo info;
	info.num = g_callinfoNum++;
	info.time = t;
	auto iter = g_callinfoList.begin();	
	for( ; iter!= g_callinfoList.end(); iter++ ){
		if( (*iter).time > t ){
			g_callinfoList.insert( iter, info );
			break;
		}
	}
	if( iter == g_callinfoList.end() ){
		g_callinfoList.push_back( info );
	}
	
	// get globals[tn]
	lua_getglobal( L, tn );
	if( !lua_istable( L, -1 ) ){
		lua_pop( L, 1 );
		lua_newtable( L );
		lua_pushvalue( L, -1 );
		lua_setglobal( L, tn );  // create a new  globals[ tn ] = {}
	}
	
	lua_newtable( L ) ; // new table save callLater parameters
	lua_pushnumber( L, n-1 );
	lua_rawseti( L, -2, 1);
	for( int i=2; i<=n; i++ ){
		lua_pushvalue( L, i );
		lua_rawseti( L, -2, i ); 
	}
	lua_pushnumber( L, info.num );
	lua_pushvalue( L, -2 );		
	lua_rawset( L, -4); // save parameters table
	
	lua_pop( L, 1 );  // {}	
	lua_pop( L, 1 ); // pop globals[tn]
	return 0;
}

static int net_server( lua_State* L ){
	__CHECK_STACK_;
	// port
	int port = lua_tonumber( L, 1 );
	// server protocol
	// check server protocol table
	int fd = NetManager::Instance()->CreateServer( port );
	if( fd == 0 ){
		luaL_error( L, "net_server CreateServer(%d) Error", port );
		return 0;
	}

	// create server table, if not existed
	lua_getglobal( L, "server_socks" );
	if( !lua_istable( L, -1 ) ){
		lua_pop( L, 1 );
		lua_newtable( L );
		lua_pushvalue( L, -1 );
		lua_setglobal( L, "server_socks" ); // globals['server_socks'] = {}
	}

	// globals['server_socks'][fd] = { proto = args[1] }
	lua_pushnumber( L, fd );
	lua_newtable( L );
	lua_pushstring( L, "proto" );
	lua_pushvalue( L, 2 ); // push args[1]
	lua_rawset( L, -3 );   // set { proto = args[1] )

	lua_newtable( L );  // clients table
	lua_setfield( L, -2, "clients");

	lua_settable( L, -3 );
	
	lua_pop( L, 1 ); // pop globals['server_socks']

	return 0;	
}

static int net_connect( lua_State* L ){
	if( !lua_isstring( L, 1 ) ){
		luaL_error( L, "first parameter mustbe string" );
		return 0;
	}
	if( !lua_isnumber( L, 2 ) ){
		luaL_error( L, "second parameter must be number" );
		return 0;
	}

	const char *host = lua_tostring( L, 1 );
	int port = lua_tonumber( L, 2 );
	int sock = NetManager::Instance()->CreateConnection( host, port );
	if( sock ){
		lua_getglobal( L, "client_socks" );	
		if( lua_isnil( L, -1 ) ){
			lua_pop( L, 1 );
			lua_newtable( L );
			lua_pushvalue( L, -1 );
			lua_setglobal( L, "client_socks" );
		}
		lua_pushvalue( L, 3 );
		lua_rawseti( L,  -2, sock );
		lua_pop( L, 1 ); // client_socks table
	}

	return 0;
}

static const struct luaL_reg funcs[] = {
	{"version", l_version },
	{"test", l_test },
	{"callLater", net_callLater },
	{"server",    net_server },
	{"connect",   net_connect },
	{"run", net_run }, 
	{NULL, NULL }
};

static void on_signal_int( int sig ){
	NetManager::Instance()->Stop();
}

extern"C"{
 int luaopen_netext( lua_State *L ){
	g_lua_state = L;

	init_sockmetatable( L );

	signal( SIGINT, on_signal_int );
	signal( SIGTERM, on_signal_int );
	
	NetManager::Instance()->mServerCallback_OnIdle = on_idle;
	NetManager::Instance()->mServerCallback_OnConnect = on_newsock;
	NetManager::Instance()->mServerCallback_OnDisconnect = on_disconnect;
	NetManager::Instance()->mServerCallback_OnData = on_data;

	NetManager::Instance()->mClientCallback_OnConnect = on_client_connect;
	NetManager::Instance()->mClientCallback_OnConnectFailed = on_client_connect_failed;
	luaL_register( L, "netext", funcs );
	return 1;
 }
}
