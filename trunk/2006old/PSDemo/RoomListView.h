#pragma once

class CRoomListView :public CBaseView, public CWindowImpl<CRoomListView>
	, public CMessageFilter

{
public:
	DECLARE_WND_CLASS(NULL)

	enum Colume
	{


	COL_ID,
	COL_NAME,
	COL_BNEEDPASS,
	COL_AREAID,
	COL_ROOMTYPE,
	COL_STATUS,
	COL_USERNUM,
	COL_CS,
		
	};


	BOOL PreTranslateMessage(MSG* pMsg);


	CSortListViewCtrl m_rlist;

	_dword AreaID;

	BEGIN_MSG_MAP(CRoomListView)
		//CHAIN_MSG_MAP(CMDIChildWindowImpl<CRoomListView>)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_FORWARDMSG, OnForwardMsg)

		COMMAND_ID_HANDLER(ID_POP_CREATEROOM, OnCreateRoom)
		COMMAND_ID_HANDLER(ID_POP_REFRESH, OnRefresh)

		//WM_NOTIFY_REFLECTOR()
		// list
		NOTIFY_CODE_HANDLER(NM_DBLCLK, OnRoomDBClick)
		
	END_MSG_MAP()

public:
	void SetRooms(const Array<stCP_RoomInfo> & list);

	_dword getType() const;

public:
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
public:
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnForwardMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	
	LRESULT OnCreateRoom(WORD Code, WORD nID, HWND hwnd, BOOL & bHandled);
	LRESULT OnRefresh(WORD Code, WORD nID, HWND hwnd, BOOL & bHandled);

	LRESULT OnRoomDBClick(int , LPNMHDR pnmh, BOOL& );
};
