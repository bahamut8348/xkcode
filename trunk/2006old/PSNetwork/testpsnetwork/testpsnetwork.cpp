// testpsnetwork.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#define  SESSIONID 20
#include <Windows.h>
#include <Winnt.h>
bool bClient;

INetworkManager *g_pMan;


int ServerPacketNum;



DWORD last;
static _char buffer[1024];

void OnCreateConnectionCb( _dword sessionid, _dword connectionid )
{
	Console::Print(L"CreateConnection\n");

	if(bClient)
	{
		
		INetworkSession *pSession = g_pMan->SearchSession(sessionid);
		pSession->SendPacket( connectionid, buffer, sizeof( buffer ));
	}

}


void OnDeleteConnectionCb( _dword sessionid, _dword connectionid )
{
	INetworkSession *pSession = g_pMan->SearchSession(sessionid);
	_dword Reason = pSession->GetConnectionCloseReason(connectionid);
	Console::Print(L"DeleteConnection %d\n", Reason);
}

void OnReceivePacketCb( _dword sessionid, _dword connectionid, _byte* buffer, _dword size )
{

	INetworkSession *pSession = g_pMan->SearchSession(sessionid);
	if(!bClient)
	{
		ServerPacketNum ++;
		if(ServerPacketNum == 10000)
		{
			std::cout<<"Packet: "<<ServerPacketNum<< ", Time: "<<::GetTickCount()<<" Interval: "<<GetTickCount() - last<<std::endl;
			last = ::GetTickCount();
			ServerPacketNum = 0;
		}
		
	}
	else
	{
		//std::cout<<"Packet \n";
		//::Sleep(10000);
		
	}
	pSession->SendPacket(connectionid, buffer, size);
}

void OnJoinFailedCb( _dword sessionid, _dword connectionid )
{
	INetworkSession *pSession = g_pMan->SearchSession(sessionid);
	_dword Reason = pSession->GetConnectionCloseReason(connectionid);
	Console::Print(L"Join Session Failed %d\n", Reason);
}


void Server( int port)
{


	INetworkSession *pSession = g_pMan->CreateSession(SESSIONID,100);
	g_pMan->SetCreateConnectionCallback(OnCreateConnectionCb, SESSIONID);
	g_pMan->SetDeleteConnectionCallback(OnDeleteConnectionCb, SESSIONID);
	g_pMan->SetReceivePacketCallback(OnReceivePacketCb, SESSIONID);
	g_pMan->SetJoinFailedCallback(OnJoinFailedCb, SESSIONID);
	g_pMan->SetSessionPollingMode(SESSIONID, _true);

	Address Addr;
	Addr.mIP =0 ;
	Addr.mPort = port;
	Addr.mType = Address::_TYPE_TCP;
	pSession->OpenPort(Addr);
}

void Client( const _char *  ip, int port)
{
	INetworkSession *pSession = g_pMan->CreateSession(SESSIONID, 100);
	g_pMan->SetCreateConnectionCallback(OnCreateConnectionCb, SESSIONID);
	g_pMan->SetDeleteConnectionCallback(OnDeleteConnectionCb, SESSIONID);
	g_pMan->SetReceivePacketCallback(OnReceivePacketCb, SESSIONID);
	g_pMan->SetJoinFailedCallback(OnJoinFailedCb, SESSIONID);
	g_pMan->SetSessionPollingMode(SESSIONID, _true);
	Address remote = g_pMan->CreateAddress(Address::_TYPE_TCP, ip, port);
	pSession->JoinSession(remote);
}

int _tmain(int argc, _TCHAR* argv[])
{
	
	ServerPacketNum = 0;
	last = 0;
	INetworkManager *pMan =	CreateNetworkManager();
	g_pMan = pMan;

	bClient = 0;


	if(argv[1][0] == 'c' )
	{
		bClient = 1;
		Client(argv[2], Long(argv[3]));
	
	}
	else
	{
		Server(Long(argv[2]) );
	}
	
	while (1)
	{
		Sleep(10);
		g_pMan->Update(100);
		
	}
	
	return 0;
}

