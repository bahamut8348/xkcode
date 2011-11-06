#include "StdAfx.h"
#include "LogDlg.h"


LRESULT CLogDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO: Add your message handler code here and/or call default
	m_edit.SubclassWindow(GetDlgItem(IDC_LOGEDIT));
	return TRUE;
}
