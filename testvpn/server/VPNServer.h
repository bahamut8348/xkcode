#pragma once

class VPNServer
{
public:
	VPNServer(void);
	~VPNServer(void);

	UDTSOCKET CreateServer( u_short port );

	void Init();
	void Update();

	void AddSocket( UDTSOCKET sock )
	{
		mAllrs.insert( sock );
	}

	void AddClient( UDTSOCKET sock );
	
	typedef stdext::hash_map<UDTSOCKET, Client*> ClientMap;

	ClientMap mClientMap;

	UDT::UDSET mAllrs;
	UDTSOCKET mServer;
	UDTSOCKET mPreServer;

};
