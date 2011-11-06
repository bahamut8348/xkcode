#include "StdAfx.h"

#include "CommonContrl.h"
#include "ValidEdit.h"

#include "ChangePassDlg.h"


LRESULT CChangePassDlg::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled )
{
	EndDialog(IDCANCEL);
	return 0;
}

LRESULT CChangePassDlg::OnCancel( WORD nCode, WORD nID, HWND hwnd, BOOL & bHandled )
{
	EndDialog(IDCANCEL);
	return 0;
}

LRESULT CChangePassDlg::OnOK( WORD nCode, WORD nID, HWND hwnd, BOOL & bHandled )
{
	DoDataExchange(TRUE);
	EndDialog(IDOK);
	return 0;
}

LRESULT CChangePassDlg::OnInitDlg( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled )
{
	CenterWindow(GetParent());
	return 0;
}