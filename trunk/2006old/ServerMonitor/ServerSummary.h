#pragma once



class CServerSummary : public CFrameWindowImpl<CServerSummary>
	, public CUpdateUI<CServerSummary>,public CMessageFilter, public CIdleHandler
	, virtual public ViewType
{
public:

	DECLARE_FRAME_WND_CLASS(L"ServerSummaryView", NULL)

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_MSG_MAP(CServerSummary)
		NOTIFY_CODE_HANDLER(LVN_GETDISPINFO, OnLVGetDispInfo)
		MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		NOTIFY_CODE_HANDLER(NM_DBLCLK, OnDbClick)
		CHAIN_MSG_MAP(CFrameWindowImpl<CServerSummary>)
		
	END_MSG_MAP()


	BEGIN_UPDATE_UI_MAP(CServerSummary)

	END_UPDATE_UI_MAP()

	virtual DWORD type(){return SUMMARY_TYPE;}


	CSortListViewCtrl m_ServerList;


	void AddServer(const CServerStatus *pServer);
	void DelServer(const CServerStatus *pServer);
	
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	
	LRESULT OnContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLVGetDispInfo(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/) ;
	LRESULT OnDbClick(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/) ;
	
	
};
