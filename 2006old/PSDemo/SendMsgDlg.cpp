#include "StdAfx.h"
#include "SendMsgDlg.h"


BOOL CSendMsgDlg::PreTranslateMessage( MSG* pMsg )
{
	
	return IsDialogMessage(pMsg);
}

BOOL CSendMsgDlg::OnIdle()
{
	return FALSE;
}
LRESULT CSendMsgDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO: Add your message handler code here and/or call default
	DlgResize_Init(FALSE,FALSE,WS_CLIPCHILDREN);
	return FALSE;
}

LRESULT CSendMsgDlg::OnForwardMsg( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	LPMSG pMsg = (LPMSG)lParam;
	if(pMsg->hwnd == m_hWnd)
		return this->PreTranslateMessage(pMsg);
	else
	{
		return FALSE;
	}
}



LRESULT CSendMsgDlg::OnCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	UINT msg = LOWORD(wParam);
	switch(msg)
	{
	case IDC_BNSEND:
	case IDC_MSG:
	case IDC_COMBO1:
		this->SendMessage(GetParent().GetParent(), WM_COMMAND, wParam, lParam);
		return 0;
		break;
	}

	bHandled = FALSE;
	return 0;
}