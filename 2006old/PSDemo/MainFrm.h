// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CommonContrl.h"
#include "ValidEdit.h"
#include "DetailInfoDlg.h"

static _dword GetTreeItemData(HWND tree, HTREEITEM item)
{
	TVITEM tv;
	ZeroMemory(&tv, sizeof(tv));
	tv.hItem = item;
	tv.mask = TVIF_PARAM;
	BOOL ret =  TreeView_GetItem(tree, &tv);
	if(ret)
	{
		return (_dword) tv.lParam;
	}
	return -1;
}

static HTREEITEM GetItemByUserData(HWND tree, HTREEITEM item, _dword data)
{
	if(GetTreeItemData(tree,item) == data)
		return item;

	HTREEITEM citem = TreeView_GetChild(tree , item);
	if(citem == NULL) return NULL;
	HTREEITEM nitem = citem;
	while(1)
	{
		HTREEITEM retItem = GetItemByUserData(tree, nitem, data);
		if(retItem != NULL) return retItem;
		nitem = TreeView_GetNextSibling(tree, nitem);
		if(nitem == NULL) break;
	}

	return NULL;
}

static HTREEITEM GetItemByUserData(CTreeViewCtrlEx & tree, _dword data)
{
	if (tree.GetCount() == 0) return NULL;
	CTreeItem root= tree.GetRootItem();
	HTREEITEM nitem = root.m_hTreeItem;
	while (1)
	{
		HTREEITEM retItem = GetItemByUserData(tree, nitem, data);
		if(retItem != NULL) return retItem;
		nitem = TreeView_GetNextSibling(tree.m_hWnd, nitem);
		if(nitem == NULL) break;
	}

	return NULL;
}

