#include "StdAfx.h"
#include "NetworkSession.h"


PSNetwork::CNetworkSession::CNetworkSession( INetworkSession *session )
{
	m_pSessoin = session;
	m_UserData = NULL;
}


PSNetwork::CNetworkSession::~CNetworkSession(void)
{
}

PSNetwork::CNetworkSession & PSNetwork::CNetworkSession::operator=( const CNetworkSession & session )
{
	m_pSessoin = session.m_pSessoin;
	m_Callback = session.m_Callback;
	m_UserData = session.m_UserData;
	return *this;
}



bool PSNetwork::CNetworkSession::OpenPort( const Address & addr, _dword GroupId )
{
	return m_pSessoin->OpenPort(addr, GroupId);
}

bool PSNetwork::CNetworkSession::ClosePort( const Address & addr )
{
	m_pSessoin->ClosePort(addr);
	return true;
}

bool PSNetwork::CNetworkSession::JoinSession4( const Address& remoteaddress, _dword groupid /*= 0*/, PyObject *data, _dword SessionID)
{
	Py_XINCREF(data);
	if(SessionID == 0) SessionID = m_pSessoin->GetSessionID();
	return m_pSessoin->JoinSession(remoteaddress, groupid, (_dword)data, SessionID );
}

bool PSNetwork::CNetworkSession::JoinSession1( const Address& remoteaddress )
{
	return m_pSessoin->JoinSession(remoteaddress, 0, 0 );
}

bool PSNetwork::CNetworkSession::JoinSession2( const Address& remoteaddress, PyObject *data )
{
	return JoinSession4( remoteaddress, 0, data, 0);
}
void PSNetwork::CNetworkSession::CloseSession( _bool bNeedCallback )
{
	m_pSessoin->CloseSession(bNeedCallback);
}


bool PSNetwork::CNetworkSession::IsUseable() const
{
	return m_pSessoin != _null;
}

std::wstring PSNetwork::CNetworkSession::GetDesc() const
{
	CString ret;
	ret.Format(L"<PSNetworkSession (ID=%d, CurNum=%d)>", GetID(), m_pSessoin->GetSessionConnectionNumber());
	return (LPCTSTR)ret;
}

PSDash::_dword PSNetwork::CNetworkSession::GetID() const
{
	return m_pSessoin->GetSessionID();
}

PyObject * PSNetwork::CNetworkSession::GetUserData() const
{
	return m_UserData;
}

void PSNetwork::CNetworkSession::SetUserData( PyObject *obj )
{
	PyObject *old = (PyObject *)GetUserData();
	Py_XDECREF(old);
	Py_XINCREF(obj);
	m_UserData = obj;
}

void PSNetwork::CNetworkSession::Release()
{
	SetUserData(NULL);
	GetNetworkManagerSingleton()->ReleaseSession(m_pSessoin);
}

bool PSNetwork::CNetworkSession::operator==( const CNetworkSession & sess )
{
	return GetID() == sess.GetID();
}