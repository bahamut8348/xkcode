#pragma once

class CLoginDlg : public CDialogImpl<CLoginDlg>, public CUpdateUI<CLoginDlg>,
	public CMessageFilter, public CIdleHandler, public IClientUI
	,public CWinDataExchange<CLoginDlg>
	, public CEditValid<CLoginDlg, RGB(255,0,0)>

{
public:

	typedef CEditValid<CLoginDlg, RGB(255,0,0)> EditValidBase;

	enum { IDD = IDD_LOGIN };
	
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle();


	CConfig m_cfg;

	stAP_CLIENT_INFO m_RegInfo;
	
	CMenuButton m_mbtn;
	
	

public:
	_void OnConnectIS	(_bool bConnect, const PSUID is_id, _dword Reason, const _char *desp = _null);
	_void OnFSListRecv	(_dword Callback,const Array<stIP_FSNode> & FsList);
	_void OnValidateResult(_dword byCode);
	_void OnCreateSessionResult(_dword dwCode);





	virtual _bool OnSystemError(_bool bNeedChangeIS) { return _false;};

	virtual _void OnBroadCastMsg(const _char *msg){};


	virtual _void OnRegisterResult(_dword Callback, const RegisterResult & Result);

	virtual _void OnISListRecv(_dword Callback, _dword Result, const Array<ISNode> & IsList) {};;
	virtual _void ShowLSInfo(_bool bShow, const PSUID& ls_id, _dword dwServerType,_dword ls_ver) {};;
	virtual _void ShowLocalInfo(_bool bOnline, const PSUID& local_id) {};;



	virtual _void OnAreaListRecv(_dword Result, const Array<stCP_AreaInfo> & list, _dword AreaID) {};;
	virtual _void OnRoomListRecv(_dword Result, _dword AreaID,_byte Num, const Array<stCP_RoomInfo> & RoomList, _dword RoomType) {};;
	virtual _void OnUserListRecv(_dword RoomID, _byte SyncType, _byte Num, const Array<stCP_UserInfo> & UserList) {};;
	virtual _void OnCreateRoomRecv(_dword Callback, _dword Result, const RoomInfo & Info){};;
	virtual _void OnJoinRoomRecv(_dword Callback,	_dword Result, const RoomInfo & Info){};;
	virtual _void OnRecvChatMsg(_byte datatype, _dword RoomID, const PSUID & ToPSUID, const PSUID & FromPSUID, const _char *Msg) {}; ;
	virtual _void OnUserInvited	(const PSUID & InviterPSUID, const _char *InviterName,const _char * RoomName,   _dword RoomID,_bool usepass, const _char *password){};;
	virtual _void OnInviteUserReturn(const PSUID & InviteeID, const _char * InviteeName, _dword Result,
		const _char *RoomName , _dword RoomID, const _char *reason){};

	virtual _void OnQuitRoomResult(_dword Callback, _dword Result,  _dword RoomID){};

	virtual _void OnSetRoomInfoResult(_dword Callback, _dword Result, _dword RoomID){};
	virtual _void OnRoomInfoChange(_dword RoomID, const CP_Cmd_RoomInfo & info){};

	virtual _void OnSetUserInfoResult(_dword Callback, _dword Result, _dword RoomID){};
	virtual _void OnGameUserInfoChange(_dword RoomID, const stCP_GameUserInfo & Info){};

	virtual _void OnSendGameCommandResult(_dword Callback, _dword Result, _dword RoomID){};	
	virtual _void OnGameCommandRecv(_dword RoomID, const stCP_Cmd_GameCmd & cmd){};

	virtual _void OnStartGameResult(_dword Callback, _dword Result, _dword RoomID){};
	virtual _void OnStartGameRecv(_dword RoomID, const  stCP_Cmd_StartGame & StartGameInfo){};

	virtual _void OnMatchGameResult(_dword RoomID,const  stCP_Cmd_MatchResult & MatchResult){};



	virtual _void OnQuerySession(_dword Callback, _dword result, const PSUID & Psuid,  _byte SessionType, _byte SessionSize, 
		const _byte *info) {};;


	virtual _void QueryBuddySession(_dword Callback, const PSUID & BuddyPSUID,  _dword SessionType) {};;


	virtual _void OnDeleteBuddyRes(_dword Callback, _dword Result, _dword BuddyAccoutID) {};;
	virtual _void OnAddBuddyRes(_dword Callback, _dword Result, const _char *BuddyNick, 
		_dword BuddyAccID, const PSUID & BuddyUid) {};;


	virtual _void OnBuddyListRecv(_dword _Result, _dword Callback,  const Array<BUDDY_INFO> & BuddyList) {};;

	virtual _void OnBuddyStateChange(_dword ChangeType, const PSUID & BuddyPSUID,
		_dword BuddyAccountID) {};;

	virtual _void OnAddBuddyReq(_dword Callback, const _char *NickName, const _char *Reason,
		const PSUID & BuddyID, _dword BuddyAccID) {};;


	virtual _void OnQueryAccountInfoRecv(_dword dwCallback, const _dword Result, 
		const stAP_CLIENT_INFO * pUserInfo) {};;

	virtual _void OnModifyAccountInfoRecv(_dword dwCallback, _dword ErrorNum, _dword *Result) {};;
	virtual _void OnPasswordChangeRecv(_dword dwCallback, _dword ErrorNum, _dword *Errors) {};;

	virtual _void OnSendPacketErrorRecv(_dword Result) {};


	/************************************************************************/
	/* Game Interface                                                       */
	/************************************************************************/
	virtual _void OnLoginMMSResult( _dword Result){};
	virtual _void OnDispatchGame(_dword GameID, _word MaxUserNum);





	BEGIN_UPDATE_UI_MAP(CLoginDlg)
		UPDATE_ELEMENT(IDC_BUTTON4, UPDUI_CHILDWINDOW)
		UPDATE_ELEMENT(ID_LOGIN, UPDUI_CHILDWINDOW)
		UPDATE_ELEMENT(IDC_REGISTER, UPDUI_CHILDWINDOW)
		UPDATE_ELEMENT(IDC_APPID, UPDUI_CHILDWINDOW)
		UPDATE_ELEMENT(IDC_SUBID, UPDUI_CHILDWINDOW)
		UPDATE_ELEMENT(IDC_COMBO1, UPDUI_CHILDWINDOW)

	END_UPDATE_UI_MAP()


	BEGIN_DDX_MAP(CLoginDlg)
		//DDX_CONTROL(IDC_USERNAME, mc_UserName)
	END_DDX_MAP()


	BEGIN_MSG_MAP(CLoginDlg)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_MOVE, OnMove)
		COMMAND_HANDLER(IDC_COMBO1, CBN_SELCHANGE, OnSelectServer)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_HANDLER(IDC_BUTTON4, BN_CLICKED, OnBnConnect)
	
		COMMAND_ID_HANDLER(ID_POP_LOBBYSERVERS, OnBnLobbyServer)
		COMMAND_ID_HANDLER(ID_POP_FILESERVERS, OnBnFileServer)
		COMMAND_ID_HANDLER(ID_POP_GAMELOGIN, OnGame)
		COMMAND_ID_HANDLER(ID_POP_SHOWLOG, OnBnShowLog)

		COMMAND_HANDLER(ID_LOGIN, BN_CLICKED, OnBnLogin)
		COMMAND_HANDLER(IDC_Game, BN_CLICKED, OnGame)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnCancel)
		COMMAND_HANDLER(IDC_REGISTER, BN_CLICKED, OnBnClickedRegister)
		CHAIN_MSG_MAP( EditValidBase )

		//REFLECT_NOTIFICATIONS()
		//CHAIN_MSG_MAP_MEMBER(mc_UserName)
	END_MSG_MAP()


public:
	

public:
	void MoveLogDlg();

public:
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSelectServer(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

public:
	LRESULT OnBnConnect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnShowLog(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnLobbyServer(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnFileServer(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnLogin(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnGame (WORD nCode, WORD nID, HWND hwnd, BOOL & bHandled);
public:
	LRESULT OnBnClickedRegister(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
