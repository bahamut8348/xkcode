#include "StdAfx.h"
#include "NetworkConnection.h"



PSNetwork::CNetworkConnection::CNetworkConnection( INetworkSession *session, _dword Connectionid ):
	m_pSession(session),
	m_Connectionid(Connectionid)
{

}

PSNetwork::CNetworkConnection::~CNetworkConnection(void)
{

}

bool PSNetwork::CNetworkConnection::SendData( object & buf )
{
	str s(buf);
	return m_pSession->SendPacket(GetID(), extract<char *>(s), Len(s));
}

PSDash::_dword PSNetwork::CNetworkConnection::GetID() const
{
	return m_Connectionid;
}

void PSNetwork::CNetworkConnection::SetUserData( PyObject *obj )
{
	PyObject *old = (PyObject *)GetUserData();
	Py_XDECREF(old);
	Py_XINCREF(obj);
	return m_pSession->SetConnectionUserData(GetID(), (_dword)obj);
}

PyObject * PSNetwork::CNetworkConnection::GetUserData() const
{
	return (PyObject *) m_pSession->GetConnectionUserData(GetID());
}

std::wstring PSNetwork::CNetworkConnection::GetDesc() const
{
	CString s;
	s.Format(L"<PSNetwork.Connection (Id=%d, Session=%d, Group=%d)>",GetID(),m_pSession->GetSessionID(), GetGroupID());
	return (LPCTSTR)s;
}

void PSNetwork::CNetworkConnection::CloseConnection( bool bNeedCallback )
{
	m_pSession->CloseConnection(GetID(), bNeedCallback);
}

PSNetwork::CNetworkConnection & PSNetwork::CNetworkConnection::operator=( const CNetworkConnection & conn )
{
	m_pSession = conn.m_pSession;
	m_Connectionid = conn.m_Connectionid;
	return *this;
}

PSDash::_dword PSNetwork::CNetworkConnection::GetGroupID() const
{
	return m_pSession->GetConnectionGroup(m_Connectionid);
}

void PSNetwork::CNetworkConnection::SetGroupID(_dword GroupID)
{
	m_pSession->SetConnectionGroup(m_Connectionid, GroupID);
}

PSDash::_dword PSNetwork::CNetworkConnection::GetCloseReason() const
{
	return m_pSession->GetConnectionCloseReason(GetID());
}

bool PSNetwork::CNetworkConnection::operator==( const CNetworkConnection & conn )
{
	return (m_Connectionid == conn.m_Connectionid) && ( m_pSession->GetSessionID() == conn.m_pSession->GetSessionID() );
}

bool PSNetwork::CNetworkConnection::operator!=( const CNetworkConnection & conn )
{
	return (m_Connectionid != conn.m_Connectionid) || ( m_pSession->GetSessionID() != conn.m_pSession->GetSessionID() );
}