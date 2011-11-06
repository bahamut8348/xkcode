#include "StdAfx.h"
#include "CreateRoomDlg.h"

//LRESULT CCreateRoomDlg::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//{
//	// TODO: Add your message handler code here and/or call default
//
//	return 0;
//}

LRESULT CCreateRoomDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO: Add your message handler code here and/or call default
	CenterWindow();
	DoDataExchange();
	return TRUE;
}

LRESULT CCreateRoomDlg::OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	DoDataExchange(TRUE);

	// 0 --> 2
	// 1 --> 1
	m_RoomType  = CP_RoomType_Chat - m_RoomType;
	EndDialog(IDOK);
	return 0;
}

LRESULT CCreateRoomDlg::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(IDCANCEL);
	
	return 0;
}
