#include "StdAfx.h"
#include "LobbyServerdlg.h"

CLobbyServerdlg::CLobbyServerdlg(void)
{
}

CLobbyServerdlg::~CLobbyServerdlg(void)
{
}



LRESULT CLobbyServerdlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DlgResize_Init();
	m_serverlist.SubclassWindow(GetDlgItem(IDC_LIST1));
	m_serverlist.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT);
	//m_serverlist.Create(m_hWnd, rcDefault, NULL);

	// TODO: Add your message handler code here and/or call default
	CListViewCtrl list = GetDlgItem(IDC_LIST1);
	list.InsertColumn(0, L"PSUID");
	list.InsertColumn(1, L"ServerType");
	list.InsertColumn(2, L"ServerVersion");


	for(_dword a=0; a<m_servers.Number(); a++)
	{
		stIP_LobbyNode & node = m_servers[a];
		list.InsertItem(a, ClientCommon::PSUID2Str(node.ServerID));
		list.SetItemText(a, 1, ClientCommon::GetServerDespByType(node.ServerType));
		String s;
		s.Format(L"%d", node.ServerVer);
		list.SetItemText(a, 2, s);
		
	}


//	list.SetRedraw(FALSE);
	gResizeList(list);
	
//	list.SetRedraw(TRUE);

	CenterWindow(GetParent());

	return TRUE;
}

LRESULT CLobbyServerdlg::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: Add your message handler code here and/or call default
	EndDialog(0);
	return 0;
}

LRESULT CLobbyServerdlg::OnOK( UINT uCode, int nID, HWND hwndCtrl, BOOL& bHandled )
{
	EndDialog(0);
	return 0;
}
