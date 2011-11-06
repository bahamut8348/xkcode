#pragma once

class CServerStatus : public CFrameWindowImpl<CServerStatus>
	, public CUpdateUI<CServerStatus>,public CMessageFilter, public CIdleHandler
	, virtual public ViewType
{
	

public:
	DECLARE_FRAME_WND_CLASS(L"ServerConnectionView", NULL)
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_MSG_MAP(CServerStatus)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		NOTIFY_CODE_HANDLER(LVN_GETDISPINFO, OnLVGetDispInfo)
		CHAIN_MSG_MAP(CFrameWindowImpl<CServerStatus>)
	END_MSG_MAP()

	
	BEGIN_UPDATE_UI_MAP(CServerStatus)
		
	END_UPDATE_UI_MAP()


	virtual DWORD type(){return STATUS_TYPE;}

	typedef stdext::hash_set<const MIB_TCPROW_OWNER_PID *, Utils::IPComp > CONNMAP;
	CONNMAP  m_ConnMap;
	
	
	CSplitterWindow  m_vSplit;
	CHorSplitterWindow m_hSplit;
	
	Utils::ProcessInfo m_ProcInfo;
	CString m_CmdLine;
	CString m_Title;

	CString m_ServiceName;

	FLOAT m_CPUUsage;
	DWORD m_MemUsage;
	DWORD m_VMemUsage;

	CPUsage *m_pCpu;

	HWND m_hwndProc;

	CSortListViewCtrl m_ListenList;
	CSortListViewCtrl m_ConnectionList;


	CPaneContainer m_PanelListen;
	CPaneContainer m_PanelConnection;

	DOUBLE  m_LastUpdate;

	

	enum PORTTYPE{
		PORTTYPE_TCP,
		PORTTYPE_UDP,
	};

	enum CONNDIRECT
	{
		CONNDIRECT_OUT,
		CONNDIRECT_IN,
	};

	struct ConnectionInfo
	{
		
		
		DWORD       dwState;
		DWORD       dwLocalAddr;
		DWORD       dwLocalPort;
		DWORD       dwRemoteAddr;
		DWORD       dwRemotePort;
		PORTTYPE	ePortType;
		CONNDIRECT  eConnDirect;
		DWORD Desp;

	};

	struct ListenInfo
	{
		PORTTYPE	ePortType;
		DWORD       dwLocalAddr;
		DWORD       dwLocalPort;
		DWORD		dwCount;
	};

	std::list<ConnectionInfo *> m_Connections;
	std::vector<ListenInfo *> m_Listens;

	void RefreshUdp( const vector<MIB_UDPROW_OWNER_PID> & table );
	void RefreshTcp( const vector<MIB_TCPROW_OWNER_PID> & table );

	void DeleteItem( ConnectionInfo & Info );
	void DeleteItem( ListenInfo & Info );
	void UpdateItem( ListenInfo & Info );
	void AddItem( ConnectionInfo & Info );
	void AddItem( ListenInfo & Info );
	

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnForwardMsg( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/ );

	LRESULT OnLVGetDispInfo(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/) ;


};
