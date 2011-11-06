// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "PSDemoView.h"
#include "LogEdit.h"
#include "LogWindow.h"
#include "Mytabview.h"
#include "CreateRoomDlg.h"

#include "SendMsgDlg.h"
#include "RoomListView.h"
#include "ChatRoomView.h"
#include "LobbyServerdlg.h"
#include "FileServerDlg.h"

#include "CommonContrl.h"
#include "ValidEdit.h"
#include "RegisterDlg.h"
#include "DetailInfoDlg.h"
#include "GameRoomView.h"
#include "ChangePassDlg.h"

#include "MainFrm.h"


BOOL CMainFrame::PreTranslateMessage(LPMSG pMsg)
{
    if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg) == TRUE)
		return TRUE;
	if(pMsg->hwnd == m_hWndStatusBar && pMsg->message == WM_LBUTTONDOWN)
	{
		OnStatusBarLButtonDown(pMsg->message, pMsg->wParam, pMsg->lParam);
	}
	
	return m_view.PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle()
{
//	UIUpdateToolBar();
	//GetFramework()->Update();
	UISetCheck(ID_VIEW_LOG, m_logSplit.GetSinglePaneMode() == SPLIT_PANE_NONE);
	m_sb.SetPaneText(IDI_IP, m_IpInfo.strs[m_IpInfo.CurSel]);
	return FALSE;
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	GdiplusStartupInput gdiplusStartupInput; 
	ULONG_PTR gdiplusToken; 
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL); 
	
	//m_docklog.Create(m_hWnd, rcDefault);
	//m_logedit.Create(m_docklog, rcDefault);
	//m_docklog.SetExtendedDockStyle(DCK_EX_DESTROYONCLOSE);
	m_RetCode = 1;
	::SetTimer(m_hWnd, 1,100, NULL);

	m_Detaildlg.Create(m_hWnd);

	RECT rect={0,0,100,100};
	m_logwin.Create(m_hWnd,rect, L"LogWin", WS_CHILD | WS_VISIBLE);

	GetFramework()->SetLogHandle(&m_logwin.m_logedit);
	
	GetFramework()->SetUIClient(this);
	GetProcessor()->QueryAreaList(0, CP_Root_Area_ID);
	GetProcessor()->QueryBuddy(0);
	
	// create command bar window
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	// attach menu
	m_CmdBar.AttachMenu(GetMenu());
	// load command bar images
	m_CmdBar.LoadImages(IDR_MAINFRAME);
	// remove old menu
	SetMenu(NULL);
	//m_CmdBar.m_bFlatMenus = TRUE;
	//m_CmdBar.m_bAlphaImages = TRUE;
	m_CmdBar.AddIcon(IDI_CLOSEROOMLIST, ID_APP_EXIT);
	

	//HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	//AddSimpleReBarBand(hWndToolBar, NULL, TRUE);
	

	CreateSimpleStatusBar();
	m_sb.Attach(m_hWndStatusBar);
	int panels[] = {IDI_START, IDI_ACCOUNTID, IDI_psuid, IDI_IP, IDI_VERSION};
	
	m_sb.SetPanes(panels, sizeof(panels)/sizeof(panels[0]), FALSE);
	int widths[] = {180, 150,160,250, 100};
	for(int a=0; a<sizeof(panels)/sizeof(panels[0]); a++)
	{
		m_sb.SetPaneWidth(panels[a], widths[a]);
	}

	CString s;
	s.Format(L"LocalIP: %s", ClientCommon::Address2Str(GetFramework()->GetLocalIP()));
	m_IpInfo.strs.SetAt(ID_POP_LOCALIP, s);
	s.Format(L"PulicIP: %s", ClientCommon::Address2Str(GetFramework()->GetPublicIP()));
	m_IpInfo.strs.SetAt(ID_POP_PUBLICIP, s);
	s.Format(L"IS IP: %s", ClientCommon::Address2Str(GetFramework()->GetRemoteIP()));
	m_IpInfo.strs.SetAt(ID_POP_ISIP, s);
	m_IpInfo.CurSel = ID_POP_ISIP;
	
	s.Format(IDI_ACCOUNTID, GetFramework()->GetAccountID());
	m_sb.SetPaneText(IDI_ACCOUNTID, s);

	s.Format(IDI_psuid, 
		GetFramework()->GetLocalPSUID().ToString());
	m_sb.SetPaneText(IDI_psuid, s);


