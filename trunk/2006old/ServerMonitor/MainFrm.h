// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#define WINDOW_MENU_POSITION	2

class CMainFrame : public CFrameWindowImpl<CMainFrame>, public CUpdateUI<CMainFrame>,
		public CMessageFilter, public CIdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(L"PSSERVERMORVIEW", IDR_MAINFRAME)

	CTabView m_view;
	CCommandBarCtrl m_CmdBar;

	int m_TimerCount;
	int m_RefreshCount;
	
	CPUsage *m_pCPU;
	NetUsage *m_pNet;

	CServerSummary * m_hSummary;

	CMenu  m_NetPopup;
	DWORD m_CurrentSelNetIfIndex;

	CMultiPaneStatusBarCtrl m_sb;

	vector< std::vector<MIB_TCPROW_OWNER_PID> * > m_PreVec;

	virtual BOOL PreTranslateMessage(PMSG pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_SERVERS_RESTART,UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_SERVERS_RESTARTALL,UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_WINDOW_CLOSE,UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_WINDOW_CLOSE_ALL,UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_REFRESHTIME_1SECOND,UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_REFRESHTIME_2SECONDS,UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_REFRESHTIME_5SECONDS,UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_REFRESHTIME_10SECONDS,UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_REFRESHTIME_STOP,UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_TOFRAME, OnFrameMessage)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(ID_WINDOW_CLOSE, OnWindowClose)
		
		COMMAND_ID_HANDLER(ID_WINDOW_CLOSE_ALL, OnWindowCloseAll)
		COMMAND_RANGE_HANDLER(ID_WINDOW_TABFIRST, ID_WINDOW_TABLAST, OnWindowActivate)
		COMMAND_ID_HANDLER(ID_REFRESHTIME_2SECONDS, OnRefreshtime2seconds)
		COMMAND_ID_HANDLER(ID_REFRESHTIME_1SECOND, OnRefreshtime1second)
		COMMAND_ID_HANDLER(ID_REFRESHTIME_5SECONDS, OnRefreshtime5seconds)
		COMMAND_ID_HANDLER(ID_REFRESHTIME_10SECONDS, OnRefreshtime10seconds)
		COMMAND_ID_HANDLER(ID_REFRESHTIME_STOP, OnRefreshtimeStop)
		COMMAND_ID_HANDLER(ID_REFRESHTIME_REFRESHNOW, OnRefreshtimeRefreshnow)
		COMMAND_ID_HANDLER(ID_SERVERS_RESTART, OnServersRestart)
		COMMAND_ID_HANDLER(ID_SERVERS_RESTARTALL, OnServersRestartall)
		NOTIFY_CODE_HANDLER(TBVN_PAGEACTIVATED,      OnPageActiveted)
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)


	BOOL NewPage(Utils::ProcessInfo & Info);
	BOOL RemovePage( int n);
	void RefreshConn();
	void RefreshStatusBar();
	void ClearMenu();

	LRESULT OnStatusBarLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
	LRESULT OnPageActiveted( int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/ );
		
	LRESULT OnFrameMessage(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnWindowClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnWindowCloseAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnWindowActivate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnRefreshtime2seconds(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnRefreshtime1second(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnRefreshtime5seconds(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnRefreshtime10seconds(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnRefreshtimeStop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnRefreshtimeRefreshnow(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnServersRestart(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnServersRestartall(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
