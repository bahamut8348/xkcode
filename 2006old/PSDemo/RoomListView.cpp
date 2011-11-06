#include "StdAfx.h"
#include "PromoteDlg.h"
#include "resource.h"
#include "LogEdit.h"
#include "LogWindow.h"
#include "Mytabview.h"
#include "MainFrm.h"
#include "CreateRoomDlg.h"
#include "RoomListView.h"

BOOL CRoomListView::PreTranslateMessage( MSG* pMsg )
{
	BSTR text = 0;
	if(pMsg->hwnd)
	{
		CWindow w = pMsg->hwnd;
		w.GetWindowText(text);
	}
	
	if(pMsg->message == WM_KEYDOWN)
	{
		return SendMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
		
	}
	if(pMsg->message == WM_COMMAND)
	{
		return SendMessage(WM_COMMAND, pMsg->wParam, pMsg->lParam);
		
	}
	//this->ProcessWindowMessage(m_hWnd, pMsg->message, pMsg->wParam, pMsg->lParam)
	return FALSE;
}
LRESULT CRoomListView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: Add your message handler code here and/or call default
	RECT rclist = {0,0,1000,1000};
	m_rlist.Create(m_hWnd, rcDefault, NULL,WS_HSCROLL| WS_VISIBLE | WS_CHILDWINDOW 
		| LVS_REPORT |LVS_SINGLESEL |WS_CLIPCHILDREN);
	
	m_rlist.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES );
	AreaID = 0;

	struct ColumnHeader
	{
		_char *name;
	} headers[] = {L"ID       ",L"Name              ", L"Use Password", L"AreaID", L"RoomType", L"Status", L"User Number", L"Chat Server"};
	
	for(_dword a=0; a<(sizeof(headers)/sizeof(headers[0])); a++)
	{
		m_rlist.InsertColumn(a, headers[a].name);
	}
	
	CImageList il;
	il.Create(24,24,ILC_COLOR32,5,5);
	il.AddIcon(AtlLoadIcon(IDI_ROOM));
	il.AddIcon(AtlLoadIcon(IDI_LOCK));
	m_rlist.SetImageList(il,LVSIL_SMALL );
	
	gResizeList(m_rlist);
	return 0;
}
#define ITOS(i) ::_itow(i, tmp, 10)
void CRoomListView::SetRooms( const Array<stCP_RoomInfo> & list )
{
	
	m_rlist.DeleteAllItems();
	_char tmp[100];
	LVITEM vim;
	for(_dword a=0; a<list.Number(); a++)
	{
		int n = m_rlist.InsertItem(a, ITOS(list[a].ID), 0);
		m_rlist.SetItemData(n, list[a].ID);
		m_rlist.SetItemText(n, COL_NAME, list[a].Desp);
		m_rlist.SetItemText(n, COL_BNEEDPASS, ITOS(list[a].beNeedPass));
		{
			vim.iImage = 1;
			vim.iItem = n;
			vim.iSubItem = COL_BNEEDPASS;
			vim.mask =  LVIF_IMAGE;
			if(list[a].beNeedPass != 0) 
				m_rlist.SetItem(&vim);
		}
		m_rlist.SetItemText(n, COL_AREAID, ITOS(list[a].AreaID));
		m_rlist.SetItemText(n, COL_ROOMTYPE, ITOS(list[a].RoomType));
		m_rlist.SetItemText(n, COL_STATUS, ITOS(list[a].Status));
		m_rlist.SetItemText(n, COL_USERNUM, ITOS(list[a].UserNum));
		m_rlist.SetItemText(n, COL_CS, ClientCommon::PSUID2Str(list[a].ChatServerUID));

	}
	
}
LRESULT CRoomListView::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	// TODO: Add your message handler code here and/or call default
	m_rlist.ResizeClient(LOWORD(lParam), HIWORD(lParam));
	return 0;
}

PSDash::_dword CRoomListView::getType() const
{
	return BV_ROOMLIST;
}

LRESULT CRoomListView::OnRoomDBClick( int , LPNMHDR pnmh, BOOL& )
{
	LPNMITEMACTIVATE item = (LPNMITEMACTIVATE)pnmh;
	CPromoteDlg dlg;
	TCHAR *Password=NULL;
	_dword RoomID = m_rlist.GetItemData(item->iItem);
	stCP_RoomInfo rInfo = GetProcessor()->GetRoomBaseInfo(RoomID);
	if(rInfo.beNeedPass)
	{
		
		dlg.Set(L"Join", L"Cancel", L"Type password:" , L"Need Password");
		if(ID_BNLEFT == dlg.DoModal(m_hWnd))
		{
			Password = dlg.m_Msg.GetBuffer(1024);
		}
		else
		{
			return 0;
		}
	}

	GetProcessor()->JoinRoom(1,rInfo.ChatServerUID, RoomID, GetFramework()->GetMyUserName(),Password);
	
	
	return 0;
}

LRESULT CRoomListView::OnContextMenu( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	CMenu m;
	m.LoadMenu(IDR_ROOMLIST);
	CMenuHandle hm = m.GetSubMenu(0);
	CPoint pt;
	::GetCursorPos(&pt);
	int ret = GetFrame()->m_CmdBar.TrackPopupMenu(hm,0,pt.x, pt.y);

	return 0;
}

LRESULT CRoomListView::OnKeyDown( UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	u_int key = wParam;
	if(key == VK_F5) SendMessage(WM_COMMAND, ID_POP_REFRESH);
	return 0;
}

LRESULT CRoomListView::OnForwardMsg( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/ )
{
	return this->PreTranslateMessage((LPMSG)lParam);
}

LRESULT CRoomListView::OnCreateRoom( WORD Code, WORD nID, HWND hwnd, BOOL & bHandled )
{
	CCreateRoomDlg dlg;
	dlg.SetParam((LPCTSTR)GetFramework()->GetMyUserName(),L"", L"", FALSE, 10, 0);
	int sel = dlg.DoModal(m_hWnd);
	if(sel == IDOK)
	{
		GetProcessor()->CreateRoom(0, AreaID, dlg.m_NickName,dlg.m_RoomName,
			dlg.m_bUsePass, dlg.m_Password, dlg.m_MaxUser, dlg.m_RoomType);
	}
	return 0;
}

LRESULT CRoomListView::OnRefresh( WORD Code, WORD nID, HWND hwnd, BOOL & bHandled )
{
	GetProcessor()->QueryRoomList(0, AreaID, -1);
	m_rlist.DeleteAllItems();
	return 0;
}