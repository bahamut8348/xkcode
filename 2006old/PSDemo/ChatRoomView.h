#pragma once

class CChatRoomView : public CBaseView , public CFrameWindowImpl<CChatRoomView>
	, public CUpdateUI<CChatRoomView>,public CMessageFilter, public CIdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(L"ChatRoomView", IDR_ROOMTB)
	

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual _dword getType() const;
	virtual BOOL OnIdle();

	virtual _void InitToolBar(HWND toolbar ){}
	
	CSplitterWindow  m_vSplit;
	CHorSplitterWindow m_hSplit;

	CHorSplitterWindow m_chatSplit;

	CPaneContainer m_userPane;
	CListViewCtrl	m_userList;
	CRichEditCtrl	m_ChatEdit;
	//CBitmapButton;
	_dword m_RoomID;

	CTabbedPopupFrame<CDotNetTabCtrl<CTabViewTabItem> > m_ChatFrame;

	CSendMsgDlg  *m_pSendDlg;



public:
	void AttachUsers(const Array<stCP_UserInfo> & list);

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		//UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		//UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()
		
	BEGIN_MSG_MAP(CChatRoomView)
		
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_FORWARDMSG, OnForwardMsg)
		MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)

		COMMAND_ID_HANDLER(IDC_BNSEND, OnClickSend)
		COMMAND_ID_HANDLER(ID_ROOM_QUIT, OnQuitRoom);
		NOTIFY_CODE_HANDLER(TTN_NEEDTEXT, OnToolbarNeedText)

		CHAIN_MSG_MAP(CUpdateUI<CChatRoomView>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CChatRoomView>)
	END_MSG_MAP()


public:
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnForwardMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	
	LRESULT OnClickSend(UINT w, UINT l, HWND hwnd, BOOL& /*bHandled*/);
	LRESULT OnQuitRoom(UINT w, UINT l, HWND hwnd, BOOL& /*bHandled*/);

	LRESULT OnToolbarNeedText(int , LPNMHDR lParam, BOOL & bHandled);


};
