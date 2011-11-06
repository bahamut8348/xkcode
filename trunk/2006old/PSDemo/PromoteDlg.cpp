#include "StdAfx.h"
#include "PromoteDlg.h"


LRESULT CPromoteDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO: Add your message handler code here and/or call default
	m_BnCmd1 = GetDlgItem(ID_BNLEFT);
	m_BnCmd2 = GetDlgItem(ID_BNRIGHT);
	m_BnCmd1.SetWindowText(m_Cmd1);
	m_BnCmd2.SetWindowText(m_Cmd2);
	SetWindowText(m_Title);
	DoDataExchange();

	CenterWindow(GetParent());

	CMenuHandle m = GetSystemMenu(FALSE);
	m.EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);
	

	return TRUE;
}

LRESULT CPromoteDlg::OnLeftCmd( WORD nCode, WORD nID, HWND hwnd, BOOL & bHandled )
{
	EndDialog(ID_BNLEFT);
	return 0;
}

LRESULT CPromoteDlg::OnRightCmd( WORD nCode, WORD nID, HWND hwnd, BOOL & bHandled )
{
	EndDialog(ID_BNRIGHT);
	return 0;
}
