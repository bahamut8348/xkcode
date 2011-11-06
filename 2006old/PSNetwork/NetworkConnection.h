#pragma once


namespace PSNetwork
{
	
	class CNetworkConnection
	{
	public:
		CNetworkConnection( INetworkSession *session, _dword Connectionid);
		~CNetworkConnection(void);

		_dword GetID() const;

		void SetUserData(PyObject *obj);
		PyObject *GetUserData() const;

		bool SendData(object & buf);

		_dword GetGroupID() const;
		void SetGroupID(_dword GroupID);

		_dword GetCloseReason() const;

		void CloseConnection(bool bNeedCallback = false);
		
		CNetworkConnection & operator = (const CNetworkConnection & conn);

		bool operator == (const CNetworkConnection & conn);
		bool operator != (const CNetworkConnection & conn);
		std::wstring GetDesc() const;

	private:
		INetworkSession *m_pSession;
		_dword			m_Connectionid;

	};


	static void InitNetworkConnection()
	{
		class_<CNetworkConnection>("Connection",no_init)
			.def("SendData",		&CNetworkConnection::SendData)
			.def( self == self )
			.def ( self != self )
			.def("Close",			&CNetworkConnection::CloseConnection)
			.add_property("id",		&CNetworkConnection::GetID)
			.add_property("data",	&CNetworkConnection::GetUserData, &CNetworkConnection::SetUserData)
			.add_property("group",  &CNetworkConnection::GetGroupID, &CNetworkConnection::SetGroupID)
			.def("__str__",			&CNetworkConnection::GetDesc)
			.add_property("closereason", &CNetworkConnection::GetCloseReason)
			.def("__repr__",		&CNetworkConnection::GetDesc)
			;
	}

}