class CMainFrame : public CFrameWindowImpl<CMainFrame>, public CUpdateUI<CMainFrame>,
		public CMessageFilter, public CIdleHandler
		, public IClientUI
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	CMytabview m_view;
	CXKCommandBarCtrl m_CmdBar;
	CSplitterWindow  m_vertSplit;
	CHorSplitterWindow m_horSplit;
	CHorSplitterWindow m_logSplit;

	CPaneContainer m_aPane;
	CPaneContainer m_bPane;

	CTreeViewCtrlEx   m_aTree;
	CListViewCtrl	m_bList;

	CMultiPaneStatusBarCtrl m_sb;

	struct _tagMyInfo
	{
		CAtlMap<int, CString> strs;
		int CurSel;
	}m_IpInfo, m_VerInfo;


	CLogEdit			m_logedit;

	CDetailInfoDlg m_Detaildlg;

	CLogWindow  m_logwin;

	int m_RetCode;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		//UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_POP_ISIP, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_POP_LOCALIP, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_POP_PUBLICIP, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_LOG, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_FILE_PERINFO, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_LOGOUT, OnFileNew)
		COMMAND_ID_HANDLER(ID_FILE_PERINFO, OnFileInfo)
		COMMAND_ID_HANDLER(ID_FILE_CHANGEPASSWORD, OnChangePassword)
		COMMAND_ID_HANDLER(ID_FILE_CREATEAGAMEROOM, OnCreateAGameRoom)
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(ID_WINDOW_CLOSE, OnWindowClose)
		COMMAND_ID_HANDLER(ID_WINDOW_CLOSE_ALL, OnWindowCloseAll)
		COMMAND_ID_HANDLER(ID_VIEW_LOG, OnViewLog)
		COMMAND_RANGE_HANDLER(ID_WINDOW_TABFIRST, ID_WINDOW_TABLAST, OnWindowActivate)
		COMMAND_ID_HANDLER(ID_VIEW_LOBBYSERVERS, OnViewLobbyservers)
	

		// tree
		NOTIFY_CODE_HANDLER(TVN_ITEMEXPANDING, OnTVItemExpanding)
		NOTIFY_CODE_HANDLER(NM_DBLCLK, OnLVItemDBClick)

		
		

		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()



	// network function
	virtual _void OnAreaListRecv(_dword Result, const Array<stCP_AreaInfo> & list, _dword AreaID);
	virtual _void OnRoomListRecv(_dword Result,_dword AreaID, _byte Num, const Array<stCP_RoomInfo> & RoomList, _dword RoomType);
	virtual _void OnJoinRoomRecv(_dword Callback,	_dword Result, const stCP_RoomInfo & Info);
	virtual _void OnUserListRecv(_dword RoomID, _byte SyncType, _byte Num, const Array<stCP_UserInfo> & UserList);

	
	virtual _void OnBuddyListRecv(_dword _Result, _dword Callback,  const Array<BUDDY_INFO> & BuddyList);
	virtual _bool OnSystemError(_bool bNeedChangeIS) { return _false;};
	virtual _void OnBroadCastMsg(const _char *msg);
	virtual _void OnValidateResult(_dword byCode) {};
	virtual _void OnCreateSessionResult(_dword dwCode) {};;
	virtual _void OnRegisterResult(_dword Callback, const RegisterResult & Result) {};;
	virtual _void OnConnectIS(_bool bConnect, const PSUID is_id, _dword Reason, const _char *desp = _null);
	virtual _void OnISListRecv(_dword Callback, _dword Result, const Array<ISNode> & IsList) {};;
	virtual _void OnFSListRecv(_dword Callback,const Array<stIP_FSNode> & FsList) {};;
	virtual _void ShowLSInfo(_bool bShow, const PSUID& ls_id, _dword dwServerType,_dword ls_ver);
	virtual _void ShowLocalInfo(_bool bOnline, const PSUID& local_id) {};;
	virtual _void OnCreateRoomRecv(_dword Callback, _dword Result, const stCP_RoomInfo & Info);
	virtual _void OnRecvChatMsg(_byte datatype, _dword RoomID, const PSUID & ToPSUID, const PSUID & FromPSUID, const _char *Msg);
	virtual _void OnUserInvited	(const PSUID & InviterPSUID, const _char *InviterName,const _char * RoomName,   _dword RoomID, const PSUID & CharServer,_bool usepass, const _char *password);
	virtual _void OnInviteUserReturn(const PSUID & InviteeID, const _char * InviteeName, _dword Result,
		const _char *RoomName , _dword RoomID, const _char *reason);
	virtual _void OnQuitRoomResult(_dword Callback, _dword Result,  _dword RoomID, const stCP_RoomInfo & RoomInfo );
	virtual _void OnSetRoomInfoResult(_dword Callback, _dword Result, _dword RoomID){};
	virtual _void OnRoomInfoChange(_dword RoomID, const CP_Cmd_RoomInfo & info){};
	virtual _void OnSetUserInfoResult(_dword Callback, _dword Result, _dword RoomID){};
	virtual _void OnGameUserInfoChange(_dword RoomID, const PSUID & UserID,  const CP_Cmd_GameRoomUserInfo & Info);
	virtual _void OnSendGameCommandResult(_dword Callback, _dword Result, _dword RoomID){};	
	virtual _void OnGameCommandRecv(_dword RoomID, const stCP_Cmd_GameCmd & cmd){};
	virtual _void OnStartGameResult(_dword Callback, _dword Result, _dword RoomID){};
	virtual _void OnStartGameRecv(_dword RoomID, const  stCP_Cmd_StartGame & StartGameInfo){};
	virtual _void OnMatchGameResult(_dword Result, _dword RoomID, _dword GameID, _dword Token, const SessionAddress & saddr);
	virtual _void OnQuerySession(_dword Callback, _dword result, const PSUID & Psuid,  _byte SessionType, _byte SessionSize, 
		const _byte *info) {};;
	virtual _void QueryBuddySession(_dword Callback, const PSUID & BuddyPSUID,  _dword SessionType) {};;
	virtual _void OnDeleteBuddyRes(_dword Callback, _dword Result, _dword BuddyAccoutID);
	virtual _void OnAddBuddyRes(_dword Callback, _dword Result, const _char *BuddyNick, 
		_dword BuddyAccID, const PSUID & BuddyUid);
	virtual _void OnBuddyStateChange(_dword ChangeType, const PSUID & BuddyPSUID,
		_dword BuddyAccountID);
	virtual _void OnAddBuddyReq(_dword Callback, const _char *NickName, const _char *Reason,
		const PSUID & BuddyID, _dword BuddyAccID);
	virtual _void OnQueryAccountInfoRecv(_dword dwCallback, const _dword Result, 
		const stAP_CLIENT_INFO * pUserInfo);
	virtual _void OnModifyAccountInfoRecv(_dword dwCallback, _dword ErrorNum, _dword *Result);
	virtual _void OnPasswordChangeRecv(_dword dwCallback, _dword ErrorNum, _dword *Errors);
	virtual _void OnSendPacketErrorRecv(_dword Result) {};;


	/************************************************************************/
	/* Game Interface                                                       */
	/************************************************************************/
	virtual _void OnLoginMMSResult( _dword Result) {};
	virtual _void OnDispatchGame(_dword GameID, _word MaxUserNum) {}; 


// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnStatusBarLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
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
	LRESULT OnViewLog(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileInfo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	
	LRESULT OnChangePassword (WORD nCode, WORD nID, HWND hwnd, BOOL & bHandled);
	LRESULT OnCreateAGameRoom (WORD nCode, WORD nID, HWND hwnd, BOOL & bHandled);
	LRESULT OnTVItemExpanding(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/);
	LRESULT OnLVItemDBClick(int , LPNMHDR pnmh, BOOL& );
public:
	LRESULT OnViewLobbyservers(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	

public :  
	void AttachAreaToTree(HTREEITEM item, const Array<stCP_AreaInfo> & list);
	void AttachRoomToAPage(int a, const Array<stCP_RoomInfo> & list);
	int  NewRoomListPage(_dword AreaID);
	void AttachBuddyToBList(const Array<BUDDY_INFO> & list);
	void NewGameRoom(_dword RoomID);
	void SendAllRoomMyInfo();
	_dword GetRoomPageByID( _dword RoomID );
	
public:
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};
