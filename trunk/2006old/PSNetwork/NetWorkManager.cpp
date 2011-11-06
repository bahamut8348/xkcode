#include "StdAfx.h"
#include <boost/python/slice.hpp>
#include <boost/python/raw_function.hpp>
#include "NetworkSession.h"
#include "NetWorkManager.h"
#include "NetworkConnection.h"


PyObject *PSNetwork::CNetWorkManager::m_Callback = NULL;

PSNetwork::CNetWorkManager *g_NetWork;

Array< PSNetwork::CNetWorkManager::TimerInfo> PSNetwork::CNetWorkManager::s_CallArray = Array<PSNetwork::CNetWorkManager::TimerInfo>();

PSNetwork::CNetWorkManager::CNetWorkManager()
{
	m_NwManager = GetNetworkManagerSingleton();
	
	if(m_NwManager == _null)
	{
		m_NwManager = CreateNetworkManager();
	}

	g_NetWork = this;

	
}

PSNetwork::CNetWorkManager::~CNetWorkManager(void)
{
	m_NwManager->Release();
}

PSNetwork::CNetworkSession PSNetwork::CNetWorkManager::CreateSession( _dword sessionid, _dword maxconnection, PyObject *callback)
{
	INetworkSession *pSession  = GetNetworkManagerSingleton()->CreateSession(sessionid, maxconnection);
	GetNetworkManagerSingleton()->SetSessionPollingMode( sessionid, _true );
	PyAssert(pSession, "CreateSession Error");
	CNetworkSession * pnewsession =  new CNetworkSession(pSession);
	pnewsession->m_Callback = callback;
	GetNetworkManagerSingleton()->SetCreateConnectionCallback(OnCreateConnectionCb, sessionid);
	GetNetworkManagerSingleton()->SetDeleteConnectionCallback(OnDeleteConnectionCb, sessionid);
	GetNetworkManagerSingleton()->SetJoinFailedCallback		(OnJoinFailedCb,	sessionid);
	GetNetworkManagerSingleton()->SetReceivePacketCallback	(OnReceivePacketCb, sessionid);
	GetNetworkManagerSingleton()->SetSessionUserData(sessionid, (_dword)pnewsession );

	return *pnewsession;
}

void PSNetwork::CNetWorkManager::OnCreateConnectionCb( _dword sessionid, _dword connectionid )
{
	CNetworkSession * pSession = (CNetworkSession *)GetNetworkManagerSingleton()->GetSessionUserData(sessionid);
	PyAssert(pSession, "OnCreateConnectionCb Error");
	call_method<void>(pSession->m_Callback, "OnCreateConnection", *pSession,
			CNetworkConnection(*pSession, connectionid));
	
}

void PSNetwork::CNetWorkManager::OnDeleteConnectionCb( _dword sessionid, _dword connectionid )
{
	CNetworkSession * pSession = (CNetworkSession *)GetNetworkManagerSingleton()->GetSessionUserData(sessionid);
	PyAssert(pSession, "OnDeleteConnectionCb Error");
	call_method<void>(pSession->m_Callback, "OnDeleteConnection", *pSession, 
				CNetworkConnection(*pSession, connectionid));
	
}

void PSNetwork::CNetWorkManager::OnReceivePacketCb( _dword sessionid, _dword connectionid, _byte* buffer, _dword size )
{
	CNetworkSession * pSession = (CNetworkSession *)GetNetworkManagerSingleton()->GetSessionUserData( sessionid );
	PyAssert(pSession, "OnReceivePacketCb Error");
	call_method<void>(pSession->m_Callback, 
		"OnReceivePacke",*pSession, CNetworkConnection(*pSession, connectionid )
		, str( (const char *)buffer, size ));
}

void PSNetwork::CNetWorkManager::OnJoinFailedCb( _dword sessionid, _dword connectionid )
{
	CNetworkSession * pSession = (CNetworkSession *)GetNetworkManagerSingleton()->GetSessionUserData( sessionid );
	PyAssert(pSession, "OnJoinFailedCb Error");
	call_method<void>(pSession->m_Callback, "OnJoinFailed", *pSession, 
		CNetworkConnection(*pSession, connectionid ));
}

void PSNetwork::CNetWorkManager::OnUpdateSessionCb( _dword sessionid )
{
	// pass
}

void PSNetwork::CNetWorkManager::OnNetworkTimerCb( _dword tickcount )
{

}

void PSNetwork::CNetWorkManager::Update()
{
	GetNetworkManagerSingleton()->Update(0);
	while( 1 )
	{
		if( CallFunction()==0)
			break;
	}
	return;
}

void PSNetwork::CNetWorkManager::Release()
{
	GetNetworkManagerSingleton()->Release();
}

object PSNetwork::CNetWorkManager::CallLater( boost::python::tuple args, dict kw )
{
	float seconds = extract<float>(args[0]);
	object fun = args[1];
	
	TimerInfo ti;
	ti.mCalltime = ::GetTickCount() + seconds * 1000;
	ti.mFun = fun;
	ti.mArgs = boost::python::tuple(args.slice(2,_));
	ti.mArgskw = kw;
	s_CallArray.Append( ti );
	return object();
}

void PSNetwork::CNetWorkManager::Run()
{
	while(1)
	{
		Update();
		::Sleep(1);
	}
}

int PSNetwork::CNetWorkManager::CallFunction()
{
	for( _dword a = 0; a < s_CallArray.Number(); a++ )
	{
		TimerInfo & ti = s_CallArray[a];
		if( ti.mCalltime <= ::GetTickCount() )
		{
			PyObject_Call( ti.mFun.ptr( ), ti.mArgs.ptr( ), ti.mArgskw.ptr( ) );
			s_CallArray.Remove( a );
			return 1;
		}
	}
	return 0;
}

PSNetwork::CNetworkSession PSNetwork::CNetWorkManager::CreateSession2( _dword sessionid, PyObject *callback )
{
	return CreateSession(sessionid, 0xffff, callback );
}

namespace PSNetwork
{
	void InitNetworkManager()
	{
		CreateNetworkManager();
		def("CreateSession", CNetWorkManager::CreateSession);
		def("CreateSession", CNetWorkManager::CreateSession2);
		def("Update", CNetWorkManager::Update);
		def("Release", CNetWorkManager::Release);
		def("CallLater", raw_function( CNetWorkManager::CallLater) );
		def("Run", CNetWorkManager::Run );		
	}

}

