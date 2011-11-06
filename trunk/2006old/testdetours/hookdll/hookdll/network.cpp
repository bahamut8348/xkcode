#include "StdAfx.h"
#include "DetoursWapper.h"
#include "DataManager.h"
#include "network.h"




static int ( WINAPI*  True_send)( SOCKET s, const char FAR* buf, int len, int flags ) = send;
int WINAPI New_send( SOCKET s, const char FAR* buf, int len, int flags )
{


	printf("dll: Send: %d\n", len);
	DataManager::Instance()-> SetData( ::GetCurrentProcessId(), 1, DataManager::NetInfo::_SEND, s, (BYTE*) buf, len, SOCKADDR_IN() );
	return True_send(s, buf, len, flags);
}

static int ( WINAPI*  True_sendto)( SOCKET s,   const char FAR* buf,  int len, int flags, const struct sockaddr FAR* to,  int tolen  ) = sendto;
int WINAPI New_sendto( SOCKET s,const char FAR* buf,  int len, int flags, const struct sockaddr FAR* to,  int tolen )
{
	printf("dll: Sendto: %d\n", len);
	return True_sendto(s, buf, len, flags, to, tolen);
}

static int ( WINAPI*  True_recv)(
		 SOCKET s,
		 char FAR* buf,
		 int len,
		 int flags
		 ) = recv;
int WINAPI New_recv( SOCKET s, char FAR* buf, int len, int flags )
{
	int ret = True_recv( s, buf, len, flags );
	printf("dll: Recv: %d\n", ret);
	return ret;
}

static int ( WINAPI*  True_recvfrom)(
			 SOCKET s,
			 char* buf,
			 int len,
			 int flags,
			struct sockaddr* from,
			int* fromlen
			) = recvfrom;
int WINAPI New_recvfrom( SOCKET s,
				char* buf,
				int len,
				int flags,
			struct sockaddr* from,
				int* fromlen )
			{

				int ret = True_recvfrom(s, buf, len, flags, from, fromlen);
				printf("dll: recv from: %d", ret);
				return ret;
			}


static int (WINAPI* True_WSASend)(
				SOCKET s,
				LPWSABUF lpBuffers,
				DWORD dwBufferCount,
				LPDWORD lpNumberOfBytesSent,
				DWORD dwFlags,
				LPWSAOVERLAPPED lpOverlapped,
				LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
				) = WSASend;

int WINAPI New_WSASend( SOCKET s,
					   LPWSABUF lpBuffers,
					   DWORD dwBufferCount,
					   LPDWORD lpNumberOfBytesSent,
					   DWORD dwFlags,
					   LPWSAOVERLAPPED lpOverlapped,
					   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	int ret = True_WSASend( s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpOverlapped, lpCompletionRoutine);
	printf("dll: WSASend: %d", dwBufferCount);
	return ret;
}








int DetoursNetwork()
{
	Detours::Instance()->Attach( True_send, New_send );
	Detours::Instance()->Attach( True_sendto, New_sendto );
	Detours::Instance()->Attach( True_recv, New_recv);
	Detours::Instance()->Attach( True_recvfrom, New_recvfrom);
	return 1;
}