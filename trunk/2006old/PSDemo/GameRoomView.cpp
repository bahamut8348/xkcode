//============================================================================
// CGameRoomView.cpp
//
// Copyright 2006 Possibility Space Incorporated.
//============================================================================


#include "stdafx.h"
#include "SendMsgDlg.h"
#include "GameRoomView.h"

PSDash::_dword CGameRoomView::getType() const
{
	return BV_GAME;
}

LRESULT CGameRoomView::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled )
{
	CChatRoomView::OnCreate(uMsg, wParam, lParam, bHandled);
	


	return 0;
}

PSDash::_void CGameRoomView::InitToolBar( HWND toolbar )
{
	m_ToolBar = toolbar;
	CImageList il = m_ToolBar.GetImageList();
	il.AddIcon(AtlLoadIcon(IDI_STARTGAME));
	//m_ToolBar.SetExtendedStyle(TBSTYLE_EX_MIXEDBUTTONS);
	//m_ToolBar.AddString(IDI_STARTGAME);
	TBBUTTON button;
	button.bReserved[0] = 0;
	button.bReserved[1]= 0;
	button.dwData =0;
	button.fsState = TBSTATE_ENABLED ;
	button.fsStyle = BTNS_SHOWTEXT ;
	button.iBitmap = 2;
	button.idCommand = IDI_STARTGAME;
	button.iString =0;// IDI_STARTGAME;//(INT_PTR)L"Start Game";

	m_ToolBar.AddButton(&button);	//m_ToolBar.ShowWindow(SW_HIDE);

}

LRESULT CGameRoomView::OnStartGame( WORD nCode, WORD nID, HWND hwnd, BOOL & bHandled )
{
	GetProcessor()->StartGame(0, m_RoomID);
	return 0;
}