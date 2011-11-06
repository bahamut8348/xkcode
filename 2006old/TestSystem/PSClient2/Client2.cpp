#include "StdAfx.h"
#include "MessageCallback.h"
#include "Client2.h"

using namespace PyPSClient2;

CClient2::CClient2(void)
{
	m_pFramework = CreateClientFramework();
	
	m_Callback = new CMessageCallback();
	m_pLog = new Clog;
	m_pFramework->Initialize(m_Callback);
	m_pFramework->SetLogHandle(m_pLog);
}

CClient2::~CClient2(void)
{
	delete m_Callback;
	delete m_pLog;
	GetFramework()->Uninitialize();
}

void CClient2::RegisterMessageCallback( object & obj )
{
	m_MsgCallback = obj;
	m_Callback->m_CbHandler = obj.ptr();
}

void CClient2::SetLogHandler( object & obj )
{
	m_pLog->mObj = obj.ptr();
}

void PyPSClient2::CClient2::SetInitIS( list & islist )
{
	std::vector<Address> v;
	for(int a=0; a< Len(islist); a++ )
	{
		Address addr = extract<Address>(islist[a]);
		v.push_back(addr);
	}
	GetFramework()->SetInitIS((_dword)v.size(), &v[0]);

}

IClientFramework* PyPSClient2::CClient2::GetFramework() const
{
	return m_pFramework;
}

void PyPSClient2::CClient2::ConnectToIS()
{
	GetProcessor()->ConnectToIS();
}

IUIProcessor * PyPSClient2::CClient2::GetProcessor() const
{
	return GetFramework()->GetProcessor();
}

void PyPSClient2::CClient2::Update()
{
	return GetFramework()->Update();
}

void PyPSClient2::CClient2::SetAppInfo( _word AppID, _word SubID, _word MajorVer, _word SubVer )
{
	GetFramework()->SetAppInfo(AppID, SubID, MajorVer, SubVer);
}

void PyPSClient2::CClient2::Login( std::wstring username, std::wstring password )
{
	GetProcessor()->Login(username.c_str(), password.c_str());
}

void PyPSClient2::CClient2::SetIS( const Address & addr )
{
	GetFramework()->SetISAddress(addr);
}

std::wstring PyPSClient2::CClient2::GetError( _dword ErrorCode )
{
	return (LPCTSTR)GetFramework()->GetError(ErrorCode);
}

void PyPSClient2::CClient2::QueryBuddyList()
{
	GetProcessor()->QueryBuddy(0);
}

void PyPSClient2::CClient2::LoginToMMS( _dword PayLoad )
{
	GetProcessor()->LoginToMMS(PayLoad);
}

void PyPSClient2::CClient2::HandleDispatchGame( _dword Result, _dword GameID, list tokens, const SessionAddress & saddr )
{
	Array<_dword> tokenAry;
	ListToArray(tokens, tokenAry);
	GetProcessor()->HandleDispatchGame(Result, GameID, (_word)tokenAry.Number(), &tokenAry[0], saddr);
}

void PyPSClient2::CClient2::QueryAreaList(_dword AreaID)
{
	GetProcessor()->QueryAreaList(0, AreaID);
}

void PyPSClient2::CClient2::QueryRoomList( _dword AreaID, _dword RoomType )
{
	GetProcessor()->QueryRoomList(0, AreaID, RoomType);
}

void PyPSClient2::CClient2::Disconnect()
{
	GetFramework()->Disconnect();
}