#include "StdAfx.h"
#include "resource.h"
#include "Anotherwindow.h"

CAnotherwindow::CAnotherwindow(void)
{
}
CAnotherwindow::~CAnotherwindow(void)
{
}

LRESULT CAnotherwindow::OnBnClickedButton1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	MessageBox(L"Test");
	return 0;
}

LRESULT CAnotherwindow::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: Add your message handler code here and/or call default
	EndDialog(1);
	return 0;
}