//	UIAddToolBar(hWndToolBar);
//	UISetCheck(ID_VIEW_TOOLBAR, 1);
	UISetCheck(ID_VIEW_STATUS_BAR, 1);
	CRect rcVert;
	GetClientRect(&rcVert);

	m_logSplit.Create(m_hWnd, rcVert, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	m_logSplit.m_bFullDrag = FALSE;
	m_logwin.SetParent(m_logSplit);

	m_vertSplit.Create(m_logSplit, rcVert, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	m_vertSplit.m_cxyMin = 35; // minimum size
	m_vertSplit.SetSplitterPos(150); // from left
	m_vertSplit.m_bFullDrag = false; // ghost bar enabled

	m_vertSplit.SetSplitterExtendedStyle ( SPLIT_NONINTERACTIVE );

	m_logSplit.SetSplitterPanes(m_vertSplit, m_logwin);
	m_logSplit.SetSplitterPos(rcVert.Height() - 100);

	RECT  rcP1;
	m_vertSplit.GetSplitterPaneRect(1, &rcP1);

	m_view.Create(m_vertSplit, rcP1, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);
	m_vertSplit.SetSplitterPane(1, m_view);

	RECT rcP0;
	m_vertSplit.GetSplitterPaneRect(0, &rcP0);
	m_horSplit.Create(m_vertSplit, rcVert, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	m_horSplit.m_cxyMin = 50;
	m_horSplit.SetSplitterPos(250);
	m_horSplit.m_bFullDrag = false;

	m_vertSplit.SetSplitterPane(0, m_horSplit);

	m_aPane.Create(m_horSplit);
	m_aPane.SetTitle(L"Area List");
	m_aPane.SetPaneContainerExtendedStyle ( PANECNT_NOCLOSEBUTTON );
	m_horSplit.SetSplitterPane(0, m_aPane);

	m_aTree.Create(m_aPane, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | 
		TVS_HASLINES | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,
		WS_EX_CLIENTEDGE);

	m_aPane.SetClient(m_aTree);

	m_bPane.Create(m_horSplit);
	m_bPane.SetTitle(L"Buddy");
	m_bPane.SetPaneContainerExtendedStyle ( PANECNT_NOCLOSEBUTTON );
	m_horSplit.SetSplitterPane(1, m_bPane);
	m_bList.Create(m_bPane, rcDefault, NULL,WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | 
		LVS_REPORT | LVS_AUTOARRANGE | LVS_SHOWSELALWAYS | LVS_SHAREIMAGELISTS |LVS_SINGLESEL |LVS_NOSORTHEADER ,
		WS_EX_CLIENTEDGE);
	
	CImageList bil;
	bil.Create(24,24,ILC_COLOR32,5,5);
	bil.AddIcon(AtlLoadIcon(IDI_OFFLINE));
	bil.AddIcon(AtlLoadIcon(IDI_ONLINE));
	m_bList.SetImageList(bil, LVSIL_SMALL);
	
	m_bPane.SetClient(m_bList);
	struct ColumnHeader
	{
		_char *name;
	} headers[] = {L"Name            ", L"AccountID", L"PSUID"};

	for(_dword a=0; a<(sizeof(headers)/sizeof(headers[0])); a++)
	{
		m_bList.InsertColumn(a, headers[a].name);
	}
	gResizeList(m_bList);


	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	//CMenuHandle menuMain = m_CmdBar.GetMenu();
	//m_view.SetWindowMenu(menuMain.GetSubMenu(WINDOW_MENU_POSITION));

	CImageList il;
	il.Create(16,16,ILC_COLOR32,5,5);
	il.AddIcon(AtlLoadIcon(IDI_SERVER));
	il.AddIcon(AtlLoadIcon(IDI_ROOM));
	il.AddIcon(AtlLoadIcon(IDI_STARTGAME));
	m_view.SetImageList(il);
	m_aTree.SetImageList(il);
	//m_view.SetTitleBarWindow(m_hWnd);

	m_hWndClient = m_logSplit;

	//m_docklog.SetClient(m_hWndClient);
	//m_docklog.AddWindow(m_logwin);
	//m_docklog.DockWindow(m_logwin, DOCK_BOTTOM);
	
	SetWindowText(GetFramework()->GetMyUserName());

	//UIEnable(ID_FILE_PERINFO, FALSE);

	SendMessage(WM_COMMAND, MAKEWPARAM(ID_FILE_CREATEAGAMEROOM, 0));
	
	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// unregister message filtering and idle updates
	m_sb.Detach();
	PostQuitMessage(2);
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	ULONG_PTR gdiplusToken = NULL; 
	GdiplusShutdown(gdiplusToken);

	
	bHandled = FALSE;
	return 2;
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
{
	//OnDestroy(0,0,0,bHandled);
	m_RetCode = 2;
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static BOOL bVisible = TRUE;	// initially visible
	bVisible = !bVisible;
	CReBarCtrl rebar = m_hWndToolBar;
	int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
	rebar.ShowBand(nBandIndex, bVisible);
	UISetCheck(ID_VIEW_TOOLBAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
	::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainFrame::OnWindowClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int nActivePage = m_view.GetActivePage();
	if(nActivePage != -1)
		m_view.RemovePage(nActivePage);
	else
		::MessageBeep((UINT)-1);

	return 0;
}

LRESULT CMainFrame::OnWindowCloseAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_view.RemoveAllPages();

	return 0;
}

LRESULT CMainFrame::OnWindowActivate(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int nPage = wID - ID_WINDOW_TABFIRST;
	m_view.SetActivePage(nPage);

	return 0;
}

PSDash::_void CMainFrame::OnAreaListRecv(_dword Result, const Array<stCP_AreaInfo> & list, _dword AreaID )
{
	if(Result != CP_E_QuerySuccess)
	{	
		AtlMessageBox(m_hWnd, (LPCTSTR)GetFramework()->GetError(Result));
		return ;
	}
	Array<stCP_AreaInfo> areas = GetProcessor()->GetAreasIn(AreaID);
	if(AreaID == CP_Root_Area_ID)
	{
		m_aTree.DeleteAllItems();
		
		for(_dword a = 0; a<areas.Number(); a++)
		{
			const stCP_AreaInfo & Info = areas[a];
			CTreeItem item = m_aTree.InsertItem(Info.Desp, TVI_ROOT, TVI_LAST);
			item.SetData(Info.ID);
			
		}
		
	}
	else
	{
		HTREEITEM item = GetItemByUserData(m_aTree, AreaID);
		AttachAreaToTree(item, areas);
		TreeView_Expand(m_aTree.m_hWnd,item,TVM_EXPAND);
	}
}

LRESULT CMainFrame::OnViewLog( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	m_logSplit.SetSinglePaneMode( 
		m_logSplit.GetSinglePaneMode() == SPLIT_PANE_TOP?
		SPLIT_PANE_NONE
		:SPLIT_PANE_TOP);

	
	
	return 0;
}
LRESULT CMainFrame::OnViewLobbyservers(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your command handler code here
	CLobbyServerdlg dlg;
	dlg.m_servers = GetFramework()->GetLobbyServers();
	dlg.DoModal();
	
	return 0;
}


LRESULT CMainFrame::OnTVItemExpanding( int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/ )
{
	return 0;
}

LRESULT CMainFrame::OnLVItemDBClick( int CtrlID, LPNMHDR pnmh, BOOL& )
{
	if(pnmh->hwndFrom == m_aTree)
	{
		CTreeItem item = m_aTree.GetSelectedItem();
		u_int b = item.GetData();
		_dword depth = GetProcessor()->GetAreaDepth(b);
		if(depth == 1)
		{
			Array<stCP_AreaInfo> list = GetProcessor()->GetAreasIn(b);
			if(list.Number() == 0)
			{
				GetProcessor()->QueryAreaList(0, b);
			}
			else
			{
				AttachAreaToTree(item, list);

			}
		}
		else if(depth == 2)
		{
			Array<stCP_RoomInfo> list = GetProcessor()->GetRoomsIn(b, -1);
			if(list.Number() == 0)
			{
				GetProcessor()->QueryRoomList(0, b);
			}
			else
			{
				int a = NewRoomListPage(b);
				m_view.SetActivePage(a);
			}

		}
	}
	
	

	return 0;
}

void CMainFrame::AttachAreaToTree( HTREEITEM item, const Array<stCP_AreaInfo> &  list )
{
	HWND tree = m_aTree.m_hWnd;
	while(1)
	{
		HTREEITEM citem = TreeView_GetChild(tree,item);
		if(citem == NULL) break;
		TreeView_DeleteItem(tree,citem);
	}
	
	for(u_int a=0; a<list.Number(); a++ )
	{
		TVINSERTSTRUCT tvst;
		ZeroMemory(&tvst, sizeof(tvst));
		tvst.itemex.pszText = (LPWSTR)list[a].Desp;
		tvst.itemex.cchTextMax = 255;
		tvst.itemex.lParam = list[a].ID;
		tvst.itemex.iImage = 0;
		tvst.itemex.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE;
		tvst.hInsertAfter = TVI_LAST;
		tvst.hParent = item;
		TreeView_InsertItem(tree, &tvst);
	}
}

PSDash::_void CMainFrame::OnRoomListRecv( _dword Result, _dword AreaID, _byte Num, const Array<stCP_RoomInfo> & RoomList, _dword RoomType )
{
	if(Result != CP_E_QuerySuccess)
	{	
		AtlMessageBox(m_hWnd, (LPCTSTR)GetFramework()->GetError(Result));
		return ;
	}

	Array<stCP_RoomInfo> list = GetProcessor()->GetRoomsIn(AreaID, -1);
	int a = NewRoomListPage(AreaID);
	AttachRoomToAPage(a, list);


}

void CMainFrame::AttachRoomToAPage( int a, const Array<stCP_RoomInfo> & list )
{
	CRoomListView *pview = (CRoomListView *)m_view.GetPageData(a);
	m_view.GetPageHWND(a);
	pview->SetRooms(list);
}

int CMainFrame::NewRoomListPage( _dword AreaID )
{
	for(int a =0; a< m_view.GetPageCount(); a++)
	{
		
		CBaseView *pview = (CBaseView *)m_view.GetPageData(a);
		if(pview->getType() == BV_ROOMLIST)
		{
			CRoomListView *prView = (CRoomListView *)pview;
			if(prView->AreaID == AreaID)
			{
				return a;
				break;
			}
		}
	}
	CRoomListView *pView = new CRoomListView;
	
	pView->Create(m_view, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	m_view.AddPage(pView->m_hWnd, GetProcessor()->GetAreaInfo(AreaID).Desp, 0, (LPVOID)pView);
	pView->AreaID = AreaID;
	return m_view.GetActivePage();
}


struct SendUserInfoData
{
	IClientFramework *mClient;
	CP_Cmd_GameRoomUserInfo UserInfo;
	SendUserInfoData(IClientFramework *client, const CP_Cmd_GameRoomUserInfo & Info)
	{
		mClient = client ;
		UserInfo = Info;
	}
	_void operator () (const stCP_RoomInfo & _RoomInfo) const
	{
		mClient->GetProcessor()->SetGameUserInfo(0, _RoomInfo.ID, UserInfo);
	}
};

PSDash::_void CMainFrame::OnJoinRoomRecv( _dword Callback, _dword Result, const stCP_RoomInfo & Info )
{
	if(Result == CP_E_JoinRoom_Succ)
	{
		CChatRoomView *pView = new CChatRoomView;
		pView->Create(m_view, rcDefault, NULL,WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
		m_view.AddPage(pView->m_hWnd, Info.Desp, 1, (LPVOID)pView);
		Array<stCP_UserInfo> users = GetProcessor()->GetRoomUsers(Info.ID);
		pView->AttachUsers(users);
		pView->m_RoomID = Info.ID;

		SendAllRoomMyInfo();
	}
	else
	{
		MessageBox(GetFramework()->GetError(Result),L"", 64);
	}
}

PSDash::_void CMainFrame::OnUserListRecv( _dword RoomID, _byte SyncType, _byte Num, const Array<stCP_UserInfo> & UserList )
{

	int a = GetRoomPageByID(RoomID);
	CChatRoomView *pChatView = (CChatRoomView *) m_view.GetPageData(a);
	ATLASSERT(pChatView);
	Array<stCP_UserInfo> users = GetProcessor()->GetRoomUsers(RoomID);
	pChatView->AttachUsers(users);

	
}


_void CMainFrame::OnBuddyListRecv(_dword _Result, _dword Callback,  const Array<BUDDY_INFO> & BuddyList)
{
	AttachBuddyToBList(BuddyList);
}


#define ITOS(i) ::_itow(i, tmp, 10)
void CMainFrame::AttachBuddyToBList(const Array<BUDDY_INFO> & list)
{
	m_bList.SetRedraw(FALSE);
	m_bList.DeleteAllItems();
	_char tmp[100];
	for(_dword a=0; a<list.Number(); a++)
	{
		
		int n =m_bList.InsertItem(a, list[a].BuddyNick,list[a].State ==Buddy_Offline?0:1);
		m_bList.SetItemData(n, list[a].AccID);
		m_bList.SetItemText(a, 1, ITOS(list[a].AccID));
		m_bList.SetItemText(a, 2, ClientCommon::PSUID2Str(list[a].BuddyID));
	}
	m_bList.SetRedraw(TRUE);

}

_dword CMainFrame::GetRoomPageByID( _dword RoomID )
{
	_dword ret = -1;
	for(int a=0; a<m_view.GetPageCount(); a++)
	{
		CBaseView *pView = (CBaseView *)m_view.GetPageData(a);
		if( (pView->getType() == BV_CHAT) || (pView->getType()==BV_GAME) )
		{
			CChatRoomView *pChatView = (CChatRoomView *)pView;
			if(pChatView->m_RoomID == RoomID)
			{
				ret = a;
				break;
			}
		}
	}
	return ret;
}
#undef ITOS
_void CMainFrame::OnRecvChatMsg(_byte datatype, _dword RoomID, const PSUID & ToPSUID, const PSUID & FromPSUID, const _char *Msg)
{

	stCP_UserInfo *pUser = GetProcessor()->GetRoomUser(FromPSUID, RoomID);
	CChatRoomView *pChatView = (CChatRoomView *)m_view.GetPageData(GetRoomPageByID(RoomID));
	CString s;
	if(ToPSUID.Value() == 0)
	{
		s.Format(L"%s : %s\r\n",  pUser->NickName, Msg);
	}
	else
	{
		s.Format(L"%s Said To %s: %s\r\n",  pUser->NickName, 
			pUser->NickName, 
			Msg);
	}

	
	pChatView->m_ChatEdit.AppendText(s);
	pChatView->m_ChatEdit.SendMessage(WM_VSCROLL,SB_BOTTOM,0);
	FlashWindow(_false);
}

PSDash::_void CMainFrame::OnAddBuddyReq( _dword Callback, const _char *NickName, const _char *Reason,
										const PSUID & BuddyID, _dword BuddyAccID )
{
	CString s;
	s.Format(L"%s want add you to buddy %s, click yes to agree", NickName, Reason);
	FlashWindow(TRUE);
	int ret = MessageBox(s, L"Info", MB_ICONINFORMATION | MB_YESNO);
	if(ret == IDYES)
	{
		GetProcessor()->HandleApplyBuddy(0, E_MODIFY_SUCCSS, L"", BuddyID,BuddyAccID, NickName);
		AttachBuddyToBList(GetProcessor()->GetBuddyList());
	}
	else if(ret == IDNO)
	{
		GetProcessor()->HandleApplyBuddy(0, E_MODIFY_USER_REJECT, L"", BuddyID, BuddyAccID, NickName);
	}
}

PSDash::_void CMainFrame::OnBuddyStateChange( _dword ChangeType, const PSUID & BuddyPSUID, _dword BuddyAccountID )
{
	AttachBuddyToBList(GetProcessor()->GetBuddyList());
}

PSDash::_void CMainFrame::OnAddBuddyRes( _dword Callback, _dword Result, const _char *BuddyNick, _dword BuddyAccID, const PSUID & BuddyUid )
{
	if(Result == E_MODIFY_USER_REJECT)
	{
		CString s;
		s.Format(L"%s Reject your request", BuddyNick);
		AtlMessageBox(m_hWnd, (LPCTSTR)s);
	}
	else if(E_MODIFY_SUCCSS == Result)
	{
		CString s;
		s.Format(L"%s agree to do your buddy", BuddyNick);
		AtlMessageBox(m_hWnd, (LPCTSTR)s, L"Success");
		AttachBuddyToBList(GetProcessor()->GetBuddyList());
	}
	else
	{
		CString s;
		s.Format(L"add buddy error: %s", GetFramework()->GetError(Result));
		AtlMessageBox(m_hWnd, (LPCTSTR)s);
	}
}

PSDash::_void CMainFrame::OnBroadCastMsg( const _char *msg )
{
	AtlMessageBox(m_hWnd, (LPCTSTR)msg, L"BroadCastMessage");
}

PSDash::_void CMainFrame::OnUserInvited( const PSUID & InviterPSUID, const _char *InviterName,const _char * RoomName, _dword RoomID, const PSUID & ChatServer, _bool usepass, const _char *password )
{
	CString s;
	s.Format(L"%s Invite You to room %s, do you agree?",InviterName,  RoomName);
	FlashWindow(TRUE);
	int ret = MessageBox(s,L"OnUserInvited", MB_YESNO|MB_ICONINFORMATION);
	if(ret == IDYES)
	{
		GetProcessor()->HandleInviteUser(InviterPSUID, RoomID, CP_INVITEUSER_APPLY, L"");
		GetProcessor()->JoinRoom(0, ChatServer, RoomID, GetFramework()->GetMyUserName(), password);
	}
	else if(ret == IDNO)
	{
		GetProcessor()->HandleInviteUser(InviterPSUID, RoomID, CP_INVITEUSER_REJECT, L"");
	}
}

PSDash::_void CMainFrame::OnInviteUserReturn( const PSUID & InviteeID, const _char * InviteeName, _dword Result, const _char *RoomName , _dword RoomID, const _char *reason )
{
	CString s;
	if(Result == CP_INVITEUSER_APPLY)
	{
		s.Format(L"%s agree to join room %s", InviteeName, RoomName);
	}
	else
	{
		s.Format(L"%s reject to join room %s", InviteeName, RoomName);
	}

	MessageBox(s );
	
}

PSDash::_void CMainFrame::OnQuitRoomResult( _dword Callback, _dword Result, _dword RoomID , const stCP_RoomInfo & RoomInfo)
{
	if(Result == CP_E_QuitRoom_Succ)
	{
		int a = GetRoomPageByID(RoomID);
		CChatRoomView *pView = (CChatRoomView *)m_view.GetPageData(a);
		m_view.RemovePage(a);
		delete  pView;
	}
}

LRESULT CMainFrame::OnContextMenu( UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	POINT pt;
	pt.x = GET_X_LPARAM(lParam); 
	pt.y = GET_Y_LPARAM(lParam); 
	//this->ScreenToClient(&pt);
	//::GetCursorPos(&pt);
	
	HWND hwnd =::WindowFromPoint(pt);
	//HWND hwnd = (HWND)wParam;
	if(hwnd == m_bList)
	{
		LVITEM vi;
		if(FALSE == m_bList.GetSelectedItem(&vi))
			return 0;
		_dword AccID = vi.lParam;
		BuddyInfo_Ex * pBuddy = GetProcessor()->GetBuddyInfo(AccID);
		CMenu m;
		m.LoadMenu(IDR_BUDDY);
		m.GetSubMenu(0).GetSubMenu(1).DeleteMenu(0, MF_BYPOSITION);
		Array<stCP_RoomInfo> list = GetProcessor()->GetJoinedRooms();
		for(_dword a=0; a<list.Number(); a++)
		{
			m.GetSubMenu(0).GetSubMenu(1).InsertMenu(a, 0, WM_ROOMID + list[a].ID, list[a].Desp);
		}
		
		int ret = GetFrame()->m_CmdBar.TrackPopupMenu(m.GetSubMenu(0),
			TPM_RETURNCMD, pt.x, pt.y);
		if(ret == 0) return 0;
		if(ret == ID_POP_REMOVE)
		{
			GetProcessor()->RemoveBuddy(0, AccID);
		}
		else
		{
			_dword roomid = ret -WM_ROOMID;
			GetProcessor()->InviteUserToRoom(pBuddy->BaseInfo.BuddyID, roomid, pBuddy->BaseInfo.BuddyNick);
		}
	}
	else if(hwnd == m_aTree)
	{
		UINT uFlags;
		POINT cpt = pt;
		m_aTree.ScreenToClient(&cpt);
		HTREEITEM hSelItem = m_aTree.HitTest(cpt, &uFlags);
		if ((hSelItem != NULL) && (TVHT_ONITEM & uFlags))
		{
			m_aTree.Select(hSelItem, TVGN_CARET);
		}

		CMenu m;
		m.LoadMenu(IDR_AREA);
		CMenuHandle hm = m.GetSubMenu(0);
		int ret = GetFrame()->m_CmdBar.TrackPopupMenu(hm, TPM_RETURNCMD,
			pt.x, pt.y);
		if(ret == ID_POP_REFRESHAREA)
		{
			if(hSelItem != NULL)
			{
				int AreaID = m_aTree.GetItemData(hSelItem);
				if(GetProcessor()->GetAreaDepth(AreaID) == 1)
					GetProcessor()->QueryAreaList(0, AreaID);
				else if(GetProcessor()->GetAreaDepth(AreaID) == 2)
					GetProcessor()->QueryRoomList(0, AreaID, -1);
			}
		}
		else if(ret == ID_POP_REFRESHALL)
		{
			GetProcessor()->QueryAreaList(0,CP_Root_Area_ID);
		}


	}
	bHandled = FALSE;
	return 0; 
}

PSDash::_void CMainFrame::OnDeleteBuddyRes( _dword Callback, _dword Result, _dword BuddyAccoutID )
{
	CString s;
	if(Result == E_MODIFY_SUCCSS)
	{
		s.Format(L"Delete Buddy Success");
		AttachBuddyToBList(GetProcessor()->GetBuddyList());
	}
	else
	{
		s.Format(L"Delete Buddy Failed, %s", GetFramework()->GetError(Result));
	}

	MessageBox(s);
}

PSDash::_void CMainFrame::OnCreateRoomRecv( _dword Callback, _dword Result, const stCP_RoomInfo & Info )
{
	if(Result == CP_E_CreateRoom_Succ)
	{
		if(Info.RoomType == CP_RoomType_Chat)
		{
			CChatRoomView *pView = new CChatRoomView;
			pView->Create(m_view, rcDefault, NULL,WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
			CString RoomName;
			RoomName.Format(L"%s(%d)", Info.Desp, Info.ID);
			m_view.AddPage(pView->m_hWnd, RoomName, 1, (LPVOID)pView);
			Array<stCP_UserInfo> users = GetProcessor()->GetRoomUsers(Info.ID);
			pView->AttachUsers(users);
			pView->m_RoomID = Info.ID;
		}
		else if(Info.RoomType == CP_RoomType_Game)
		{
			NewGameRoom(Info.ID);
			
		}
		else
		{
			ATLASSERT(_null == L"No Room Type");
		}

		SendAllRoomMyInfo();
		
	}
	else
	{
		AtlMessageBox(m_hWnd, (LPCTSTR)GetFramework()->GetError(Result));
	}
}

LRESULT CMainFrame::OnStatusBarLButtonDown( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam )
{
	int xPos = GET_X_LPARAM(lParam); 
	int yPos = GET_Y_LPARAM(lParam); 
	CRect rc;
	m_sb.GetPaneRect(IDI_IP, &rc);
	if(rc.PtInRect(CPoint(xPos, yPos)))
	{
		CMenu m;
		m.LoadMenu(IDR_SHOWIP);
		CMenuHandle hm = m.GetSubMenu(0);
		MENUITEMINFO mi;
		mi.cbSize = sizeof(mi);
		mi.fMask = MIIM_STRING;
		mi.fType = MFT_STRING;
		
		int fields [] = {ID_POP_LOCALIP, ID_POP_PUBLICIP, ID_POP_ISIP};
		for (int a=0; a<sizeof(fields)/sizeof(fields[0]) ;a++)
		{
			int t = fields[a];
			mi.dwTypeData =m_IpInfo.strs[t].GetBuffer(100);
			mi.cch = 100;
			hm.SetMenuItemInfo(t,  FALSE, &mi);
			m_IpInfo.strs[t].ReleaseBuffer();
			UISetCheck(t,FALSE);
		}

		
		UISetCheck(m_IpInfo.CurSel, TRUE, TRUE);
		m_sb.ClientToScreen(&rc);
		int sel = m_CmdBar.TrackPopupMenu(hm,TPM_RETURNCMD, rc.left, rc.top );
		if(sel)
		{
			m_IpInfo.CurSel = sel;
		}
		
		
		
		
		return 0;
	}
	return 0;
}

LRESULT CMainFrame::OnTimer( UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	if(wParam == 1)
	{
		GetFramework()->Update();
	}

	return 0;
}
LRESULT CMainFrame::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// TODO: Add your message handler code here and/or call default
	DestroyWindow();
	::PostQuitMessage(m_RetCode);
	return 0;
}

PSDash::_void CMainFrame::ShowLSInfo( _bool bShow, const PSUID& ls_id, _dword dwServerType,_dword ls_ver )
{
	if(dwServerType == ST_BS)
	{
		if(bShow)
		{
			GetProcessor()->QueryBuddy(0);
		}
	}
}


_void CMainFrame::OnConnectIS(_bool bConnect, const PSUID is_id, _dword Reason, const _char *desp)
{
	if(bConnect ==_false)
	{
		SendMessage(WM_COMMAND, ID_LOGOUT);
	}
}

LRESULT CMainFrame::OnFileInfo( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	
	GetProcessor()->QueryAccountInfo(GetFramework()->GetLocalPSUID(),0);
	m_Detaildlg.ShowWindow(SW_SHOWNORMAL);
	return 0;
}

PSDash::_void CMainFrame::OnModifyAccountInfoRecv( _dword dwCallback, _dword ErrorNum, _dword *Result )
{
	if(ErrorNum == 0)
	{
		m_Detaildlg.MessageBox(L"Update Account Information Success!", L"",
			MB_OK | MB_ICONINFORMATION);
	}
	else
	{	
		CString ShowMsg;
		for(DWORD a=0; a< ErrorNum; a++)
		{
			CString tmp;
			tmp.Format(L"%d. %s(Error Code: %d)\n", 
				a+1, GetFramework()->GetError(Result[a]), Result[a]);
			ShowMsg += tmp;
		}
		m_Detaildlg.MessageBox(ShowMsg, L"", MB_OK | MB_ICONEXCLAMATION);
	}
}

PSDash::_void CMainFrame::OnQueryAccountInfoRecv( _dword dwCallback, const _dword Result, const stAP_CLIENT_INFO * pUserInfo )
{
	if(Result == E_SP_QUERY_SUCCESS)
	{
		m_Detaildlg.m_RegInfo = *pUserInfo;
		m_Detaildlg.ShowWindow(SW_SHOWNORMAL);
		m_Detaildlg.SendMessage(WM_SHOWWINDOW);
	}
	else
	{
		MessageBox(GetFramework()->GetError(Result), L"Query Account Info Error", MB_OK | MB_ICONEXCLAMATION);
	}
}

LRESULT CMainFrame::OnChangePassword( WORD nCode, WORD nID, HWND hwnd, BOOL & bHandled )
{
	CChangePassDlg dlg;
	int ret = dlg.DoModal(*this);
	if(ret == IDOK)
	{
		GetProcessor()->ChangePassword(0, GetFramework()->GetMyUserName(), dlg.m_OldPassword,
			dlg.m_Password);
	}
	return 0;
}

PSDash::_void CMainFrame::OnPasswordChangeRecv( _dword dwCallback, _dword ErrorNum, _dword *Errors )
{
	if(ErrorNum == 0)
	{
		MessageBox(L"Password Changed", L"", MB_OK | MB_ICONINFORMATION);

	}
	else
	{
		CString ShowMsg;
		for(_dword a=0; a<ErrorNum; a++)
		{
			CString tmp;
			tmp.Format(L"%d. %s\n", a+1, 
				GetFramework()->GetError(Errors[a]));

			ShowMsg += tmp;
		}
		MessageBox(ShowMsg, L"", MB_OK | MB_ICONEXCLAMATION);
	}

}

void CMainFrame::NewGameRoom( _dword RoomID )
{
	CGameRoomView *pView = new CGameRoomView;
	pView->Create(m_view, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	CString GameRoomName;
	GameRoomName.Format(L"%s(%d)", L"Game Room", RoomID);
	m_view.AddPage(pView->m_hWnd, GameRoomName,2, (LPVOID)pView);
	pView->m_RoomID = RoomID;

	Array<stCP_UserInfo> users = GetProcessor()->GetRoomUsers(RoomID);
	pView->AttachUsers(users);
	pView->m_RoomID = RoomID;

}

LRESULT CMainFrame::OnCreateAGameRoom( WORD nCode, WORD nID, HWND hwnd, BOOL & bHandled )
{
	GetProcessor()->CreateRoom(0, CP_Root_Area_ID, GetFramework()->GetMyUserName(), 
		L"GameRoom",0,L"",10,CP_RoomType_Game);
	return 1;
}

PSDash::_void CMainFrame::OnMatchGameResult( _dword Result, _dword RoomID, _dword GameID, _dword Token, const SessionAddress & saddr )
{
	CString s;
	s.Format(L"OnMatchGameResult: %s", GetFramework()->GetError(Result));
	MessageBox(s,L"", 64);
}

PSDash::_void CMainFrame::OnGameUserInfoChange( _dword RoomID, const PSUID & UserID, const CP_Cmd_GameRoomUserInfo & Info )
{
	int a = GetRoomPageByID(RoomID);
	CChatRoomView *pChatView = (CChatRoomView *) m_view.GetPageData(a);
	ATLASSERT(pChatView);
	Array<stCP_UserInfo> users = GetProcessor()->GetRoomUsers(RoomID);
	pChatView->AttachUsers(users);
}

void CMainFrame::SendAllRoomMyInfo()
{
	CP_Cmd_GameRoomUserInfo GameRoomUserInfo;
	stCP_RoomInfo RInfo={0};
	GetProcessor()->GetJoinedGameRoomInfo(&RInfo);
	GameRoomUserInfo.Set(GetFramework()->GetMyUserName(), RInfo.ID, RInfo.ChatServerUID);
	Array<stCP_RoomInfo> JoinedRooms = GetProcessor()->GetJoinedRooms();
	Foreach(JoinedRooms, SendUserInfoData(GetFramework(), GameRoomUserInfo));
}