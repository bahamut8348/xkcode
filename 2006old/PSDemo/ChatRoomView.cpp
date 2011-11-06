#include "StdAfx.h"

#include "SendMsgDlg.h"
#include "Mytabview.h"
#include "LogEdit.h"
#include "LogWindow.h"
#include "MainFrm.h"
#include "ChatRoomView.h"

BOOL CChatRoomView::PreTranslateMessage(MSG* pMsg)
{
	if (CFrameWindowImpl<CChatRoomView>::PreTranslateMessage(pMsg))
		return TRUE;

	if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == m_pSendDlg->GetDlgItem(IDC_MSG))
	{
		if(pMsg->wParam == VK_RETURN)
		{
			::SendMessage(m_pSendDlg->m_hWnd,WM_COMMAND, MAKEWPARAM(IDC_BNSEND , BN_CLICKED), 0);
		}
	}

	//::SendMessage(m_chatSplit.GetSplitterPane(1), WM_FORWARDMSG, 0,(LPARAM)pMsg);
	return FALSE;
	
}

PSDash::_dword CChatRoomView::getType() const
{
	return BV_CHAT;
}
LRESULT CChatRoomView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: Add your message handler code here and/or call default
	CreateSimpleReBar(RBS_VERTICALGRIPPER|WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | RBS_TOOLTIPS | RBS_BANDBORDERS | RBS_AUTOSIZE | RBS_BANDBORDERS|RBS_FIXEDORDER  );
	HWND hToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_ROOMTB, 0, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN  | CCS_NODIVIDER | CCS_NORESIZE | CCS_NOPARENTALIGN | TBSTYLE_TOOLTIPS | TBSTYLE_FLAT );
	UIAddToolBar(m_hWndToolBar);
	
	
	AddSimpleReBarBand(hToolBar, L"Room", TRUE);

	
	CRect rc;
	
	GetParent().GetClientRect(&rc);
	m_vSplit.Create(m_hWnd, rc, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	m_hWndClient = m_vSplit;
	m_vSplit.m_cxyMin = 100;
	m_vSplit.SetSplitterPos(-1);
	
	
	m_vSplit.GetSplitterPaneRect(1,&rc);
	
	m_hSplit.Create(m_vSplit, rc, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	m_hSplit.m_cxyMin = 100;
	m_hSplit.SetSplitterPos(300);
	m_vSplit.SetSplitterPane(1, m_hSplit);
	
	m_userPane.Create(m_hSplit);
	m_userPane.SetTitle(L"User List");
	m_hSplit.SetSplitterPane(0, m_userPane);
	

	m_hSplit.GetSplitterPaneRect(0,&rc);

	CImageList il;
	il.Create(24,24, ILC_COLOR32, 5,5);
	il.AddIcon(AtlLoadIcon(IDI_ONLINE));
	m_userList.Create(m_userPane, rc, NULL, LVS_SINGLESEL| WS_HSCROLL| WS_VISIBLE | WS_CHILDWINDOW | LVS_REPORT |LVS_SINGLESEL |WS_CLIPCHILDREN,
		WS_EX_CLIENTEDGE);
	m_userList.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT);
	m_userPane.SetClient(m_userList);
	m_userList.SetImageList(il,LVSIL_SMALL);
	
	struct { _char * name; } userListCols[] = {
		L"Name        ", L"AccountID", L"PSUID               ", L"GameRoomID", L"GameRoomPSUID"
	};

	for(_dword a=0; a<(sizeof(userListCols)/sizeof(userListCols[0])); a++)
	{
		m_userList.InsertColumn(a, userListCols[a].name);
	}
	
	gResizeList(m_userList);

	// chat
	m_vSplit.GetSplitterPaneRect(0, &rc);
	m_chatSplit.Create(m_vSplit, rc, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	m_chatSplit.SetSplitterPos(-1);
	m_vSplit.SetSplitterPane(0, m_chatSplit);
	
	m_chatSplit.GetSplitterPaneRect(0, &rc);
	m_ChatFrame.Create(m_chatSplit,  rc, NULL, WS_CHILD |  WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	m_chatSplit.SetSplitterPane(0, m_ChatFrame);
	
	m_vSplit.GetClientRect(&rc);
	m_vSplit.SetSplitterPos(rc.Width() - 150);
	
	m_ChatFrame.GetClientRect(&rc);
	m_ChatEdit.Create(m_ChatFrame, rc, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL 
		| ES_MULTILINE  | ES_AUTOVSCROLL | ES_AUTOHSCROLL );
	m_ChatEdit.SetFont(AtlGetDefaultGuiFont());
	m_ChatFrame.AddTabWithIcon(m_ChatEdit, L"Main", AtlLoadIcon(IDI_MSG));

	m_chatSplit.GetSplitterPaneRect(1, &rc);
	CSendMsgDlg *pSendView = new CSendMsgDlg;
	m_pSendDlg = pSendView;
	pSendView->Create(m_chatSplit, rc);
	pSendView->BringWindowToTop();
	m_chatSplit.SetSplitterExtendedStyle ( SPLIT_BOTTOMALIGNED );
	m_chatSplit.SetSplitterPane(1, pSendView->m_hWnd);
	m_chatSplit.GetClientRect(&rc);
	
	m_chatSplit.SetSplitterPos(rc.Height()-50);

	m_hSplit.SetSinglePaneMode(SPLIT_PANE_LEFT);

	CToolBarCtrl toolbar = hToolBar;
	toolbar.DeleteButton(1);

	InitToolBar(hToolBar);

	return 0;
}

BOOL CChatRoomView::OnIdle()
{
	UIUpdateToolBar();
	return FALSE;
}

#define ITOS(i) ::_itow(i, tmp, 10)

void CChatRoomView::AttachUsers( const Array<stCP_UserInfo> & list )
{
	m_userList.DeleteAllItems();
	CComboBox box = m_pSendDlg->GetDlgItem(IDC_USERS);
	
	int a = box.InsertString(0, L"All");
	box.SetItemData(a, 0);
	_char tmp[100];
	for(_dword a=0; a<list.Number(); a++)
	{
		m_userList.InsertItem(a,list[a].NickName, 0);
		m_userList.SetItemText(a, 1, ITOS(list[a].AccID));
		m_userList.SetItemText(a, 2, ClientCommon::PSUID2Str(list[a].UserUid));
		m_userList.SetItemData(a, list[a].AccID);
		m_userList.SetItemText(a, 3, ITOS(list[a].mGameRoomID));
		m_userList.SetItemText(a, 4, ClientCommon::PSUID2Str(list[a].GameRoomUID));

		int n = box.InsertString(a+1, list[a].NickName);
		box.SetItemData(n, list[a].AccID);
	}

	box.SetCurSel(0);

	
	
}

LRESULT CChatRoomView::OnForwardMsg( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/ )
{
	//if(::SendMessage(m_chatSplit.GetSplitterPane(1), WM_FORWARDMSG, 0, lParam))
	//{
	//	return TRUE;
	//}
	return this->PreTranslateMessage((MSG*)lParam);

}



LRESULT CChatRoomView::OnClickSend( UINT w, UINT l, HWND hwnd, BOOL& /*bHandled*/ )
{
	BSTR msg = NULL;
	m_pSendDlg->GetDlgItemText(IDC_MSG, msg);
	if(msg == NULL)
		msg = L"";
	CComboBox box = m_pSendDlg->GetDlgItem(IDC_USERS);
	int userdata = box.GetItemData(box.GetCurSel());
	if(userdata == 0 || userdata == -1)
	{
		GetProcessor()->Chat(CP_DataType_Chat_Normal, m_RoomID, 
			PSUID(), (LPCTSTR)msg);
	}
	else
	{
		GetProcessor()->Chat(CP_DataType_Chat_Normal, m_RoomID, 
			GetProcessor()->GetRoomUser(userdata, m_RoomID)->UserUid, (LPCTSTR)msg);
	}
	m_pSendDlg->SetDlgItemText(IDC_MSG, NULL);
	return 0;
}

LRESULT CChatRoomView::OnContextMenu( UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	POINT pt;
	::GetCursorPos(&pt);
	HWND hwnd = ::WindowFromPoint(pt);
	if(hwnd == m_userList)
	{
		CMenu m;
		m.LoadMenu(IDR_CHATUSERS);
		
		Array<stCP_RoomInfo> list = GetProcessor()->GetJoinedRooms();
		m.GetSubMenu(0).GetSubMenu(1).DeleteMenu(0, MF_BYPOSITION);
		for(u_int a=0; a< list.Number(); a++)
		{
			m.GetSubMenu(0).GetSubMenu(1).InsertMenu(a, MF_BYPOSITION, WM_ROOMID + list[a].ID, list[a].Desp);
		}
		CMenuHandle hm = m.GetSubMenu(0);
		int n = GetFrame()->m_CmdBar.TrackPopupMenu(hm, TPM_RETURNCMD,pt.x, pt.y);
		if(n == 0) return 0;
		LVITEM vi;
		if(FALSE == m_userList.GetSelectedItem(&vi))
		{
			return 0;
		}
		_dword AccID = vi.lParam;
		stCP_UserInfo *pUser = GetProcessor()->GetRoomUser(AccID, m_RoomID);
		if(n == ID_POP_ADDTOMYBUDDYLIST)
		{
			GetProcessor()->AddBuddy(0, L"", pUser->UserUid, pUser->AccID, pUser->NickName);

		}
		else
		{
			int roomid = n-WM_ROOMID;
			GetProcessor()->InviteUserToRoom(pUser->UserUid, roomid,pUser->NickName);

		}
	}
	
	return 0;
}

LRESULT CChatRoomView::OnQuitRoom( UINT w, UINT l, HWND hwnd, BOOL& /*bHandled*/ )
{
	GetProcessor()->QuitRoom(0, m_RoomID);
	return 0;
}

LRESULT CChatRoomView::OnDestroy( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	
	return 0;
}



LRESULT CChatRoomView::OnToolbarNeedText( int , LPNMHDR lParam, BOOL & bHandled )
{
	bHandled = FALSE;
	return 0;
}