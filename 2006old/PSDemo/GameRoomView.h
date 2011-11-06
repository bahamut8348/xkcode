//============================================================================
// CGameRoomView.h
//
// Copyright 2006 Possibility Space Incorporated.
//============================================================================

#pragma once  
#include "ChatRoomView.h"

class CGameRoomView : public CChatRoomView
{

public:
	
	BEGIN_MSG_MAP(CGameRoomView)
		//MESSAGE_HANDLER(WM_CREATE, OnCreate)
		CHAIN_MSG_MAP(CChatRoomView)

		COMMAND_ID_HANDLER(IDI_STARTGAME, OnStartGame)
	END_MSG_MAP()

	LRESULT OnCreate (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT  OnStartGame(WORD nCode, WORD nID, HWND hwnd, BOOL & bHandled);
	
	virtual _dword getType() const;

	virtual _void InitToolBar(HWND toolbar);

	CToolBarCtrl m_ToolBar;


};