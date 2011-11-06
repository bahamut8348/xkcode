// testvpn.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Utils.h"
#include "Device.h"
#include "DeviceManager.h"


#include "UDPSocket.h"
#include "Net.h"

class SERVICE
{
public:
protected:
private:
};

int _tmain(int argc, _TCHAR* argv[])
{
	BufferMgr::Instance();
	
	WSADATA d;
	WSAStartup( 0x0202, &d );
	
	
	Net net;
	std::vector<Utils::TapInfo> taps = Utils::GetAllTap();
	Utils::GetPanelReg( );

	stIP ip(L"192.168.1.15");
	std::wstring s = ip.toString();
	
	Device* dev = DeviceManager::Instance()->CreateDevice( taps[0].mGuid.c_str() );
	ULONG majorver, manorver, debugver;
	dev->GetVersion( majorver, manorver, debugver );
	
	if ( argc == 2 )
	{
		dev->SetIPAddr( argv[1], L"255.255.255.0", NULL );
	}
	else
	{
		dev->SetIPAddr( L"10.8.1.5", L"255.255.255.0", NULL );
	}
	
	dev->SetConnect( TRUE );
	Sleep( 5000 );
	printf("Start\n");
	dev->StartRecv();
	
	while(1)
	{
		int a = IOCP::Instance()->Update();
	}
	


	return 0;
}

