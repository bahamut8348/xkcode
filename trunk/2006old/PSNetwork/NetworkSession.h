#pragma once


namespace PSNetwork
{
	class CNetworkSession
	{
	public:
		CNetworkSession(INetworkSession *session);

		~CNetworkSession(void);
		
		bool	OpenPort(const Address & addr, _dword GroupId);
		bool	ClosePort(const Address & addr);

		bool	JoinSession4( const Address& remoteaddress, _dword groupid, PyObject *data, _dword SessionID = 0);
		bool	JoinSession1( const Address& remoteaddress );
		bool	JoinSession2( const Address& remoteaddress, PyObject *data );
		
		void	CloseSession(_bool bNeedCallback);
		void	Release();
		
		PyObject *GetUserData() const;
		void	SetUserData( PyObject *obj);


		bool	IsUseable() const;
		_dword	GetID() const;

		std::wstring GetDesc() const;

		bool operator == (const CNetworkSession & sess );

		
		PyObject *m_Callback;


	private:
		INetworkSession *m_pSessoin;
		PyObject *m_UserData;
	public:
		operator INetworkSession * () const
		{
			return m_pSessoin;
		}
		operator const INetworkSession * () const
		{
			return m_pSessoin;
		}
		CNetworkSession & operator = (const CNetworkSession & session);

	};


	static void InitNetworkSession()
	{
		class_<CNetworkSession>("Session", no_init)
			.def("OpenPort",	(&CNetworkSession::OpenPort))
			.def("ClosePort",	(&CNetworkSession::ClosePort))
			.def("JoinSession", (&CNetworkSession::JoinSession1))
			.def("JoinSession", (&CNetworkSession::JoinSession2))
			.def("JoinSession", (&CNetworkSession::JoinSession4))
			.def("Close",		(&CNetworkSession::CloseSession))
			.def("__str__",				(&CNetworkSession::GetDesc))
			.def("__repr__",			(&CNetworkSession::GetDesc))
			.def("Release",				&CNetworkSession::Release)
			.add_property("id",			&CNetworkSession::GetID)
			.add_property("data",		&CNetworkSession::GetUserData, &CNetworkSession::SetUserData)
		//	.def(self == self )
			;
	}
}

